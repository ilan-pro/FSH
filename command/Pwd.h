

/**
 * @file Pwd.h
 * @brief Fichier d'en-tête contenant les déclarations des variables publiques et de toutes les méthodes public de Pwd.
 * 
 * Construction : <code><strong>obj_new(_Pwd_, "pwd");</strong></code>
 * `_Pwd_` désigne la classe des commandes pwd. et la chaine "pwd" est le nom de la commande.
 */

#ifndef PWD_H
#define PWD_H
#ifdef PWD_CLASS
#define SIMPLE_CMD_CLASS
#include "SimpleCmd.h"
    
    #define PWD_FILEDS \
        SIMPLE_CMD_FIELDS /* Doit toujours être déclaré en premier pour pouvoir réduire l'objet à un objet de type Command pour le polymorphisme */\ 
    
    typedef struct _Pwd_* Pwd;
    
#else
#include "SimpleCmd.h"
    
    /** @cond HIDE_THIS*/
    typedef struct __pwd_private_fields{
       // Attribut qu'on veut rendre privée ici. 
    } __pwd_private_fields;

    #define PWD_H_FIELDS \
        SIMPLE_CMD_H_FIELDS /* Doit toujours être déclaré en premier pour pouvoir réduire l'objet à un objet de type Command pour le polymorphisme */ \
        const char _Pwd_private[sizeof(__pwd_private_fields)];
    /**@endcond*/    

    /**
     * @internal
     * @brief Classe des commandes pwd.
     *
     * `pwd` est la commande qui permet d'afficher le chemin 
     * absolue du vers le répertoire dans lequel on se 
     * trouve.
     *
     * Construction <code><strong> obj_new( \_Pwd\_ )</strong>. </code>
     */
    typedef struct _Pwd_ {
       Class _Pwd_;
       PWD_H_FIELDS
    }* Pwd;

#endif

    /**
     * @internal
     * @brief Classe des commandes pwd.
     *
     * `pwd` est la commande qui permet d'afficher le chemin 
     * absolue du vers le répertoire dans lequel on se 
     * trouve.
     *
     * Construction : <code><strong>obj_new(_Pwd_, "pwd");</strong></code><br>   
     * `_Pwd_` désigne la classe des commandes pwd. et la chaine "pwd" est le nom de la commande.
     */
    extern const Class _Pwd_;

#endif
