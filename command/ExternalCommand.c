
/**
 * @file ExternalCommand.c
 * @brief Fichier contenant l'implémentation des fonctions déclarées dans `ExternalCommand.h`
 * 
 * ExternalCommand est la classe des commandes externes.
 * 
 * Construction : <code><strong>obj_new(_ExternalCommand_, "nom_de_la_commande_externe");</strong></code><br>
 * `_ExternalCommand_` désigne la classe des commandes de type ExternalCommand (commandes externes). et la 
 * chaine "nom_de_la_commande_externe" doit contenir le nom de la commande.
 */


#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../base/new.h"
#include "../shell.h"

/** @cond HIDE_THIS*/
#define EXTERNAL_COMMAND_CLASS "ExternalCommand"
#include "ExternalCommand.h"
/**@endcond*/


/*****************************************************/
/************ Déclaration des fonctions **************/

/**
 * Constructeur d'une commande EXTERNAL_COMMAND.
 * 
 * @param self_ L'objet alloué par calloc().
 * @param args La liste des arguements (Nomalement elle ne doit contenir que le nom de la commande externe).
 * @return Un objet de type ExternalCommand, initialisé.
 */
static void* ExternalCommand_constructor(void* self_, va_list* args);

/**
 * Destructeur d'un objet de type commande EXTERNAL_COMMAND.
 *
 * Le destructeur va juste appeller le destructeur du parent, 
 * qui va désallouer les ressources qui sont héritées.
 * 
 * @param self_ L'objet à désallouer.
 * @return Un objet dont tout l'intértieur desalloué.
 */
static void* ExternalCommand_destroy(void* self_);

/**
 * @brief Fonction qui va exécuter la commande.
 *
 * Cette fonction est une redéfinition de la fonction `Command_execute()` de 
 * la classe `_Command_` (appelée par laison dynamique).
 *
 * @param self La commande de type ExternalCommand à faire exécuter.
 * @return La valeur de retour de la commande après exécution.
 */
static int ExternalCommand_execute(Command self_);

/**
 * Fonction qui va afficher la commande externe.
 */
static void ExternalCommand_print(const void* self_);

/*****************************************************/
/************* Structure de la classe ****************/


/**
 * @brief Classe `ExternalCommand` de la commande ExternalCommand qui affiche 
 * le chemin absolu du répertoire courant.
 *
 * Construction : <code><strong>obj_new(_ExternalCommand_, "nom_de_la_commande_externe");</strong></code><br>
 * `_ExternalCommand_` désigne la classe des commandes de type ExternalCommand (commandes externes). et la 
 * chaine "nom_de_la_commande_externe" doit contenir le nom de la commande.
 */
typedef struct _ExternalCommand_ {
    
    /**
     * @brief Classe des commandes externes.
     *
     * ExternalCommand est la classe des commandes externes.
     * 
     * Construction : <code><strong>obj_new(_ExternalCommand_, "nom_de_la_commande_externe");</strong></code><br>
     * `_ExternalCommand_` désigne la classe des commandes de type ExternalCommand (commandes externes). et la 
     * chaine "nom_de_la_commande_externe" doit contenir le nom de la commande.
     */
    Class _ExternalCommand_;
    EXTERNAL_COMMAND_FILEDS

}* ExternalCommand;

static struct _Class_ __ExternalCommand__ = {
    
    NULL,
    EXTERNAL_COMMAND_CLASS,
    sizeof(struct _ExternalCommand_),
    ExternalCommand_constructor,
    ExternalCommand_destroy,
    0,
    0,
    0,
    ExternalCommand_print


};

/**
 * @brief Classe des commandes externes.
 *
 * ExternalCommand est la classe des commandes externes.
 * 
 * Construction : <code><strong>obj_new(_ExternalCommand_, "nom_de_la_commande_externe");</strong></code><br>
 * `_ExternalCommand_` désigne la classe des commandes de type ExternalCommand (commandes externes). et la 
 * chaine "nom_de_la_commande_externe" doit contenir le nom de la commande.
 */
const Class _ExternalCommand_ = &__ExternalCommand__;

/***************************************************/
/************** Corps des fonctions ****************/

// Constructeur.
static void* ExternalCommand_constructor(void* self_, va_list* args){
    
    // Vérification des arguments.
    assert(self_ && args);    
    
    // Conversion en objet de type ExternalCommand.
    ExternalCommand self = (ExternalCommand) self_;

    // On ajoute le parent à EXTERNAL_COMMAND (en contournant la contrainte sur const).
    *(Class*)&(self->_ExternalCommand_->_super_) = _Command_; 

        
    // Ajout de la fonction execute de la classe fille.
    *((int (**) (Command)) &(self->execute)) = ExternalCommand_execute;

    // Les champs arguments et paramètres ( qui sont des listes chainées ) ne seront pas initialisés.
    
    // Récupération du nom de la commande.
    char* cmd_name = va_arg(*args, char*);
    *(char**)&self->name = cmd_name; // Mise à jour du nom de la commande.
    
    int cmd_length = strlen(cmd_name);
    char* command = malloc((2 + cmd_length) * sizeof(char));
    assert(command);
    strcpy(command, cmd_name);
    
    command[cmd_length] = ' ';
    command[cmd_length+1] = '\0';
    cmd_length ++;

    int quoted = 0; // Booléean indiquant si oui ou non on se trouve dans des guillemets.
    int double_quoted = 0; // Booléean indiquant si oui ou non on se trouve dans des guillemets double.
    int esc = 0; // Booléean indiquant si oui ou non le caractère suivant est echappé.

    /** 
     * Lecture du prompt dans la boucle tant que le caractère pointé est 
     * valide (voir documentation de la fonction actual_char_is_legal()).
     */
    while(Command_actual_char_is_legal(quoted, double_quoted, esc)){
        
        // Récupération du caractère pointé par le pointeur, et incémentation de la posisiton.
        char c = *(usr_prompt++);
        printf("%c\n", c);
        switch (c){
            default : 
                h_default: // label 

                esc = 0; // On met l'echappement à 0 (si jamais le caractère était échappé)
                // Incrémentation du compteur.
                cmd_length++;
                // Réallocation pour accueillir le nouveau caractère.
                command = realloc(command, (1 + cmd_length) * sizeof(char));
                assert(command);
                command[cmd_length] = '\0';
                command[cmd_length- 1] = c;
                break; 


            case '"': // Le cas où on a des guillemets doubles
                if (!quoted && !esc) // S'ils sont dans des guillemets simples ou explicitement echapés alors ils sont échapés
                    double_quoted = !double_quoted;

                goto h_default;
                break;

            case '\'': // Le cas où on a des guillemets simples
                if (!double_quoted && !esc) // S'ils sont dans des guillemets doubles ou explicitement echapés alors ils sont échapés
                    quoted = !quoted;

                goto h_default;
                break;
            
            case '\\': // Les echapement peuvent exister dans les commandes externes ex: find ... -exec rm {} \; ici ; est echapé
            
                if (esc) // Si il est echappé
                    goto h_default;
                
                esc = 1;
                break;
        }
    }
    
    // Mise à jour de la commande
    self->command = command;

    // La commande externe est correctement initialisée.
    return self;
}

// Destructeur.
static void* ExternalCommand_destroy(void* self_){
    
    // Vérification des arguments.
    assert(self_);    
    assert(obj_instance_of(self_, _ExternalCommand_));

    ExternalCommand self = (ExternalCommand) self_;
    
    // Libération de la mémoire pour la chaine de la commande.
    free(self->command);
    
    // Aucun autre champ n'est à libérer il n'y a pas de liste d'argument ni de liste de paramètre.

    return self;
}

/**
 * Fonction qui va afficher la commande externe.
 */
static void ExternalCommand_print(const void* self_){
    
    // Vérification de l'argument.
    assert(self_);    
    assert(obj_instance_of(self_, _ExternalCommand_));
    
    // Conversion en objet de type ExternalCommand.
    ExternalCommand self = (ExternalCommand) self_;
    
    write(1, "Commande externe : ", 19);
    write(1, self->name, strlen(self->name));
    write(1, " | expr : ", 10);
    write(1, self->command, strlen(self->command));
    
}

// Execute.
static int ExternalCommand_execute(Command self_){
        
    //// Ecrire le corps de la fonction ici ! 

    return 0;
}












