

/**
 * @file Cd.h
 * @brief Fichier d'en-tête contenant les déclarations des variables publiques et de toutes les méthodes public de Cd.
 * 
 * Contruction : <code><strong>obj_new(_Cd_, "cd");</strong></code>
 * `_Cd_` désigne la classe des commandes cd. et la chaine "cd" est le nom de la commande.
 */

#ifndef CD_H
#define CD_H
#ifdef CD_CLASS
#define COMMAND_CLASS
#include "Command.h"
    
    #define CD_FILEDS \
        COMMAND_FIELDS /* Doit toujours être déclaré en premier pour pouvoir réduire l'objet à un objet de type Command pour le polymorphisme */\ 
    
    typedef struct _Cd_* Cd;
    
#else
#include "Command.h"
    
    /** @cond HIDE_THIS*/
    typedef struct __cd_private_fields{
       // Attribut qu'on veut rendre privée ici. 
    } __cd_private_fields;

    #define CD_H_FIELDS \
        COMMAND_H_FIELDS /* Doit toujours être déclaré en premier pour pouvoir réduire l'objet à un objet de type Command pour le polymorphisme */ \
        const char _Cd_private[sizeof(__cd_private_fields)];
    /**@endcond*/    

    /**
     * @internal
     * @brief Classe des commandes cd.
     *
     * Cd est une commande qui va changer le répertoire vers courant vers un nouveau répertoire.
     *
     * Construction <code><strong> obj_new( \_Cd\_ )</strong>. </code>
     */
    typedef struct _Cd_ {
       Class _Cd_;
       CD_H_FIELDS
    }* Cd;

#endif

    /**
     * @internal
     * @brief Classe des commandes cd.
     *
     * Cd est une commande qui va changer le répertoire vers courant vers un nouveau répertoire.
     *
     * Contruction : <code><strong>obj_new(_Cd_, "cd");</strong></code><br>   
     * `_Cd_` désigne la classe des commandes cd. et la chaine "cd" est le nom de la commande.
     */
    extern const Class _Cd_;

#endif
