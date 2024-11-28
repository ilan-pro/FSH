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

#ifdef PARSE_HANDLER_INTERPRET_CLASS
    
    #define PARSE_HANDLER_INTERPRET_H_FIELDS \
        /** Pas d'attribut de classe */        
    
    typedef struct _ParseHandler_* ParseHandler;    

#else

    /**@cond HIDE_THIS*/
    
    typedef struct __parse_handler_interp_prv_f {
        /** Au cas où */
    } __parse_handler_interpret_private_fields;    

    #define PARSE_HANDLER_INTERPRET_H_FIELDS \
        const char _parser [ sizeof(__parse_handler_interpret_private_fields) ];
    
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
         * @brief Classe des objets de type `ParseHandler`.
         *
         * Construction : <code><strong> obj_new(`_ParseHandlerInterpret_`) </strong></code><br>
         * où `_ParseHandlerInterpret_` désigne la classe des objets de type `ParseHandlerInterpret`
         */
        Class _ParseHandlerInterpret_;
        PARSE_HANDLER_INTERPRET_H_FIELDS
    }* ParseHandler;

#endif
    // Méthodes publics
        

#endif

