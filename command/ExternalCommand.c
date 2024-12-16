
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


#include "../shell.h"
#include <sys/wait.h>
#include <sys/types.h>
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
    
    /// Mise à jour de _Cd_ en tant que héritier de _Command_ (si ce n'est pas déjà le cas)
    if(!_ExternalCommand_->_super_)
        *(Class*)& (_ExternalCommand_->_super_) = _Command_;

    // Vérification des arguments.
    assert(self_ && args);    
    
    // Conversion en objet de type ExternalCommand.
    ExternalCommand self = (ExternalCommand) self_;

    // Appel du constructeur de la classe Command ( parent )
    _Command_->ctor(self, args);

    // On met la fonction print du parent comme fonction d'affichage de l'enfant.
    *((void (**)) &(self->_ExternalCommand_->print)) = _Command_->print;
        
    // Ajout de la fonction execute de la classe fille.
    *((int (**) (Command)) &(self->execute)) = ExternalCommand_execute;
    
    // Ajout du nom de la commande comme premier argument.
    ArrayList_add_first(self->args, obj_new(_String_, self->name), 0);

    // Récupération du reste du prompt.
    char** prompt = va_arg(*args, char**);
    
    // Récupération des arguments.
    if (!Command_parse_all_sub(self->args, prompt, 0, self->redirector)){
        Command_fail_with_message((Command)self, Command_FAIL_CODE);
        return NULL;
    }

    // La commande externe est correctement initialisée.
    return self;
}

// Destructeur.
static void* ExternalCommand_destroy(void* self_){
    
    // Vérification des arguments.
    assert(self_);    
    assert(obj_instance_of(self_, _ExternalCommand_));

    return _Command_->dtor(self_);
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
            {
                // le fils
                // ExternalCommand cmd = _ExternalCommand_->ctor(self_, );
                ExternalCommand self = (ExternalCommand) (void*) self_;
                
                /**
                 * Construction de la liste des arguments à donner à l'appel de la commande 
                 * avec exec().
                 */
                int size = ArrayList_size(self->args); // Récupération de la taille de la liste des arguments.
                char **cmd = malloc((size + 1) * sizeof(char*)); // Création du tableau d'arguments.
    
                if(!cmd){
                    perror("malloc");
                    exit(1);
                }
                
                /** Il s'agit de la taille réel qu'aura la liste des arguments 
                 * cette taille peut être différente de `size` car avant d'exécuter la 
                 * commande, La fonction `Command_execute` remplace les argments de la 
                 * forme `$...` (qui sont des identificateurs de variables) par leur valeur,
                 * et met NULL si jamais aucune valeur n'est trouvée. Lorsque ce dernier cas 
                 * arrive (l'argument est mis à NULL) alors on ne l'insère pas dans le tableau 
                 * `cmd` (car sinon il marquerai la fin de la liste), ainsi à la fin rien ne 
                 * garenti que `size` est la vrai taille `cmd` d'où le faite de créer une 
                 * variable `cmd_size` qui va compter exactement le nombre de commande ajoutés.
                 */
                int cmd_size = 0;
                
                /**
                 * Parcours de la liste des arguments.
                 * Les arguments marqués à NULL sont sautés.
                 */
                for(int i=0; i < size; i++){
                    // Récupération de l'argument.
                    String arg = (String) ArrayList_get(self->args, i, 0);
                    // Si l'argument est NULL
                    if (!arg) 
                        continue; // Alors on continue.
                    
                    /**
                     * Copie de la valeur de l'argument.
                     */
                    char* tmp = obj_to_string(arg); // Récupération de la chaine de l'argument.
                    cmd[i] = malloc((1 + strlen(tmp)) * sizeof(char)); // Création d'une zone mémoire pour la copie. 
                    strcpy(cmd[i], tmp); // Copie de la valeur de l'argument.
                    cmd_size ++; // incrémentation du nombre d'argument de la liste.
                }

                cmd[cmd_size] = NULL; // Ajout de NULL pour marquer la fin de la liste.
                        
                
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
                
                // Libération de toutes les chaines qui ont servie à la création de la liste des arguments.
                for(int i=0; i < size; i++)
                    free(cmd[i]);
                free(cmd);
                
                exit(0);
               }
        case -1: // Echec du fork.
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
