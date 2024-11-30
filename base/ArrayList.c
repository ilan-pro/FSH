/**
 * @file ArrayList.c
 * @brief Fichier contenant l'implémentation de la classe `_ArrayList_`.
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

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "new.h"

/**@cond HIDE_THIS*/
#define ARRAY_LIST_CLASS "ArrayList"
#include "ArrayList.h"

/**@endcond*/

/*****************************************************/
/************ Déclaration des fonctions **************/
    
/**
 * Constructeur d'une `ArrayList`. 
 *
 * Construction : obj_new( `_ArrayList_`, int initial_size )
 * où :
 *   `_ArrayList_` désigne la classe des objets de type `ArrayList`.
 *   `initial_size` désigne la taille initiale de la liste. Idéale si on a l'intention de stocker une grande quantité de donnée sans trop redimensionner.
 */
static void* ArrayList_constructor(void*, va_list*);
    
/**
 * Destructeur d'une `ArrayList`.
 */
static void* ArrayList_destructor(void*);
    
/**
 * Fonction qui va comparer si deux ArrayList sont égales.
 *
 * Deux ArrayList sont égales si et seulement si ils ont la même taille 
 * et qu'ils ont les même objets, sur les mêmes indices.
 *
 * La fonction `obj_equals()` sera appelée sur chaque objet de la liste.
 */
static int ArrayList_equals(const void*, const void*);
    
/**
 * Fonction qui va cloner la liste.
 *
 * La fonction `obj_clone()` sera appelée sur chaque objet de la liste.
 */
static void* ArrayList_clone(const void*);

/**
 * Fonction qui va afficher la liste.
 *
 * La fonction `obj_print()` sera appelée sur chaque objet de la liste.
 */
static void ArrayList_print(const void*);
    
/**
 * Fonction qui va vérifier la taille de la liste si, on peut rajouter un élément 
 * dans la liste. 
 * Si la fonction trouve qu'on ne peut pas ajouter d'élément alors elle ralonge la liste de size/2.
 */
static void check_size(ArrayList);

/*****************************************************/
/************* Structure de la classe ****************/

/**
 * @internal
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
    ARRAY_LIST_FIELDS

}* ArrayList;

static struct _Class_ __ArrayList__ = {
    
    NULL,
    ARRAY_LIST_CLASS,
    sizeof(struct _ArrayList_),
    ArrayList_constructor,
    ArrayList_destructor,
    0,
    ArrayList_equals,
    ArrayList_clone,
    ArrayList_print
};

/**
 * Classe de tous les objets de type `_ArrayList_`.
 * 
 * Construction <code><strong> obj_new( `_ArrayList_`, int initial_size ) </strong></code>
 * où : <ul>
 *          <li> `_ArrayList_` désigne la classe des objets de type `ArrayList`.</li>
 *          <li> <code><strong> initial_size </strong></code> désigne la taille initiale de la liste. 
 *               Idéale si on a l'intention de stocker une grande quantité de donnée sans trop redimensionner.</li>
 *      </ul>
 *
 * Elle est définie dans `ArrayList.c`
 */
const Class _ArrayList_ = &__ArrayList__;

/***************************************************/
/************** Corps des fonctions ****************/


/**
 * Fonction qui va vérifier la taille de la liste si, on peut rajouter un élément 
 * dans la liste. 
 * Si la fonction trouve qu'on ne peut pas ajouter d'élément alors elle ralonge la liste de size/2.
 */
static void check_size( ArrayList self ){
    
    // Vérification
    assert(self);
    assert(self->last_index<=self->size); // Si l'indice du dernier élément n'est pas inférieur ou égal alors c'est mauvais signe.

    if (self->last_index < self->size) // Si l'indice est strictement inférieur alors on peut toujours rajouter un élément.
        return;
    
    printf("Ici : new_size=%lu\n", self->size + self->size/2);

    // Sinon on ralonge la liste de size/2
    self->dyn_array = realloc(self->dyn_array, sizeof(void**)*(self->size + self->size/2));
    assert(self->dyn_array);
    self->size = self->size + self->size/2;

    return;

}
    

/**
 * Constructeur d'une `ArrayList`. 
 *
 * Construction : obj_new( `_ArrayList_`, int initial_size )
 * où :
 *   `_ArrayList_` désigne la classe des objets de type `ArrayList`.
 *   `initial_size` désigne la taille initiale de la liste. Idéale si on a l'intention de stocker une grande quantité de donnée sans trop redimensionner.
 */
static void* ArrayList_constructor( void* self_ , va_list* args ){
        
    // Vérifications.
    assert(self_);
    assert(args);
    assert(obj_instance_of(self_, _ArrayList_));

    // Conversion en objet de type ArrayList
    ArrayList self = (ArrayList) self_;

    // Récupération de la taille de départ.
    size_t init_size = va_arg(*args, size_t);
    init_size = (init_size>1) ? init_size : 2; // Au cas où c'est 0 ou 1.
    
    // Initialisation du tableau.
    self->dyn_array = (void**) malloc(sizeof(void**) * init_size);
    assert(self->dyn_array);
    self->size = init_size;
    self->last_index = 0;

    return self;

}



/**
 * Destructeur d'une `ArrayList`.
 */
static void* ArrayList_destructor(void* self_){
    // Vérifications.
    assert(self_);
    assert(obj_instance_of(self_, _ArrayList_));
    
    
    // Conversion en objet de type ArrayList
    ArrayList self = (ArrayList) self_;
    
    // destruction de tous les objets qui sont dans le tableau.
    for (size_t i = 0; i<ArrayList_size(self); i++)
        obj_destroy(self->dyn_array);
    
    free(self->dyn_array);

    return self;

}


/**
 * Fonction qui va comparer si deux ArrayList sont égales.
 *
 * Deux ArrayList sont égales si et seulement si ils ont la même taille 
 * et qu'ils ont les même objets, dans le même ordre.
 *
 * La fonction `obj_equals` sera appelé sur chaque objet de la liste.
 *
 * @return 1 si les deux listes sont les égaux. 0 sinon.
 */
static int ArrayList_equals( const void* self_, const void* object_ ){

    // Vérifications.
    assert(self_);
    assert(object_);
    assert(obj_instance_of(self_, _ArrayList_));
    assert(obj_instance_of(object_, _ArrayList_));

    // Conversion en objets de type ArrayList
    ArrayList self = (ArrayList) self_;
    ArrayList object = (ArrayList) object_;

    // Si les deux listes n'ont pas la même taille.
    if (ArrayList_size(self) != ArrayList_size(object))
        return 0;

    // Si on est ici c'est que les deux listes ont la même taille.
    // Vérification des objets.
    
    for (size_t i = 0; i<ArrayList_size(self); i++){
        
        // Si il existe deux objets distincts alors les deux listes sont différentes
        if (!obj_equals(self->dyn_array[i], object->dyn_array[i]))
            return 0;

    }
    
    return 1;

}


/**
 * Fonction qui va cloner la liste.
 *
 * La fonction `obj_clone()` sera appelée sur chaque objet de la liste.
 */
static void* ArrayList_clone( const void* self_ ){
    
    // Vérifications.
    assert(self_);
    assert(obj_instance_of(self_, _ArrayList_));
    
    // Conversion en objets de type ArrayList
    ArrayList self = (ArrayList) self_;
    
    // Création de la copie.
    ArrayList copy = obj_new(_ArrayList_, ArrayList_size(self)); // On lui donne initialement la taille de self pour éviter le redimensionnement.
    
    for (size_t i = 0; i<ArrayList_size(self); i++)
        ArrayList_add_last(copy, self->dyn_array[i], 1); // On ajoute une copie du i-ème élément à la fin.

    return copy;
}


/**
 * Fonction qui va afficher la liste.
 *
 * La fonction `obj_print()` sera appelée sur chaque objet de la liste.
 */
static void ArrayList_print( const void* self_ ){
    
    // Vérifications.
    assert(self_);
    assert(obj_instance_of(self_, _ArrayList_));
    
    // Conversion en objets de type ArrayList
    ArrayList self = (ArrayList) self_;
    
    write(1, "[ ", 2); 
    for (size_t i = 0; i<self->size; i++){

        if (i<self->last_index) obj_print(self->dyn_array[i]);
        else write(1, " _, ", 4);
        if (i<ArrayList_size(self)-1)
            write(1, ", ", 2);
    }
    write(1, " ]", 2); 

}



/**
 * @internal
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
size_t ArrayList_size( ArrayList self ){

    // Vérifications.
    assert(self);
    assert(obj_instance_of(self, _ArrayList_));
    
    // Renvoie l'indice de la dernière place libre.
    return self->last_index;
    
}




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
int ArrayList_add_first( ArrayList self, void* object, int clone ){
    
    return ArrayList_add(self, object, 0, clone);

}


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
int ArrayList_add_last( ArrayList self, void* object, int clone ){

    return ArrayList_add(self, object, self->last_index, clone);

}


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
int ArrayList_add( ArrayList self, void *object, size_t index, int clone ){

    
    // Vérifications.
    assert(self);
    assert(obj_instance_of(self, _ArrayList_));
    
    // Vérification de l'indice.
    if(index<0 || index > self->last_index){
            
        char err[] = "\e[1;31m[Erreur] L'indice de l'élément est en dehors de la liste !\e[1;0m\n";
        write(2, err, strlen(err));
        abort();
        return 0;

    }
    
    // Vérification de la taille (va rallonger la taille si besoin).
    check_size(self);
    
    printf("index = %lu\n", index);

    // Décalage de tous les éléments de 1 à partir de index.
    for (size_t i = self->last_index; i>index; i--)
        self->dyn_array[i] = self->dyn_array[i-1];
        
    // Ajout de l'objet (ou son clone)
    self->dyn_array[index] = (clone) ? obj_clone(object) : object;
    self->last_index++; // Mise à jour de l'indice du dernier élément.

    return 1;

}
