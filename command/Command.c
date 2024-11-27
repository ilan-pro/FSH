/**
 * @file Command.c
 * @brief  Fichier source de la classe `_Command_` contenant l'implémentation des variables et de toutes les méthodes de l'interface.
 * 
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../base/new.h"
#include "../base/String.h"
#include "../shell.h"
/**@cond HIDE_THIS*/
#define COMMAND_CLASS "Command"  
#include "Command.h"
/**@endcond*/

/*****************************************************/
/************ Déclaration des fonctions **************/
    
/**
 * Il s'agit du constructeur de la commande.
 *
 * La fonction va récupérer le pointeur vers la saisie de l'utilisateur (usr_prompt dans le fichier `shell.h`) et va 
 * continuer la lecture (en incrémentant le pointeur) jusqu'à ce qu'il tombe sur 
 * le caractère `;`, `|`, `}`.
 *
 * pendant la lecture la fonction va mettre à jour les champs `params` s'il 
 * tombe sur des paramètres de commande (- suivi d'une séquence de lettres) 
 * et `args` si elle tombe sur des arguments (tout ce qui ne commence pas 
 * par un -). Plus tard (si ce n'est pas déjà fait), la fonction va également 
 * reconnaitre les redirections s'il y en a ( `>` ).
 *
 * @param self_ La référence vers la commande.
 * @return La commande avec tous les éléments initialisés.
 */
static void* Command_constructor(void* self_, va_list*);
    
/**
 * Destructeur d'une commande cette fonction va désallouer et détruire les 
 * listes d'arguments et de paramètres.
 *
 * @param self_ La commande à désallouer.
 * @return La commandes avec les ressources désalloués, pour qu'elle puisse être désallouée 
 * à son tour.
 */
static void* Command_destroy(void* self_);    
    
/**
 * Fonction qui va afficher la commande à l'écran.
 *
 * La fonction va afficher une chaine du genre.
 *
 * Command : <Nom_de_la_command>, Params=[...], Args=[...] 
 * @param self_ La commande à afficher.
 */
static void Command_print(const void* self_);
    
/**
 * @internal
 * @brief Fonction qui va exécuter la commande.
 *
 * Cette fonction est abstraite (déclanche une erreur si aucune redéfinition
 * n'est trouvée), à chaque appel la fonction va appeller la méthode `execute()` 
 * de l'objet (liason dynamique). Cela va permettre plus tard de créer une 
 * file de commandes toutes distinctes et de les éxécuter.
 * 
 * @param self La commande à faire exécuter.
 * @return La valeur de retour de la commande après exécution.
 */
int Command_execute(Command self);

/*****************************************************/
/************* Structure de la classe ****************/

typedef struct _Command_ {

    /**
     * @brief Classe de toutes les commandes.
     * 
     * Il s'agit d'une classe abstraite qui représente une commande interne du shell. 
     * Elle possède une méthode abstraite à redéfinir dans tout sous-type de commande 
     * (sinon cela va déclancher une erreur), qui la méthode `execute()` appelée par 
     * `Command_execute()`, la fonction appelée pour exécuter la commande.
     * 
     * Construction : `obj_new(_Command_, <Nom_de_la_commande> )`
     */
    Class _Command_;
    COMMAND_FIELDS
}* Command;

static struct _Class_ __Command__ = {
    NULL,
    COMMAND_CLASS,
    sizeof(struct _Command_),
    Command_constructor,
    Command_destroy,
    0,
    0,
    0,
    Command_print
};

/**
 * @brief Classe de toutes les commandes.
 * 
 * Il s'agit d'une classe abstraite qui représente une commande interne du shell. 
 * Elle possède une méthode abstraite à redéfinir dans tout sous-type de commande 
 * (sinon cela va déclancher une erreur), qui la méthode `execute()` appelée par 
 * `Command_execute()`, la fonction appelée pour exécuter la commande.
 * 
 * Construction : `obj_new(_Command_, <Nom_de_la_commande> )`
 */
const Class _Command_ = &__Command__;

/***************************************************/
/************** Corps des fonctions ****************/

/**
 * @internal
 * @brief Fonction qui vérifie si oui ou non le caractère actuellement pointé par 
 * le pointeur vers le prompte de l'utilisateur est valide, c'est à dire 
 * si on doit oui ou non arrêter la parsing de la commande.
 * 
 * Un caractère est valide si et seulement si : 
 *  - Ce n'est pas le caracètre null (fin de chaine).
 *  - Ce n'est pas l'un des caracètres '|', ';' ou '}'.
 * 
 * Si le caractère n'est pas null (fin de chaine) et qu'il est entre guillemets (simple ou double) ou échapé 
 * alors il est autamatiquement valide ('|', ';' et '}' seront considérés comme des caracètres normaux).
 * 
 * @param quoted Booléen indiquant si le caracètre est entre guillemets simple.
 * @param double_quoted Booléen indiquant si le caracètre est entre guillemets doubles.
 * @param esc Booléen indiquant si le caracètre est echapé par \ .
 * @return Si oui ou non le caractère actuellement pointé par 
 * le pointeur vers le prompte de l'utilisateur est valide.
 */
int Command_actual_char_is_legal(int quoted, int double_quoted, int esc){

    return *usr_prompt && (quoted || double_quoted || esc || ((*usr_prompt) != '|' && (*usr_prompt) != ';' && (*usr_prompt) != '}'));

}

/**
 * Fonction qui va lire les paramètres d'une commande.
 *
 * Cette fonction est appelée par la fonction `Command_parse_usr_prompt()` 
 * au moment elle rencontre un caractère '-'. 
 * La fonction va lire toutes les lettres qu'elle croisera comme étant un 
 * paramètre, jusqu'à ce qu'elle rencontre `;`, `}` ou `|`, ou bien que la 
 * chaine soit terminé (la chaine est toujours le prompt de l'utilisateur).
 * 
 * @param self La commande en cours de construction.
 */
static void Command_parse_usr_prompt_params(Command self){


    /** 
     * Lecture du prompt dans la boucle tant que l'on a pas fini le prompt 
     * ou qu'on a pas rencontré l'un des caractères `;`, `}` ou `|`
     */
    while(*usr_prompt && (*usr_prompt) != '|' && (*usr_prompt) != ';' && (*usr_prompt) != '}' ){
        
        if (*usr_prompt == ' ') // Si on tombe sur un espace alors on a fini de lire.
            break;

        // On crée une chaine à un seul caractère pour le paramètre.
        // TODO : Penser à créer une classe Character (si le temps le permet)
        char param[2];
        param[0] = *(usr_prompt++);
        param[1] = '\0';
        
        // Ajout du paramètre dans la liste.
        LinkedList_add_last(self->params, obj_new(_String_, param), 0);

    }
}


/**
 * Fonction qui va continuer la lecture du prompt de l'utilisateur en récupérant tous 
 * les paramètres de la commande ainsi que les arguments.
 *
 * La fonction s'arrête si le prompt est fini, ou si elle rencontre `;`, `}` ou `|` 
 * pendant le parcours.
 * Si la fonction rencontre l'un des caractères le pointeur n'est pas incrémenté (
 * c'est à dire que l'on ne va pas référencer le pointeur au caractère suivant). 
 * 
 * L'implémentation de la fonction est inspirée de la fonction `parse_prompt()` 
 * écrite dans le fichier `shell.c`
 *
 * @param self La commande en cours de construction.
 */
static void Command_parse_usr_prompt(Command self){
    
    // Nombre de caractère lu. (Si ce nombre vaut 0 alors on a encore rien lu dans le prompt et donc tous les espaces sont ignorés. Le nombre est mis à 0 après chaque lecture d'un mot)
    int char_compteur = 0;
    
    /** 
     * Chaine de caractère qui va contenir le mot en cours de lecture, 
     * cette chaine est troquée après chaque lecture d'un mot (après 
     * avoir rencontré le premier caractère espace depuis la lecture du mot).
     */
    char* mot = (char*)malloc(sizeof(char));
    assert(mot);
    // On ajoute le caractère de fin de chaine au mot.
    mot[0] = '\0';
    
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
        switch (c) {
            default : // par défaut on ajoute le caractère à la chaine en cours de lecture. 
                h_default: // label 

                esc = 0; // On met l'echappement à 0 (si jamais le caractère était échappé)
                // Incrémentation du compteur.
                char_compteur ++;
                // Réallocation pour accueillir le nouveau caractère.
                mot = realloc(mot, (1 + char_compteur) * sizeof(char));
                mot[char_compteur] = '\0';
                mot[char_compteur - 1] = c;
                break; 

            case ' ': // Le cas où le caractère lu est un espace.
    
                if (quoted || double_quoted || esc) // Si on est dans une expression entre guillemets ou que ' ' est echapé
                    goto h_default; // On va dans default

                if (char_compteur){ // Si il y a au moins une lettre lue.
                    
                    // Ajout du mot dans la liste des arguments de la commande.
                    // L'ajout se fait à la fin pour préserver l'ordre de déclaration des arguments (sait-on jamais).
                    LinkedList_add_last(self->args, obj_new(_String_, mot), 0);
                    
                    // Remise à 0 du compteur
                    char_compteur = 0;

                    // troncage du mot en mot vide.
                    mot = realloc(mot, sizeof(char));
                    assert(mot);
                    mot[0] = '\0';
                    continue;

                }
                else // Si on aucun caractère n'est lu alors on ignore l'espace.
                    continue;    
            
            case '-' :  // Pour les paramètres.

                if (char_compteur) {
                    /** 
                     * Si il y a au moins une lettre lue, alors 
                     * le caractère '-' est considéré comme faisant partie du mot 
                     * Donc on va exécuter `default` à la place
                     */
                    goto h_default;
                }
                else if (!Command_actual_char_is_legal(quoted, double_quoted, esc) || *(usr_prompt)==' '){
                    /**
                     * Si le prochain caractère (car on a fait usr_prompt++) n'est pas valide
                     * (et donc qu'il n'y aura pas de prochaine itération) ou que le prochain 
                     * caractère est un espace, alors il n'y pas de paramètre à chercher '-' 
                     * est considéré comme un argument (ex : cd - ou cat - ).
                     */
                    goto h_default;
                }


                // Sinon on récupère les paramètres.
                Command_parse_usr_prompt_params(self);
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
            
            case '\\': // Il n'y a pas d'échappement mais ça ne coûte rien
            
                if (esc) // Si il est echappé
                    goto h_default;
                
                esc = 1;
                break;

            // TODO on peut rajouter le case '>' pour les redirection (pas pour le moment).
            
            
        }

    }
    
    if(char_compteur)  // Si on a lu au moin un mot.
        LinkedList_add_last(self->args,obj_new(_String_, mot), 0); // On ajoute le mot

    free(mot);

}


// Constructeur
static void* Command_constructor(void* self_, va_list* args){
        
    // Vérification de self.
    assert(self_);
    // Conversion en objet Command
    Command self = (Command) self_;
    
    // Initialisation de la liste des arguments et des paramètres.
    self->args = obj_new(_LinkedList_);
    self->params = obj_new(_LinkedList_);
        
    // Récupération du nom de la commande
    char* cmd_name = va_arg(*args, char*);

    char *name = malloc(sizeof(char) * (1+strlen(cmd_name)));
    strcpy(name, cmd_name);

    *((char**) &self->name) = name; // On change le nom (en trichant un peu, cela viole la contrainte sur les const)

    // La fonction execute() est mis à 0 pour déclancher une erreur si la classe fille n'a pas fourni une implémentation.
    *(int(**))&self->execute = 0;

    // Lecture du prompt de l'utilisateur
    Command_parse_usr_prompt(self);

    return self;
}

// Destructeur
static void* Command_destroy(void* self_){
        
    // vérification de self_.
    assert(self_);
    
    // Conversion en Command.
    Command self = (Command) self_;

    //Destructions des listes.
    obj_destroy(self->args);
    obj_destroy(self->params);
    free(self->name);
    return self;

}

// Print
static void Command_print(const void* self_){
        
    // vérification de self_.
    assert(self_);
    
    // Conversion en Command.
    Command self = (Command) self_;
    write(1, "Commande : ", 11);
    write(1, self->name, strlen(self->name));
    write(1, ". Args=", 7);
    obj_print(self->args);
    write(1, ". Params=", 9);
    obj_print(self->params);

}



/**
 * @internal
 * @brief Fonction qui va exécuter la commande.
 *
 * Cette fonction est abstraite (déclanche une erreur si aucune redéfinition
 * n'est trouvée), à chaque appel la fonction va appeller la méthode `execute()` 
 * de l'objet (liason dynamique). Cela va permettre plus tard de créer une 
 * file de commandes toutes distinctes et de les éxécuter.
 * 
 * @param self La commande à faire exécuter.
 * @return La valeur de retour de la commande après exécution.
 */
int Command_execute(Command self){
    
    // Vérification de self
    assert(self);

    // Vérification de si self est un sous-type.
    assert(obj_instance_of(self, _Command_));

    if (!self->execute){ // Si self n'a pas définie une méthode execute() on arrête tout.
        char tmp1[] = "\e[1;31m[Erreur] La commande \"";
        write(2, tmp1, strlen(tmp1));
        write(2, self->name, strlen(self->name));
        char tmp2[] = "\" ne possède pas de fonction `execute()`, pensez à en écrire une !\n\e[0m";
        write(2, tmp2, strlen(tmp2));
        abort();
    }

    // Sinon on éxécute la commande.
    return self->execute(self);

}
























