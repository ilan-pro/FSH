/**
 * @file LinkedList.h
 * @brief Fichier d'en-tête contenant les déclarations de toutes les méthodes 
 * utilisées pour manipuler les objets de la classe LinkedList.
 *
 * Les LinkedList sont des objets représantant des structures de données de type 
 * liste doublement chainée.<br>Le type LinkedList est générique c'est à dire qu'il 
 * peut contenir n'importe quel type d'objet. On peut même mélanger différents
 * types d'objets dans la même liste.
 * <p>
 * 
 * La liste des méthodes agissant sur les listes doublement chainée n'est pas exhaustive, 
 * mais elle est suffisante pour que la classe soit utilisée dans le projet.
 */
#include "ListCell.h"
#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#ifdef LINKEDLIST_CLASS
    
    /**
     *
     * Il s'agit des champs d'un objet de type LinkedList.
     * <p>
     * Cette macro n'est visible que sur des sources dont la définition LINKEDLIST_CLASS est faite.
     * Cela permet d'avoir une encasulation des attributs en les rendant privés dans toute implémentation 
     * ne possèdant pas la décrlatation LINKEDLIST_CLASS.
     * <p>
     * Cela permet également pour l'héritage de classe, de récrire PARENT_FIELD (Ici LINKEDLIST_FIELDS)
     * pour inclure les arrtibuts du parents dans ceux de l'enfant.
     */
    #define LINKEDLIST_FIELDS \
        size_t length; \
        ListCell first; \
        ListCell last; 
    
    /**
     * L'implémentation de struct _LinkedList_ est dans 
     * `LinkedList.c`
     */
    typedef struct _LinkedList_* LinkedList;

#else

    /**
     * @cond HIDE_THIS // Pas inclu dans la documentation à générer.
     *
     * Attribut privés des objets de type LinkedList.
     *
     * On les déclrare ici pour permettre de reserver un espace mémoire vide (qui ne contient aucune information utile)
     * afin que le deux structures contenant les attributs de classe (celle visible publiquement ici et celle qui 
     * contient les champs privés visibles uniquement dans le fichiers source) aient la même taille.
     */
    typedef struct _linkedlist_private{
    } __linkedlist_private_field;
    
    /**
     * Attribut des objets de type LinkedList.
     * 
     * Il s'agit des attributs de la structure contenant les champs de l'objet.
     * Ces attributs seront visibles pour tous ceux qui "include" cette en-tête.
     *
     * Le tableau `const char _[sizeof(__linkedlist_private_field)];` contiendera tous les champs 
     * que l'objet aura rendu privé, ce pointeur n'est là que pour que les deux structures 
     * contenant les attributs de classe (celle visible publiquement ici et celle qui
     * contient les champs privés visibles uniquement dans le fichiers source) aient la même taille.
     */
    #define LINKEDLIST_H_FIELDS \
        const char _[sizeof(__linkedlist_private_field)];
    
    /**@endcond*/

    /**
     * @internal
     * @brief Classe `LinkedList` des listes doublement chainées.
     *
     * Cette définition est celle que tous les fichiers sources 
     * qui utiliseront `LinkedList.h` verront (ils ne pourront pas accéder aux champs 
     * privés).
     * 
     * Voir suite de la documentation dans `LinkedList.c`
     *
     * Construction : obj_new( _LinkedList_);
     */
    typedef struct _LinkedList_{
        Class _LinkedList_;
        LINKEDLIST_H_FIELDS
    }* LinkedList;

#endif
    
    /**
     * @internal
     * Classe de tous les objets de type LinkedList.
     *
     * Elle est définie dans `LinkedList.c`
     */
    extern Class _LinkedList_;

        
    /**
     * @brief Fonction qui renvoie la taille de la liste.
     *
     * Cette fonction renvoie la taille de la liste, elle peut être appelé pour tout 
     * objet de type `_LinkedList_` ou sous-type de `_LinkedList_`.
     *
     * <p>La récupération de la taille se fait en temps constant O(1).
     *
     * @param self_ L'objet de type `LinkedList` (ou sous-type).
     * @return La taille de la liste.
     */
    size_t LinkedList_size(LinkedList self_);
        
    /**
     * @brief Fonction qui va rajouter un élément en tête de liste.
     *
     * **ATTENTION** : Au moment de la destruction de la liste les valeurs 
     * sont également désallouées. Faire une copie (avec `obj_clone()`) (ou 
     * mettre le paramètre `clone` à 1 pour que la fonction le face)
     * de l'objet avant de l'ajouter dans la liste, si on tient à garder l'objet 
     * intact après la destruction de la liste.
     * <p>Si on souhaite insérer 2 fois un élément (avec la même référence) dans la 
     * liste il est important de mettre `clone` à 1 pour que la fonction puisse 
     * clonner l'objet car au moment de la destruction il sera supprimé deux fois 
     * ce qui va générer une erreur.
     *  
     * <p>
     * La fonction peut être appelée pour tout objet de type `_LinkedList_` 
     * ou sous-type de `_LinkedList_`.
     *
     * <p>Chaque ajout se fait en temps constant O(1).
     *
     * @param self_ La liste dans laquelle on souhaite ajouter l'objet.
     * @param object_ L'objet à insérer dans la liste (copier l'objet si on 
     * souhaite le garder après la destruction de la liste).
     *
     * @param clone Boolean dont 1 permet d'indiquer à la fonction si on souhaite qu'elle 
     * ajoute une copie de l'objet (copié avec `obj_clone()`) plutôt que l'objet lui-même. 
     * Cela va permettre de garentir que l'objet ne sera pas détruit avec la liste. Mettre 
     * à 0 sinon.
     *
     * @return 0 si l'ajout à échoué, 1 sinon.
     * @throw Abort Le programme s'arrête si l'objet à ajouter n'est pas du même type 
     * (ou sous-type) que les autres.
     */
    int LinkedList_add_first(LinkedList self_, void* object_, int clone);
    
    /**
     * @brief Fonction qui va rajouter un élément en fin de liste.
     *
     * **ATTENTION** : Au moment de la destruction de la liste les valeurs 
     * sont également désallouées. Faire une copie (avec `obj_clone()`) (ou 
     * mettre le paramètre `clone` à 1 pour que la fonction le fasse)
     * de l'objet avant de l'ajouter dans la liste, si on tient à garder l'objet 
     * intact après la destruction de la liste.
     * <p>Si on souhaite insérer 2 fois un élément (avec la même référence) dans la 
     * liste il est important de mettre `clone` à 1 pour que la fonction puisse 
     * clonner l'objet car au moment de la destruction il sera supprimé deux fois 
     * ce qui va générer une erreur.
     * 
     * <p>
     * La fonction peut être appelée pour tout objet de type `_LinkedList_` 
     * ou sous-type de `_LinkedList_`.
     *
     * <p>Chaque ajout se fait en temps constant O(1).
     *  
     * @param self_ La liste dans laquelle on souhaite ajouter l'objet.
     * @param object_ L'objet à insérer dans la liste (copier l'objet si on 
     * souhaite le garder après la destruction de la liste).
     *
     * @param clone Boolean dont 1 permet d'indiquer à la fonction si on souhaite qu'elle 
     * ajoute une copie de l'objet (copié avec `obj_clone()`) plutôt que l'objet lui-même. 
     * Cela va permettre de garentir que l'objet ne sera pas détruit avec la liste. Mettre 
     * à 0 sinon.
     *
     * @return 0 si l'ajout à échoué, 1 sinon.
     * 
     * @throw Abort Le programme s'arrête si l'objet à ajouter n'est pas du même type 
     * (ou sous-type) que les autres.
     */
    int LinkedList_add_last(LinkedList self, void* object_, int clone);
    
    /**
     * @brief Cette fonction va retirer le premier élément de la liste.
     *
     * Si la liste est vide alors la fonction va renvoyer NULL.
     *
     * ** Important ** : La cellule qui sera retirée sera immédiatement détruite (appelle à 
     * son destructeur). Ne pas rajouter d'objet dont on souhaite garder la référence après 
     * son retrait dans la liste.
     *
     *<p>
     * La fonction peut être appelée pour tout objet de type `_LinkedList_` 
     * ou sous-type de `_LinkedList_`.
     * 
     * <p>Chaque suppression se fait en temps constant O(1).
     * 
     * @param self La liste dont on souhaite retirer le premier élément.
     * @param clone Booléen dont 1 permet d'indiquer à la fonction qu'on souhaite qu'elle nous renvoie 
     * une copie de l'objet après suppression. Pour rappel, l'objet sera détruit après sa suppression 
     * de la liste.
     * 
     * @return La valeur retirée (ou NULL si on ne souhaite pas la récupérer) Si le premier 
     * élément est bien retiré de la liste, NULL sinon.
     */
    void* LinkedList_remove_first(LinkedList self, int clone);

    /**
     * @brief Cette fonction va retirer le dernier élément de la liste.
     *
     * Si la liste est vide alors la fonction va renvoyer NULL.
     *
     * ** Important ** : La cellule qui sera retirée sera immédiatement détruite (appelle à 
     * son destructeur). Ne pas rajouter d'objet dont on souhaite garder la référence après 
     * son retrait dans la liste.
     *<p>
     * La fonction peut être appelée pour tout objet de type `_LinkedList_` 
     * ou sous-type de `_LinkedList_`.
     *
     * <p>Chaque suppression se fait en temps constant O(1).
     * 
     * @param self La liste dont on souhaite retirer le dernier élément.
     * @param clone Booléen dont 1 permet d'indiquer à la fonction qu'on souhaite qu'elle nous renvoie 
     * une copie de l'objet après suppression. Pour rappel, l'objet sera détruit après sa suppression 
     * de la liste.
     * 
     * @return La valeur retirée (ou NULL si on ne souhaite pas la récupérer) Si le dernier
     * élément est bien retiré de la liste, NULL sinon.
     */
    void* LinkedList_remove_last(LinkedList self, int clone);
    
    /**
     * @brief Fonction qui renvoie la valeur du premier élément de la liste.
     * 
     * Mettre le paramètre `clone` à 1 pour renvoyer une copie au lieu de la 
     * la vraie valeur.
     * 
     *<p>
     * La fonction peut être appelée pour tout objet de type `_LinkedList_` 
     * ou sous-type de `_LinkedList_`.
     * 
     * @param self La liste dont on cherche le premier élément.
     * @param clone Booléen dont 1 permet d'indiquer à la fonction qu'on souhaite qu'elle renvoie 
     * une copie de l'objet au lieu de la vraie référence. Au cas où on souhaite la récupérer 
     * avant la destruction de la liste.
     * 
     * @return La valeur du premier élément ou NULL si la liste est vide.
     */
    void* LinkedList_get_first(LinkedList self, int clone);

    /**
     * @brief Fonction qui renvoie la valeur du dernier élément de la liste.
     * 
     * Mettre le paramètre `clone` à 1 pour renvoyer une copie au lieu de la 
     * la vraie valeur.
     * 
     *<p>
     * La fonction peut être appelée pour tout objet de type `_LinkedList_` 
     * ou sous-type de `_LinkedList_`.
     * 
     * @param self La liste dont on cherche le dernier élément.
     * @param clone Booléen dont 1 permet d'indiquer à la fonction qu'on souhaite qu'elle renvoie 
     * une copie de l'objet au lieu de la vraie référence. Au cas où on souhaite la récupérer 
     * avant la destruction de la liste.
     * 
     * @return La valeur du dernier élément ou NULL si la liste est vide.
     */
    void* LinkedList_get_last(LinkedList self, int clone);

    /**
     * @brief Fonction qui renvoie la valeur d' élément de la liste qui se trouve à une position donnée.
     * 
     * Mettre le paramètre `clone` à 1 pour renvoyer une copie au lieu de la 
     * la vraie valeur.
     * 
     * <p>Une recherche ne se fait pas en temps constant, O(index).
     *<p>
     * La fonction peut être appelée pour tout objet de type `_LinkedList_` 
     * ou sous-type de `_LinkedList_`.
     * 
     * @param self La liste dont on cherche l' élément.
     * @param index La position de l'élément.
     * @param clone Booléen dont 1 permet d'indiquer à la fonction qu'on souhaite qu'elle renvoie 
     * une copie de l'objet au lieu de la vraie référence. Au cas où on souhaite la récupérer 
     * avant la destruction de la liste.
     * 
     * @return La valeur de l'élément ou NULL si la liste est vide.
     */
    void* LinkedList_get(LinkedList self, int index,  int clone);

    /**
     * @brief Fonction qui ajoute un élément à une position précise dans la liste.
     * 
     * Mettre le paramètre `clone` à 1 pour renvoyer une copie au lieu de la 
     * la vraie valeur.
     * 
     * **ATTENTION** : Au moment de la destruction de la liste les valeurs 
     * sont également désallouées. Faire une copie (avec `obj_clone()`) (ou 
     * mettre le paramètre `clone` à 1 pour que la fonction le fasse)
     * de l'objet avant de l'ajouter dans la liste, si on tient à garder l'objet 
     * intact après la destruction de la liste.
     * <p>Si on souhaite insérer 2 fois un élément (avec la même référence) dans la 
     * liste il est important de mettre `clone` à 1 pour que la fonction puisse 
     * clonner l'objet car au moment de la destruction il sera supprimé deux fois 
     * ce qui va générer une erreur.
     * 
     * <p>
     * La fonction peut être appelée pour tout objet de type `_LinkedList_` 
     * ou sous-type de `_LinkedList_`.
     *
     * <p>Chaque ajout ne se fait pas en temps constant O(index).
     * 
     * @param self La liste dont on cherche le dernier élément.
     * @param object_ L'objet à insérer dans la liste (copier l'objet si on 
     * souhaite le garder après la destruction de la liste).
     * @param index Position dans laquelle on souhaite insérer l'objet dans la liste.
     * @param clone Boolean dont 1 permet d'indiquer à la fonction si on souhaite qu'elle 
     * ajoute une copie de l'objet (copié avec `obj_clone()`) plutôt que l'objet lui-même. 
     * Cela va permettre de garentir que l'objet ne sera pas détruit avec la liste. Mettre 
     * à 0 sinon.
     * 
     * @return 0 Si l'ajout a échoué 1 sinon.
     */
    int LinkedList_add( LinkedList self, void* object, int index, int clone );

    /**
     * @brief Fonction qui va retirer un élément de la liste qui se trouve à une position donnée.
     *
     * Si la liste est vide alors la fonction va renvoyer NULL.
     *
     * ** Important ** : La cellule qui sera retirée sera immédiatement détruite (appelle à 
     * son destructeur). Ne pas rajouter d'objet dont on souhaite garder la référence après 
     * son retrait dans la liste.
     *<p>
     * La fonction peut être appelée pour tout objet de type `_LinkedList_` 
     * ou sous-type de `_LinkedList_`.
     *
     * <p>Chaque suppression se fait en temps constant O(1).
     * 
     * @param self La liste dont on souhaite retirer le dernier élément.
     * @param clone Booléen dont 1 permet d'indiquer à la fonction qu'on souhaite qu'elle nous renvoie 
     * une copie de l'objet après suppression. Pour rappel, l'objet sera détruit après sa suppression 
     * de la liste.
     * 
     * @return La valeur retirée (ou NULL si on ne souhaite pas la récupérer) Si le dernier
     * élément est bien retiré de la liste, NULL sinon.
     */
    void* LinkedList_remove( LinkedList self, int index, int clone );

    /**
     * @brief Fonction qui vérifie si oui ou non un élément se trouve dans la liste.
     * 
     * La fonction va comparer les éléments avec la méthode `obj_equals()`, qui va appeler 
     * dynamiquement sa redéfinition sur les objets de la liste (si redefinie).
     * 
     * 
     * @param self La liste à parcourir.
     * @param object L'élément à rechercher.
     * @return 1 Si l'élément se trouve dans la liste et 0 sinon.
     * @see obj_equals()
     */
    int LinkedList_contains( LinkedList self, void* object );

    /**
     * @brief Fonction qui renvoie l'indice de la première occurence d'un élément 
     * qui se trouve dans la liste.
     * 
     * La fonction va comparer les éléments avec la méthode `obj_equals()`, qui va appeler 
     * dynamiquement sa redéfinition sur les objets de la liste (si redefinie).
     * 
     * 
     * @param self La liste à parcourir.
     * @param object L'élément à rechercher.
     * @return L'indice dans la liste de l'élément. -1 Si l'élément n'est pas dans la liste.
     * @see obj_equals()
     */
    long long int LinkedList_index_of( LinkedList self, void* object);

#endif








