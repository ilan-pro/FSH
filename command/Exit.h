

/**
 * @file Exit.h
 * @brief Fichier d'en-tête contenant les déclarations des variables publiques et de toutes les méthodes public de Exit.
 * 
 * Construction : <code><strong>obj_new(_Exit_, "exit");</strong></code>
 * `_Exit_` désigne la classe des commandes exit. et la chaine "exit" est le nom de la commande.
 */

#ifndef EXIT_H
#define EXIT_H
#ifdef EXIT_CLASS
#define SIMPLE_CMD_CLASS
#include "SimpleCmd.h"
    
    #define EXIT_FILEDS \
        SIMPLE_CMD_FIELDS /* Doit toujours être déclaré en premier pour pouvoir réduire l'objet à un objet de type Command pour le polymorphisme */\ 
    
    typedef struct _Exit_* Exit;
    
#else
#include "SimpleCmd.h"
    
    /** @cond HIDE_THIS*/
    typedef struct __exit_private_fields{
       // Attribut qu'on veut rendre privée ici. 
    } __exit_private_fields;

    #define EXIT_H_FIELDS \
        SIMPLE_CMD_H_FIELDS /* Doit toujours être déclaré en premier pour pouvoir réduire l'objet à un objet de type Command pour le polymorphisme */ \
        const char _Exit_private[sizeof(__exit_private_fields)];
    /**@endcond*/    

    /**
     * @internal
     * @brief Classe des commandes exit.
     *
     * `exit` est la commande qui permet de quitter le terminal.
     *
     * Construction <code><strong> obj_new( \_Exit\_ )</strong>. </code>
     */
    typedef struct _Exit_ {
       Class _Exit_;
       EXIT_H_FIELDS
    }* Exit;

#endif

    /**
     * @internal
     * @brief Classe des commandes exit.
     *
     * `exit` est la commande qui permet de quitter le terminal.
     *
     * Construction : <code><strong>obj_new(_Exit_, "exit");</strong></code><br>   
     * `_Exit_` désigne la classe des commandes exit. et la chaine "exit" est le nom de la commande.
     */
    extern const Class _Exit_;

#endif
