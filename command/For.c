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
#include <stdio.h>
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
ArrayList For_ids;

    
/**
 * Il s'agit de la liste contenant les noms des fichiers en cours de 
 * traitement dans la boucle `for`.
 */
ArrayList For_files;
    

/***************************************************/
/************** Corps des fonctions ****************/


static void For_print_syntax_err( const char* msg ){

    char tmp[] = "\e[1;31m[Erreur] Boucle for mal écrite : ";
    write(2,tmp, strlen(tmp));
    write(2, msg, strlen(msg));
    write(2, "\n\e[1;0m", 7);

}

static int next_word(char** mot, char** prompt){

    int break_loop = 0;
    
    // Nombre de caractère lu. (Si ce nombre vaut 0 alors on a encore rien lu 
    // dans le prompt et donc tous les espaces sont ignorés. Le nombre est mis à 0 après chaque lecture d'un mot)
    int char_compteur = 0;

    /** 
     * Lecture du prompt dans la boucle tant que le caractère pointé est 
     * n'est pas \0`(fin de l chaine)
     */
    while(**prompt 
            && **prompt != '{' && 
            !break_loop){
        
        char c = **prompt;
        
        switch (c) {
            default: 
                char_compteur ++;
                // Réallocation pour accueillir le nouveau caractère.
                *mot = realloc(*mot, (1 + char_compteur) * sizeof(char));
                (*mot)[char_compteur] = '\0';
                (*mot)[char_compteur - 1] = c;
                break;
            case ' ': // Le cas où le caractère lu est un espace.
    
                if (char_compteur){ // Si il y a au moins une lettre lue.
                    break_loop = 1;
                    continue;
                }
                break;  
        
            
        }
        
        (*prompt)++;

    }    
    
        if(**mot=='\0') // Si on a rien lu on renvoie 0
            return 0;
        return 1;

}

/**
 * Fonction qui ajoute l'identificateur de variable de boucle (qui va stocker le fichier en 
 * cours de traitement dans la boucle) de la boucle `for`.
 *
 * @return 1 si l'idtentificateur est trouvé. 0 sinon (avec affichage d'une erreur).
 */
static int add_file_id( For self, char** prompt ){
         
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
    next_word(&mot, prompt);
    if(!**prompt){
        For_print_syntax_err("Syntaxe incomplète !");
        free(mot);
        return 0;
    }
    // Création de l'objet String de l'identificateur.
    String s = obj_new(_String_, mot);
    // Vérification de l'existance de l'identificateur (S'il existe alors il y a une erreur).
    if (ArrayList_contains(For_ids, s)){
        char tmp[50];
        snprintf(tmp, 50, "L'indentificateur '%s' existe déjà !", obj_to_string(s));
        For_print_syntax_err(tmp);
        obj_destroy(s);
        free(mot);
        return 0;
    }

    // Le mot parsé est l'identificateur des fichiers dans la boucle.
    // Ajout de ce fichier.
    ArrayList_add_last(For_ids,s, 0);
    char* tmp = malloc((1 + strlen(mot) * sizeof(char)));
    assert(tmp);
    strcpy(tmp, mot);
    self->id_var = tmp;
    
    free(mot);
    return 1;
}

/**
 * Fonction qui vérifie si l'opérateur in est bien le prochain mot parsé.
 *
 * @return 1 Si `in` est le prochain mot. 0 sinon (avec affichage d'une erreur).
 */
static int check_in( char** prompt ){

    /** 
     * Chaine de caractère qui va contenir le mot en cours de lecture, 
     * cette chaine est troquée après chaque lecture d'un mot (après 
     * avoir rencontré le premier caractère espace depuis la lecture du mot).
     */
    char* mot = (char*)malloc(sizeof(char));
    assert(mot);
    // On ajoute le caractère de fin de chaine au mot.
    mot[0] = '\0';
    
    // Récupération du prochain mot `in`
    // Si le mot lu n'est pas "in" ou si aucun mot n'est lu (fin)
    if(!next_word(&mot, prompt) || strcmp(mot, "in")){
        For_print_syntax_err("Il manque l'opérateur `in`");
        free(mot);
        return 0;
    }

    // Si le prompt est fini.
    if(!**prompt){
        For_print_syntax_err("Syntaxe incomplète !");
        free(mot);
        return 0;
    }
    
   free(mot);
   return 1;
}
/**
 * Fonction qui ajoute le répertoire de boucle (dans lequel les fichiers seront chargés  
 * pour être traité dans la boucle) de la boucle `for`.
 * 
 * La fonction ne va pas vérifier si le répertoire est valide.
 *
 * @return 1 si le répertoire est saisie. 0 sinon (avec affichage d'une erreur).
 */
static int add_dir( For self, char** prompt ){

    /** 
     * Chaine de caractère qui va contenir le mot en cours de lecture, 
     * cette chaine est troquée après chaque lecture d'un mot (après 
     * avoir rencontré le premier caractère espace depuis la lecture du mot).
     */
    char* mot = (char*)malloc(sizeof(char));
    assert(mot);

    // On ajoute le caractère de fin de chaine au mot.
    mot[0] = '\0';
   
    // Récupération du chemin vers le répertoire.
    if (!next_word(&mot, prompt)){
        For_print_syntax_err("Répertoire non-renseigné !");
        free(mot);
        return 0;
    }

    if(!**prompt){
        For_print_syntax_err("Syntaxe incomplète !");
        free(mot);
        return 0;
    }
    // Ajout du repertoire de recherche.
    char* tmp = malloc((1 + strlen(mot)) * sizeof(char));
    assert(tmp);
    assert(strcpy(tmp, mot));
    self->dir = tmp;
    
    free(mot);
    return 1;
}

static int add_param_aux( For self, char ** prompt ){
    
        
    char tmp[20];
    char* mot;

    while (**prompt && **prompt != ' ' && **prompt != '{'){
        char c = *(*prompt)++;
        switch (c) {
            
            case 'A' :
            case 'r' :
                snprintf(tmp,20, "%c", c); 
                break;

            case 'e' :
            case 't' :
            case 'p' :
                
                if (**prompt != ' '){ // Si le prochain caractère n'est pas un espace.
                    goto h_default;
                }

                // Récupération du prochain mot.
                mot = malloc(sizeof(char));
                assert(mot);
                mot[0] = '\0';
                    
                if(!next_word(&mot, prompt)){ // Si aucun mot n'est lu après
                    // Alors la boucle est mal écrite.
                    For_print_syntax_err("Aucun argument n'est trouvé !");
                    return 0;
                }
                
                snprintf(tmp, 20, "%c %s", c, mot);
                
                free(mot);
                break;

            default : 
                h_default :
                
                // Alors la boucle est mal écrite.
                For_print_syntax_err("Paramètre inconnu !");
                return 0;

        }
        
        // Ajout du paramètre ()
        ArrayList_add_last(self->params, obj_new(_String_, tmp), 0);
    }

    return 1;
}

static int add_params( For self,  char ** prompt ){

    while(**prompt!= '{'){
            
        // caractère pointé par le prompt
        char c = *(*prompt);

        // Si on arrive à la fin de la chaine avant d'avoir rencontré un '{'
        if(!**prompt){
            // Alors la boucle est mal écrite.
            For_print_syntax_err("Aucune commande trouvé !");
            return 0;
        }
        
        if(c == '-'){
            (*prompt)++;
            if (!add_param_aux(self, prompt))
                return 0;
            continue;
        }
        
        else if (c == ' '){
            (*prompt)++;
            continue;
        }
        else{ // on est tombé sur un mot qui n'est pas un paramètre.
            (*prompt)++;
            For_print_syntax_err("Argument non-identifié.");
            return 0;
        }
        
    }

    // On décale de 1 pour ignorer le caractère '{'.
    (*prompt)++;
    
    return 1;
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
    va_arg(*args, char*);
    //Récupération du reste du prompt.
    char** prompt = va_arg(*args, char**);
    
    
    self->params = obj_new(_ArrayList_, 0);
    self->args = obj_new(_ArrayList_, 0);
    
    // Ajout de l'identificateur de la variable de boucle
    if (!add_file_id(self, prompt)){
        obj_destroy(self);
        return NULL;
    }

    // Vérification de l'existance de `in`
    if (!check_in(prompt)){
        obj_destroy(self);
        return NULL;
    }

    // Ajout du répertoire
    if (!add_dir(self, prompt)){
        obj_destroy(self);
        return NULL;
    }
   

    // Ajout des paramètres.
    if (!add_params(self, prompt)){
        obj_destroy(self);
        return NULL;
    }
    
    // Augmentation du nombre d'accolades ouvrantes
    opened_brackets ++;

    // Récupération des commandes de for
    // Appel de Shell_parse avec le reste du prompt.
    self->commands = Shell_parse(prompt); 
    if(!self->commands) {// Si self->command = NULL alors il y a eut une erreur de syntaxe quelque part pendant le parsing.
        obj_destroy(self);
        return NULL; // On renvoie NULL également.
    }

    // Si la caractère pointé par le prompt n'est pas '}'
    if (**prompt != '}'){
        // Alors l'arrêt du parsing de Shell_parse() n'est pas causé par '}'
        // mais par '\0' (il n'y a que ces deux cas qui arrêtent Shell_parse()).
        For_print_syntax_err("Il manque l'accolade fermante !"); // On signale l'erreur de syntaxe.
        obj_destroy(self);
        return NULL;
    }
    
    // Diminution du nombre d'accolades ouvertantes
    opened_brackets--;
    (*prompt)++;

    return self;

}

// Destructeur.
static void* For_destroy(void* self_){
    
    // Vérification des arguments.
    assert(self_);    
    
    For self = (For) self_;
    obj_destroy(self->commands);
    free(self->dir);
    free(self->id_var);

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

    For self = (For) self_;

    write(1, "For : ", 6);
    
    // Ecriture de l'ID
    write(1, "ID= \"",5);
    write(1, self->id_var, strlen(self->id_var));
    write(1, "\"  ", 3);

    // Ecriture du répertoire de travail de for
    write(1, "Dir= \"",6);
    write(1, self->dir, strlen(self->dir));
    write(1, "\"  ", 3);

    

    write(1, "Params=",7);
    obj_print(self->params);

    write(1, " Commands=",10);
    obj_print(self->commands);  
    write(1, " ", 1);
        
}
