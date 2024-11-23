
/**
 * @file Cd.c
 * @brief Fichier contenant l'implémentation des fonctions déclarées dans `Cd.h`
 * 
 * Cd est une commande qui va changer le répertoire vers courant vers un nouveau répertoire.
 * 
 * Usage : <code><strong>cd [REF | - ]</strong></code>
 * 
 * Contruction : <code><strong>obj_new(_Cd_, "cd");</strong></code><br>
 * `_Cd_` désigne la classe des commandes cd. et la chaine "cd" est le nom de la commande.
 */


#include <assert.h>

/** @cond HIDE_THIS*/
#define CD_CLASS "Cd"
#include "Cd.h"
/**@endcond*/


/*****************************************************/
/************ Déclaration des fonctions **************/

/**
 * Constructeur d'une commande CD.
 * 
 * @param self_ L'objet alloué par calloc().
 * @param args La liste des arguements (Nomalement elle ne doit contenir que "Cd").
 * @return Un objet de type Cd, initialisé.
 */
static void* Cd_constructor(void* self_, va_list* args);

/**
 * Destructeur d'un objet de type commande CD.
 *
 * Le destructeur va juste appeller le destructeur du parent, 
 * qui va désallouer les ressources qui sont héritées.
 * 
 * @param self_ L'objet à désallouer.
 * @return Un objet dont tout l'intértieur desalloué.
 */
static void* Cd_destroy(void* self_);

/**
 * @brief Fonction qui va exécuter la commande.
 *
 * Cette fonction est une redéfinition de la fonction `Command_execute()` de 
 * la classe `_Command_` (appelée par laison dynamique).
 *
 * @param self La commande cd à faire exécuter.
 * @return La valeur de retour de la commande après exécution.
 */
static int Cd_execute(Command self_);


/*****************************************************/
/************* Structure de la classe ****************/


/**
 * @brief Classe `Cd` de la commande Cd qui affiche 
 * le chemin absolu du répertoire courant.
 *
 * Construction : <code><strong> obj_new( `_Cd_`, "cd") </strong></code>
 * <br>
 * `_Cd_` étant la classe de la commande cd.
 */
typedef struct _Cd_ {
    
    /**
     * @brief Classe des commandes cd.
     *
     * `cd` est une commande qui va changer le répertoire vers courant vers 
     * un nouveau répertoire.
     *
     * Contruction : <code><strong>obj_new(_Cd_, "cd");</strong></code><br>   
     * `_Cd_` désigne la classe des commandes cd. et la chaine "cd" est le nom de la commande.
     */
    Class _Cd_;
    CD_FILEDS

}* Cd;

static struct _Class_ __Cd__ = {
    
    NULL,
    CD_CLASS,
    sizeof(struct _Cd_),
    Cd_constructor,
    Cd_destroy,
    0,
    0,
    0,
    0 // Sera celui du parent.


};

/**
 * @brief Classe des commandes cd.
 *
 * `cd` est la commande qui permet d'afficher le chemin 
 * absolue du vers le répertoire dans lequel on se 
 * trouve.
 *
 * Contruction : <code><strong>obj_new(_Cd_, "cd");</strong></code>
 * `_Cd_` désigne la classe des commandes cd. et la chaine "cd" est le nom de la commande.
 */
const Class _Cd_ = &__Cd__;

/***************************************************/
/************** Corps des fonctions ****************/


// Constructeur.
static void* Cd_constructor(void* self_, va_list* args){
    
    // Vérification des arguments.
    assert(self_ && args);    
    
    // Conversion en objet de type Cd.
    Cd self = (Cd) self_;

    // On ajoute le parent à Cd (en controunant la contrainte sur const).
    *(Class*)&(self->_Cd_->_super_) = _Command_; 
    // On met la fonction print du parent comme fonction d'affichage de l'enfant.
    *((void (**)) &(self->_Cd_->print)) = _Command_->print;
        
    // Ajout de la fonction execute de la classe fille.
    *((int (**) (Command)) &(self->execute)) = Cd_execute;

    return _Command_->ctor(self, args);
    
}


// Destructeur.
static void* Cd_destroy(void* self_){
    
    // Vérification des arguments.
    assert(self_);    
    
    return _Command_->dtor(self_);
    
}

// Execute.
static int Cd_execute(Command self_){
        
    //// Ecrire le corps de la fonction ici ! 

    return 0;
}











