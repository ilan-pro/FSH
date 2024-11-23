
/**
 * @file Ftype.c
 * @brief Fichier contenant l'implémentation des fonctions déclarées dans `Ftype.h`
 * 
 * `ftype` est la commande qui donne le type d'un fichier.
 * 
 * Construction : <code><strong>obj_new(_Ftype_, "ftype");</strong></code><br>
 * `_Ftype_` désigne la classe des commandes ftype. et la chaine "ftype" est le nom de la commande.
 */


#include <assert.h>

/** @cond HIDE_THIS*/
#define FTYPE_CLASS "Ftype"
#include "Ftype.h"
/**@endcond*/


/*****************************************************/
/************ Déclaration des fonctions **************/

/**
 * Constructeur d'une commande FTYPE.
 * 
 * @param self_ L'objet alloué par calloc().
 * @param args La liste des arguements (Nomalement elle ne doit contenir que "Ftype").
 * @return Un objet de type Ftype, initialisé.
 */
static void* Ftype_constructor(void* self_, va_list* args);

/**
 * Destructeur d'un objet de type commande FTYPE.
 *
 * Le destructeur va juste appeller le destructeur du parent, 
 * qui va désallouer les ressources qui sont héritées.
 * 
 * @param self_ L'objet à désallouer.
 * @return Un objet dont tout l'intértieur desalloué.
 */
static void* Ftype_destroy(void* self_);

/**
 * @brief Fonction qui va exécuter la commande.
 *
 * Cette fonction est une redéfinition de la fonction `Command_execute()` de 
 * la classe `_Command_` (appelée par laison dynamique).
 *
 * @param self La commande ftype à faire exécuter.
 * @return La valeur de retour de la commande après exécution.
 */
static int Ftype_execute(Command self_);


/*****************************************************/
/************* Structure de la classe ****************/


/**
 * @brief Classe `Ftype` de la commande Ftype qui affiche 
 * le chemin absolu du répertoire courant.
 *
 * Construction : <code><strong> obj_new( `_Ftype_`, "ftype") </strong></code>
 * <br>
 * `_Ftype_` étant la classe de la commande ftype.
 */
typedef struct _Ftype_ {
    
    /**
     * @brief Classe des commandes ftype.
     *
     * `ftype` est la commande qui donne le type d'un fichier.
     *
     * Construction : <code><strong>obj_new(_Ftype_, "ftype");</strong></code><br>   
     * `_Ftype_` désigne la classe des commandes ftype. et la chaine "ftype" est le nom de la commande.
     */
    Class _Ftype_;
    FTYPE_FILEDS

}* Ftype;

static struct _Class_ __Ftype__ = {
    
    NULL,
    FTYPE_CLASS,
    sizeof(struct _Ftype_),
    Ftype_constructor,
    Ftype_destroy,
    0,
    0,
    0,
    0 // Sera celui du parent.


};

/**
 * @brief Classe des commandes ftype.
 *
 * `ftype` est la commande qui donne le type d'un fichier.
 *
 * Construction : <code><strong>obj_new(_Ftype_, "ftype");</strong></code>
 * `_Ftype_` désigne la classe des commandes ftype. et la chaine "ftype" est le nom de la commande.
 */
const Class _Ftype_ = &__Ftype__;

/***************************************************/
/************** Corps des fonctions ****************/


// Constructeur.
static void* Ftype_constructor(void* self_, va_list* args){
    
    // Vérification des arguments.
    assert(self_ && args);    
    
    // Conversion en objet de type Ftype.
    Ftype self = (Ftype) self_;

    // On ajoute le parent à FTYPE (en controunant la contrainte sur const).
    *(Class*)&(self->_Ftype_->_super_) = _Command_; 
    // On met la fonction print du parent comme fonction d'affichage de l'enfant.
    *((void (**)) &(self->_Ftype_->print)) = _Command_->print;
        
    // Ajout de la fonction execute de la classe fille.
    *((int (**) (Command)) &(self->execute)) = Ftype_execute;

    return _Command_->ctor(self, args);
    
}

// Destructeur.
static void* Ftype_destroy(void* self_){
    
    // Vérification des arguments.
    assert(self_);    
    
    return _Command_->dtor(self_);
    
}

// Execute.
static int Ftype_execute(Command self_){
        
    //// Ecrire le corps de la fonction ici ! 

    return 0;
}











