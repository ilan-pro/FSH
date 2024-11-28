/**
 * @file CommandParseHandler.h
 * @brief Fichier contenant les déclarations des fonctions de l'interface de la classe `_CommandPromptHandler_` sous-type de `_ParseHandler_`.
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

#ifndef COMMAND_PARSE_HANDLER_H
#define COMMAND_PARSE_HANDLER_H

#ifdef COMMAND_PARSE_HANDLER_CLASS
    #define PARSE_HANDLER_CLASS
    #include "ParseHandler.h"
    
    #define COMMAND_PARSE_HANDLER_FIELDS \
        PARSE_HANDLER_FIELDS
    
    typedef struct _CommandParseHandler_* CommandParseHandler;    

#else
    #include "ParseHandler.h"

    /**@cond HIDE_THIS*/
    
    typedef struct __command_parse_handler_prv_f {
    } __command_parse_handler_private_fields;    

    #define COMMAND_PARSE_HANDLER_H_FIELDS \
        PARSE_HANDLER_H_FIELDS
        const char _command_parse_hand [ sizeof(__command_parse_handler_private_fields) ];
    
    /**@endcond*/


    /**
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
     * Construction : <code><strong> obj_new(`_CommandParseHandler_`,  CommandPromptHandlerInterpret interpretor ) </strong></code><br>
     * où : <br>
     * <ul>
     *  <li> `_CommandParseHandler_` désigne la classe des objets de type `CommandParseHandler`</li> 
     *  <li> <strong>`interpretor`</strong> est un objet `_CommandPromptHandlerInterpret_` qui va interpréter les différents caracètres "spéciaux" 
     * reconnus par l'objet `CommandParseHandler`.
     * </ul>
     */
    typedef struct _CommandParseHandler_{
        /**
         * @brief Classe des objets de type `CommandParseHandler`.
         *
         * Construction : <code><strong> obj_new(`_CommandParseHandler_`,  ParseHandlerInterpret interpretor ) </strong></code><br>
         * où : <br>
         * Construction : <code><strong> obj_new(`_CommandParseHandler_`,  CommandPromptHandlerInterpret interpretor ) </strong></code><br>
         * où : <br>
         * <ul>
         *  <li> `_CommandParseHandler_` désigne la classe des objets de type `CommandParseHandler`</li> 
         *  <li> <strong>`interpretor`</strong> est un objet `_CommandPromptHandlerInterpret_` qui va interpréter les différents caracètres "spéciaux" 
         * reconnus par l'objet `CommandParseHandler` </li>
         * </ul>
         */
        Class _CommandParseHandler_;
        COMMAND_PARSE_HANDLER_H_FIELDS
    }* CommandParseHandler;

#endif
        
    /**
     * @brief Classe des objets de type `CommandParseHandler`.
     *
     * Construction : <code><strong> obj_new(`_CommandParseHandler_`,  CommandPromptHandlerInterpret interpretor ) </strong></code><br>
     * où : <br>
     * <ul>
     *  <li> `_CommandParseHandler_` désigne la classe des objets de type `CommandParseHandler`</li> 
     *  <li> <strong>`interpretor`</strong> est un objet `_CommandPromptHandlerInterpret_` qui va interpréter les différents caracètres "spéciaux" 
     * reconnus par l'objet `CommandParseHandler` </li>
     * </ul>
     */
    extern Class _CommandParseHandler_;



#endif

