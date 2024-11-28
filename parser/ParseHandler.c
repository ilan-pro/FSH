
/**
 * @file ParseHandler.c
 * @brief Fichier contenant l'implémentation de la classe `_ParseHandler_`.
 *
 * Un objet ParseHandler est un objet qui définir comment interpréter les différents caractères lus dans 
 * un objet de type `_Parser_`, il le fait par le biais d'une fonction que le parseur va appeller 
 * à chaque caractère lu.
 * <br>
 * Les objets `_ParseHandler_` sont composés d'autres objets de type `_ParseHandlerInterpret_` qui sont des objets 
 * permettant de définissant certains actions à faire lors de la rencontre de certains caractères.
 *
 * En gros L'objet `_ParseHandler_` va reconnaitre cetrains caractères "spéciaux" et `_ParseHandlerInterpret_` va définir 
 * les actions à faire lorsqu'on rencontre ces caractères.
 * 
 * <br> Cette classe est donc une classe abstraite qui doit être redéfinie sur des classes plus spécifiques 
 * dans leur interprétation de caractères.
 *
 * Construction : <code><strong> obj_new(`_ParseHandler_`,  ParseHandlerInterpret interpretor ) </strong></code><br>
 * où : <br>
 * <ul>
 *  <li> `_ParseHandler_` désigne la classe des objets de type `ParseHandler`</li> 
 *  <li> <strong>`interpretor`</strong> est un objet `ParseHandlerInterpret` qui va interpréter les différents caracètres "spéciaux" reconnu par l'objet `ParseHandler`.
 * </ul>
 */

#include "../base/new.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/** @cond HIDE_THIS */
#define PARSE_HANDLER_CLASS "ParseHandler"
#include "ParseHandler.h"
/** @endcond */



/*****************************************************/
/************ Déclaration des fonctions **************/

/**
 * Constructeur d'un objet ParseHandler.
 *
 * Le constructeur va initialiser les attributs.
 * 
 * Construction : <code><strong> obj_new(`_ParseHandler_`,  ParseHandlerInterpret interpretor ) </strong></code><br>
 * où : <br>
 * <ul>
 *  <li> `_ParseHandler_` désigne la classe des objets de type `ParseHandler`</li> 
 *  <li> <strong>`interpretor`</strong> est un objet `ParseHandlerInterpret` qui va interpréter les différents caracètres "spéciaux" reconnu par l'objet `ParseHandler`.
 * </ul>
 */
static void* ParseHandler_constructor (void*, va_list*);
    
/**
 * Fonction qui va détruire l'objet `self` 
 *
 */
static void* ParseHandler_destructor( void* );
    

/*****************************************************/
/************* Structure de la classe ****************/

/**
 * @internal
 * @brief Classe des objets de type `ParseHandler`.
 *
 * Un objet ParseHandler est un objet qui définir comment interpréter les différents caractères lus dans 
 * un objet de type `_Parser_`, il le fait par le biais d'une fonction que le parseur va appeller 
 * à chaque caractère lu.
 * <br>
 * Les objets `_ParseHandler_` sont composés d'autres objets de type `_ParseHandlerInterpret_` qui sont des objets 
 * permettant de définissant certains actions à faire lors de la rencontre de certains caractères.
 *
 * En gros L'objet `_ParseHandler_` va reconnaitre cetrains caractères "spéciaux" et `_ParseHandlerInterpret_` va définir 
 * les actions à faire lorsqu'on rencontre ces caractères.
 * 
 * <br> Cette classe est donc une classe abstraite qui doit être redéfinie sur des classes plus spécifiques 
 * dans leur interprétation de caractères.
 *
 * Construction : <code><strong> obj_new(`_ParseHandler_`,  ParseHandlerInterpret interpretor ) </strong></code><br>
 * où : <br>
 * <ul>
 *  <li> `_ParseHandler_` désigne la classe des objets de type `ParseHandler`</li> 
 *  <li> <strong>`interpretor`</strong> est un objet `ParseHandlerInterpret` qui va interpréter les différents caracètres "spéciaux" reconnu par l'objet `ParseHandler`.
 * </ul>
 */
typedef struct _ParseHandler_ {
    Class _ParseHandler_;
    PARSE_HANDLER_FIELDS
}* ParseHandler;

static struct _Class_ __ParseHandler__ = {
    NULL,
    PARSE_HANDLER_CLASS,
    sizeof(struct _ParseHandler_),
    ParseHandler_constructor,
    ParseHandler_destructor,
    0,
    0,
    0,
    0
};


/**
 * @internal
 * @brief Classe des objets de type `ParseHandler`.
 *
 * Construction : <code><strong> obj_new(`_ParseHandler_`,  ParseHandlerInterpret interpretor ) </strong></code><br>
 * où : <br>
 * <ul>
 *  <li> `_ParseHandler_` désigne la classe des objets de type `ParseHandler`</li> 
 *  <li> <strong>`interpretor`</strong> est un objet `ParseHandlerInterpret` qui va interpréter les différents caracètres "spéciaux" reconnu par l'objet `ParseHandler`.
 * </ul>
 */
Class _ParseHandler_ = &__ParseHandler__;

/***************************************************/
/************** Corps des fonctions ****************/

// Constructor
static void* ParseHandler_constructor(void* self_, va_list* args){
        
    // Vérificiation de self et args
    assert(self_);
    assert(args);
        
    // Conversion en objet de type ParseHandler.
    ParseHandler self = (ParseHandler) self_;

    // Récupération des arguments
    ParseHandlerInterpret handler_interp = va_arg(*args, ParseHandlerInterpret);
        
    // Vérification de str et handler.
    assert(handler_interp);

    self->interpretor = handler_interp; 
    
    return self;
}


// Destructor
static void* ParseHandler_destructor(void* self_){
    
    // Vérfication de self_
    assert(self_);
    assert(obj_instance_of(self_, _ParseHandler_));

    ParseHandler self = (ParseHandler) self_;
    
    // Destruction de l'interpreteur.
    obj_destroy(self->interpretor);
    
    return self;
}


        
/**
 * @internal
 * @brief Fonction qui interprète un caractère lu par le parseur.
 *
 * Cette fonction va appeller une redéfinition (obligatoire sinon il y aura une erreur) 
 * de la fonction `interpret(ParseHandler self, struct _Parser_* parser, char c)` 
 * dans une classe fille.
 *
 * <br>C'est à la classe fille d'indiquer comment elle décide d'interpréter le caractère (si 
 * ce caractère est considéré comme spécial par exemple).
 *
 * @param self L'objet qui va gérer tous les caractères lu par le parseur.
 * @param parser Le parseur qui a lu le caractère (pour pouvoir lui demander de l'accepter par exemple).
 * @param c Le caractère lu par le parseur.
 */
void ParseHandler_interpret(ParseHandler self, struct _Parser_* parser, char c){
    
    // Vérfication de self
    assert(self && parser);
    assert(obj_instance_of(self, _ParseHandler_));

    if (!self->interpret){

        char tmp1[] = "\e[1;31m[Erreur] La classe \"";
        write(2, tmp1, strlen(tmp1));
        write(2, self->_ParseHandler_->name, strlen(self->_ParseHandler_->name));
        char tmp2[] = "\" ne possède pas de fonction `interpret()`, pensez à en écrire une !\n\e[0m";
        write(2, tmp2, strlen(tmp2));
        abort();

    }   

    // Appelle de la fonction.
    self->interpret(self, parser, c);

}



/**
 * @internal
 * @brief Fonction qui exécute des instructions à la fin du parsing de la chaine, par le parseur.
 * 
 * Elle peut être utile pour travailler sur la dernière valeur qui se trouve dans le buffer à la fin 
 * du parsing.
 * 
 * La fonction va appeller la fonction `ParseHandlerInterpret_action_on_parse_finish()` de son 
 * interpreteur.
 * 
 * @param self L'objet `ParseHandler` qui va gérer la fin du parsing.
 * @param parser Le parseur qui vient de finir son parsing.
 *  
 */
void ParseHandler_action_on_parse_finish(ParseHandler self, struct _Parser_* parser){

    
    // Vérfication de self
    assert(self && parser);
    assert(obj_instance_of(self, _ParseHandler_));
    assert(self->interpretor);
    
    if (self->interpretor->action_on_parse_finish)
        self->interpretor->action_on_parse_finish(self->interpretor, parser);
    
}
















