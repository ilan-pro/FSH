/**
 * @file ListCell.h
 * @brief Fichier d'en-tête contenant les déclarations de toutes les méthodes utilisées pour manipuler les objets de la classe ListCell.
 */
#ifndef LISTCELL_H
#define LISTCELL_H
#include "Class.h"
#ifdef LISTCELL_CLASS
    
    #define LISTCELL_FIELDS \
        void* value; \
        ListCell next; \
        ListCell previous;

    typedef struct _ListCell_* ListCell;

#else

    /**
     * @cond HIDE_THIS // Pas inclu dans la documentation à générer.
     *
     * Attribut privés des objets de type ListCell.
     *
     * On les déclrare ici pour permettre de reserver un espace mémoire vide (qui ne contient aucune information utile)
     * afin que le deux structures contenant les attributs de classe (celle visible publiquement ici et celle qui 
     * contient les champs privés visibles uniquement dans le fichiers source) aient la même taille.
     */
    typedef struct __list_cell_prv{
        void* value;
        void* next; // Même taille que ListCell qui est aussi un pointeur.
        void* previous; // Même taille que ListCell qui est aussi un pointeur.
    }* __list_cell_private_fields;
    
    /**
     * Attribut des objets de type ListCell.
     * 
     * Il s'agit des attributs de la structure contenant les champs de l'objet.
     * Ces attributs seront visibles pour tous ceux qui "include" cette en-tête.
     *
     * Le tableau `const char _[sizeof(__list_cell_private_fields)];` contiendera tous les champs 
     * que l'objet aura rendu privé, ce pointeur n'est là que pour que les deux structures 
     * contenant les attributs de classe (celle visible publiquement ici et celle qui
     * contient les champs privés visibles uniquement dans le fichiers source) aient la même taille.
     */
    #define LISTCELL_H_FIELDS \
        const char _[sizeof(__list_cell_private_fields)];
    
    /**@endcond*/
    
    /**
     * @internal
     * Classe `ListCell` des cellules d'une liste doublement chainée.
     *
     * Cette définition est celle que tous les fichiers sources 
     * qui utiliseront `ListCell.h` verront (ils ne pourront pas accéder aux champs 
     * privés).
     * 
     * Voir suite de la documentation dans `ListCell.c`
     *
     * Construction : obj_new( _ListCell_, void* value );
     */
    typedef struct _ListCell_{
        Class _ListCell_;
        LISTCELL_H_FIELDS
    }* ListCell ;   

#endif

    /**
     * @internal
     * @brief Classe de tous les objets de type ListCell.
     *
     * Elle est définie dans `ListCell.c`
     */
    extern Class _ListCell_;

    /**
     * @brief Fonction qui ajoute un object à une position donnée dans la liste, à partir de cette cellule.
     *
     * @param self La cellule où la recherche de la bonne position va débuter.
     * @param object L'objet à ajouter dans la liste.
     * @param index la position où l'on doit ajoute l'objet.
     * @return La dernière cellule de la chaine.
     */
    int ListCell_add(ListCell self, void* object, int index);

    /**
     * @brief Fonction qui va retirer un object à une position donnée, à partir de cette cellule.
     * 
     * @param self La cellule où la recherche de la bonne position àr retirer va débuter.
     * @param index la position où l'on doit ajoute l'objet.
     * @param clone Booléen dont 1 permet d'indiquer à la fonction qu'on souhaite qu'elle nous renvoie 
     * une copie de l'objet après suppression. Pour rappel, l'objet sera détruit après sa suppression 
     * de la liste.
     * @return La dernière cellule de la chaine.
     */
    void* ListCell_remove(ListCell self, int index, int clone);

    /**
     * @brief Fonction qui va rechercher une valeur à l'aide de l'indice de la cellule 
     * qui la contient, à partir de cette cellule. 
     *
     * @param self La cellule où la recherche va débuter.
     * @param clone Booléen dont 1 permet d'indiquer à la fonction qu'on souhaite qu'elle renvoie 
     * une copie de l'objet après au lieux de la vraie référence.
     * 
     * @return La dernière cellule de la chaine.
     */
    void* ListCell_get(const ListCell self, int index, int clone);

    /**
     * @brief Fonction qui va rechercher à partir de cette cellule le dernier 
     * élément de la chaine.
     *
     * @param self_ La cellule où la recherche va débuter.
     * @return La dernière cellule de la chaine.
     */
    ListCell ListCell_get_last(const ListCell self_);

    /**
     * @brief Fonction qui va renvoyer la valeur contenu dans la cellule
     *
     * @param self La cellule dont on cherche la valeur.
     * @return La valeur stockée dans la cellule.
     */
    void* ListCell_get_value(const ListCell self);
        
    /**
     * @brief Fonction qui renvoie la cellule qui précède `self`.
     *
     * @param self La cellule dont on cherche celle qui la précède.
     * @return La cellule précédant `self`.
     */
    ListCell ListCell_get_previous(const ListCell self);

    /**
     * @brief Fonction qui renvoie la cellule qui suit `self`.
     *
     * @param self La cellule dont on cherche celle qui la suit.
     * @return La cellule suivant `self`.
     */ 
    ListCell ListCell_get_next(const ListCell self);

    /**
     * @brief Fonction qui modifie la cellule qui précède `self`.
     *
     * @param self La cellule dont on veut changer celle qui la précède.
     * @param La nouvelle cellule précédant `self`.
     *
     * @return L'ancienne cellule précédant `self`.
     */
    ListCell ListCell_set_previous(const ListCell self, const ListCell new_prev);

    /**
     * @brief Fonction qui modifie la cellule qui suit `self`.
     * 
     * @param self La cellule dont on veut modifier celle qui la suit.
     * @param La nouvelle cellule suivant `self`.
     *
     * @return L'ancienne cellule qui suivait `self`.
     */
    ListCell ListCell_set_next(const ListCell self, const ListCell new_next);

#endif
