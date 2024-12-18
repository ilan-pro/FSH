
/**
 * @file Pwd.c
 * @brief Fichier contenant l'implémentation des fonctions déclarées dans `Pwd.h`
 * 
 * Pwd est une commande qui va afficher le chemin absolue vers le répertoire courant.
 * 
 * Construction : <code><strong>obj_new(_Pwd_, "pwd");</strong></code><br>
 * `_Pwd_` désigne la classe des commandes pwd. et la chaine "pwd" est le nom de la commande.
 */


#include "../shell.h"

/** @cond HIDE_THIS*/
#define PWD_CLASS "Pwd"
#include "Pwd.h"
/**@endcond*/


/*****************************************************/
/************ Déclaration des fonctions **************/

/**
 * Constructeur d'une commande PWD.
 * 
 * @param self_ L'objet alloué par calloc().
 * @param args La liste des arguements (Nomalement elle ne doit contenir que "Pwd").
 * @return Un objet de type Pwd, initialisé.
 */
static void* Pwd_constructor(void* self_, va_list* args);

/**
 * Destructeur d'un objet de type commande PWD.
 *
 * Le destructeur va juste appeller le destructeur du parent, 
 * qui va désallouer les ressources qui sont héritées.
 * 
 * @param self_ L'objet à désallouer.
 * @return Un objet dont tout l'intértieur desalloué.
 */
static void* Pwd_destroy(void* self_);

/**
 * @brief Fonction qui va exécuter la commande.
 *
 * Cette fonction est une redéfinition de la fonction `Command_execute()` de 
 * la classe `_Command_` (appelée par laison dynamique).
 *
 * @param self La commande pwd à faire exécuter.
 * @return La valeur de retour de la commande après exécution.
 */
static int Pwd_execute(Command self_);


/*****************************************************/
/************* Structure de la classe ****************/


/**
 * @brief Classe `Pwd` de la commande Pwd qui affiche 
 * le chemin absolu du répertoire courant.
 *
 * Construction : <code><strong> obj_new( `_Pwd_`, "pwd") </strong></code>
 * <br>
 * `_Pwd_` étant la classe de la commande pwd.
 */
typedef struct _Pwd_ {
    
    /**
     * @brief Classe des commandes pwd.
     *
     * `pwd` est la commande qui permet d'afficher le chemin 
     * absolue du vers le répertoire dans lequel on se 
     * trouve.
     *
     * Construction : <code><strong>obj_new(_Pwd_, "pwd");</strong></code><br>   
     * `_Pwd_` désigne la classe des commandes pwd. et la chaine "pwd" est le nom de la commande.
     */
    Class _Pwd_;
    PWD_FILEDS

}* Pwd;

static struct _Class_ __Pwd__ = {
    
    NULL,
    PWD_CLASS,
    sizeof(struct _Pwd_),
    Pwd_constructor,
    Pwd_destroy,
    0,
    0,
    0,
    0 // Sera celui du parent.


};

/**
 * @brief Classe des commandes pwd.
 *
 * `pwd` est la commande qui permet d'afficher le chemin 
 * absolue du vers le répertoire dans lequel on se 
 * trouve.
 *
 * Construction : <code><strong>obj_new(_Pwd_, "pwd");</strong></code>
 * `_Pwd_` désigne la classe des commandes pwd. et la chaine "pwd" est le nom de la commande.
 */
const Class _Pwd_ = &__Pwd__;

/***************************************************/
/************** Corps des fonctions ****************/


// Constructeur.
static void* Pwd_constructor(void* self_, va_list* args){
    
    /// Mise à jour de _Pwd_ en tant que héritier de _SimpleCmd_ (si ce n'est pas déjà le cas)
    if(!_Pwd_->_super_)
        *(Class*)& (_Pwd_->_super_) = _SimpleCmd_;

    // Vérification des arguments.
    assert(self_ && args);    
    
    // Conversion en objet de type Pwd.
    Pwd self = _SimpleCmd_->ctor(self_, args);

    if (!self) // Si la construction échoue alors on renvoie NULL
        return NULL;
    
    // Ajout de la fonction execute de la classe fille.
    *((int (**) (Command)) &(self->execute)) = Pwd_execute;
    
    return self;
}

// Destructeur.
static void* Pwd_destroy(void* self_){
    
    // Vérification des arguments.
    assert(self_);    
    
    return _SimpleCmd_->dtor(self_);
    
}

// Execute.
static int Pwd_execute(Command self_){
    Pwd self = (Pwd) self_;
    //// Ecrire le corps de la fonction ici ! 
    if(ArrayList_size(self->args)>0){
        Command_fail_with_message((Command)self, 3); 
        return 1;
    }
    write(1, working_dir, strlen(working_dir));
    write(1,"\n", 1); // Saut de ligne.
    return 0;
}











