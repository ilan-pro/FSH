/**
 * @file CommandParseHandlerInterpret.h
 * @brief Fichier contenant les déclarations des fonctions de l'interface de la classe `_CommandParseHandlerInterpret_`.
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
  * où `_CommandParseHandlerInterpret_` désigne la classe des objets de type `ParseHandlerInterpret`
 */

#ifndef COMMAND_PARSE_HANDLER_INTERPRET_H
#define COMMAND_PARSE_HANDLER_INTERPRET_H

#ifdef  COMMAND_PARSE_HANDLER_INTERPRET_CLASS
    #define PARSE_HANDLER_INTERPRET_CLASS
    #include "ParseHandlerInterpret.h"

    #define  COMMAND_PARSE_HANDLER_INTERPRET_FIELDS \
        PARSE_HANDLER_INTERPRET_FIELDS \
        void (*semicolon)(CommandParseHandlerInterpret, Parser); \
        void (*pipe)(CommandParseHandlerInterpret, Parser); \
        void (*space)(CommandParseHandlerInterpret, Parser); \
        void (*double_quote)(CommandParseHandlerInterpret, Parser); \
        void (*quote)(CommandParseHandlerInterpret, Parser); \
        void (*minus)(CommandParseHandlerInterpret, Parser); \
        void (*backslsh)(CommandParseHandlerInterpret, Parser); \
        void (*lt)(CommandParseHandlerInterpret, Parser); \
        void (*gt)(CommandParseHandlerInterpret, Parser); \
        void (*opening_bracket)(CommandParseHandlerInterpret, Parser); \
        void (*closing_bracket)(CommandParseHandlerInterpret, Parser); \
        void (*dollar_sign)(CommandParseHandlerInterpret, Parser); 

    typedef struct _CommandParseHandlerInterpret_* CommandParseHandlerInterpret;    

#else
    #include "ParseHandlerInterpret.h"
    /**@cond HIDE_THIS */
    typedef struct _CommandParseHandlerInterpret_* CommandParseHandlerInterpret;

    typedef struct __command_parse_handler_interp_prv_f {
        void (*semicolon)(CommandParseHandlerInterpret, Parser); 
        void (*pipe)(CommandParseHandlerInterpret, Parser); 
        void (*space)(CommandParseHandlerInterpret, Parser); 
        void (*double_quote)(CommandParseHandlerInterpret, Parser); 
        void (*quote)(CommandParseHandlerInterpret, Parser); 
        void (*minus)(CommandParseHandlerInterpret, Parser); 
        void (*backslsh)(CommandParseHandlerInterpret, Parser); 
        void (*lt)(CommandParseHandlerInterpret, Parser); 
        void (*gt)(CommandParseHandlerInterpret, Parser); 
        void (*opening_bracket)(CommandParseHandlerInterpret, Parser); 
        void (*closing_bracket)(CommandParseHandlerInterpret, Parser); 
        void (*dollar_sign)(CommandParseHandlerInterpret, Parser); 
    } __command_parse_handler_interpret_private_fields;    

    #define  COMMAND_PARSE_HANDLER_INTERPRET_H_FIELDS \
        PARSE_HANDLER_INTERPRET_H_FIELDS \
        const char _command_parse_hand_interp [ sizeof(__command_parse_handler_interpret_private_fields) ];

    /**@endcond*/


    /**
     * @brief Classe des objets de type `CommandParseHandlerInterpret`.
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
     * où `_CommandParseHandlerInterpret_` désigne la classe des objets de type `ParseHandlerInterpret`
     */
    typedef struct _CommandParseHandlerInterpret_{
       /**
         * @brief Classe des objets de type `CommandParseHandlerInterpret`.
         *
         * Construction : <code><strong> obj_new(`_CommandParseHandlerInterpret_`) </strong></code><br>
         * où `_CommandParseHandlerInterpret_` désigne la classe des objets de type `ParseHandlerInterpret`
         */
        Class _CommandParseHandlerInterpret_;
        PARSE_HANDLER_INTERPRET_H_FIELDS
    }* CommandParseHandlerInterpret;

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

    // Méthodes publics

     /**
     * @brief Classe des objets de type `CommandParseHandlerInterpret`.
     *
     * Construction : <code><strong> obj_new(`_CommandParseHandlerInterpret_`) </strong></code><br>
     * où `_CommandParseHandlerInterpret_` désigne la classe des objets de type `ParseHandlerInterpret`
     */
    extern Class _CommandParseHandlerInterpret_;
        
    /**
     * @brief Fonction qui exécute des insturctions spécifiques au moment où le 
     * parseur lit un ';'.
     *
     * Par défaut la fonction accepte, c'est à dire dit au parseur d'ajouter le caractère 
     * à son buffer.
     * 
     * @see Parser.h
     */
    void CommandParseHandlerInterpret_semicolon(CommandParseHandlerInterpret self, Parser parser);

    /**
     * @brief Fonction qui exécute des insturctions spécifiques au moment où le 
     * parseur lit un '|'.
     *
     * Par défaut la fonction accepte, c'est à dire dit au parseur d'ajouter le caractère 
     * à son buffer.
     * 
     * @see Parser.h
     */
    void CommandParseHandlerInterpret_pipe(CommandParseHandlerInterpret self, Parser parser);

    /**
     * @brief Fonction qui exécute des insturctions spécifiques au moment où le 
     * parseur lit un ' '.
     *
     * Par défaut la fonction accepte, c'est à dire dit au parseur d'ajouter le caractère 
     * à son buffer.
     * 
     * @see Parser.h
     */
    void CommandParseHandlerInterpret_space(CommandParseHandlerInterpret self, Parser parser);

    /**
     * @brief Fonction qui exécute des insturctions spécifiques au moment où le 
     * parseur lit un '"'.
     *
     * Par défaut la fonction accepte, c'est à dire dit au parseur d'ajouter le caractère 
     * à son buffer.
     * 
     * @see Parser.h
     */
    void CommandParseHandlerInterpret_double_quote(CommandParseHandlerInterpret self, Parser parser);

    /**
     * @brief Fonction qui exécute des insturctions spécifiques au moment où le 
     * parseur lit un "'" (guillemet simple).
     *
     * Par défaut la fonction accepte, c'est à dire dit au parseur d'ajouter le caractère 
     * à son buffer.
     * 
     * @see Parser.h
     */
    void CommandParseHandlerInterpret_quote(CommandParseHandlerInterpret self, Parser parser);

    /**
     * @brief Fonction qui exécute des insturctions spécifiques au moment où le 
     * parseur lit un '-'.
     *
     * Par défaut la fonction accepte, c'est à dire dit au parseur d'ajouter le caractère 
     * à son buffer.
     * 
     * @see Parser.h
     */
    void CommandParseHandlerInterpret_minus(CommandParseHandlerInterpret self, Parser parser);

    /**
     * @brief Fonction qui exécute des insturctions spécifiques au moment où le 
     * parseur lit un '\'.
     *
     * Par défaut la fonction accepte, c'est à dire dit au parseur d'ajouter le caractère 
     * à son buffer.
     * 
     * @see Parser.h
     */
    void CommandParseHandlerInterpret_backslsh(CommandParseHandlerInterpret self, Parser parser);

    /**
     * @brief Fonction qui exécute des insturctions spécifiques au moment où le 
     * parseur lit un '<' (pour les redirections en général).
     *
     * Par défaut la fonction accepte, c'est à dire dit au parseur d'ajouter le caractère 
     * à son buffer.
     * 
     * @see Parser.h
     */
    void CommandParseHandlerInterpret_lt(CommandParseHandlerInterpret self, Parser parser);

    /**
     * @brief Fonction qui exécute des insturctions spécifiques au moment où le 
     * parseur lit un '>'. (pour les redirections en général)
     *
     * Par défaut la fonction accepte, c'est à dire dit au parseur d'ajouter le caractère 
     * à son buffer.
     * 
     * @see Parser.h
     */
    void CommandParseHandlerInterpret_gt(CommandParseHandlerInterpret self, Parser parser);

    /**
     * @brief Fonction qui exécute des insturctions spécifiques au moment où le 
     * parseur lit un '{'.
     *
     * Par défaut la fonction accepte, c'est à dire dit au parseur d'ajouter le caractère 
     * à son buffer.
     * 
     * @see Parser.h
     */
    void CommandParseHandlerInterpret_opening_bracket (CommandParseHandlerInterpret self, Parser parser);

    /**
     * @brief Fonction qui exécute des insturctions spécifiques au moment où le 
     * parseur lit un '}'.
     *
     * Par défaut la fonction accepte, c'est à dire dit au parseur d'ajouter le caractère 
     * à son buffer.
     * 
     * @see Parser.h
     */
    void CommandParseHandlerInterpret_closing_bracket (CommandParseHandlerInterpret self, Parser parser);

    /**
     * @brief Fonction qui exécute des insturctions spécifiques au moment où le 
     * parseur lit un '$'.
     *
     * Par défaut la fonction accepte, c'est à dire dit au parseur d'ajouter le caractère 
     * à son buffer.
     * 
     * @see Parser.h
     */
    void CommandParseHandlerInterpret_dollar_sign (CommandParseHandlerInterpret self, Parser parser);

#endif

