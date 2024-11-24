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
    extern const Class _ListCell_;

    /**
     * @brief Fonction qui ajoute un objet à une position donnée dans la liste, à partir de cette cellule.
     *
     * @param self La cellule où la recherche de la bonne position va débuter.
     * @param object L'objet à ajouter dans la liste.
     * @param index la position où l'on doit ajouter l'objet.
     * @return 0 Si l'ojout a échoué, 1 sinon.
     */
    int ListCell_add(ListCell self, void* object, int index);

    /**
     * @brief Fonction qui va retirer un object à une position donnée, à partir de cette cellule.
     * 
     * @param self La cellule où la recherche de la bonne position à retirer va débuter.
     * @param index la position où l'on doit ajoute l'objet.
     * @param clone Booléen dont 1 permet d'indiquer à la fonction qu'on souhaite qu'elle nous renvoie 
     * une copie de l'objet après suppression. Pour rappel, l'objet sera détruit après sa suppression 
     * de la liste.
     * @return La valeur retirée (ou NULL si on ne souhaite pas la récupérer) si l'élément est bien retiré 
     * de la liste, NULL sinon.
     */
    void* ListCell_remove(ListCell self, int index, int clone);

    /**
     * @brief Fonction qui va rechercher une valeur à l'aide de l'indice de la cellule 
     * qui la contient, à partir de cette cellule. 
     *
     * @param self La cellule où la recherche va débuter.
     * @param clone Booléen dont 1 permet d'indiquer à la fonction qu'on souhaite qu'elle renvoie 
     * une copie de l'objet au lieux de la vraie référence.
     * 
     * @return La valeur qui se trouve à l'indice `index` à partir de cette cellule
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
     * @brief Fonction qui va renvoyer la valeur contenue dans la cellule
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
     * @brief Fonction qui remplace la cellule qui précède `self`.
     *
     * **ATTENTION** La cellule remplacée est imédiatement détruite 
     * avec sa valeur, mettre le paramètre `clone` à 1 pour renvoyer 
     * une copie de la valeur avant qu'elle soit détruite.
     * 
     * @param self La cellule dont on veut changer celle qui la précède.
     * @param new_prev La nouvelle cellule précédant `self`.
     * @param clone Booléen dont 1 permet d'indiquer à la fonction qu'on souhaite qu'elle renvoie 
     * une copie de la valeur qui se trouvait dans la cellule écrasée après suppression.
     * 
     * @return une copie de la valeur qui se trouvait dans la cellule écrasée ou NULL si pas 
     * de copie (`clone` mis à 0)
     */
    void* ListCell_replace_previous(const ListCell self, const ListCell new_prev, int clone);

    /**
     * @brief Fonction qui remplace la cellule qui suit `self`.
     * 
     * **ATTENTION** La cellule remplacée est imédiatement détruite 
     * avec sa valeur, mettre le paramètre `clone` à 1 pour renvoyer 
     * une copie de la valeur avant qu'elle soit détruite.
     * 
     * @param self La cellule dont on veut changer celle qui la suit.
     * @param new_prev La nouvelle cellule suivant `self`.
     * @param clone Booléen dont 1 permet d'indiquer à la fonction qu'on souhaite qu'elle renvoie 
     * une copie de la valeur qui se trouvait dans la cellule écrasée après suppression.
     * 
     * @return une copie de la valeur qui se trouvait dans la cellule écrasée ou NULL si pas 
     * de copie (`clone` mis à 0)
     */
    void* ListCell_replace_next(const ListCell self, const ListCell new_next, int clone);

    
    /**
     * @brief Fonction qui modifie la cellule qui précède `self`.
     * 
     * **ATTENTION** Risque de fuite de mémoire, rien ne sera désalloué. 
     * La fonction va seulement remplacer la référence à la cellule précédente 
     * par la nouvelle. 
     * 
     * @param self La cellule dont on veut changer celle qui la suit.
     * @param new_prev La nouvelle cellule suivant `self`.
     * @return L'ancienne cellule précédante de `self`.
     */
    ListCell ListCell_set_previous(const ListCell self, const ListCell new_next);
    
    /**
     * @brief Fonction qui modifie la cellule qui suit `self`.
     * 
     * **ATTENTION** Risque de fuite de mémoire, rien ne sera désalloué.
     * La fonction va seulement remplacer la référence à la cellule suivante 
     * par la nouvelle. 
     * 
     * @param self La cellule dont on veut changer celle qui la précède.
     * @param new_prev La nouvelle cellule précédant `self`.
     *
     * @return L'ancienne cellule suivante de `self`.
     */
    ListCell ListCell_set_next(const ListCell self, const ListCell new_next);


    /**
     * @brief Fonction qui compare la valeur contenue dans la cellule avec `value`.
     * 
     * La fonction va comparer les deux valeurs avec la méthode `obj_equals()`.
     * Si les deux valeurs sont différentes alors la fonction continue sur la cellule suivante.
     * 
     * @param self La cellule courante.
     * @param value La valeur à comparer.
     * @return 0 Si aucune cellule de la chaine ne contient `value`
     */
    int ListCell_contains(ListCell self, void* value);

    /**
     * @brief Fonction qui renvoie l'indice de la première occurence d'un élément 
     * qui se trouve dans la liste à partir de la cellule courrante.
     * 
     * La fonction va comparer les deux valeurs avec la méthode `obj_equals()`.
     * Si les deux valeurs sont différentes alors la fonction continue sur la cellule suivante.
     * 
     * @param self La cellule courante.
     * @param object L'élément à rechercher.
     * @param index L'indice de la cellule actuelle.
     * @return L'indice dans la liste de l'élément. -1 Si l'élément n'est pas dans la liste.
     * @see obj_equals()
     */
    long long int ListCell_index_of( ListCell self, void* value, long long int index );
#endif
