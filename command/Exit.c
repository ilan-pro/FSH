
/**
 * @file Exit.c
 * @brief Fichier contenant l'implémentation des fonctions déclarées dans `Exit.h`
 * 
 * `exit` est la commande qui permet de quitter le terminal.
 * 
 * Construction : <code><strong>obj_new(_Exit_, "exit");</strong></code><br>
 * `_Exit_` désigne la classe des commandes exit. et la chaine "exit" est le nom de la commande.
 */

/** @cond HIDE_THIS*/
#define EXIT_CLASS "Exit"
#include "Exit.h"
#include "../base/String.h"
#include "../shell.h"
/**@endcond*/


/*****************************************************/
/************ Déclaration des fonctions **************/

/**
 * Constructeur d'une commande EXIT.
 * 
 * @param self_ L'objet alloué par calloc().
 * @param args La liste des arguements (Nomalement elle ne doit contenir que "Exit").
 * @return Un objet de type Exit, initialisé.
 */
static void* Exit_constructor(void* self_, va_list* args);

/**
 * Destructeur d'un objet de type commande EXIT.
 *
 * Le destructeur va juste appeller le destructeur du parent, 
 * qui va désallouer les ressources qui sont héritées.
 * 
 * @param self_ L'objet à désallouer.
 * @return Un objet dont tout l'intértieur desalloué.
 */
static void* Exit_destroy(void* self_);

/**
 * @brief Fonction qui va exécuter la commande.
 *
 * Cette fonction est une redéfinition de la fonction `Command_execute()` de 
 * la classe `_Command_` (appelée par laison dynamique).
 *
 * @param self La commande exit à faire exécuter.
 * @return La valeur de retour de la commande après exécution.
 */
static int Exit_execute(Command self_);


/*****************************************************/
/************* Structure de la classe ****************/


/**
 * @brief Classe `Exit` de la commande Exit qui affiche 
 * le chemin absolu du répertoire courant.
 *
 * Construction : <code><strong> obj_new( `_Exit_`, "exit") </strong></code>
 * <br>
 * `_Exit_` étant la classe de la commande exit.
 */
typedef struct _Exit_ {
    
    /**
     * @brief Classe des commandes exit.
     *
     * `exit` est la commande qui permet de quitter le terminal.
     * 
     * Construction : <code><strong>obj_new(_Exit_, "exit");</strong></code><br>   
     * `_Exit_` désigne la classe des commandes exit. et la chaine "exit" est le nom de la commande.
     */
    Class _Exit_;
    EXIT_FILEDS

}* Exit;

static struct _Class_ __Exit__ = {
    
    NULL,
    EXIT_CLASS,
    sizeof(struct _Exit_),
    Exit_constructor,
    Exit_destroy,
    0,
    0,
    0,
    0 // Sera celui du parent.


};

/**
 * @brief Classe des commandes exit.
 *
 * `exit` est la commande qui permet de quitter le terminal.
 *
 * Construction : <code><strong>obj_new(_Exit_, "exit");</strong></code>
 * `_Exit_` désigne la classe des commandes exit. et la chaine "exit" est le nom de la commande.
 */
const Class _Exit_ = &__Exit__;

/***************************************************/
/************** Corps des fonctions ****************/


// Constructeur.
static void* Exit_constructor(void* self_, va_list* args){
    
    /// Mise à jour de _Exit_ en tant que héritier de _SimpleCmd_ (si ce n'est pas déjà le cas)
    if(!_Exit_->_super_)
        *(Class*)& (_Exit_->_super_) = _SimpleCmd_;

    // Vérification des arguments.
    assert(self_ && args);    
    
    // Conversion en objet de type Exit.
    Exit self = _SimpleCmd_->ctor(self_, args);
        
    // Ajout de la fonction execute de la classe fille.
    *((int (**) (Command)) &(self->execute)) = Exit_execute;

    return self;
    
}

// Destructeur.
static void* Exit_destroy(void* self_){
    
    // Vérification des arguments.
    assert(self_);
    return _SimpleCmd_->dtor(self_);
    
}

// Execute.
static int Exit_execute(Command self_){

    Exit self = (Exit) self_;
    
    int exit_code = previous_cmd_finish; // Par défaut, on quitte avec un code de sortie 0
    if (ArrayList_size(self->args) == 1) {
        // Tente de convertir l'argument en un entier pour le code de sortie
        char *endptr;
        char *nombre = obj_to_string((String)ArrayList_get_first(self->args, 0)) ;
        exit_code = strtol(nombre, &endptr, 10);

        if (*endptr != '\0') {

            char tmp1[] = "\e[1;31m[Erreur] La commande ne prend que des entiers\e[1;0m\n";
            write(2, tmp1, strlen(tmp1));
            if (exit_code)
                exit(exit_code);
            else
                exit(1);
        }
    }else if (ArrayList_size(self->args) > 1){
        
        char tmp1[] = "\e[1;31mexit: too many arguments\e[1;0m\n";
        write(2, tmp1, strlen(tmp1));
        return 1;

    }   

    exit(exit_code);
    return 0;

}
