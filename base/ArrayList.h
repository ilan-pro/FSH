/**
 * @file ArrayList.h
 * @brief Fichier d'en-tête contenant les déclarations des variables publiques et de toutes les méthodes public de la classe `_ArrayList_`.
 *
 * Une `ArrayList` est un tableau dynamique qui dont la taille augmente au fur et à mesure que l'on ajoute des éléments à l'intérieur. 
 * Cette structure de donnée est idéale pour récupérer les élements qui y sont stockés en temps constant (bien que la suppression d'un 
 * élément et l'ajout d'élément ailleurs qu'à la fin ne le soit pas).
 * <br>
 * Tous les objets peuvent êtres de type différent.
 * <br>
 * 
 * Construction <code><strong> obj_new( `_ArrayList_`, int initial_size ) </strong></code>
 * où : <ul>
 *          <li> `_ArrayList_` désigne la classe des objets de type `ArrayList`.</li>
 *          <li> <code><strong> initial_size </strong></code> désigne la taille initiale de la liste. Idéale si on a l'intention de stocker une grande quantité de donnée sans trop redimensionner.</li>
 *      </ul>
 */

#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include "Class.h"
#ifdef ARRAY_LIST_CLASS 

    #define ARRAY_LIST_FIELDS \
        size_t size; /** Il s'agit de la taille de la liste. */ \
        size_t last_index; /** IL s'agit de de l'indice de la première place liste dans la liste. Il permet de déterminer le nombre d'élément dans la liste, ainsi que la saturation de la liste. */ \
        void** dyn_array; /** Il s'agit du tableau dynamique qui va contenir les éléments. Il augemente de 50% à chaque fois qu'il est plein. */ \
        \
        /**  Booléen indiquant si on doit réduire la liste ou pas lorsqu'on détecte que la taille réel est trop petite que la taille en mémoire 
         *   Cela va permettre de créer une ArrayList avec initialement une grande taille (pour éviter de redimentionnement pendant l'ajout 
         *   si on ajoute une grande quantité de données), sans que la liste ne soit réduite (car si on détecte que la taille réel de la liste 
         *   est torp petite que sa taille en mémoire, on réduit sa taille en mémoire pour libérer de l'espace) à cause sa taille réel qui sera 
         *   très petite à l'initialisation.
         *
         *   En revanche à partir du moment où la liste à été saturée au moins une fois, ce champs est remis à 1 donc une fois la liste remplie 
         *   pour la première fois, on considère que si après plusieurs suppression la taille est de nouveau très faible alors on peut réduire la 
         *   taille de la liste.
         * */ \
        int reduce; 
    
    typedef struct _ArrayList_* ArrayList;
    
#else
    
    /**@cond HIDE_THIS */
    typedef struct __array_list_prv {
        size_t size;
        void** dyn_array; 
    } __array_list_prv;

    #define ARRAY_LIST_H_FIELDS \
        const char _arr_list_prv[sizeof(struct __array_list_prv )];
    /**@endcond*/    
    
    /**
     * @brief Classe des objets de type `_ArrayList_`.
     *
     * Une `ArrayList` est un tableau dynamique qui dont la taille augmente au fur et à mesure que l'on ajoute des éléments à l'intérieur. 
     * Cette structure de donnée est idéale pour récupérer les élements qui y sont stockés en temps constant (bien que la suppression d'un 
     * élément et l'ajout d'élément ailleurs qu'à la fin ne le soit pas).
     * <br>
     * <br>
     * Tous les objets peuvent êtres de type différent.
     * <br>
     * 
     * Construction <code><strong> obj_new( `_ArrayList_`, int initial_size ) </strong></code>
     * où : <ul>
     *          <li> `_ArrayList_` désigne la classe des objets de type `ArrayList`.</li>
     *          <li> <code><strong> initial_size </strong></code> désigne la taille initiale de la liste. 
     *               Idéale si on a l'intention de stocker une grande quantité de donnée sans trop redimensionner.</li>
     *      </ul>
     */
    typedef struct _ArrayList_{
        /**
         * @brief Classe des objets de type `_ArrayList_`.
         * 
         * Contient le constructeur, destructeur, ainsi que toutes les fonctions manipulant des objets de manière général. 
         * <br>Ces fonctions sont appelées par liaison dynamique.
         * <br>
         *
         * Construction <code><strong> obj_new( `_ArrayList_`, int initial_size ) </strong></code>
         * où : <ul>
         *          <li> `_ArrayList_` désigne la classe des objets de type `ArrayList`.</li>
         *          <li> <code><strong> initial_size </strong></code> désigne la taille initiale de la liste. 
         *               Idéale si on a l'intention de stocker une grande quantité de donnée sans trop redimensionner.</li>
         *      </ul>
         *
         *  @see _Class_
         *  @see _ArrayList_
         */
        Class _ArrayList_;
        ARRAY_LIST_H_FIELDS
    }* ArrayList;
    

#endif 

     
    /**
     * @internal
     * Classe de tous les objets de type ArrayList.
     *
     * Elle est définie dans `ArrayList.c`
     */
    extern const Class _ArrayList_;

        
    /**
     * @brief Fonction qui renvoie la taille de la liste.
     *
     * Cette fonction renvoie la taille de la liste, elle peut être appelé pour tout 
     * objet de type `_ArrayList_` ou sous-type de `_ArrayList_`.
     *
     * <p>La récupération de la taille se fait en temps constant O(1).
     *
     * @param self_ L'objet de type `ArrayList` (ou sous-type).
     * @return La taille de la liste.
     */
    size_t ArrayList_size( ArrayList self );
        
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
     * La fonction peut être appelée pour tout objet de type `_ArrayList_` 
     * ou sous-type de `_ArrayList_`.
     *
     * <p>L'ajout d'un élément en tête de liste est θ(n)
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
     */
    int ArrayList_add_first( ArrayList self, void* object_, int clone );
    
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
     * La fonction peut être appelée pour tout objet de type `_ArrayList_` 
     * ou sous-type de `_ArrayList_`.
     *
     * <p> Chaque ajout en fin de liste se fait en temps constant O(1).
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
     */
    int ArrayList_add_last(ArrayList self, void* object_, int clone);
    
    /**
     * @brief Cette fonction va retirer le premier élément de la liste.
     *
     * Si la liste est vide alors la fonction va renvoyer NULL.
     *
     * ** Important ** : L'objet retiré n'est pas détruit mais est renvoyé par la fonction, 
     * mettre le paramètre `destroy` à 1 pour détruire l'objet après sa suppression dans la liste, 
     * dans ce cas la fonction renvoie NULL.
     *
     *<p>
     * La fonction peut être appelée pour tout objet de type `_ArrayList_` 
     * ou sous-type de `_ArrayList_`.
     * 
     * <p>Chaque suppression en tête de liste se fait en temps θ(n).
     * 
     * @param self La liste dont on souhaite retirer le premier élément.
     * @param destroy Booléen dont 1 permet d'indiquer à la fonction qu'on souhaite qu'elle détruise 
     * l'objet après suppression. La fonction renvoie NULL alors. 
     * 
     * @return La valeur retirée (ou NULL si on ne souhaite pas la récupérer) Si le premier 
     * élément est bien retiré de la liste, NULL sinon.
     */
    void* ArrayList_remove_first(ArrayList self, int destroy);

    /**
     * @brief Cette fonction va retirer le dernier élément de la liste.
     *
     * Si la liste est vide alors la fonction va renvoyer NULL.
     *
     * ** Important ** : L'objet retiré n'est pas détruit mais est renvoyé par la fonction, 
     * mettre le paramètre `destroy` à 1 pour détruire l'objet après sa suppression dans la liste, 
     * dans ce cas la fonction renvoie NULL.
     *
     *<p>
     * La fonction peut être appelée pour tout objet de type `_ArrayList_` 
     * ou sous-type de `_ArrayList_`.
     *
     * <p>Chaque suppression en fin de liste se fait en temps constant O(1).
     * 
     * @param self La liste dont on souhaite retirer le dernier élément.
     * @param destroy Booléen dont 1 permet d'indiquer à la fonction qu'on souhaite qu'elle détruise 
     * l'objet après suppression. La fonction renvoie NULL alors. 
     * 
     * @return La valeur retirée (ou NULL si on ne souhaite pas la récupérer) si le dernier
     * élément est bien retiré de la liste, NULL sinon.
     */
    void* ArrayList_remove_last(ArrayList self, int destroy);
    
    /**
     * @brief Fonction qui renvoie la valeur du premier élément de la liste.
     * 
     * Mettre le paramètre `clone` à 1 pour renvoyer une copie au lieu de la 
     * la vraie valeur.
     * 
     *<p>
     * La fonction peut être appelée pour tout objet de type `_ArrayList_` 
     * ou sous-type de `_ArrayList_`.
     * 
     * @param self La liste dont on cherche le premier élément.
     * @param clone Booléen dont 1 permet d'indiquer à la fonction qu'on souhaite qu'elle renvoie 
     * une copie de l'objet au lieu de la vraie référence. Au cas où on souhaite la récupérer 
     * avant la destruction de la liste.
     * 
     * @return La valeur du premier élément ou NULL si la liste est vide.
     */
    void* ArrayList_get_first(ArrayList self, int clone);

    /**
     * @brief Fonction qui renvoie la valeur du dernier élément de la liste.
     * 
     * Mettre le paramètre `clone` à 1 pour renvoyer une copie au lieu de la 
     * la vraie valeur.
     * 
     *<p>
     * La fonction peut être appelée pour tout objet de type `_ArrayList_` 
     * ou sous-type de `_ArrayList_`.
     * 
     * @param self La liste dont on cherche le dernier élément.
     * @param clone Booléen dont 1 permet d'indiquer à la fonction qu'on souhaite qu'elle renvoie 
     * une copie de l'objet au lieu de la vraie référence. Au cas où on souhaite la récupérer 
     * avant la destruction de la liste.
     * 
     * @return La valeur du dernier élément ou NULL si la liste est vide.
     */
    void* ArrayList_get_last(ArrayList self, int clone);

    /**
     * @brief Fonction qui renvoie la valeur d' élément de la liste qui se trouve à une position donnée.
     * 
     * Mettre le paramètre `clone` à 1 pour renvoyer une copie au lieu de la 
     * la vraie valeur.
     * 
     * <p>Une recherche se fait en temps constant, O(1).
     *<p>
     * La fonction peut être appelée pour tout objet de type `_ArrayList_` 
     * ou sous-type de `_ArrayList_`.
     * 
     * @param self La liste dont on cherche l' élément.
     * @param index La position de l'élément.
     * @param clone Booléen dont 1 permet d'indiquer à la fonction qu'on souhaite qu'elle renvoie 
     * une copie de l'objet au lieu de la vraie référence. Au cas où on souhaite la récupérer 
     * avant la destruction de la liste.
     * 
     * @return La valeur de l'élément ou NULL si la liste est vide.
     */
    void* ArrayList_get(ArrayList self, size_t index, int clone);

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
     * <p>Si on souhaite insérer 2 fois un élément (deux fois la même référence) dans la 
     * liste il est important de mettre `clone` à 1 pour que la fonction puisse 
     * clonner l'objet car au moment de la destruction il sera supprimé deux fois 
     * ce qui va générer une erreur.
     * 
     * <p>
     * La fonction peut être appelée pour tout objet de type `_ArrayList_` 
     * ou sous-type de `_ArrayList_`.
     *
     * <p>Chaque ajout ne se fait pas en temps constant O(size-index).
     * 
     * @param self La liste dont on cherche le dernier élément.
     * @param object L'objet à insérer dans la liste (copier l'objet si on 
     * souhaite le garder après la destruction de la liste).
     * @param index Position dans laquelle on souhaite insérer l'objet dans la liste.
     * @param clone Boolean dont 1 permet d'indiquer à la fonction si on souhaite qu'elle 
     * ajoute une copie de l'objet (copié avec `obj_clone()`) plutôt que l'objet lui-même. 
     * Cela va permettre de garentir que l'objet ne sera pas détruit avec la liste. Mettre 
     * à 0 sinon.
     * 
     * @return 0 Si l'ajout a échoué 1 sinon.
     */
    int ArrayList_add( ArrayList self, void* object, size_t index, int clone );

    /**
     * @brief Fonction qui va retirer un élément de la liste qui se trouve à une position donnée.
     *
     * Si la liste est vide alors la fonction va renvoyer NULL.
     *
     * ** Important ** : L'objet retiré n'est pas détruit mais est renvoyé par la fonction, 
     * mettre le paramètre `destroy` à 1 pour détruire l'objet après sa suppression dans la liste, 
     * dans ce cas la fonction renvoie NULL.
     *
     * <p>
     * La fonction peut être appelée pour tout objet de type `_ArrayList_` 
     * ou sous-type de `_ArrayList_`.
     *
     * <p>Chaque suppression ne se fait pas en temps constant O(size-index).
     * 
     * @param self La liste dont on souhaite retirer le dernier élément.
     * @param destroy Booléen dont 1 permet d'indiquer à la fonction qu'on souhaite qu'elle détruise 
     * l'objet après suppression. La fonction renvoie NULL alors. 
     *  
     * @return La valeur retirée (ou NULL si on ne souhaite pas la récupérer) Si le dernier
     * élément est bien retiré de la liste, NULL sinon.
     */
    void* ArrayList_remove( ArrayList self, size_t index, int destroy );

    /**
     * @brief Fonction qui vérifie si oui ou non un élément se trouve dans la liste.
     * 
     * La fonction va comparer les éléments avec la méthode `obj_equals()`, qui va appeler 
     * dynamiquement sa redéfinition sur les objets de la liste (si redefinie).
     * 
     * @param self La liste à parcourir.
     * @param object L'élément à rechercher.
     * @return 1 Si l'élément se trouve dans la liste et 0 sinon.
     * @see obj_equals()
     */
    int ArrayList_contains( ArrayList self, void* object );

    /**
     * @brief Fonction qui renvoie l'indice de la première occurence d'un élément 
     * qui se trouve dans la liste.
     * 
     * La fonction va comparer les éléments avec la méthode `obj_equals()`, qui va appeler 
     * dynamiquement sa redéfinition sur les objets de la liste (si redefinie).
     * 
     * @param self La liste à parcourir.
     * @param object L'élément à rechercher.
     * @return L'indice dans la liste de l'élément. -1 Si l'élément n'est pas dans la liste.
     * @see obj_equals()
     */
    size_t ArrayList_index_of( ArrayList self, void* object);


#endif
