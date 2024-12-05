/**
 * @file If.h
 * @brief Fichier d'en-tête contenant les déclarations des variables publiques et de toutes les méthodes public de la classe `_If_`.
 *
 * La classe `_If_` est une classe de commande permettant de contrôler l'exécution 
 * d'un bloc de commandes en l'exécutant sous une certaine condition (un ensemble de commande Test).
 * 
 * Construction <code><strong> obj_new( `_If_`, char** prompt ); </strong></code>
 * où : <ul>
 *          <li>`_If_` désigne de la classe des objets `Command` de type `If`. </li>
 *          <li><code><strong>char** prompt</strong></code> La suite du prompt (qui permet 
 *          d'initialiser le reste de la commande).</li>
 * </ul>
 *
 */

#ifndef IF_H
#define IF_H

#ifdef IF_CLASS
    #define COMMAND_CLASS
    #include "Command.h"

    #define IF_FIELDS \
        COMMAND_FIELDS \
        ArrayList test_command; \
        ArrayList if_block; \
        ArrayList else_block; \

    typedef struct _If_* If;

#else
#include "Command.h"
    /**
     * @cond HIDE_THIS
     */
    typedef struct __if_priv_fi {
        ArrayList test_command;
        ArrayList if_block;
        ArrayList else_block;
    } __if_private_fields;
    
    #define IF_H_FIEDLS \
        COMMAND_H_FIELDS \
        const char __if_private[sizeof(__if_private_fields)];
    
    /**@endcond*/
    
   /**
    * Structure de la classe `_If_`.
    *
    * Construction <code><strong> obj_new( `_If_`, char** prompt ); </strong></code>
    * où : <ul>
    *          <li>`_If_` désigne de la classe des objets `Command` de type `If`. </li>
    *          <li><code><strong>char** prompt</strong></code> La suite du prompt (qui permet 
    *          d'initialiser le reste de la commande).</li>
    * </ul>
    */
    typedef struct _If_ {
        /**
         * Classe des objets de type `_If_`.
         *
         * Construction <code><strong> obj_new( `_If_`, char** prompt ); </strong></code>
         * où : <ul>
         *          <li>`_If_` désigne de la classe des objets `Command` de type `If`. </li>
         *          <li><code><strong>char** prompt</strong></code> La suite du prompt (qui permet 
         *          d'initialiser le reste de la commande).</li>
         * </ul>
         */
        Class _If_;
        IF_H_FIEDLS
    }* If;

#endif

/**
 * Classe des objets de type `_If_`.
 *
 * Construction <code><strong> obj_new( `_If_`, char** prompt ); </strong></code>
 * où : <ul>
 *          <li>`_If_` désigne de la classe des objets `Command` de type `If`. </li>
 *          <li><code><strong>char** prompt</strong></code> La suite du prompt (qui permet 
 *          d'initialiser le reste de la commande).</li>
 * </ul>
 */
extern Class _If_;

#endif
