/**
 * @file For.c
 * @brief Fichier contenant l'implémentation des fonctions déclarées dans `For.h`
 * 
 * For est une commande qui exécute un certain nombre de commandes pour chaque fichier d'un répertoire.
 * 
 * Construction : <code><strong>obj_new(_For_, "for");</strong></code><br>
 * `_For_` désigne la classe des commandes for. et la chaine "for" est le nom de la commande.
 */


#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../shell.h"
#include "../base/new.h"
#include "../base/String.h"

/** @cond HIDE_THIS*/
#define FOR_CLASS "For"
/**@endcond*/
#include "For.h"


/*****************************************************/
/************ Déclaration des fonctions **************/

/**
 * Constructeur d'une commande FOR.
 * 
 * @param self_ L'objet alloué par calloc().
 * @param args La liste des arguements (Nomalement elle ne doit contenir que "For").
 * @return Un objet de type For, initialisé.
 */
static void* For_constructor(void* self_, va_list* args);

/**
 * Destructeur d'un objet de type commande FOR.
 *
 * Le destructeur va juste appeller le destructeur du parent, 
 * qui va désallouer les ressources qui sont héritées.
 * 
 * @param self_ L'objet à désallouer.
 * @return Un objet dont tout l'intértieur desalloué.
 */
static void* For_destroy(void* self_);

/**
 * @brief Fonction qui va exécuter la commande.
 *
 * Cette fonction est une redéfinition de la fonction `Command_execute()` de 
 * la classe `_Command_` (appelée par laison dynamique).
 *
 * @param self La commande for à faire exécuter.
 * @return La valeur de retour de la commande après exécution.
 */
static int For_execute(Command self_);

static void For_print(const void* self_);
/*****************************************************/
/************* Structure de la classe ****************/


/**
 * @brief Classe `For` de la commande For qui affiche 
 * le chemin absolu du répertoire courant.
 *
 * Construction : <code><strong> obj_new( `_For_`, "for") </strong></code>
 * <br>
 * `_For_` étant la classe de la commande for.
 */
typedef struct _For_ {
    
    /**
     * @brief Classe des commandes for.
     *
     * `for` est la commande qui permet d'afficher le chemin 
     * absolue du vers le répertoire dans lequel on se 
     * trouve.
     *
     * Construction : <code><strong>obj_new(_For_, "for");</strong></code><br>   
     * `_For_` désigne la classe des commandes for. et la chaine "for" est le nom de la commande.
     */
    Class _For_;
    FOR_FILEDS

}* For;

static struct _Class_ __For__ = {
    
    NULL,
    FOR_CLASS,
    sizeof(struct _For_),
    For_constructor,
    For_destroy,
    0,
    0,
    0,
    0 // Sera celui du parent.


};

/**
 * @brief Classe des commandes for.
 *
 * `for` est la commande qui permet d'afficher le chemin 
 * absolue du vers le répertoire dans lequel on se 
 * trouve.
 *
 * Construction : <code><strong>obj_new(_For_, "for");</strong></code>
 * `_For_` désigne la classe des commandes for. et la chaine "for" est le nom de la commande.
 */
const Class _For_ = &__For__;
    
/**
 * Il s'agit de la liste qui va contenir les identificateurs des différentes variables 
 * utilisées dans les boucles `for`.
 *
 * L'indice de l'identificateur est le même que celui du nom du fichier contenu dans 
 * L'autre liste.
 */
LinkedList For_ids;

    
/**
 * Il s'agit de la liste contenant les noms des fichiers en cours de 
 * traitement dans la boucle `for`.
 */
LinkedList For_files;
    
/**
 * Il s'agit du répertoire ouvert par `for`
 */
static char* For_dir = "";

/***************************************************/
/************** Corps des fonctions ****************/


static void next_word(char* mot, char** prompt){

    int break_loop = 0;
    

    /** 
     * Lecture du prompt dans la boucle tant que le caractère pointé est 
     * n'est pas \0`(fin de l chaine)
     */
    while(prompt && **prompt != '{' && !break_loop){
        
        char c = *(*prompt++);
        
        // Nombre de caractère lu. (Si ce nombre vaut 0 alors on a encore rien lu 
        // dans le prompt et donc tous les espaces sont ignorés. Le nombre est mis à 0 après chaque lecture d'un mot)
        int char_compteur = 0;
        
        switch (c) {
            default: 
                char_compteur ++;
                // Réallocation pour accueillir le nouveau caractère.
                mot = realloc(mot, (1 + char_compteur) * sizeof(char));
                mot[char_compteur] = '\0';
                mot[char_compteur - 1] = c;
                break;
            case ' ': // Le cas où le caractère lu est un espace.
    
                if (char_compteur) // Si il y a au moins une lettre lue.
                    break_loop = 1;
                continue;  

            
        }

    }    
    
}

// Constructeur.
static void* For_constructor(void* self_, va_list* args){
    
    // Vérification des arguments.
    assert(self_ && args);    
    
    // Conversion en objet de type For.
    For self = (For) self_;

    // On ajoute le parent à FOR (en controunant la contrainte sur const).
    *(Class*)&(self->_For_->_super_) = _Command_; 
    // On met la fonction print du parent comme fonction d'affichage de l'enfant.
    *((void (**)) &(self->_For_->print)) = For_print;
        
    // Ajout de la fonction execute de la classe fille.
    *((int (**) (Command)) &(self->execute)) = For_execute;

    //Récupération du reste du prompt.
    char** prompt = va_arg(*args, char**);

    
    /** 
     * Chaine de caractère qui va contenir le mot en cours de lecture, 
     * cette chaine est troquée après chaque lecture d'un mot (après 
     * avoir rencontré le premier caractère espace depuis la lecture du mot).
     */
    char* mot = (char*)malloc(sizeof(char));
    assert(mot);
    // On ajoute le caractère de fin de chaine au mot.
    mot[0] = '\0';
        
    // Réucpération du prochain mot
    next_word(mot, prompt);
    if(!*prompt){
        char tmp[] = "\e[1;31mErreur : boucle for mal écrite !\n\e[1;0m";
        write(2,tmp, strlen(tmp));
    }
    // Le mot parsé est l'identificateur des fichiers dans la boucle.
    // Ajout de ce fichier.
    LinkedList_add_last(For_ids, obj_new(_String_, mot), 0);
    
    // troncage du mot en mot vide.
    mot = realloc(mot, sizeof(char));
    assert(mot);
    mot[0] = '\0';
    
    // Récupération du prochain mot `in`
    next_word(mot, prompt);
    if(!*prompt){
        char tmp[] = "\e[1;31mErreur : boucle for mal écrite !\n\e[1;0m";
        write(2,tmp, strlen(tmp));
    }
    if(!strcmp(mot, "in")){
        char tmp[] = "\e[1;31mErreur : boucle for mal écrite !2\n\e[1;0m";
        write(2,tmp, strlen(tmp));
    }
    
    // troncage du mot en mot vide.
    mot = realloc(mot, sizeof(char));
    assert(mot);
    mot[0] = '\0';
    
    // Récupération du chemin vers le répertoire.
    next_word(mot, prompt);
    if(!*prompt){
        char tmp[] = "\e[1;31mErreur : boucle for mal écrite !\n\e[1;0m";
        write(2,tmp, strlen(tmp));
    }

    For_dir = malloc((1 + strlen(mot)) * sizeof(char));
    strcpy(For_dir, mot);
    
    
    while(**prompt!= '{'){
            
        if(!*prompt){
            char tmp[] = "\e[1;31mErreur : boucle for mal écrite !\n\e[1;0m";
            write(2,tmp, strlen(tmp));
            break;
        }

        (*prompt)++;
        
    }
    (*prompt)++;
    
    // Récupération des commandes de for.
    self->commands = Shell_parse(); 

    free(mot);
   
    return self;

}

// Destructeur.
static void* For_destroy(void* self_){
    
    // Vérification des arguments.
    assert(self_);    
    
    For self = (For) self_;
    obj_destroy(self->commands);

    return _Command_->dtor(self_);
    
}

// Execute.
static int For_execute(Command self_){
        
    //// Ecrire le corps de la fonction ici ! 
    
    return 0;
}


static void For_print (const void* self_) {

    assert(self_);
    assert(obj_instance_of(self_, _For_));
    write(1, "For ", 4);
    obj_print(((For)self_)->commands);  
        
}