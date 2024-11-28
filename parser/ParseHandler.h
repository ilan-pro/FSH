/**
 * @file ParseHandler.h
 * @brief Fichier contenant les déclarations des fonctions de l'interface de la classe `_ParseHandler_`.
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

#ifndef PARSE_HANDLER_H
#define PARSE_HANDLER_H
#include "ParseHandlerInterpret.h"

#ifdef PARSE_HANDLER_CLASS
    
    #define PARSE_HANDLER_FIELDS \
        ParseHandlerInterpret interpretor; /** Il s'agit de l'interprète, l'objet qui va indiquer les actions à faire lorsqu'on rencontre certains caractères */\
        void ( * interpret ) (ParseHandler, Parser, char); /** Fonction qui va interpréter un caractère lu par le parseur */
    
    typedef struct _ParseHandler_* ParseHandler;    

#else

    /**@cond HIDE_THIS*/
    
    typedef struct __parse_handler_prv_f {
        ParseHandlerInterpret interpretor; /** Il s'agit de l'interprète, l'objet qui va indiquer les actions à faire lorsqu'on rencontre certains caractères */
    } __parse_handler_private_fields;    

    #define PARSE_HANDLER_H_FIELDS \
        const char _parser_hand [ sizeof(__parse_handler_private_fields) ];
    
    /**@endcond*/


    /**
     * @brief Classe des objets de type `ParseHandler`.
     *
     * Un objet ParseHandler est un objet qui défini comment interpréter les différents caractères lus dans 
     * un objet de type `_Parser_`, il le fait par le biais d'une fonction que le parseur va appeller 
     * à chaque caractère lu.
     * <br>
     *
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
    typedef struct _ParseHandler_{
        /**
         * @brief Classe des objets de type `ParseHandler`.
         *
         * Construction : <code><strong> obj_new(`_ParseHandler_`,  ParseHandlerInterpret interpretor ) </strong></code><br>
         * où : <br>
         * <ul>
         *  <li> `_ParseHandler_` désigne la classe des objets de type `ParseHandler`</li> 
         *  <li> <strong>`interpretor`</strong> est un objet `ParseHandlerInterpret` qui va interpréter les différents caracètres "spéciaux" reconnu par l'objet `ParseHandler`.
         * </ul>
         */
        Class _ParseHandler_;
        PARSE_HANDLER_H_FIELDS
    }* ParseHandler;

#endif
        
    /**
     * @cond HIDE_THIS
     *
     * Il s'agit d'un objet Parser défini dans `Parser.h`, on ne peut pas l'inclure ici 
     * car `Parser.h` inclu déjà ce header (cela créerait une inclusion circulaire).
     *
     * Donc on le déclare tout simplement ici, et le linker se chargera du reste.
     * C'est largement suffisant pour définir des pointeurs `struct _Parser_ *` qui sont 
     * des `Parser`.
     */
    typedef struct _Parser_* Parser;
    /**@endcond*/

    /**
     * @brief Classe des objets de type `ParseHandler`.
     *
     * Construction : <code><strong> obj_new(`_ParseHandler_`,  ParseHandlerInterpret interpretor ) </strong></code><br>
     * où : <br>
     * <ul>
     *  <li> `_ParseHandler_` désigne la classe des objets de type `ParseHandler`</li> 
     *  <li> <strong>`interpretor`</strong> est un objet `ParseHandlerInterpret` qui va interpréter les différents caracètres "spéciaux" reconnu par l'objet `ParseHandler`.
     * </ul>
     */
    extern Class _ParseHandler_;

    // Méthodes publics
        
    /**
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
    void ParseHandler_interpret(ParseHandler self, Parser parser, char c);

    /**
     * @brief Fonction qui exécute des instructions à la fin du parsing de la chaine, par le parseur.
     * 
     * Elle peut être utile pour travailler sur la dernière valeur qui se trouve dans le buffer à la fin 
     * du parsing.
     * 
     * La fonction va appeller la fonction `ParseHandlerInterpret_action_on_parse_finish()` de son 
     * interpreteur. Si elle n'est pas définie aucune erreur n'est pas levée.
     * 
     * @param self L'objet `ParseHandler` qui va gérer la fin du parsing.
     * @param parser Le parseur qui vient de finir son parsing.
     *  
     */
    void ParseHandler_action_on_parse_finish(ParseHandler self, Parser parser);


#endif

