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

    
    typedef struct _For_* For;
    
#else
#include "Command.h"
    
    /** @cond HIDE_THIS*/
    typedef struct __for_private_fields{
       ArrayList commands; 
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
    
    /**
     * @internal 
     * @brief Fonction qui renvoie le nom du fichier identifié par un identificateur (ex : $F ou $Poisson).
     *
     * Dans le fichier `for.c` se trouvent les identificateurs données aux noms des fichiers 
     * en cours de traitement dans la boucle ainsi que le nom des fichiers qu'ils désignent. 
     * 
     * La fonction va donc rechercher l'indentificateur et renvoyer le chaine contenant le nom du fichier 
     * correspondant.
     *
     * @param id Identificateur du fichier.
     * @return Le nom du fichier identifié, ou NULL si l'identificateur n'existe pas dans la boucle.
     */
    char* For_get_file(char* id);
    
    /**
     * @internal 
     * @brief Fonction qui va renvoyer si oui ou non il existe 
     * une boucle for en cours d'exécution.
     *
     * Cette fonction va servir au moment de la rencontre d'un caractère `}` 
     * pendant le parsing de la saisie de l'utilisateur pour tester si oui ou non 
     * il fait partie d'une boucle for (ou `if` pour la fonction définie dans `If.h`).
     *
     * @return Si oui ou non il y a une boucle en cours d'exécution.
     */
    int For_exists();
    

    /** 
     * @brief Il s'agit de la liste qui va contenir les identificateurs des différentes variables 
     * utilisées dans les boucles `for`.
     *
     * L'indice de l'identificateur est le même que celui du nom du fichier contenu dans 
     * L'autre liste qui contient les noms (une HashMap serait plus intérressant mais pas le temps).
     */
    extern ArrayList For_ids;
    
    /**
     * @brief Il s'agit de la liste contenant les noms des fichiers en cours de 
     * traitement dans la boucle `for`.
     */
    extern ArrayList For_files;

#endif





