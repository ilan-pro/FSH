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
        ArrayList_add_last(copy, ArrayList_get(self, i, 1), 0); // On ajoute une copie du i-ème élément à la fin.

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
    for (size_t i = 0; i<ArrayList_size(self); i++){
        obj_print(ArrayList_get(self, i, 0));
        if (i<ArrayList_size(self)-1)
            write(1, ", ", 2);
    }
    write(1, " ]", 2); 

}



