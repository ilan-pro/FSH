/**
 * @file new.c
 * @brief Fichier contenant l'implémentation de toutes les méthodes génériques agissant 
 * sur des objets déclarées dans l'interface.
 *
 * Ces méthode sont générique pour permettre la création et la manipulation d'objets 
 * sans connaitre réelment leurs types et leurs structures. <br>Indispensable si on veut faire 
 * de la programmation orienté objet de manière plus efficace.
 */
#include "new.h"
#include "assert.h"
#include "stdlib.h"
#include <stdio.h>
#include <string.h>


/**
 * @internal 
 *
 * buffer utilisé dans les fonctions to_string.
 */
char to_string_buffer[1024];

/**
 * @internal
 *
 * Fonction qui alloue et construit l'objet demandé.
 * Voir DOC. dans new.h
 */
void* obj_new(void* __class__, ...){
    
    // On cast en Class le pointeur (Class = struct _Class_* )
    Class _class_ = (Class) __class__;
    assert(_class_); // Vérification.
    // Allocation de la mémoire pour l'objet.
    void* object = calloc(1, _class_->size);
    assert(object); // Au cas où.
    
    // AJout de la classe dans l'objet.
    *(Class*) object = _class_;
    if (_class_->ctor){
        // Création de la liste d'arguments.
        va_list arguments;
        va_start(arguments, __class__);
        
        // Appelle du constructeur de l'objet.
        object = _class_->ctor(object,&arguments);
        va_end(arguments);
    }

    return object;

}

/**
 * @internal 
 *
 * Fonction qui desalloue et détruit l'objet donné en argument.
 *
 * (Voir DOC dans `new.h`)
 */
void obj_destroy(void* self_){
    
    // Récupération de la classe de l'objet.
    // On suppose que le premier champs de la structure 
    // de l'objet est sa classe (lire DOC `Class.h`)
    Class _class_ = *(Class*) self_;
    assert(self_ && _class_ && _class_->dtor);
        
    // Destruction de l'objet.
    self_ = _class_->dtor(self_);
    
    // Libération de la mémoire allouée.
    free(self_);
}
    
/**
 * @internal 
 *
 * Fonction qui renvoie une chaine de caractère décrivant l'objet.
 *
 * (Voir DOC `new.h`)
 */
char* obj_to_string(const void* self_){
    
    // Récupération de la classe de l'objet.
    Class _class_ = *(Class*) self_;
    assert(_class_);

    if (_class_->to_string)
        return _class_->to_string(self_);

    snprintf(to_string_buffer, 100, "%s:%p", _class_->name, _class_);
    return to_string_buffer;

}


/**
 * @internal 
 *
 * Fonction qui compare deux objets.
 *
 * Voir DOC `new.h`
 */
int obj_equals(const void* self_, const void* object_){
    
    // Vérification.
    assert(self_ && object_);
    
    // Si le c'est le même pointeur alors on renvoie vrai.
    if (self_ == object_) return 1;
    
    // Sinon ..
    // Récupération de la classe de self_
    // On suppose que le premier champs de la structure 
    // de l'objet est sa classe (lire DOC `Class.h`)
    Class _self_ = *(Class*) self_;
    assert(_self_);

    // Test de si la méthode existe.
    if (_self_->equals){
        return _self_->equals(self_, object_);
    }
    
    //Sinon on retourne la comparaison bit à bit des deux objets.
    return memcmp(self_, object_, _self_->size)==0;
        
}

/**
 * Fonction qui vérifie récursivement si `_object_` ou un de ses ancètre 
 * est `_class_`.
 */
static int instanceof(const Class _object_, const Class _class_){
    
    if (_object_ == _class_)
        return 1;
    else if (_object_->_super_)
        return instanceof(_object_->_super_, _class_);
    else 
        return 0;

}

/**
 * @internal
 * 
 * Fonction qui vérifie si un objet est l'instance d'un classe donnée.
 *
 * Voir DOC `new.h`
 */
int obj_instance_of(const void* object, const Class _class_){
    
    assert(object && _class_);
    // Récupération de la classe de l'objet.
    // On suppose que le premier champs de la structure 
    // de l'objet est sa classe (lire DOC `Class.h`)
    Class _object_ = *(Class*) object;
    
    return instanceof(_object_, _class_);
    
}


