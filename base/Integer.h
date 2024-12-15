/**
 * @file Integer.h 
 * @brief Fichier d'en-tête contenant les déclarations des variables publiques et de toutes les méthodes public de Integer.
 */


#include "Class.h"
#ifndef INTEGER_H
#define INTEGER_H


/**
 * Il s'agit des champs d'un objet de type Integer.
 * <p>
 * Cette macro n'est visible que sur des sources dont la définition INTEGER_CLASS est faite.
 * cela permet d'avoir une encasulation des attributs en les rendant privés dans toute implémentation 
 * ne possèdant pas la décrlatation INTEGER_CLASS.
 * <p>
 * Cela permet également pour l'héritage de classe, de récrire PARENT_FIELD (Ici INTEGER_FIELD)
 * pour inclure les arrtibuts du parents dans ceux de l'enfant.
 */
#define INTEGER_FIELD \
    const int val; 
/**
 * @internal
 * 
 * Classe `Integer` des entiers naturels.
 *
 * Il s'agit d'un boxing du type `int` en objet de 
 * type Integer.
 * <p>Il permet nottement de les utiliser dans des structure de donnée génériques comme 
 * les `_LinkedList_` ou `_ArrayList_`.
 * 
 * Construction : obj_new( _Integer_, int value );
 */
typedef struct _Integer_ {
    const Class _Integer_;
    INTEGER_FIELD
}* Integer;


/**
 * @internal
 * Classe de tous les objets de type Integer.
 * 
 * Elle est défini dans le fichier `Integer.c`
 */
extern const Class _Integer_;

#endif
