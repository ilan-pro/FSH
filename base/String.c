/**
 * @file String.c 
 * @brief Fichier contenant l'implémentation des variables et de toutes les méthodes de l'interface.
 *
 * Ce fichier contient le code sources des fonctions de l'interface `String.h` mais aussi 
 * les constructeurs, les destructeurs, etc ...
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "new.h"
/**@cond HIDE_THIS*/
#define STRING_CLASS "String"
#include "String.h"
/**@endcond*/

/**
 * Déclaration des fonctions.
 *********************************/

/**
 * Fonction qui construit initialise l'objet (de type String) donnée en argument 
 * en initialisant sont champs `str` avec la valeur donnée au constructeur (qui 
 * se trouve dans `va_list* args`).
 *
 * @param `self_` L'objet alloué par le constructeur générique de `new.h`.
 * @param `args` Une liste d'argument qui doit contenir la chaine à copier dans le champs `str` de l'objet.
 *
 * @return L'objet après avoir été initialisé.
 */
static void* String_construct(void*, va_list*);

/**
 * Cette fonction va désallouer la chaine de caractère encapsulé 
 * dans l'objet.
 * 
 * @param `self_` la référence vers l'objet.
 * @return la même référence après avoir desalloué la chaine.
 */
static void* String_destroy(void*);

/**
 * Fonction qui renvoie une copie de la chaine de caractère 
 * encapsulé dans l'objet, cette copie est écrite dans le buffer 
 * définie dans `new.h`.
 * 
 * Si la chaine de caractère est trop grande elle sera tronquée.
 *
 * @param self_ L'objet de type String dont on souhaite récupérer la chaine.
 * @return Une copie de la chaine stockée dans l'objet.
 */
static char* String_to_string(void*);

/*****************************************************/
/************* Structure de la classe ****************/

/**
 * @brief Classe `String` des chaines de caractères.
 *
 * Il s'agit d'un boxing du type `char*` en object de 
 * type String.
 * <p>Il permet nottement de les utiliser dans des structure de donnée génériques comme 
 * les listes.
 * 
 * Construction : obj_new( _String_, <chaine_de_caractere> );
 */
typedef struct _String_ {
    /**
     * @brief Classe d'un objet String
     */
    Class _String_; 
    STRING_FIELD    // Champs de l'objet String (déclarés dans le header)
}* String ;

/**
 * Initialisation de la classe String.
 */
static struct _Class_ __String__ = {
    NULL,
    STRING_CLASS,
    sizeof(struct _String_),
    0, String_destroy, 0, 0
};

/**
 * Classe de tous les objets de type String.
 */
Class _String_ = &__String__;


/***************************************************/
/************** Corps des fonctions ****************/

// Constructeur
static void* String_construct(void* self_, va_list* args){
    
    assert(self_ && args); // Différent de NULL
    
    // Assert également que l'objet est bien de la class String.
    assert(obj_instance_of(self_, _String_));

    String self = (String) self_; // cast de self_ en String 
        
    // Récupération de l'argument
    char* str_arg = va_arg(*args, char*);
    
    // Allocation du champs str (en contournant const)
    *(char**)&self->str = malloc(sizeof(char) * (1 + strlen(str_arg)));
    
    // Copie dans la chaine (en contournant encore const)
    strcpy(*(char**)&self->str, str_arg);
    
    return self;
}

// Destructeur
static void* String_destroy(void* self_){
        
    assert(obj_instance_of(self_, _String_));
    String self = (String) self_;
    assert(self); // Si le programme plante ici alors self est NULL.
    
    // Desallocation de la chaine de caractère.
    free((void*)self->str); // 

    return self;

}

/*****************************************************/
/*****************************************************/
/*****************************************************/



