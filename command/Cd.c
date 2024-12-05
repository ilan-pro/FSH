
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



/** @cond HIDE_THIS*/
#define CD_CLASS "Cd"
#include "Cd.h"
/**@endcond*/

#include "../shell.h"
#include "../base/String.h"

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
    
    /// Mise à jour de _Cd_ en tant que héritier de _SimpleCmd_ (si ce n'est pas déjà le cas)
    if(!_Cd_->_super_)
        *(Class*)& (_Cd_->_super_) = _SimpleCmd_;

    // Vérification des arguments.
    assert(self_ && args);    
    
    // Conversion en objet de type Cd.
    Cd self = _SimpleCmd_->ctor(self_, args);
        
    // Ajout de la fonction execute de la classe fille.
    *((int (**) (Command)) &(self->execute)) = Cd_execute;

    return self;
    
}


// Destructeur.
static void* Cd_destroy(void* self_){
    
    // Vérification des arguments.
    assert(self_);    
    
    return _SimpleCmd_->dtor(self_);
    
}

// variable globale prev_path
static char prev_path[4096]= "";

// Execute.
static int Cd_execute(Command self_){
        
    Cd self = (Cd) self_;
    char *path;

    if (ArrayList_size(self->args) > 1){
        char tmp1[] = "\e[1;31m[Erreur] de syntaxe : la commande ne prend qu'un seul argument.\e[1;0m";
        write(2, tmp1, strlen(tmp1));
        return 1;
    }
    else if(ArrayList_size(self->args)== 0){
        path = getenv("HOME");
        assert(path != NULL);
    }
    else{
        String tmp = (String) ArrayList_get_first(self->args, 0);
        if (!tmp) // Si pas d'arguement ($ qui n'existe pas)
            path = getenv("HOME"); // On va dans home.
        else 
            path = obj_to_string(tmp);
    }

    if(strcmp(path, "-")==0){
        // prev_pass garde le dernier chemin depuis lequel on à fait cd
        if(strcmp(prev_path, "") == 0){

            char tmp1[] = "\e[1;31m[Erreur] : prev_path non défini\e[1;0m\n";
            write(2, tmp1, strlen(tmp1));
            return 1;

        }else{
            int res = chdir(prev_path);
            if(res != 0){
                perror("cd");
                return 1;
            }else{
                strcpy(prev_path, working_dir); 
                goto maj_working_dir;
            }
        }
    }
    if(chdir(path) != 0){ 

        perror("cd");
        return 1;

    }else{

        strcpy(prev_path, working_dir); 
    
    }   

    maj_working_dir:

    // mettre à jour working_dir
    if(getcwd(working_dir, sizeof(working_dir)) == NULL){
        return 1;
    }
    
    return 0;
}







