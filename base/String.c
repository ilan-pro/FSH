/**
 * @file String.c 
 * @brief Fichier contenant l'implémentation des variables et de toutes les méthodes de l'interface.
 *
 * Ce fichier contient le code sources des fonctions de l'interface `String.h` mais aussi 
 * les constructeurs, les destructeurs, etc ...
 */

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
 * se trouve dans `va_list* args`, et que la fonction copie). 
 *
 * @param `self_` L'objet alloué par le constructeur générique de `new.h`.
 * @param `args` Une liste d'argument qui doit contenir la chaine à copier dans le champs `str` de l'objet.
 *
 * @return L'objet après avoir été initialisé.
 */
static void* String_construct(void* self_, va_list* args);

/**
 * Cette fonction va désallouer la chaine de caractère encapsulé 
 * dans l'objet.
 * 
 * @param `self_` la référence vers l'objet.
 * @return la même référence après avoir desalloué la chaine.
 */
static void* String_destroy(void* self_);

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
static char* String_to_string(const void* self_);

/**
 * Fonction qui va comparer l'objet String avec un autre afin de vérifier 
 * si les deux chaines sont oui ou non égales.
 * 
 * @param self_ L'objet String dont on souhaite comparer la chaine.
 * @param object_ L'objet auquel on va comparer.
 *
 * @return si oui (1) ou non (0) les deux chaines sont les mêmes.
 */
static int String_equals(const void* self_, const void* object_);
    
/**
 * Fonction qui va renvoyer une copie de l'objet String, self_.
 *
 * @param self_ L'objet String à copier.
 * @return Un nouvel objet de type String, copie de la chaine.
 */
static void* String_clone(const void* self_);

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
 * Construction : obj_new( \_String\_, <chaine_de_caractere> );
 */
typedef struct _String_ {
    /**
     * @brief Classe d'un objet String
     */
    const Class _String_; 
    STRING_FIELD    // Champs de l'objet String (déclarés dans le header)
}* String ;

/**
 * Initialisation de la classe String.
 */
static struct _Class_ __String__ = {
    NULL,
    STRING_CLASS,
    sizeof(struct _String_),
    String_construct, String_destroy, String_to_string, String_equals, String_clone
};

/**
 * Classe de tous les objets de type String.
 */
const Class _String_ = &__String__;


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
    
    // Mise à jour de la taille de la chaine.
    *(int*)&self->size = strlen(str_arg);

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

// Fonction to_string
static char* String_to_string(const void* self_){
    
    // Vérification si on a pas NULL
    assert(self_);
    // Vérification si cet objet est un String
    assert(obj_instance_of(self_, _String_));
    
    // Conversion de l'objet en String.
    String self = (String) self_;
    
    // Copie de la chaine dans to_string_buffer puis 
    // retour du pointeur vers to_string_buffer.
    return strcpy(to_string_buffer, self->str);
    
}

// Fonction equals.
static int String_equals(const void* self_, const void* object_){
    
    // Vérification des pointeur (pas de NULL)
    assert(self_ && object_);
    // Vérification de l'appartenance de self_ et object_  à la classe String
    if (obj_instance_of(self_, _String_) && obj_instance_of(object_, _String_)){
    
        // Conversion des pointeurs en String.
        String self = (String) self_;
        String object = (String) object_;

        // Comparaison des chaines.
        return strcmp(self->str, object->str) == 0;
    
    }

    return 0;

}

// Fonction clone.
static void* String_clone(const void* self_){
    
    // On s'assure que l'objet n'est pas NULL et 
    // Qu'il est bien de type String.
    assert(self_ && obj_instance_of(self_, _String_)); 
    String self = (String) self_;

    // Constructtion d'une nouvelle chaine à partir de celle-ci.
    return obj_new(_String_, self->str);

}

/*****************************************************/
/*****************************************************/
/*****************************************************/



