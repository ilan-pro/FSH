
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
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

/** @cond HIDE_THIS*/
#define EXTERNAL_COMMAND_CLASS "ExternalCommand"
/**@endcond*/
#include "ExternalCommand.h"
#include <sys/types.h>
#include "../base/String.h"


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
    0


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

    // On met la fonction print du parent comme fonction d'affichage de l'enfant.
    *((void (**)) &(self->_ExternalCommand_->print)) = _Command_->print;
        
    // Ajout de la fonction execute de la classe fille.
    *((int (**) (Command)) &(self->execute)) = ExternalCommand_execute;

    // Les champs arguments et paramètres ( qui sont des listes chainées ) ne seront pas initialisés.
    
    // Récupération du nom de la commande.
    char* cmd_name = va_arg(*args, char*);
    // printf("'%s'\n", cmd_name);
    char* tmp = malloc((1 + strlen(cmd_name)) * sizeof(char));
    *(char**)&self->name = strcpy(tmp, cmd_name); // Mise à jour du nom de la commande.
    
    // Récupération du reste du prompt.
    char** prompt = va_arg(*args, char**);
    // Initialisation de la liste des arguments et des paramètres de commandes
    self->args = obj_new(_ArrayList_, 0);
    self->params = obj_new(_ArrayList_, 0);
    self->args_tmp =obj_new(_ArrayList_, 0);

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
    while(Command_actual_char_is_legal(quoted, double_quoted, esc, prompt)){
        
        // Récupération du caractère pointé par le pointeur, et incémentation de la posisiton.
        char c = *((*prompt)++);
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


            case ' ': // Le cas où le caractère lu est un espace.
    
                if (quoted || double_quoted || esc) // Si on est dans une expression entre guillemets ou que ' ' est echapé
                    goto h_default; // On va dans default

                if (cmd_length){ // Si il y a au moins une lettre lue.
                    
                    // Ajout du mot dans la liste des arguments de la commande.
                    // L'ajout se fait à la fin pour préserver l'ordre de déclaration des arguments (sait-on jamais).
                    ArrayList_add_last(self->args, obj_new(_String_, command), 0);
                    
                    // Remise à 0 du compteur
                    cmd_length = 0;

                    // troncage du mot en mot vide.
                    command = realloc(command, sizeof(char));
                    assert(command);
                    command[0] = '\0';
                    continue;

                }
                else // Si on aucun caractère n'est lu alors on ignore l'espace.
                    continue;    

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
    
    if (cmd_length){ // Si il y a au moins une lettre lue.
        
        // Ajout du mot dans la liste des arguments de la commande.
        // L'ajout se fait à la fin pour préserver l'ordre de déclaration des arguments (sait-on jamais).
        ArrayList_add_last(self->args, obj_new(_String_, command), 0);
        
        // Remise à 0 du compteur
        cmd_length = 0;

        // troncage du mot en mot vide.
        command = realloc(command, sizeof(char));
        assert(command);
        command[0] = '\0';

    }
    
    // Mise à jour de la commande
    free(command);

    // La commande externe est correctement initialisée.
    return self;
}

// Destructeur.
static void* ExternalCommand_destroy(void* self_){
    
    // Vérification des arguments.
    assert(self_);    
    assert(obj_instance_of(self_, _ExternalCommand_));

    ExternalCommand self = _Command_->dtor(self_);
    // Aucun autre champ n'est à libérer il n'y a pas de liste d'argument ni de liste de paramètre.

    return self;
}

// Execute.
static int ExternalCommand_execute(Command self_){
    
    // Code de retour du processus fils.
    int process_ret;
    /**Nous allons fork afin de pouvoir exécuter les commandes externes car les commandes externes, aux antipodes des commandes
     * internes,
     * nous devons les exécuter dans un processus différent car il ne vont pas modifier l'état du shell.
     */
    pid_t pid = fork();
    
    switch(pid){
        case 0:
            // le fils
            // ExternalCommand cmd = _ExternalCommand_->ctor(self_, );
            ExternalCommand self = (ExternalCommand) (void*) self_;
            int size = ArrayList_size(self->args);
            char **cmd = malloc((size + 1) * sizeof(char*));

            if(!cmd){
                perror("malloc");
                exit(1);
            }

            // Taille de cmd (au cas où des variables $ n'existe pas donc sont sautés)
            int cmd_size = 0;
            
            for(int i=0; i < size; i++){
                String arg = (String) ArrayList_get(self->args, i, 0);
                if (!arg) 
                    continue;
                char* tmp = obj_to_string(arg);
                cmd[i] = malloc((1 + strlen(tmp)) * sizeof(char));
                cmd_size ++;
                strcpy(cmd[i], tmp);
            }
            cmd[cmd_size] = NULL;
            
            if(execvp(self->name, cmd) < 0 ){
                for(int i=0; i < size; i++)
                    free(cmd[i]);
                free(cmd);

                write(2,"\e[1;31m", 7);
                write(2,self->name, strlen(self->name));
                write(2," : ", 3);
                char* err = strerror(errno);
                write(2,err,strlen(err));
                write(2,"\e[1;0m", 6);
                write(2,"\n", 1);
                exit(1);
            }

            for(int i=0; i < size; i++)
                free(cmd[i]);
            free(cmd);
            
            exit(0);
        case -1:
            perror("fork,");
            return 1;

        default:
            // le père
            if(waitpid(pid, &process_ret, 0) < 0){
                perror("waitpid");
                return 1;
            }
            break;
    }

    return WEXITSTATUS(process_ret);
}
