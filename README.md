# corbielle_broyeur_integration_Bash-C


Guide d'utilisation des scripts:

---------------------------------------------------------------------------------------------------------
1. Initialisation des scripts
    Pour initialiser les scripts, donnez-leur les droits d'exécution et supprimez les fichiers inutiles:

        bash:
            ./chmod u+x init-trashbox.sh
            ./chmod u+x sae_delete.sh
            ./chmod u+x sae_restore.sh
            ./chmod u+x sae_trashbox_ls.sh
ou bien:

       bash:
            ./chmod u+x *

---------------------------------------------------------------------------------------------------------
2. Suppression d'un fichier/dossier
    Pour supprimer un fichier ou un dossier, utilisez :
    bash:
        ./sae_delete.sh chemin_du_fichier [chemin_du_fichier_2 ...]

        Les fichiers/dossiers seront déplacés dans .sh-trashbox.
        Le fichier INDEX garde une trace des suppressions.

        Vous aurez besoin d'un mot de passe pour pouvoir supprimer des fichiers, afin d'assurer la sécurité des données.
        Ce mot de passe sera automatiquement demandé par le script après son exécution.


---------------------------------------------------------------------------------------------------------
3. Restauration d'un fichier/dossier
    Utilisez sae_restore.sh pour restaurer un élément depuis .sh-trashbox :

    Restaurer à l'emplacement d'origine :
        bash:
            ./sae_restore.sh -r nom_element

    Restaurer dans un dossier spécifique :
        bash:
            ./sae_restore.sh -d nom_dossier nom_element

    Restaurer dans le dossier courant :
        bash:
            ./sae_restore.sh nom_element
    
        Vous aurez aussi besoin de mot de passe utilisé pour la suppression pour pouvoir recupérer ces fichiers, afin d'assurer la sécurité des données.
        Ce mot de passe sera automatiquement demandé par le script après son exécution.

---------------------------------------------------------------------------------------------------------
4. Liste des éléments supprimés
    Pour afficher le contenu de la corbeille, utilisez :
        bash:
        ./sae_trashbox_ls.sh

---------------------------------------------------------------------------------------------------------
Note : Les scripts doivent être exécutés dans le dossier sae_broyeur (le dossier contenant .sh-trashbox).


---------------------------------------------------------------------------------------------------------
Vous trouverez ci-dessous un jeu d'instructions pour tester les codes :----------------------------------

tester avec le dossier:
                            ├── test
                            │   ├── d
                            │   │   ├── d1
                            │   │   │   ├── d2
                            │   │   │   │   └── f3
                            │   │   │   └── f2
                            │   │   └── f1
                            │   └── images
                            │       ├── d
                            │       └── img.jpg

----------------------------------------------------------------------------------------------------------
%./sae_delete.sh test/d/f1
Affiche:
        Le dossier .sh-trashbox a été créé avec succès.
        Les fichiers nécessaires ont été créés.
        -----------------------------------------------
        Veuillez saisir un mot de passe : 
        test
        Le fichier "f1" a été supprimé avec succès !
        -----------------------------------------------


%./sae_trashbox_ls.sh
Affiche:
        -----------------------------------------------
        Liste des fichiers supprimés :
        ------------------------------
        .sh-trashbox:
        f1 ----(est un fichier) : suprimé le : 2024/12/18/20:12:24

        -----------------------------------------------


%tree test
Affiche:
        test
        ├── d
        │   └── d1
        │       ├── d2
        │       │   └── f3
        │       └── f2
        └── images
            ├── d
            └── img.jpg


%./sae_delete.sh test/d
Affiche:
        -----------------------------------------------
        Veuillez saisir un mot de passe : 
        test
        Le dossier "d" a été supprimé avec succès !
        -----------------------------------------------


%tree test
Affiche:
        test
        └── images
            ├── d
            └── img.jpg


%./sae_delete.sh test
Affiche:
        -----------------------------------------------
        Veuillez saisir un mot de passe : 
        test
        Le dossier "test" a été supprimé avec succès !
        -----------------------------------------------


%tree test
Affiche:
        test  [error opening dir]

        0 directories, 0 files
        

%./sae_trashbox_ls.sh 
Affiche:
        -----------------------------------------------
        Liste des fichiers supprimés :
        ------------------------------
        .sh-trashbox:
        f1 ----(est un fichier) : suprimé le : 2024/12/18/20:12:24

        d ----(est un dossier) : suprimé le : 2024/12/18/20:15:03
        |__   d1 ----(est un dossier) : suprimé le : 2024/12/18/20:15:03
        |__    |__   d2 ----(est un dossier) : suprimé le : 2024/12/18/20:15:03
        |__    |__    |__   f3 ----(est un fichier) : suprimé le : 2024/12/18/20:15:03
        |__    |__   f2 ----(est un fichier) : suprimé le : 2024/12/18/20:15:03

        test ----(est un dossier) : suprimé le : 2024/12/18/20:16:45
        |__   images ----(est un dossier) : suprimé le : 2024/12/18/20:16:45
        |__    |__   d ----(est un dossier) : suprimé le : 2024/12/18/20:16:45
        |__    |__   img.jpg ----(est un fichier) : suprimé le : 2024/12/18/20:16:45

        -----------------------------------------------




-----------------------------------------------
%mkdir test2
%./sae_restore.sh -d test2 f1
Affiche:
        -----------------------------------------------
        Veuillez saisir un mot de passe : 
        test
        Le fichier "f1" a été récupéré avec succès !
        -----------------------------------------------


%tree test2
Affiche:
        test2
        └── f1

        0 directories, 1 file



%mkdir test2
%./sae_restore.sh -d test2  d/d1/f2
Affiche:
        -----------------------------------------------
        Veuillez saisir un mot de passe : 
        test
        Le fichier "f2" a été récupéré avec succès !
        -----------------------------------------------


%tree test2
Affiche:
        ├── test2
        │   ├── f1
        │   └── f2

%./sae_restore.sh -r images/img.jpg
Affiche:
        -----------------------------------------------
        Veuillez saisir un mot de passe : 
        test
        Le fichier "img.jpg" a été récupéré avec succès !
        -----------------------------------------------

%tree images
Affiche:
        images
        └── img.jpg
