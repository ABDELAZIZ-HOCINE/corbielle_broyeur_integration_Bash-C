#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include <dirent.h>
#include <stdbool.h>

// Fonction pour afficher un tableau de caractères (tableau 2D)
void print_mat(char** tab, int nb_lignes, int nb_columns) {
    // Parcours de chaque ligne du tableau
    for (int i = 0; i < nb_lignes; i++) {
        printf("|");
        // Parcours de chaque colonne de la ligne
        for (int j = 0; j < nb_columns; j++) {
            printf("%c|", tab[i][j]);
        }
        printf("\n");
    }
}

// Fonction pour fusionner deux chaînes triées
void fusion(char *chaine, int debut, int milieu, int fin) {
    // Calculer la taille des sous-ensembles
    int tailleGauche = milieu - debut + 1;
    int tailleDroite = fin - milieu;

    // Créer des chaînes temporaires
    char *gauche = (char *)malloc(tailleGauche * sizeof(char));
    char *droite = (char *)malloc(tailleDroite * sizeof(char));

    // Copier les données dans les chaînes temporaires
    for (int i = 0; i < tailleGauche; i++) {
        gauche[i] = chaine[debut + i];
    }
    for (int i = 0; i < tailleDroite; i++) {
        droite[i] = chaine[milieu + 1 + i];
    }

    // Fusionner les deux chaînes triées dans la chaîne originale
    int i = 0, j = 0, k = debut;

    // Comparer et fusionner les caractères des deux sous-tableaux
    while (i < tailleGauche && j < tailleDroite) {
        if (toupper(gauche[i]) <= toupper(droite[j])) {
            chaine[k++] = gauche[i++];
        } else {
            chaine[k++] = droite[j++];
        }
    }
    // Copier les éléments restants de gauche (s'il y en a)
    while (i < tailleGauche) {
        chaine[k++] = gauche[i++];
    }
    // Copier les éléments restants de droite (s'il y en a)
    while (j < tailleDroite) {
        chaine[k++] = droite[j++];
    }
    // Libérer la mémoire allouée pour les chaînes temporaires
    free(gauche);
    free(droite);
}

// Fonction de tri fusion
void triFusion(char *chaine, int debut, int fin) {
    if (debut < fin) {
        // Calculer l'indice du milieu
        int milieu = debut + (fin - debut) / 2;
        // Trier récursivement les deux sous-chaînes
        triFusion(chaine, debut, milieu);
        triFusion(chaine, milieu + 1, fin);
        // Fusionner les deux chaînes triées
        fusion(chaine, debut, milieu, fin);
    }
}

// Fonction pour trier une chaîne de caractères
char* trier(char* chaine) {
    int len = strlen(chaine);
    
    // Allouer de la mémoire pour la copie de la chaîne
    char* ch_t = (char*)malloc((len + 1) * sizeof(char));  // +1 pour le caractère nul '\0'

    // Copier le tableau original dans mdp_t
    for (int i = 0; i < len; i++) {
        ch_t[i] = chaine[i];
    }
    ch_t[len] = '\0';

    // Appliquer le tri fusion sur la copie de la chaîne
    triFusion(ch_t, 0, len - 1);

    return ch_t;
}

void extract_file_and_dir(const char* path, char** dirname, char** filename) {
    // Localiser la dernière occurrence de '/'
    const char* last_slash = strrchr(path, '/');
    
    if (last_slash) {
        // Le répertoire correspond à tout avant le dernier '/'
        size_t dir_len = last_slash - path;
        *dirname = (char*)malloc((dir_len + 1) * sizeof(char));
        strncpy(*dirname, path, dir_len);
        (*dirname)[dir_len] = '\0';  // Ajouter le caractère de fin

        // Le fichier correspond à tout après le dernier '/'
        *filename = strdup(last_slash + 1);
    } else {
        // Si aucun '/' n'est trouvé, c'est un fichier dans le répertoire courant
        *dirname = strdup(".");
        *filename = strdup(path);
    }
}



// Fonction pour lire un fichier, remplir et transposer le tableau
void transpose(char* path, char* mdp) {
    //printf("Transposition en cours...\n");

    // Extraire le dirname et le filename
    char* dirname = NULL;
    char* filename = NULL;
    extract_file_and_dir(path, &dirname, &filename);

    int k = strlen(mdp);

    // Ouvrir le fichier en mode binaire (lecture)
    FILE* my_file = fopen(path, "rb");
    if (!my_file) {
        printf("Erreur d'ouverture de fichier !\n");
        return;
    }

    int nb_char = 0;
    char buffer[1024];  // Taille du buffer, peut être ajustée si nécessaire
    size_t bytes_read;

    // Compter le nombre total de caractères dans le fichier
    while ((bytes_read = fread(buffer, sizeof(char), sizeof(buffer), my_file)) > 0) {
        nb_char += bytes_read;
    }

    // Revenir au début du fichier
    rewind(my_file);

    // Calculer le nombre de colonnes et de lignes pour la matrice
    int nb_colonnes = k;
    int nb_lignes = nb_char / k;
    if (nb_char % k != 0) {
        nb_lignes++;
    }

    // Allocation mémoire pour le tableau transposé
    char **tab_transpose = (char**)malloc(nb_lignes * sizeof(char*));
    for (int i = 0; i < nb_lignes; i++) {
        tab_transpose[i] = (char*)malloc(nb_colonnes * sizeof(char));
    }

    int cpt = 0;

    // Remplir le tableau transposé avec les caractères du fichier
    for (int i = 0; i < nb_lignes; i++) {
        for (int j = 0; j < nb_colonnes; j++) {
            if (cpt < nb_char) {
                fread(&tab_transpose[i][j], sizeof(char), 1, my_file);
                cpt++;
            } else {
                tab_transpose[i][j] = '\0';  // Remplir avec des espaces si nécessaire
            }
        }
    }
    // Revenir au début du fichier
    rewind(my_file);

    // Fermer le fichier
    fclose(my_file);

    // Débogage (optionnel, à activer si nécessaire) ----------
    // Afficher le tableau transposé
    // print_mat(tab_transpose, nb_lignes, nb_colonnes);

    // Trier  le mot de passe
    char* mdp_t = trier(mdp);

    //Débogage (optionnel, à activer si nécessaire)----------
    //printf("mdp_t: %s\n", mdp_t);
    //printf("mdp: %s\n", mdp);

    int j = 0;

    char* subfilename = (char*)malloc(20 * sizeof(char));

    // Tableau pour suivre les indices déjà traités
    bool* indices_traites = (bool*)calloc(k, sizeof(bool)); // Tableau pour savoir si l'indice i a déjà été traité
    while (j < k)
    {
        for (int i = 0; i < k; i++) {
            if (mdp_t[j] != mdp[i] || indices_traites[i]) {
                continue;
            } else {
                // Créer un nom de fichier pour le fichier enfant
                sprintf(subfilename, "%s-KEY-%s-PART-%d",mdp, filename, j + 1);
                char full_path[1024];  // Taille fixe pour le chemin complet
                snprintf(full_path, sizeof(full_path), "%s/%s", dirname, subfilename);

                // Ouvrir le fichier avec le chemin complet
                FILE* subfile = fopen(full_path, "wb");
                for (int ligne = 0; ligne < nb_lignes; ligne++) {
                    fwrite(&tab_transpose[ligne][i], sizeof(char), 1, subfile);
                }

                // Marquer l'indice comme traité
                indices_traites[i] = true;
                j++;
                fclose(subfile);
                break;
            }
        }
    }
    free(indices_traites);  // Libère la mémoire allouée pour les indices traités
    // Supprimer le fichier original
    if (remove(path) != 0) {
        perror("Erreur lors de la suppression du fichier enfant");
    }

    // Une fois la transposition terminée, affichez un message de confirmation
    //printf("Transposition terminée avec succès.\n");

    // Libération de la mémoire allouée
    free(subfilename); // Libère la mémoire pour le nom du fichier enfant
    for (int i = 0; i < nb_lignes; i++) free(tab_transpose[i]);  // Libère chaque ligne du tableau
    free(tab_transpose);  // Libère le tableau principal
    free(mdp_t);  // Libère la mémoire pour le mot de passe trié

    
    // Nettoyer la mémoire
    free(dirname);
    free(filename);
}

int count_file_with_prefix(const char *directory_path, const char* password, const char* prefix) {
    struct dirent *entry;               // Structure pour stocker chaque entrée du répertoire
    DIR *dir = opendir(directory_path); // Ouvrir le répertoire

    if (dir == NULL) {  // Si l'ouverture du répertoire échoue
        perror("Erreur lors de l'ouverture du répertoire");
        return 0;  // Retourner 0, car aucun fichier ne peut être compté
    }

    int count = 0;  // Initialisation du compteur de fichiers valides
    // Parcourir toutes les entrées du répertoire
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {  // Vérifier si l'entrée est un fichier régulier
            // Vérifier si le nom du fichier commence par le mot de passe
            if (strncmp(entry->d_name, password, strlen(password)) == 0) {
                // Chercher la position de "_KEY-" et "_PART-"
                const char *key_pos = strstr(entry->d_name, "-KEY-");
                const char *part_pos = strstr(entry->d_name, "-PART-");

                // Vérifier que "_KEY-" et "_PART-" existent et que "_PART-" vient après "_KEY-"
                if (key_pos != NULL && part_pos != NULL && part_pos > key_pos) {
                    // Extraire la partie entre "mdp-" et "_KEY-" pour vérifier le préfixe
                    const char *prefix_start = key_pos + 5;  // Après "_KEY-"
                    int prefix_len = part_pos - prefix_start;
                    
                    // Vérifier que la partie avant "_PART-" correspond au préfixe donné
                    if (prefix_len == strlen(prefix) && strncmp(prefix_start, prefix, prefix_len) == 0) {
                        // Extraire la partie après "_PART-" et vérifier si elle est numérique
                        const char *part_end = part_pos + 6;  // Après "_PART-"
                        if (isdigit(part_end[0])) {
                            int is_valid = 1;
                            for (int i = 0; part_end[i] != '\0'; i++) {
                                if (!isdigit(part_end[i])) {
                                    is_valid = 0;
                                    break;  // Si un caractère n'est pas un chiffre, marquer comme invalide
                                }
                            }

                            // Si tout est valide, incrémenter le compteur
                            if (is_valid) {
                                count++;
                            }
                        }
                    }
                }
            }
        }
    }

    closedir(dir);  // Fermer le répertoire une fois le traitement terminé

    // Si un fichier a été trouvé
    if (count == 0) {
        printf("Aucun fichier trouvé dans le répertoire.\n");
        exit(EXIT_FAILURE); // Termine le programme avec un code d'erreur
    } else {
        return count;   // Retourner le nombre de fichiers valides trouvés
    }

}

int size_largest_file(const char *directory_path, const char* password, const char* prefix) {
    DIR *dir = opendir(directory_path);  // Ouvrir le répertoire spécifié
    struct dirent *entry;                // Structure pour lire les entrées du répertoire
    long max_chars = 0;                  // Variable pour stocker le nombre maximal de caractères trouvé
    char largest_file[256] = "";         // Variable pour stocker le nom du fichier avec le plus grand nombre de caractères
    char full_path[1024];                // Variable pour stocker le chemin complet du fichier

    if (dir == NULL) {  // Si l'ouverture du répertoire échoue
        perror("Erreur lors de l'ouverture du répertoire");
        return 0;  // Retourner 0 si le répertoire ne peut pas être ouvert
    }

    // Parcourir toutes les entrées du répertoire
    while ((entry = readdir(dir)) != NULL) {
        // Vérifier si l'entrée est un fichier régulier et commence par le mot de passe
        if (entry->d_type == DT_REG && strncmp(entry->d_name, password, strlen(password)) == 0) {
            // Chercher la position de "-KEY-" et "-PART-"
            const char *key_pos = strstr(entry->d_name, "-KEY-");
            const char *part_pos = strstr(entry->d_name, "-PART-");

            // Vérifier que "-KEY-" et "-PART-" existent et que "-PART-" vient après "-KEY-"
            if (key_pos != NULL && part_pos != NULL && part_pos > key_pos) {
                // Extraire la partie entre "-KEY-" et "-PART-" pour vérifier si c'est le bon préfixe
                const char *prefix_start = key_pos + 5;  // Après "-KEY-"
                int prefix_len = part_pos - prefix_start;
                
                // Vérifier que la partie entre "-KEY-" et "-PART-" correspond au préfixe donné
                if (prefix_len == strlen(prefix) && strncmp(prefix_start, prefix, prefix_len) == 0) {
                    // Extraire la partie après "-PART-" et vérifier si elle est numérique
                    const char *part_end = part_pos + 6;  // Après "-PART-"
                    if (isdigit(part_end[0])) {
                        int is_valid = 1;
                        for (int i = 0; part_end[i] != '\0'; i++) {
                            if (!isdigit(part_end[i])) {
                                is_valid = 0;
                                break;  // Si un caractère n'est pas un chiffre, marquer comme invalide
                            }
                        }

                        // Si tout est valide, ouvrir le fichier et compter le nombre de caractères
                        if (is_valid) {
                            snprintf(full_path, sizeof(full_path), "%s/%s", directory_path, entry->d_name);

                            FILE *file = fopen(full_path, "rb");  // Ouvrir le fichier en mode binaire
                            if (file != NULL) {
                                int char_count = 0;  // Variable pour compter le nombre de caractères dans le fichier
                                char buffer[1024];    // Définir un buffer pour lire plusieurs caractères à la fois
                                size_t bytes_read;

                                // Lire le fichier par blocs de 1024 caractères
                                while ((bytes_read = fread(buffer, sizeof(char), sizeof(buffer), file)) > 0) {
                                    char_count += bytes_read;  // Ajouter le nombre de caractères lus dans le buffer au total
                                }

                                fclose(file);  // Fermer le fichier après lecture

                                // Si ce fichier a plus de caractères que le fichier précédent, mettre à jour le fichier le plus grand
                                if (char_count > max_chars) {
                                    max_chars = char_count;  // Mettre à jour le nombre maximal de caractères
                                    strncpy(largest_file, entry->d_name, sizeof(largest_file));  // Sauvegarder le nom du fichier
                                }
                            } else {
                                perror("Erreur lors de l'ouverture d'un fichier");  // Si l'ouverture du fichier échoue
                            }
                        }
                    }
                }
            }
        }
    }

    closedir(dir);  // Fermer le répertoire après avoir traité toutes les entrées
    return max_chars;  // Retourner le nombre maximal de caractères trouvé
}


//fonction pour extraire le mot de passe 
char* extract_password_from_filename(const char *directory_path, const char* prefix) {

    DIR* dir = opendir(directory_path);  // Open the directory
    struct dirent* entry;  // Structure to read directory entries
    char* password = NULL;  // Variable to store extracted password

    if (dir == NULL) {
        perror("Error opening directory");
        return NULL;
    }

    while ((entry = readdir(dir)) != NULL) {
        char* key_pos = strstr(entry->d_name, "-KEY-");
        if (key_pos != NULL) {
            char* part_pos = strstr(key_pos + 5, "-PART-");
            if (part_pos != NULL) {
                // Extract portion between "_KEY_" and "_PART_"
                int len_between_key_part = part_pos - (key_pos + 5);
                char* extracted_prefix = (char*)malloc((len_between_key_part + 1) * sizeof(char));
                if (extracted_prefix != NULL) {
                    strncpy(extracted_prefix, key_pos + 5, len_between_key_part);
                    extracted_prefix[len_between_key_part] = '\0';

                    if (strcmp(extracted_prefix, prefix) == 0) {
                        int len_before_key = key_pos - entry->d_name;
                        password = (char*)malloc((len_before_key + 1) * sizeof(char));

                        if (password != NULL) {
                            strncpy(password, entry->d_name, len_before_key);
                            password[len_before_key] = '\0';
                            free(extracted_prefix);  // Clean up
                            closedir(dir);  // Close directory
                            return password;  // Return extracted password
                        }
                    }
                    free(extracted_prefix);
                }
            }
        }
    }

    closedir(dir);  // Close directory
    return NULL;  // Return NULL if no valid password is found
}

// Fonction pour reconstruire un fichier à partir de la matrice transposée et supprimer les fichiers enfants
int reconstruire_depuis_matrice(char** tab_transpose, int nb_lignes, int nb_colonnes, const char* password,const char *directory_path,const char* output_filename) {

    char o_full_path[1024];  // Taille fixe pour le chemin complet
    snprintf(o_full_path, sizeof(o_full_path), "%s/%s", directory_path, output_filename);
    // Ouverture du fichier principal en écriture
    FILE* output_file = fopen(o_full_path, "wb");
    if (!output_file) {  // Si l'ouverture du fichier échoue
        perror("Erreur lors de l'ouverture du fichier principal");  // Afficher l'erreur
        return 0;  // Retourner 0 pour indiquer une erreur
    }

    // Reconstruction du fichier principal depuis la matrice transposée
    //printf("Reconstruction du fichier '%s' depuis la matrice...\n", output_filename);
    for (int i = 0; i < nb_lignes; i++) {
        for (int j = 0; j < nb_colonnes; j++) {
            // Éviter d'écrire des caractères vides ('\0') dans le fichier
            if (tab_transpose[i][j] != '\0') {
                fwrite(&tab_transpose[i][j], sizeof(char), 1, output_file);  // Écrire le caractère dans le fichier
            }
        }
    }
    fclose(output_file);  // Fermer le fichier après avoir écrit toutes les données
    //printf("Reconstruction terminée avec succès.\n");

    // Suppression des fichiers enfants ayant le préfixe donné
    DIR* dir = opendir(directory_path);  // Ouvrir le répertoire courant
    if (!dir) {  // Si l'ouverture du répertoire échoue
        perror("Erreur lors de l'ouverture du répertoire");
        return 0;  // Retourner 0 pour indiquer une erreur
    }

    struct dirent* entry;
    char filepath[1024];  // Variable pour stocker le chemin complet du fichier
    int count_deleted = 0;  // Compteur pour suivre le nombre de fichiers supprimés
    char* prefix = (char*)malloc(256 * sizeof(char));  // Allouer de la mémoire pour le préfixe
    sprintf(prefix, "%s-KEY-%s-PART-",password, output_filename);  // Créer le préfixe basé sur le nom du fichier de sortie
    //printf("Suppression des fichiers enfants avec le préfixe '%s'...\n", prefix);

    // Parcourir toutes les entrées du répertoire
    while ((entry = readdir(dir)) != NULL) {
        // Vérifier si le fichier commence par le préfixe
        if (strncmp(entry->d_name, prefix, strlen(prefix)) == 0) {
            snprintf(filepath, sizeof(filepath), "%s/%s", directory_path, entry->d_name);  // Construire le chemin complet du fichier

            // Supprimer le fichier enfant
            if (remove(filepath) == 0) {
                //printf("Fichier '%s' supprimé.\n", filepath);  // Afficher un message de succès
                count_deleted++;  // Incrémenter le compteur de fichiers supprimés
            } else {
                perror("Erreur lors de la suppression d'un fichier enfant");  // Afficher une erreur si la suppression échoue
            }
        }
    }

    closedir(dir);  // Fermer le répertoire après avoir traité toutes les entrées

    free(prefix);  // Libérer la mémoire allouée pour le préfixe
    //printf("Suppression terminée : %d fichiers supprimés.\n", count_deleted);  // Afficher le nombre de fichiers supprimés
    return 1;  // Retourner 1 pour indiquer un succès
}

// Fonction pour lire un fichier, remplir et transposer le tableau
int detranspose(char* path, char* mdp) {

    // Extraire le dirname et le filename
    char* dirname = NULL;
    char* filename = NULL;
    extract_file_and_dir(path, &dirname, &filename);

    int k = strlen(mdp);  // Taille du mot de passe
    char* mdp_t = trier(mdp);  // Trie le mot de passe

    char* prefix = (char*)malloc(256 * sizeof(char));  // Allocation de mémoire pour le préfixe du fichier
    sprintf(prefix, "%s", filename);  // Crée le préfixe en ajoutant un espace après le nom du fichier

    int nb_subfile = count_file_with_prefix(dirname,mdp,prefix);  // Compte le nombre de fichiers avec ce préfixe

    // Débogage:-------------------------------------------------------
    // printf("Taille du mot de passe : %d\n", k);
    // printf("Nombre de fichiers trouvés : %d\n", nb_subfile);

    // Vérifie si le nombre de fichiers avec le préfixe correspond à la taille du mot de passe
    if (nb_subfile != k) {
        printf("Erreur : le fichier %s ne peut pas être reconstruit !\n", filename);
        free(mdp_t);  // Libère la mémoire allouée pour le mot de passe trié
        return 0;  // Retourne 0 si une erreur se produit
    }

    // Détermine le nombre de lignes du tableau (la taille de la plus grande ligne des fichiers enfants)
    int nb_lignes = size_largest_file(dirname,mdp,prefix);
    //printf("nb_lignes: %d\n",nb_lignes);
    free(prefix);  // Libère la mémoire allouée pour le préfixe
    // printf("Nombre de lignes dans le tableau : %d\n", nb_lignes);

    // Allocation de la mémoire pour le tableau transposé (une matrice de caractères)
    char** tab_transpose = (char**)malloc(nb_lignes * sizeof(char*));
    if (!tab_transpose) {
        perror("Erreur d'allocation mémoire pour le tableau");
        free(mdp_t);  // Libère la mémoire du mot de passe trié
        return 0;  // Retourne 0 en cas d'erreur d'allocation
    }
    for (int i = 0; i < nb_lignes; i++) {
        tab_transpose[i] = (char*)calloc(k, sizeof(char));  // Initialise chaque ligne avec des zéros
        if (!tab_transpose[i]) {
            perror("Erreur d'allocation mémoire pour une ligne du tableau");
            // Libère la mémoire en cas d'erreur d'allocation pour une ligne
            for (int j = 0; j < i; j++) free(tab_transpose[j]);
            free(tab_transpose);
            free(mdp_t);  // Libère le mot de passe trié
            return 0;  // Retourne 0 en cas d'erreur
        }
    }

    // Allocation de la mémoire pour le nom du fichier enfant temporaire
    char* subfile = (char*)malloc(256 * sizeof(char));
    if (!subfile) {
        perror("Erreur d'allocation mémoire pour subfile");
        // Libère les ressources en cas d'erreur d'allocation
        for (int i = 0; i < nb_lignes; i++) free(tab_transpose[i]);
        free(tab_transpose);
        free(mdp_t);
        return 0;
    }

    // Tableau pour suivre les indices déjà traités (empêche de traiter plusieurs fois le même fichier)
    bool* indices_traites = (bool*)calloc(k, sizeof(bool));

    int j = 0;
    while (j < k) {
        for (int i = 0; i < k; i++) {
            // Si le caractère dans le mot de passe trié ne correspond ou si l'indice a déjà été traité, on passe au suivant
            if (mdp_t[j] != mdp[i] || indices_traites[i]) {
                continue;
            } else {
                // Si une correspondance est trouvée dans le mot de passe, on traite le fichier
                sprintf(subfile, "%s-KEY-%s-PART-%d",mdp, filename, j + 1);  // Crée le nom du fichier enfant

                char sub_full_path[1024];  // Taille fixe pour le chemin complet
                snprintf(sub_full_path, sizeof(sub_full_path), "%s/%s", dirname, subfile);

                FILE* file = fopen(sub_full_path, "rb");  // Ouvre le fichier en lecture binaire
                if (!file) {
                    perror("Erreur d'ouverture du fichier");
                    return 0;  // Passe au fichier suivant si une erreur d'ouverture survient
                }

                int ligne = 0;
                char c;
                char buffer[nb_lignes];  // Buffer pour stocker les caractères lus
                while ((fread(buffer, sizeof(char), 1, file)) == 1 && ligne < nb_lignes) {
                    tab_transpose[ligne][i] = buffer[0];  // Remplir la colonne correspondante du tableau
                    ligne++;
                }
                rewind(file);  // Remet le curseur au début du fichier
                fclose(file);  // Ferme le fichier après lecture
                indices_traites[i] = true;  // Marque cet indice comme traité
                j++;  // Passe au caractère suivant du mot de passe trié
                break;  // Passe au caractère suivant du mot de passe trié
            }
        }
    }

    free(indices_traites);  // Libère la mémoire allouée pour les indices traités

    // Débogage:-----------------------------------------------------
    // Affichage du tableau transposé
    // print_mat(tab_transpose, nb_lignes, k);

    // Reconstruction du fichier principal à partir de la matrice transposée et suppression des fichiers enfants
    reconstruire_depuis_matrice(tab_transpose, nb_lignes, k,mdp, dirname, filename);

    // Libération de la mémoire allouée
    free(subfile);  // Libère la mémoire pour le nom du fichier enfant
    for (int i = 0; i < nb_lignes; i++) free(tab_transpose[i]);  // Libère chaque ligne du tableau
    free(tab_transpose);  // Libère le tableau principal
    free(mdp_t);  // Libère la mémoire pour le mot de passe trié

    return 1;  // Retourne 1 pour indiquer que l'opération a réussi
}



// Fonction principale
int main(int argc, char* argv[]) {
    // Vérifie si le nombre d'arguments est correct (entre 3 et 4)
    if ((argc < 3 || argc > 4 ) || (argc == 4 && strcmp(argv[1], "-d") != 0) || (argc == 3 && strcmp(argv[1], "-d") == 0) ){
        fprintf(stderr, "Usage: %s <fichier> <Mot de passe> [--pour la transposition]\n", argv[0]);
        fprintf(stderr, "Usage: %s [option: -d ] <fichier> <Mot de passe> [--pour la détransposition]\n", argv[0]);
        return 0;
    }

    // Cas de la détransposition (reconstruction du fichier à partir de la matrice transposée)
    if (argc == 4 && strcmp(argv[1], "-d") == 0) {
        char* path = argv[2];  // Le nom du fichier à reconstruire

        char* my_pass= argv[3];

        // Extraire le dirname et le filename
        char* dirname = NULL;
        char* filename = NULL;
        extract_file_and_dir(path, &dirname, &filename);

        //extraction de mot de passe a partir des fichier trouvés
        char* password = extract_password_from_filename(dirname,filename);

        if (password == NULL) {
            printf("Aucun fichier correspondant trouvé.\n");
            free(password);  // N'oubliez pas de libérer la mémoire après utilisation
            return 1;// Termine le programme avec un code d'erreur
        }else{
            if (strcmp(password, my_pass)!= 0){
                printf("Mot de passe incorect.\n");
                free(password);  // N'oubliez pas de libérer la mémoire après utilisation
                return 2;// Termine le programme avec un code d'erreur
            }
        }

        // Appel de la fonction de détransposition
        int result = detranspose(path, password);
        // Libération de la mémoire allouée pour le mot de passe
        free(password);
    }

    // Cas de la transposition (diviser un fichier en plusieurs fichiers enfants)
    else if (argc == 3 && strcmp(argv[1], "-d") != 0) {
        char* path = argv[1];  // Le fichier à transposer
        char* password = argv[2];  // Le mot de passe utilisé pour la transposition

        // Appel de la fonction de transposition
        transpose(path, password);
    }

    return 0;  // Fin du programme avec succès
}