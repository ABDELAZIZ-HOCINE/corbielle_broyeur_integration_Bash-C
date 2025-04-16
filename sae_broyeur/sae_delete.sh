#!/bin/bash

#sae_delete.sh------------------------------------------------------

#Vérifier si le script est exécuté dans le dossier spécifié
working_directory=sae_broyeur
if ! pwd | grep $working_directory > /dev/null 2>&1 ; then
  echo "Vous n'êtes pas dans le dossier $working_directory !!";
  exit 1
fi

#Donner les permissions d'exécution au script init-trashbox.sh et l'exécuter
chmod u+x init-trashbox.sh
./init-trashbox.sh

#Vérifier si au moins un argument est fourni
if [ $# -lt 1 ]; then
    echo "usage: $0 nom_element"
    exit 1 
fi
echo "-----------------------------------------------"

encode_items() {
    local path="$1"
    local mdp="$2"

    if [ -d "$path" ]; then
        # Si c'est un dossier, parcourir ses éléments
        for item in "$path"/*; do
            if [ -d "$item" ]; then
                # Si c'est un sous-dossier, appel récursif
                encode_items "$item" "$mdp"
            elif [ -f "$item" ]; then
                # Si c'est un fichier
                uuencode "$item" "$item" > "$item".uu
                mv "$item".uu "$item"

                gcc -c transpose_mdp.c -o transpose_mdp.o
                gcc -o transpose_mdp transpose_mdp.o
                ./transpose_mdp "$item" "$mdp"
                rm -f transpose_mdp.o transpose_mdp
            fi
        done
    else
        # Si c'est un fichier
        uuencode "$path" "$path" > "$path".uu
        mv "$path".uu "$path"

        gcc -c transpose_mdp.c -o transpose_mdp.o
        gcc -o transpose_mdp transpose_mdp.o
        ./transpose_mdp "$path" "$mdp"
        rm -f transpose_mdp.o transpose_mdp
    fi
}

crush_items(){
    local path=$1
    if [ -d $path ];then
        for item in "$path"/*;do
            if [ -d $item ];then
                crush_items $item
            elif [ -f $item ];then
                echo " "> "$item"
            fi
        done
    elif [ -f $path ];then
        echo " "> "$path"
    fi
}

#Créer une boucle pour traiter les éléments passés en paramètres
for item in "$@"; do
    find . -type f -name '*Zone*' -exec rm -f {} \;
    #Vérifier si le chemin existe
    if [ -e "$item" ]; then
        #Extraire le nom de l'élément sans le chemin du répertoire parent
        FILENAME=$(echo "$item" | sed "s|$(dirname "$item")/||")
        #Déterminer l'adressage complet de l'élément à supprimer
        if [ "$FILENAME" == "$(echo "$item" | sed "s|\./||")" ]; then
            DIRNAME="$(dirname "$item")"
            FULL_PATH="$DIRNAME/$FILENAME"
            FULL_PATH_b="$DIRNAME/:$FILENAME"

        else
            DIRNAME="./$(dirname "$(echo "$item" | sed "s|\./||") ")"
            FULL_PATH="$DIRNAME/$FILENAME"
            FULL_PATH_b="$DIRNAME/:$FILENAME"
        fi

        #Vérifier si l'élément est un dossier ou un fichier
        if [ -d "$FULL_PATH" ]; then
            item_type='d'  
        elif [ -f "$FULL_PATH" ]; then
            item_type='f'
        fi
        
        #Lire le numéro d'ordre actuel depuis .sh-trashbox/ID
        order_number=$(cat ".sh-trashbox/ID" | head -n 1)

        #Déplacer l'élément vers .sh-trashbox sous le nom correspondant à order_number et en l'encode  avec uuencode
        mv -f "$FULL_PATH" ".sh-trashbox/$order_number"
        #echo $FULL_PATH

        #saisir le mot de passe
        echo "Veuillez saisir un mot de passe : "
        read password
        #Vérifier si le déplacement a été réussi
        if [ $? -eq 0 ]; then
            #Afficher un message en fonction du type de l'élément
            if [ "$item_type" == 'd' ]; then
                echo "Le dossier \"$FILENAME\" a été supprimé avec succès !"
            elif [ "$item_type" == 'f' ]; then
                echo "Le fichier \"$FILENAME\" a été supprimé avec succès !"
            fi

            #Ajouter une référence de l'élément supprimé dans INDEX
            echo "$order_number:/$FULL_PATH_b:($(date +"%Y/%m/%d/%H:%M:%S"))" >> .sh-trashbox/INDEX
            #Incrémenter le numéro d'ordre pour la prochaine suppression
            echo "$((order_number+1))" > .sh-trashbox/ID

                    #Débogage (optionnel, à activer si nécessaire)
                    #echo "-----------------------------------------------"
                    #Afficher l'adressage précédent et le chemin complet
                    #echo "L'adressage précédent est : $DIRNAME"
                    #echo "Chemin complet : $FULL_PATH"
                    #Afficher le numéro d'ordre
                    #echo "Le numéro d'ordre est : $order_number"
                    #Afficher le prochain numéro d'ordre
                    #echo "Prochain ordre : $(cat .sh-trashbox/ID)"  
                    #echo "-----------------------------------------------"

        fi
        #echo "Transposition en cours..."
        cp -r ".sh-trashbox/$order_number" ".sh-trashbox/.crusher/$order_number"
        crush_items ".sh-trashbox/$order_number"
        encode_items ".sh-trashbox/.crusher/$order_number" "$password"
        #echo "Transposition terminée avec succès."
        echo "-----------------------------------------------"
    else
        #Afficher un message d'erreur si l'élément n'existe pas
        echo "L'élément \"$item\" n'existe pas"
        echo "-----------------------------------------------"
        #Passer à l'argument suivant si l'élément n'est pas supprimé
        shift  
    fi
done