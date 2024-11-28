/**
 * @file ParseHandlerInterpret.h
 * @brief Fichier contenant les déclarations des fonctions de l'interface de la classe `_ParseHandlerInterpret_`.
 *
 * Un objet ParseHandlerInterpret est un objet qui possède un certain nombre de fonctions qui seront appelées par l'objet 
 * `_ParseHandler_` qui le contient, ces fonctions doivent définir les différentes actions à faire au moment de la 
 * rencontre de certains caractères spéciaux reconnu par l'objet `_ParseHandler_`.
 * 
 * Construction : <code><strong> obj_new(`_ParseHandlerInterpret_`) </strong></code><br>
  * où `_ParseHandlerInterpret_` désigne la classe des objets de type `ParseHandlerInterpret`
 */

#ifndef PARSE_HANDLER_INTERPRET_H
#define PARSE_HANDLER_INTERPRET_H
#include "../base/Class.h"

#ifdef PARSE_HANDLER_INTERPRET_CLASS
    
    #define PARSE_HANDLER_INTERPRET_FIELDS \
        void ( * action_on_parse_finish ) (ParseHandlerInterpret, Parser);
    
    typedef struct _ParseHandlerInterpret_* ParseHandlerInterpret;    

#else

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
    
    typedef struct _ParseHandlerInterpret_* ParseHandlerInterpret;
    typedef struct __parse_handler_interp_prv_f {
        /** Au cas où */
    } __parse_handler_interpret_private_fields;    

    #define PARSE_HANDLER_INTERPRET_H_FIELDS \
        void ( * action_on_parse_finish ) (ParseHandlerInterpret, Parser);\
        const char _parse_hand_interp [ sizeof(__parse_handler_interpret_private_fields) ];
    
    /**@endcond*/


    /**
     * @brief Classe des objets de type `ParseHandlerInterpret`.
     *
     * Un objet ParseHandlerInterpret est un objet qui possède un certain nombre de fonctions qui seront appelées par l'objet 
     * `_ParseHandler_` qui le contient, ces fonctions doivent définir les différentes actions à faire au moment de la 
     * rencontre de certains caractères spéciaux reconnu par l'objet `_ParseHandler_`.
     * 
     * Construction : <code><strong> obj_new(`_ParseHandlerInterpret_`) </strong></code><br>
     * où `_ParseHandlerInterpret_` désigne la classe des objets de type `ParseHandlerInterpret`
     */
    typedef struct _ParseHandlerInterpret_{
        /**
         * @brief Classe des objets de type `ParseHandlerInterpret`.
         *
         * Construction : <code><strong> obj_new(`_ParseHandlerInterpret_`) </strong></code><br>
         * où `_ParseHandlerInterpret_` désigne la classe des objets de type `ParseHandlerInterpret`
         */
        Class _ParseHandlerInterpret_;
        PARSE_HANDLER_INTERPRET_H_FIELDS
    }* ParseHandlerInterpret;

#endif
    // Méthodes publics

    /**
     * @brief Classe des objets de type `ParseHandlerInterpret`.
     *
     * Construction : <code><strong> obj_new(`_ParseHandlerInterpret_`) </strong></code><br>
     * où `_ParseHandlerInterpret_` désigne la classe des objets de type `ParseHandlerInterpret`
     */
    extern Class _ParseHandlerInterpret_;
        

#endif

