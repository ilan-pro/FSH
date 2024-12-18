/**
 * @file For.h
 * @brief Fichier d'en-tête contenant les déclarations des variables publiques et de toutes les méthodes public de For.
 * 
 * Construction : <code><strong>obj_new(_For_, "for");</strong></code>
 * `_For_` désigne la classe des commandes for. et la chaine "for" est le nom de la commande.
 */

#ifndef FOR_H
#define FOR_H
#ifdef FOR_CLASS
#define COMMAND_CLASS
#include "Command.h"
    
    #define FOR_FILEDS \
        COMMAND_FIELDS /* Doit toujours être déclaré en premier pour pouvoir réduire l'objet à un objet de type Command pour le polymorphisme */\
        ArrayList commands; /* Il s'agit de la commande structruée à exécuter dans le `for`.*/ \
        size_t index_dir ; /** Indice du répertoire de trvaille de For dans la liste des arguments*/\
        char* id_var; /** identificateur de la variable de boucle */
    
    typedef struct _For_* For;
    
#else
#include "Command.h"
    
    /** @cond HIDE_THIS*/
    typedef struct __for_private_fields{
       ArrayList commands; 
       char* dir;
       char* id_var;
    } __for_private_fields;

    #define FOR_H_FIELDS \
        COMMAND_H_FIELDS /* Doit toujours être déclaré en premier pour pouvoir réduire l'objet à un objet de type Command pour le polymorphisme */ \
        const char _For_private[sizeof(__for_private_fields)];
    /**@endcond*/    

    /**
     * @internal
     * @brief Classe des commandes for.
     *
     * `for` est la commande qui permet d'afficher le chemin 
     * absolue du vers le répertoire dans lequel on se 
     * trouve.
     *
     * Construction <code><strong> obj_new( \_For\_ )</strong>. </code>
     */
    typedef struct _For_ {
       Class _For_;
       FOR_H_FIELDS
    }* For;

#endif

    /**
     * @internal
     * @brief Classe des commandes for.
     * `for` est la commande qui permet d'afficher le chemin 
     *
     * absolue du vers le répertoire dans lequel on se 
     * trouve.
     *
     * Construction : <code><strong>obj_new(_For_, "for");</strong></code><br>   
     * `_For_` désigne la classe des commandes for. et la chaine "for" est le nom de la commande.
     */
    extern const Class _For_;
    

#endif





