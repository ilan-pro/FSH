/**
 * @file CommandParseHandlerInterpret.c
 * @brief Fichier contenant l'implémentation de la classe `_CommandParseHandlerInterpret_`.
 *
 * Un objet ParseHandlerInterpret est un objet qui possède un certain nombre de fonctions qui seront appelées par l'objet 
 * `_ParseHandler_` qui le contient, ces fonctions doivent définir les différentes actions à faire au moment de la 
 * rencontre de certains caractères spéciaux reconnu par l'objet `_ParseHandler_`.
 *  
 * Cette classe est un sous-type qui définie des fonctions pour gérer les caractères spéciaux des commandes du shell tels que 
 * ';', '|', etc.
 * 
 * Cette classe a les méthodes public qui doivent toutes être redéfinies (mais ne déclanchent pas d'erreur si ce n'est pas le cas) elles ont 
 * une implémentation par défaut, qui d'accepte (ajoute le caractère au tampon du parser) tous les caractères spéciaux. Donc 
 * le mieux c'est de redéfinir celles dont on souhaite interpréter autrement.
 * 
 * Construction : <code><strong> obj_new(`_CommandParseHandlerInterpret_`) </strong></code><br>
  * où `_CommandParseHandlerInterpret_` désigne la classe des objets de type `CommandParseHandlerInterpret`
 */

#include "../base/new.h"
#include <assert.h>

/** @cond HIDE_THIS */
#define COMMAND_PARSE_HANDLER_INTERPRET_CLASS "CommandParseHandlerInterpret"
#include "CommandParseHandlerInterpret.h"
#include "Parser.h"
/** @endcond */



/*****************************************************/
/************ Déclaration des fonctions **************/

/**
 * Constructeur d'un objet CommandParseHandlerInterpret.
 *
 * Construction : <code><strong> obj_new(`_CommandParseHandlerInterpret_`) </strong></code><br>
 * où `_CommandParseHandlerInterpret_` désigne la classe des objets de type `CommandParseHandlerInterpret`
 */
static void* CommandParseHandlerInterpret_constructor (void*, va_list*);
    
/**
 * Fonction qui va détruire l'objet `self` 
 */
static void* CommandParseHandlerInterpret_destructor( void* );

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
 * Cette classe est un sous-type qui définie des fonctions pour gérer les caractères spéciaux des commandes du shell tels que 
 * ';', '|', etc.
 * 
 * Cette classe a les méthodes public qui doivent toutes être redéfinies (mais ne déclanchent pas d'erreur si ce n'est pas le cas) elles ont 
i * une implémentation par défaut, qui d'accepte (ajoute le caractère au tampon du parser) tous les caractères spéciaux. Donc 
 * le mieux c'est de redéfinir celles dont on souhaite interpréter autrement.
 * 
 * Construction : <code><strong> obj_new(`_CommandParseHandlerInterpret_`) </strong></code><br>
 * où `_CommandParseHandlerInterpret_` désigne la classe des objets de type `CommandParseHandlerInterpret`
 */
typedef struct _CommandParseHandlerInterpret_ {
     Class _CommandParseHandlerInterpret_;
     COMMAND_PARSE_HANDLER_INTERPRET_FIELDS
}* CommandParseHandlerInterpret;

static struct _Class_ __CommandParseHandlerInterpret__ = {
    NULL,
    COMMAND_PARSE_HANDLER_INTERPRET_CLASS,
    sizeof(struct _CommandParseHandlerInterpret_),
    CommandParseHandlerInterpret_constructor,
    CommandParseHandlerInterpret_destructor,
    0,
    0,
    0,
    0
};


/**
 * @internal
 * @brief Classe des objets de type `Parser`.
 * 
 * Un objet ParseHandlerInterpret est un objet qui possède un certain nombre de fonctions qui seront appelées par l'objet 
 * `_ParseHandler_` qui le contient, ces fonctions doivent définir les différentes actions à faire au moment de la 
 * rencontre de certains caractères spéciaux reconnu par l'objet `_ParseHandler_`.
 *  
 * Cette classe est un sous-type qui définie des fonctions pour gérer les caractères spéciaux des commandes du shell tels que 
 * ';', '|', etc.
 * 
 * Cette classe a les méthodes public qui doivent toutes être redéfinies (mais ne déclanchent pas d'erreur si ce n'est pas le cas) elles ont 
 * une implémentation par défaut, qui d'accepte (ajoute le caractère au tampon du parser) tous les caractères spéciaux. Donc 
 * le mieux c'est de redéfinir celles dont on souhaite interpréter autrement.
 * 
 * Construction : <code><strong> obj_new(`_CommandParseHandlerInterpret_`) </strong></code><br>
 * où `_CommandParseHandlerInterpret_` désigne la classe des objets de type `CommandParseHandlerInterpret`
 */
Class _CommandParseHandlerInterpret_ = &__CommandParseHandlerInterpret__;

/***************************************************/
/************** Corps des fonctions ****************/

// Constructor
static void* CommandParseHandlerInterpret_constructor(void* self_, va_list* args){
        
    // Vérificiation de self et args
    assert(self_);
    assert(args);
        
    // Conversion en objet de type ParseHandlerInterpret
    CommandParseHandlerInterpret self = _ParseHandlerInterpret_->ctor(self_, args);

    // mise à jour du parent
    *(Class*)&self->_CommandParseHandlerInterpret_->_super_ = _ParseHandlerInterpret_;
    // Toutes les fonctions sont à 0 (à cause de calloc()).
    
    return self;
}


// Destructor
static void* CommandParseHandlerInterpret_destructor(void* self_){
    
    // Vérfication de self_
    assert(self_);
    
    // Rien à desallouer

    return self_;
}


    /**
     * @internal
     * @brief Fonction qui exécute des insturctions spécifiques au moment où le 
     * parseur lit un ';'.
     *
     * Par défaut la fonction accepte, c'est à dire dit au parseur d'ajouter le caractère 
     * à son buffer.
     * 
     * @see Parser.h
     */
    void CommandParseHandlerInterpret_semicolon(CommandParseHandlerInterpret self, Parser parser){
           
        if (self->semicolon)
            self->semicolon(self, parser);
        else 
            Parser_accept(parser);
    
    }

    /**
     * @internal
     * @brief Fonction qui exécute des insturctions spécifiques au moment où le 
     * parseur lit un '|'.
     *
     * Par défaut la fonction accepte, c'est à dire dit au parseur d'ajouter le caractère 
     * à son buffer.
     * 
     * @see Parser.h
     */
    void CommandParseHandlerInterpret_pipe(CommandParseHandlerInterpret self, Parser parser){
           
        if (self->pipe)
            self->pipe(self, parser);
        else 
            Parser_accept(parser);
    
    }

    /**
     * @internal
     * @brief Fonction qui exécute des insturctions spécifiques au moment où le 
     * parseur lit un ' '.
     *
     * Par défaut la fonction accepte, c'est à dire dit au parseur d'ajouter le caractère 
     * à son buffer.
     * 
     * @see Parser.h
     */
    void CommandParseHandlerInterpret_space(CommandParseHandlerInterpret self, Parser parser){
           
        if (self->space)
            self->space(self, parser);
        else 
            Parser_accept(parser);
    
    }

    /**
     * @internal
     * @brief Fonction qui exécute des insturctions spécifiques au moment où le 
     * parseur lit un '"'.
     *
     * Par défaut la fonction accepte, c'est à dire dit au parseur d'ajouter le caractère 
     * à son buffer.
     * 
     * @see Parser.h
     */
    void CommandParseHandlerInterpret_double_quote(CommandParseHandlerInterpret self, Parser parser){
           
        if (self->double_quote)
            self->double_quote(self, parser);
        else 
            Parser_accept(parser);
    
    }

    /**
     * @internal
     * @brief Fonction qui exécute des insturctions spécifiques au moment où le 
     * parseur lit un "'" (guillemet simple).
     *
     * Par défaut la fonction accepte, c'est à dire dit au parseur d'ajouter le caractère 
     * à son buffer.
     * 
     * @see Parser.h
     */
    void CommandParseHandlerInterpret_quote(CommandParseHandlerInterpret self, Parser parser){
           
        if (self->quote)
            self->quote(self, parser);
        else 
            Parser_accept(parser);
    
    }

    /**
     * @internal
     * @brief Fonction qui exécute des insturctions spécifiques au moment où le 
     * parseur lit un '-'.
     *
     * Par défaut la fonction accepte, c'est à dire dit au parseur d'ajouter le caractère 
     * à son buffer.
     * 
     * @see Parser.h
     */
    void CommandParseHandlerInterpret_minus(CommandParseHandlerInterpret self, Parser parser){
           
        if (self->minus)
            self->minus(self, parser);
        else 
            Parser_accept(parser);
    
    }

    /**
     * @internal
     * @brief Fonction qui exécute des insturctions spécifiques au moment où le 
     * parseur lit un '\'.
     *
     * Par défaut la fonction accepte, c'est à dire dit au parseur d'ajouter le caractère 
     * à son buffer.
     * 
     * @see Parser.h
     */
    void CommandParseHandlerInterpret_backslsh(CommandParseHandlerInterpret self, Parser parser){
           
        if (self->backslsh)
            self->backslsh(self, parser);
        else 
            Parser_accept(parser);
    
    }

    /**
     * @internal
     * @brief Fonction qui exécute des insturctions spécifiques au moment où le 
     * parseur lit un '<' (pour les redirections en général).
     *
     * Par défaut la fonction accepte, c'est à dire dit au parseur d'ajouter le caractère 
     * à son buffer.
     * 
     * @see Parser.h
     */
    void CommandParseHandlerInterpret_lt(CommandParseHandlerInterpret self, Parser parser){
           
        if (self->lt)
            self->lt(self, parser);
        else 
            Parser_accept(parser);
    
    }

    /**
     * @internal
     * @brief Fonction qui exécute des insturctions spécifiques au moment où le 
     * parseur lit un '>'. (pour les redirections en général)
     *
     * Par défaut la fonction accepte, c'est à dire dit au parseur d'ajouter le caractère 
     * à son buffer.
     * 
     * @see Parser.h
     */
    void CommandParseHandlerInterpret_gt(CommandParseHandlerInterpret self, Parser parser){
           
        if (self->gt)
            self->gt(self, parser);
        else 
            Parser_accept(parser);
    
    }

    /**
     * @internal
     * @brief Fonction qui exécute des insturctions spécifiques au moment où le 
     * parseur lit un '{'.
     *
     * Par défaut la fonction accepte, c'est à dire dit au parseur d'ajouter le caractère 
     * à son buffer.
     * 
     * @see Parser.h
     */
    void CommandParseHandlerInterpret_opening_bracket (CommandParseHandlerInterpret self, Parser parser){
           
        if (self->opening_bracket)
            self->opening_bracket(self, parser);
        else 
            Parser_accept(parser);
    
    }

    /**
     * @internal
     * @brief Fonction qui exécute des insturctions spécifiques au moment où le 
     * parseur lit un '}'.
     *
     * Par défaut la fonction accepte, c'est à dire dit au parseur d'ajouter le caractère 
     * à son buffer.
     * 
     * @see Parser.h
     */
    void CommandParseHandlerInterpret_closing_bracket (CommandParseHandlerInterpret self, Parser parser){
           
        if (self->closing_bracket)
            self->closing_bracket(self, parser);
        else 
            Parser_accept(parser);
    
    }

    /**
     * @internal
     * @brief Fonction qui exécute des insturctions spécifiques au moment où le 
     * parseur lit un '$'.
     *
     * Par défaut la fonction accepte, c'est à dire dit au parseur d'ajouter le caractère 
     * à son buffer.
     * 
     * @see Parser.h
     */
    void CommandParseHandlerInterpret_dollar_sign (CommandParseHandlerInterpret self, Parser parser){
           
        if (self->dollar_sign)
            self->dollar_sign(self, parser);
        else 
            Parser_accept(parser);
    
    }

















