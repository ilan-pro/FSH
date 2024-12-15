/**
 * @file Integer.c 
 * @brief Fichier contenant l'implémentation des variables et de toutes les méthodes de l'interface.
 *
 * Ce fichier contient le code sources des fonctions de l'interface `Integer.h` mais aussi 
 * les constructeurs, les destructeurs, etc ...
 */

#include "new.h"

/**@cond HIDE_THIS*/
#define INTEGER_CLASS "Integer"
#include "Integer.h"
/**@endcond*/

/**
 * Déclaration des fonctions.
 *********************************/

/**
 * Fonction qui construit initialise l'objet (de type Integer) donnée en argument 
 * en initialisant sont champs `val` avec la valeur donnée au constructeur (qui 
 * se trouve dans `va_list* args`). 
 *
 * @param `self_` L'objet alloué par le constructeur générique de `new.h`.
 * @param `args` Une liste d'argument qui doit contenir la valeur à insérer dans 
 * le champs `val` de l'objet.
 *
 * @return L'objet après avoir été initialisé.
 */
static void* Integer_construct(void* self_, va_list* args);

/**
 * @brief Fonction qui va détruire l'objet de type `_Integer_`.
 * 
 * Cette fonction ne fait rien car aucune ressource n'est allouée ici.
 *
 * @param `self_` la référence vers l'objet.
 * @return la même référence après avoir desalloué la chaine.
 */
static void* Integer_destroy(void* self_);

/**
 * Fonction qui renvoie une chaine de caractère contant la valeur stockée 
 * par l'objet.
 *
 * @param self_ L'objet de type Integer dont on souhaite récupérer la valeur.
 * @return Une chaine contenant la valeur stockée dans l'objet.
 */
static char* Integer_to_string(const void* self_);

/**
 * Fonction qui va comparer l'objet Integer avec un autre afin de vérifier 
 * si les deux valeurs sont oui ou non égales.
 * 
 * @param self_ L'objet Integer dont on souhaite comparer la valeur.
 * @param object_ L'objet auquel on va comparer.
 *
 * @return si oui (1) ou non (0) les deux valeurs sont les mêmes.
 */
static int Integer_equals(const void* self_, const void* object_);
    
/**
 * Fonction qui va renvoyer une copie de l'objet Integer, self_.
 *
 * @param self_ L'objet Integer à copier.
 * @return Un nouvel objet de type Integer, copie de la valeur.
 */
static void* Integer_clone(const void* self_);

/*****************************************************/
/************* Structure de la classe ****************/

/**
 * Initialisation de la classe Integer.
 */
static struct _Class_ __Integer__ = {
    NULL,
    INTEGER_CLASS,
    sizeof(struct _Integer_),
    Integer_construct, Integer_destroy, Integer_to_string, Integer_equals, Integer_clone
};

/**
 * Classe de tous les objets de type Integer.
 */
const Class _Integer_ = &__Integer__;


/***************************************************/
/************** Corps des fonctions ****************/

// Constructeur
static void* Integer_construct(void* self_, va_list* args){
    
    assert(self_ && args); // Différent de NULL
    
    // Assert également que l'objet est bien de la class Integer.
    assert(obj_instance_of(self_, _Integer_));

    Integer self = (Integer) self_; // cast de self_ en Integer 
        
    // Récupération de l'argument
    int value = va_arg(*args, int);
        
    // Mise à jour de la valeur du int.
    *(int*)&self->val = value;
    
    return self;
}

// Destructeur
static void* Integer_destroy(void* self_){
            
    // On ne fait rien du tout.

    return self_;

}

/**
 * Fonction qui renvoie la puissance la plus grande puissance de 10 
 * tel que le quotient de la division n'est pas nul.
 */
static int get_pow_ten( int val ){
    
    int pow = 1;
    int ten  = 10;
    while (val / ten != 0){
        ten *=10;
        pow++;
    }

    return ten/10;

}
    

// Fonction to_string
static char* Integer_to_string(const void* self_){
    
    // Vérification si on a pas NULL
    assert(self_);
    // Vérification si cet objet est un Integer
    assert(obj_instance_of(self_, _Integer_));
    
    // Conversion de l'objet en Integer.
    Integer self = (Integer) self_;
        
    // Récupération de la valeur.
    int val = self->val;  
    
    // Indice de parcourt
    int index = 0;
   
    if (val < 0){
        to_string_buffer[index++] = '-';
        val *= -1;
    }

    // Récupération de la puissance de 10 de poid fort.
    int pow = get_pow_ten (val);

    while (pow != 0){
        int digit = val/pow;
        val %= pow;
        pow /= 10;
        to_string_buffer[index++] = '0' + digit;
    }
    to_string_buffer[index] = '\0';
    return to_string_buffer;
}

// Fonction equals.
static int Integer_equals(const void* self_, const void* object_){
    
    // Vérification des pointeur (pas de NULL)
    assert(self_ && object_);
    // Vérification de l'appartenance de self_ et object_  à la classe Integer
    if (obj_instance_of(self_, _Integer_) && obj_instance_of(object_, _Integer_)){
    
        // Conversion des pointeurs en Integer.
        Integer self = (Integer) self_;
        Integer object = (Integer) object_;

        // Comparaison des valeurs.
        return self->val == object->val;
             
    }

    return 0;

}

// Fonction clone.
static void* Integer_clone(const void* self_){
    
    // On s'assure que l'objet n'est pas NULL et 
    // Qu'il est bien de type Integer.
    assert(self_ && obj_instance_of(self_, _Integer_)); 
    Integer self = (Integer) self_;

    // Constructtion d'un nouvel objet à partir de celui-ci.
    return obj_new(_Integer_, self->val);

}

/*****************************************************/
/*****************************************************/
/*****************************************************/



