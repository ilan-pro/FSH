
/**
 * @file CommandParseHandler.c
 * @brief Fichier contenantl'implémentation de la classe `_CommandPromptHandler_` sous-type de `_ParseHandler_`.
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
 * <br> Cette classe est un sous-type de `_ParseHandler_` reservée à l'extraction des commandes à partir du prompt de l'utilisateur. 
 * L'objet de type `_CommandPromptHandler_` reconnait donc certains caractères qu'il considère comme spéciaux ('-', ';', '|', etc.). 
 * Il appelle son interpreteur de type `_CommandPromptHandlerInterpret_` pour qu'il les interprète. <p>
 * 
 * Par exemple un caractère `;` pour un interpreteur qui crée et ajoute des pipelines des commandes, signifie pour lui la fin d'une pipeline et donc
 * qu'il doit ajouter la pipline courante dans la file principale et en créer une nouvelle pour la suite, tandis que pour un interpreteur qui 
 * construit une commande ce caractère signifie qu'il doit stoper son parseur car la construction de la commande est terminée.
 *
 * Construction : <code><strong> obj_new(`_CommandParseHandler_`,  ParseHandlerInterpret interpretor ) </strong></code><br>
 * où : <br>
 * <ul>
 *  <li> `_CommandParseHandler_` désigne la classe des objets de type `CommandParseHandler`</li> 
 *  <li> <strong>`interpretor`</strong> est un objet `_CommandPromptHandlerInterpret_` qui va interpréter les différents caracètres "spéciaux" 
 * reconnus par l'objet `CommandParseHandler` </li>
 * </ul>
 */

#include "../base/new.h"
#include "CommandParseHandlerInterpret.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/** @cond HIDE_THIS */
#define COMMAND_PARSE_HANDLER_CLASS "CommandParseHandler"
#include "CommandParseHandler.h"
#include "Parser.h"
/** @endcond */



/*****************************************************/
/************ Déclaration des fonctions **************/

/**
 * Constructeur d'un objet CommandParseHandler.
 *
 * Le constructeur va initialiser les attributs.
 * 
 * Construction : <code><strong> obj_new(`_CommandParseHandler_`,  CommandParseHandlerInterpret interpretor ) </strong></code><br>
 * où : <br>
 * <ul>
 *  <li> `_CommandParseHandler_` désigne la classe des objets de type `CommandParseHandler`</li> 
 *  <li> <strong>`interpretor`</strong> est un objet `_CommandPromptHandlerInterpret_` qui va interpréter les différents caracètres "spéciaux" 
 * reconnus par l'objet `CommandParseHandler` </li>
 * </ul>
 */
static void* CommandParseHandler_constructor (void*, va_list*);
    
/**
 * Fonction qui va détruire l'objet `self` 
 */
static void* CommandParseHandler_destructor( void* );
    

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
 * <br> Cette classe est un sous-type de `_ParseHandler_` reservée à l'extraction des commandes à partir du prompt de l'utilisateur. 
 * L'objet de type `_CommandPromptHandler_` reconnait donc certains caractères qu'il considère comme spéciaux ('-', ';', '|', etc.). 
 * Il appelle son interpreteur de type `_CommandPromptHandlerInterpret_` pour qu'il les interprète. <p>
 * 
 * Par exemple un caractère `;` pour un interpreteur qui crée et ajoute des pipelines des commandes, signifie pour lui la fin d'une pipeline et donc
 * qu'il doit ajouter la pipline courante dans la file principale et en créer une nouvelle pour la suite, tandis que pour un interpreteur qui 
 * construit une commande ce caractère signifie qu'il doit stoper son parseur car la construction de la commande est terminée.
 *
 * Construction : <code><strong> obj_new(`_CommandParseHandler_`,  ParseHandlerInterpret interpretor ) </strong></code><br>
 * où : <br>
 * <ul>
 *  <li> `_CommandParseHandler_` désigne la classe des objets de type `CommandParseHandler`</li> 
 *  <li> <strong>`interpretor`</strong> est un objet `_CommandPromptHandlerInterpret_` qui va interpréter les différents caracètres "spéciaux" 
 * reconnus par l'objet `CommandParseHandler` </li>
 * </ul>
 */
typedef struct _CommandParseHandler_ {
    Class _CommandParseHandler_;
    COMMAND_PARSE_HANDLER_FIELDS
}* CommandParseHandler;

static struct _Class_ __CommandParseHandler__ = {
    NULL,
    COMMAND_PARSE_HANDLER_CLASS,
    sizeof(struct _CommandParseHandler_),
    CommandParseHandler_constructor,
    CommandParseHandler_destructor,
    0,
    0,
    0,
    0
};


/**
 * @internal
 * @brief Classe des objets de type `CommandParseHandler`.
 *
 * Construction : <code><strong> obj_new(`_CommandParseHandler_`,  ParseHandlerInterpret interpretor ) </strong></code><br>
 * où : <br>
 * <ul>
 *  <li> `_CommandParseHandler_` désigne la classe des objets de type `CommandParseHandler`</li> 
 *  <li> <strong>`interpretor`</strong> est un objet `_CommandPromptHandlerInterpret_` qui va interpréter les différents caracètres "spéciaux" 
 * reconnus par l'objet `CommandParseHandler` </li>
 * </ul>
 */
Class _CommandParseHandler_ = &__CommandParseHandler__;

/***************************************************/
/************** Corps des fonctions ****************/

// Constructor
static void* CommandParseHandler_constructor(void* self_, va_list* args){
        
    // Vérificiation de self et args
    assert(self_);
    assert(args);
        
    // Conversion en objet de type ParseHandler.
    CommandParseHandler self = _ParseHandler_->ctor(self_, args);
    
    // mise à jour du parent.
   *(Class *)&self->_CommandParseHandler_->_super_ = _ParseHandler_;

    return self;
}


// Destructor
static void* CommandParseHandler_destructor(void* self_){
    
    // Vérfication de self_
    assert(self_);
    assert(obj_instance_of(self_, _ParseHandler_));

    return _ParseHandler_->dtor(self_);

}


static void CommandParseHandler_action_on_semicolon(CommandParseHandler self, Parser parser){
    
    // Toutes les vérifications sont déjà faites.
    
    assert(obj_instance_of(self->interpretor, _CommandParseHandlerInterpret_));

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
 * @param self_ L'objet qui va gérer tous les caractères lu par le parseur.
 * @param parser Le parseur qui a lu le caractère (pour pouvoir lui demander de l'accepter par exemple).
 * @param c Le caractère lu par le parseur.
 */
static void CommandParseHandler_interpret(ParseHandler self_, Parser parser, char c){
    
    // Vérfication de self
    assert(self_ && parser);
    assert(obj_instance_of(self_, _CommandParseHandler_));
    
    CommandParseHandler self = (CommandParseHandler) self_;
    
    // Vérification de l'interpréteur.
    assert(obj_instance_of(self->interpretor, _CommandParseHandlerInterpret_));

    switch (c){
        
        default :
            Parser_accept(parser);
            break;

        case ';':
            CommandParseHandlerInterpret_semicolon((CommandParseHandlerInterpret) self->interpretor, parser);
            break;

        case '|':
            CommandParseHandlerInterpret_pipe((CommandParseHandlerInterpret) self->interpretor, parser);
            break;

        case ' ':
            CommandParseHandlerInterpret_space((CommandParseHandlerInterpret) self->interpretor, parser);
            break;

        case '"':
            CommandParseHandlerInterpret_double_quote((CommandParseHandlerInterpret) self->interpretor, parser);
            break;

        case '\'':
            CommandParseHandlerInterpret_quote((CommandParseHandlerInterpret) self->interpretor, parser);
            break;

        case '-':
            CommandParseHandlerInterpret_minus((CommandParseHandlerInterpret) self->interpretor, parser);
            break;

        case '\\':
            CommandParseHandlerInterpret_backslsh((CommandParseHandlerInterpret) self->interpretor, parser);
            break;

        case '<':
            CommandParseHandlerInterpret_lt((CommandParseHandlerInterpret) self->interpretor, parser);
            break;

        case '>':
            CommandParseHandlerInterpret_gt((CommandParseHandlerInterpret) self->interpretor, parser);
            break;

        case '{':
            CommandParseHandlerInterpret_opening_bracket((CommandParseHandlerInterpret) self->interpretor, parser);
            break;

        case '}':
            CommandParseHandlerInterpret_closing_bracket((CommandParseHandlerInterpret) self->interpretor, parser);
            break;

        case '$':
            CommandParseHandlerInterpret_closing_dollar_sign((CommandParseHandlerInterpret) self->interpretor, parser);
            break;

    
    } 
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

    return; 
    
}
















