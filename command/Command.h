/**
 * @file Command.h
 * @brief  Fichier d'en-tête contenant les déclarations des variables publiques et de toutes les méthodes public de `_Command_`.
 * 
 * Ce fichier d'en-tête définie les attributs publics et privés de la classe `_Command_` et déclare la méthode abstraite 
 * `Command_execute()`.
 * 
 */


#include "../base/LinkedList.h"
#ifndef COMMAND_H 
/**@cond HIDE_THIS */
#define COMMAND_H
/**@endcond */

#ifdef COMMAND_CLASS

    // Attributs d'une commande.
    #define COMMAND_FIELDS \
        const char* const name; /* Nom de la commande */ \
        int (*const execute) (struct _Command_*); /* Fonction qui va faire exécuter la commande */\
        LinkedList params; /* L'ensemble des paramètres de la commande */ \
        LinkedList args; /* L'ensemble des arguments de la commande */  
        // Ajouter d'autres attributs ici.
    
    typedef struct _Command_* Command;
        
#else
    
    /** @cond HIDE_THIS*/
    // Champs privées des commandes.
    typedef struct __command_private_fields{
        LinkedList params;
        LinkedList args;
    } __command_priv_fields;
    
    // Champs visible depuis l'extérieur, les champs privés sont cachés par un pointeur opaque
    #define COMMAND_H_FIELDS \
        const char* const name; \
        int (*const execute) (struct _Command_*); \
        const char _[sizeof(__command_priv_fields)];
    /**@endcond*/
    
    /**
     * @brief Classe de toutes les commandes.
     * 
     * Il s'agit d'une classe abstraite qui représente une commande interne du shell. 
     * Elle possède une méthode abstraite à redéfinir dans tout sous-type de commande 
     * (sinon cela va déclancher une erreur), qui est la méthode `execute()` appelée par 
     * `Command_execute()`, la fonction appelée pour exécuter la commande.
     * 
     * Construction : <code><strong>obj_new(\_Command\_, "Nom_de_la_commande" )</strong></code>
     */
    typedef struct _Command_ {
        Class _Command_;
        COMMAND_H_FIELDS
    }* Command;
        
#endif
    
    /**
     * @internal
     * @brief Objet Class de toutes les commandes.
     * 
     * Construction : `obj_new(_Command_, <Nom_de_la_commande> )`
     */
    extern Class _Command_;
    
    /**
     * @brief Fonction qui va exécuter la commande.
     *
     * Cette fonction est abstraite (déclanche une erreur si aucune redéfinition
     * n'est trouvée), à chaque appel la fonction va appeller la méthode `execute()` 
     * de l'objet (liason dynamique). Cela va permettre plus tard de créer une 
     * file de commandes toutes distinctes et de les éxécuter.
     * 
     * @param self La commande à faire exécuter.
     * @return La valeur de retour de la commande après exécution.
     */
    int Command_execute(Command self);

#endif
