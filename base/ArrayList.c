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
    0,
    0,
    0
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






