

/**
 * @file Ftype.h
 * @brief Fichier d'en-tête contenant les déclarations des variables publiques et de toutes les méthodes public de Ftype.
 * 
 * Contruction : <code><strong>obj_new(_Ftype_, "ftype");</strong></code>
 * `_Ftype_` désigne la classe des commandes ftype. et la chaine "ftype" est le nom de la commande.
 */

#ifndef FTYPE_H
#define FTYPE_H
#ifdef FTYPE_CLASS
#define COMMAND_CLASS
#include "Command.h"
    
    #define FTYPE_FILEDS \
        COMMAND_FIELDS /* Doit toujours être déclaré en premier pour pouvoir réduire l'objet à un objet de type Command pour le polymorphisme */\ 
    
    typedef struct _Ftype_* Ftype;
    
#else
#include "Command.h"
    
    /** @cond HIDE_THIS*/
    typedef struct __ftype_private_fields{
       // Attribut qu'on veut rendre privée ici. 
    } __ftype_private_fields;

    #define FTYPE_H_FIELDS \
        COMMAND_H_FIELDS /* Doit toujours être déclaré en premier pour pouvoir réduire l'objet à un objet de type Command pour le polymorphisme */ \
        const char _Ftype_private[sizeof(__ftype_private_fields)];
    /**@endcond*/    

    /**
     * @internal
     * @brief Classe des commandes ftype.
     *
     * `ftype` est la commande qui donne le type d'un fichier.
     *
     * Construction <code><strong> obj_new( \_Ftype\_ )</strong>. </code>
     */
    typedef struct _Ftype_ {
       Class _Ftype_;
       FTYPE_H_FIELDS
    }* Ftype;

#endif

    /**
     * @internal
     * @brief Classe des commandes ftype.
     *
     * `ftype` est la commande qui donne le type d'un fichier.
     *
     * Contruction : <code><strong>obj_new(_Ftype_, "ftype");</strong></code><br>   
     * `_Ftype_` désigne la classe des commandes ftype. et la chaine "ftype" est le nom de la commande.
     */
    extern Class _Ftype_;

#endif
