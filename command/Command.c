/**
 * @file Command.c
 * @brief  Fichier source de la classe `_Command_` contenant l'implémentation des variables et de toutes les méthodes de l'interface.
 * 
 * Construction : `obj_new(`_Command_`, char* nom_de_la_commande, char** prompt )`
 * Avec : <ul>
 *      <li> `_Command_` : La classe des objets de type `Command`. </li>
 *      <li> <code><strong> nom_de_la_commande </strong></code> : Le nom de la commande. </li>
 *      <li> <code><strong> prompt </strong></code> Un pointeur vers le reste du `prompt` (les commandes 
 *      prennent souvent un certain nombre de paramètre ainsi que des arguments).<br>. `char **` permet de 
 *      modifier directement la chaine du prompt sans faire de copie. Alors NB que la chaine  
 *      est modifiée pendant la création de la commande.</li>
 *  </ul>
 */

#include <time.h>
#include "../base/String.h"
#include "../shell.h"
#include <ctype.h>
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
     * Construction : `obj_new(`_Command_`, char* nom_de_la_commande, char** prompt )`
     * Avec : <ul>
     *      <li> `_Command_` : La classe des objets de type `Command`. </li>
     *      <li> <code><strong> nom_de_la_commande </strong></code> : Le nom de la commande. </li>
     *      <li> <code><strong> prompt </strong></code> Un pointeur vers le reste du `prompt` (les commandes 
     *      prennent souvent un certain nombre de paramètre ainsi que des arguments).<br>. `char **` permet de 
     *      modifier directement la chaine du prompt sans faire de copie. Alors NB que la chaine  
     *      est modifiée pendant la création de la commande.</li>
     *  </ul>
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
 * `command_execute()`, la fonction appelée pour exécuter la commande.
 * 
 * Construction : `obj_new(`_Command_`, char* nom_de_la_commande, char** prompt )`
 * Avec : <ul>
 *      <li> `_Command_` : La classe des objets de type `Command`. </li>
 *      <li> <code><strong> nom_de_la_commande </strong></code> : Le nom de la commande. </li>
 *      <li> <code><strong> prompt </strong></code> Un pointeur vers le reste du `prompt` (les commandes 
 *      prennent souvent un certain nombre de paramètre ainsi que des arguments).<br>. `char **` permet de 
 *      modifier directement la chaine du prompt sans faire de copie. Alors NB que la chaine  
 *      est modifiée pendant la création de la commande.</li>
 *  </ul>
 */
const Class _Command_ = &__Command__;

/**
 * @internal
 * @brief Il s'agit du nombre d'accolades ouverts pendant le traitement d'un prompt.
 *
 * Ce champs permet de savoir si une accolade fermante ferme réelement une accolande ouvrante.
 *
 */
int opened_brackets = 0;

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
 * @param prompt Le prompt en cours de paring.
 * @return Si oui ou non le caractère actuellement pointé par 
 * le pointeur vers le prompte de l'utilisateur est valide.
 */
int Command_actual_char_is_legal(int quoted, int double_quoted, int esc, char** prompt){
    
    return **prompt
    && (quoted || double_quoted || esc || (
        *(*prompt) != '|' && 
        *(*prompt) != ';' && 
        *(*prompt) != '{' &&
        *(*prompt) != '}')
    );

}

// Constructeur
static void* Command_constructor(void* self_, va_list* args){
        
    // Vérification de self.
    assert(self_);
    // Conversion en objet Command
    Command self = (Command) self_;
    
    // Initialisation de la liste des arguments et des paramètres.
    self->args = obj_new(_ArrayList_, 0);
    self->params = obj_new(_ArrayList_, 0);
    
    // Cette liste servira de copie des arguments pour remplacer les variables $ par leurs 
    // valeurs si elles existent au moment de l'exécution de la commande. 
    // On fait une copie pour ne pas modifier la liste originale. 
    self->args_tmp = obj_new(_ArrayList_, 0); 
        
    // Récupération du nom de la commande
    char* cmd_name = va_arg(*args, char*);
    char *name = malloc(sizeof(char) * (1+strlen(cmd_name)));
    strcpy(name, cmd_name);
    *((char**) &self->name) = name; // On change le nom (en trichant un peu, cela viole la contrainte sur les const)

    // La fonction execute() est mise à 0 pour déclancher une erreur si la classe fille n'a pas fourni une implémentation.
    *(int(**))&self->execute = 0;

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
    obj_destroy(self->args_tmp);
    free(*((char**) &self->name));
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
    write(1, " \e[1;0m", 7);

}

static ArrayList getSplit(String arg){
    char* debut;
    char* fin;

    size_t size_of_arg = arg->size;
    char* str = malloc(sizeof(char) * (size_of_arg + 1));
    strcpy(str, obj_to_string(arg));

    debut = str;
    fin = str;

    ArrayList rad = obj_new(_ArrayList_, 0);
    ArrayList var = obj_new(_ArrayList_, 0);

    while (*fin){

        if (*fin == '$'){
            *fin = '\0';
            ArrayList_add_last(rad, obj_new(_String_, debut), 0);
            fin++;
            debut = fin;

            while (*fin){

                if (isalnum(*fin) || *fin == '_'){
                    fin++;
                    if(*fin)
                        continue;
                }
                
                if (*fin == '$'){
                    *fin = '\0';
                    ArrayList_add_last(var, obj_new(_String_, debut), 0);
                    ArrayList_add_last(rad, obj_new(_String_, ""), 0);
                    fin++;
                    debut = fin;
                    continue;
                }


                if (!*fin) {
                    ArrayList_add_last(var, obj_new(_String_, debut), 0);
                    debut = fin;
                    goto continuer;
                }

                char tmp = *fin;
                    
                *fin = '\0';
                ArrayList_add_last(var, obj_new(_String_, debut), 0);

                *fin = tmp;
                
                debut = fin++;
                goto continuer;
            }
        }
        fin++;
        continuer :
        continue;
    }
    ArrayList_add_last(rad, obj_new(_String_, debut), 0);
    ArrayList res_split = obj_new(_ArrayList_, 0);

    ArrayList_add_last(res_split, rad, 0);
    ArrayList_add_last(res_split, var, 0);
    free(str);
    return res_split;
}

/**
 * Fonction qui va parcourir les arguments de la command et 
 * va remplacer s'il trouve, les variable de boucles par leur valeurs.
 * 
    
 * @param self La commande à traiter.
 */
static void Command_remplace_variable( Command self, ArrayList new_args ){

    // Parcours des arguments.
    for (size_t i = 0; i < ArrayList_size(self->args); i++){

        String arg = ArrayList_get(self->args, i, 0);
        ArrayList split = getSplit(arg);
        ArrayList rad = ArrayList_get_first(split, 0);
        ArrayList var = ArrayList_get_last(split, 0);
        
        assert(ArrayList_size(rad) == ArrayList_size(var)+1);

        arg = ArrayList_get_first(rad, 1);
        
        for (size_t j = 0; j < ArrayList_size(var); j++){
            String s1 = ArrayList_get(var, j, 1);
            String s2 = ArrayList_get(rad, j + 1, 1);
            const char* value = Shell_get_val(obj_to_string(s1));
            
            if (value){
                obj_destroy(s1);
                s1 = obj_new(_String_, value);
            }else{
                value = getenv(obj_to_string(s1));
                if (value){
                    obj_destroy(s1);
                    s1 = obj_new(_String_, value);
                }else{
                    obj_destroy(s1);
                    obj_destroy(s2);
                    obj_destroy(arg);
                    arg = NULL;
                    break;
                }
            }

            // arg = String_concat(arg, s1);
            String tmp = String_concat(arg, s1);
            obj_destroy(arg);
            arg = tmp;
            tmp = String_concat(arg, s2);
            obj_destroy(arg);
            arg = tmp;
            obj_destroy(s1);
            obj_destroy(s2);
        }
        // obj_print(arg);
        // write(1, "\n", 1);         
        ArrayList_add_last(new_args, arg, 1);
        obj_destroy(arg);
        obj_destroy(split);
    }
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


    // Parcours des arguments pour vérifier s'il n'y a pas de variable $...
    Command_remplace_variable(self, self->args_tmp);
    
    // On garde la liste originale des arguments pour ne pas perdre sa référence.
    ArrayList origin_args = self->args;
    
    // Remplacement de la liste des arguments par la nouvelle.
    self->args = self->args_tmp;

    // Sinon on éxécute la commande.
    int ret_val = self->execute(self);
    
    // Changement de la liste des arguments, remise de l'original.
    self->args = origin_args;
    // suppression de tous les éléments de la liste temporaire.
    ArrayList_clear(self->args_tmp);
    
    return ret_val;
}






















