
/**
 * @file ParseHandlerInterpret.c
 * @brief Fichier contenant l'implémentation de la classe `_ParseHandlerInterpret_`.
 *
 * Un objet ParseHandlerInterpret est un objet qui possède un certain nombre de fonctions qui seront appelées par l'objet 
 * `_ParseHandler_` qui le contient, ces fonctions doivent définir les différentes actions à faire au moment de la 
 * rencontre de certains caractères spéciaux reconnu par l'objet `_ParseHandler_`.
 * 
 * Construction : <code><strong> obj_new(`_ParseHandlerInterpret_`) </strong></code><br>
  * où `_ParseHandlerInterpret_` désigne la classe des objets de type `ParseHandlerInterpret`
 */
#include "../base/new.h"
#include <assert.h>

/** @cond HIDE_THIS */
#define PARSE_HANDLER_INTERPRET_CLASS "ParseHandlerInterpret"
#include "Parser.h"
/** @endcond */



/*****************************************************/
/************ Déclaration des fonctions **************/

/**
 * Constructeur d'un objet ParseHandlerInterpret.
 *
 * Construction : <code><strong> obj_new(`_ParseHandlerInterpret_`) </strong></code><br>
  * où `_ParseHandlerInterpret_` désigne la classe des objets de type `ParseHandlerInterpret`
 */
static void* ParseHandlerInterpret_constructor (void*, va_list*);
    
/**
 * Fonction qui va détruire l'objet `self` 
 */
static void* ParseHandlerInterpret_destructor( void* );


/**
 * @internal
 * @brief Fonction qui exécute des instructions à la fin du parsing de la chaine, par le parseur.
 * 
 * Elle peut être utile pour travailler sur la dernière valeur qui se trouve dans le buffer à la fin 
 * du parsing.
 * 
 * @param self L'objet `ParseHandlerInterpret` qui va gérer la fin du parsing.
 * @param parser Le parseur qui vient de finir son parsing.
 *  
 */
static void ParseHandlerInterpret_action_on_parse_finish(ParseHandlerInterpret self, Parser parser);

/*****************************************************/
/************* Structure de la classe ****************/

/**
 * @internal
 * @brief Classe des objets de type `ParseHandlerInterpret`.
 *
 * Un objet ParseHandlerInterpret est un objet qui possède un certain nombre de fonctions qui seront appelées par l'objet 
 * `_ParseHandler_` qui le contient, ces fonctions doivent définir les différentes actions à faire au moment de la 
 * rencontre de certains caractères spéciaux reconnu par l'objet `_ParseHandler_`.
 * 
 * Construction : <code><strong> obj_new(`_ParseHandlerInterpret_`) </strong></code><br>
  * où `_ParseHandlerInterpret_` désigne la classe des objets de type `ParseHandlerInterpret`
 */
typedef struct _ParseHandlerInterpret_ {
     Class _ParseHandlerInterpret_;
     PARSE_HANDLER_INTERPRET_FIELDS
}* ParseHandlerInterpret;

static struct _Class_ __ParseHandlerInterpret__ = {
    NULL,
    PARSE_HANDLER_INTERPRET_CLASS,
    sizeof(struct _ParseHandlerInterpret_),
    ParseHandlerInterpret_constructor,
    ParseHandlerInterpret_destructor,
    0,
    0,
    0,
    0
};


/**
 * @internal
 * @brief Classe des objets de type `Parser`.
 *
 * Un objet Parser est un objet qui va analyser une chaine de caractère et va appeler une va appeler
 * une certaine fonction (handler) à chaque caractère lu, afin qu'elle l'interprète à sa guise.
 * <br>
 * NB : Lorsqu'un parsing est terminé. L'objet n'est plus utilisable (tout appel à `Parser_parse()` finira aussitôt).
 * <br>
 * Construction : <code><strong> obj_new(`_Parser_`, char** string, ParseHandler handler) </strong></code><br>
 * où : <br>
 * <ul>
 *  <li> `_Parser_` désigne la classe des objets de type `Parser`</li>
 *  <li> <strong>`string`</strong> est un pointeur vers une chaine de caractère (Attention la chaine de
 *  caractère est modifiée pendant la parsing, alors penser à faire une copie).</li>
 *  <li> <strong>`handler`</strong> un objet de type `_ParseHandler_` qui va contenir la fonction qui va gérer chaque caractère lu. </li>
 * </ul>
 */
Class _ParseHandlerInterpret__ = &__ParseHandlerInterpret__;

/***************************************************/
/************** Corps des fonctions ****************/

// Constructor
static void* ParseHandlerInterpret_constructor(void* self_, va_list* args){
        
    // Vérificiation de self et args
    assert(self_);
    assert(args);
        
    // Conversion en objet de type ParseHandlerInterpret
    ParseHandlerInterpret self = (ParseHandlerInterpret) self_;
    
    self->action_on_parse_finish =  0;

    return self_;
}


// Destructor
static void* ParseHandlerInterpret_destructor(void* self_){
    
    // Vérfication de self_
    assert(self_);
    
    // Rien à desallouer

    return self_;
}



















