/**
 * @file ExternalCommand.h
 * @brief Fichier d'en-tête contenant les déclarations des variables publiques et de toutes les méthodes public de ExternalCommand.
 * 
 * ExternalCommand est la classe des commandes externes.
 * 
 * Construction : <code><strong>obj_new(_ExternalCommand_, "nom_de_la_commande_externe");</strong></code><br>
 * `_ExternalCommand_` désigne la classe des commandes de type ExternalCommand (commandes externes). et la 
 * chaine "nom_de_la_commande_externe" doit contenir le nom de la commande.
 */

#ifndef EXTERNAL_COMMAND_H
#define EXTERNAL_COMMAND_H
#ifdef EXTERNAL_COMMAND_CLASS
#define COMMAND_CLASS
#include "Command.h"
    
    #define EXTERNAL_COMMAND_FILEDS \
        COMMAND_FIELDS /* Doit toujours être déclaré en premier pour pouvoir réduire l'objet à un objet de type Command pour le polymorphisme */\

    typedef struct _ExternalCommand_* ExternalCommand;
    
#else
#include "Command.h"
    
    /** @cond HIDE_THIS*/
    typedef struct __external_command_private_fields{
    } __external_command_private_fields;

    #define EXTERNAL_COMMAND_H_FIELDS \
        COMMAND_H_FIELDS /* Doit toujours être déclaré en premier pour pouvoir réduire l'objet à un objet de type Command pour le polymorphisme */ \
        const char _ExternalCommand_private[sizeof(__external_command_private_fields)];
    /**@endcond*/    

    /**
     * @internal
     * @brief Classe des commandes external_command.
     *
     * `external_command` est la commande qui permet d'afficher le chemin 
     * absolue du vers le répertoire dans lequel on se 
     * trouve.
     *
     * Construction <code><strong> obj_new( \_ExternalCommand\_ )</strong>. </code>
     */
    typedef struct _ExternalCommand_ {
       Class _ExternalCommand_;
       EXTERNAL_COMMAND_H_FIELDS
    }* ExternalCommand;

#endif

    /**
     * @internal
     * @brief Classe des commandes external_command.
     *
     * `external_command` est la commande qui permet d'afficher le chemin 
     * absolue du vers le répertoire dans lequel on se 
     * trouve.
     *
     * Construction : <code><strong>obj_new(_ExternalCommand_, "external_command");</strong></code><br>   
     * `_ExternalCommand_` désigne la classe des commandes external_command. et la chaine "external_command" est le nom de la commande.
     */
    extern const Class _ExternalCommand_;
    // meme variable que celle de shell.c pour pouvoir gérer les signaux
    extern volatile sig_atomic_t signal_recu;
    // pour les fonctions, les extern sont implicites
    void handler(int sig);
    void ignore_signals();
    

#endif
