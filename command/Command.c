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

#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include "../base/String.h"
#include "../shell.h"
#include <ctype.h>
#include "Redirector.h"
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


/**
 * @internal
 * @brief Il s'agit d'un code d'echec lors de la création de la consturction d'une commande.
 * 
 * Le message d'arreur peut être affiché en faisant Command_perror(Command_FAIL_CODE).
 */
int Command_FAIL_CODE;
    
/**
 * Il s'agit d'une mémoire tampon qui permet de stocker le 
 * message d'erreur renoyé par une commande.
 */
static char FAIL_BUFF[1024];

/***************************************************/
/************** Corps des fonctions ****************/

/**
 * @internal
 * @brief Fonction qui va afficher le message d'erreur lié au code d'erreur 
 * envoyé par la dernière commande échouée.
 */
void Command_perror(){
    
    write(2, FAIL_BUFF, strlen(FAIL_BUFF));     
    *FAIL_BUFF = '\0';
}

/**
 * @internal 
 * @brief Fonction qui va mettre à jour la valeur du code d'erreur 
 * liée au résultat de la création ou exécution d'une commande.
 *
 * La fonction va également mettre à jour le  message d'erreur 
 * lié au code d'erreur
 *
 * @param self La référence vers la commande (pour récupérer son nom);
 * @param err Le code d'erreur que la commande souhaite stocker.
 */
void Command_fail_with_message(Command self, int err){
    
    // Mise à jour du code de l'erreur.
    Command_FAIL_CODE = err;
    switch (Command_FAIL_CODE){
        default:
            snprintf(FAIL_BUFF, 1024, "%s: uknown error.\n", self->name);
            break;
        case 0 : 
            snprintf(FAIL_BUFF, 1024, "%s: sucess.\n", self->name);
            break;
        case 1 : 
            snprintf(FAIL_BUFF, 1024, "%s: syntaxe error.\n", self->name);
            break;
        case 2 : 
            snprintf(FAIL_BUFF, 1024, "%s: redirection error.\n%s\n", self->name, strerror(errno));
            break;
        case 3 : 
            snprintf(FAIL_BUFF, 1024, "%s: too many arguments.\n", self->name);
            break;
        case 4 : 
            snprintf(FAIL_BUFF, 1024, "%s: arguments missing.\n", self->name);
            break;
    }

}

/**
 * @internal 
 * @brief Fonction qui va mettre à jour la valeur du code d'erreur 
 * liée au résultat de la création ou exécution d'une commande.
 *
 * @param err Le code d'erreur que la commande souhaite stocker.
 */
void Command_fail( int err ){
    
    // Mise à jour du code de l'erreur.
    Command_FAIL_CODE = err;

}

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


/**
 * @internal
 * @brief Fonction qui va récupérer tous les sous chaines séparées par des ' ' (espaces) 
 * dans le prompt jusqu'à la fin de ce dernier (rencontre avec `\0`).
 *
 * Les espace consécutives sont ignorés sauf s'ils font parti d'une chaine délimitée 
 * par des guillemets (' ou ").
 * <br>
 * Lorsque la fonction rencontre un caractère '\' le caractère suivant est échappé. Les 
 * espace sont échapés (c'est à dire qu'ils ne vont pas couper la chaine) s'ils se trouvent 
 * entre guillemets.
 * 
 * @param collect_list La liste qui va contenir les résultats.
 * @param prompt Le prompt qui sera utilisé (la chaine pointée est modifiée).
 * @param redirector L'objet qui s'occupe de la redirection de la commande au cas où un caractère '>'
 * est rencontré.
 * @param stop_in_next Booléen indiquant si oui ou non la fonction doit s'arrêter après la première sous-chaine.
 */
int Command_parse_all_sub(ArrayList collect_list, char **prompt, int stop_in_next, Redirector redirector){
    
    // La valeur de retour de la commande.
    int ret = 1;
        
    int cmd_length = 0;
    char* command = malloc((1 + cmd_length) * sizeof(char));
    assert(command);
    command[cmd_length] = '\0';

    int quoted = 0; // Booléean indiquant si oui ou non on se trouve dans des guillemets.
    int double_quoted = 0; // Booléean indiquant si oui ou non on se trouve dans des guillemets double.
    int esc = 0; // Booléean indiquant si oui ou non le caractère suivant est echappé.
    
    /** 
     * Lecture du prompt dans la boucle tant que le caractère pointé est 
     * valide (voir documentation de la fonction actual_char_is_legal()).
     */
    while(Command_actual_char_is_legal(quoted, double_quoted, esc, prompt)){
        
        // Récupération du caractère pointé par le pointeur, et incémentation de la posisiton.
        char c = *((*prompt)++);
        switch (c){
            default : 
                h_default: // label 

                esc = 0; // On met l'echappement à 0 (si jamais le caractère était échappé)
                // Incrémentation du compteur.
                cmd_length++;
                // Réallocation pour accueillir le nouveau caractère.
                command = realloc(command, (1 + cmd_length) * sizeof(char));
                assert(command);
                command[cmd_length] = '\0';
                command[cmd_length- 1] = c;
                break; 


            case ' ': // Le cas où le caractère lu est un espace.
    
                if (quoted || double_quoted || esc) // Si on est dans une expression entre guillemets ou que ' ' est echapé
                    goto h_default; // On va dans default
                
                add_cmd:
                if (cmd_length){ // Si il y a au moins une lettre lue.
                    
                    // Ajout du mot dans la liste des arguments de la commande.
                    // L'ajout se fait à la fin pour préserver l'ordre de déclaration des arguments (sait-on jamais).
                    ArrayList_add_last(collect_list, obj_new(_String_, command), 0);
                    
                    if (stop_in_next)
                        goto exit;
                    
                    /** clean_buffer: */
                    // Remise à 0 du compteur
                    cmd_length = 0;

                    // troncage du mot en mot vide.
                    command = realloc(command, sizeof(char));
                    assert(command);
                    command[0] = '\0';
                    continue;

                }
                else // Si on aucun caractère n'est lu alors on ignore l'espace.
                    continue;    

            case '"': // Le cas où on a des guillemets doubles
                if (!quoted && !esc) // S'ils sont dans des guillemets simples ou explicitement echapés alors ils sont échapés
                    double_quoted = !double_quoted;

                goto h_default;
                break;

            case '>':
            case '<':
                // Si on est dans une expression entre guillemets, que '>' est echapé ou redirector est NULL
                if (quoted || double_quoted || esc || !redirector) 
                    goto h_default; // On va dans default
                
                (*prompt)--; // On repointe sur '>'.
                assert(**prompt=='>' || **prompt=='<');

                if (!cmd_length){ // Si on a rien lu avant alors il n'y a rien à faire on redirige
                    if (!Redirector_add_redirection(redirector, prompt)){
                        ret = 0;
                        goto exit;
                    }
                    goto add_cmd;
                }
                
                // On vérifie si la lettre précédent '>' est 1 ou 2
                char prec = *((*prompt)-1);
                if (prec=='1' || prec=='2'){
                    // On diminue le compteur de 1 (pour ignorer le nombre)
                    cmd_length--;

                    // Troncage du mot.
                    command = realloc(command, (1 + cmd_length) * sizeof(char));
                    assert(command);
                    command[cmd_length] = '\0';

                    // Redirection
                    if (!Redirector_add_redirection(redirector, prompt)){
                        ret = 0;
                        goto exit;
                    }
                    goto add_cmd;

                }
                else{ 
                    // Redirection
                    if (!Redirector_add_redirection(redirector, prompt)){
                        ret = 0;
                        goto exit;
                    }
                    goto add_cmd;
                }
                break;
            case '\'': // Le cas où on a des guillemets simples
                if (!double_quoted && !esc) // S'ils sont dans des guillemets doubles ou explicitement echapés alors ils sont échapés
                    quoted = !quoted;

                goto h_default;
                break;
            
            case '\\': // Les echapement peuvent exister dans les commandes externes ex: find ... -exec rm {} \; ici ; est echapé
            
                if (esc) // Si il est echappé
                    goto h_default;
                
                esc = 1;
                break;
        }
    }
    
    if (cmd_length) // Si il y a au moins une lettre lue.
        // Ajout du mot dans la liste des arguments de la commande.
        // L'ajout se fait à la fin pour préserver l'ordre de déclaration des arguments (sait-on jamais).
        ArrayList_add_last(collect_list, obj_new(_String_, command), 0);
 
exit :
    // Mise à jour de la commande
    free(command);
    return ret;
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
        
    // Initialisation de l'objet de redirection
    self->redirector = obj_new(_Redirector_);

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
    obj_destroy(self->redirector);
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
    write(1, ". Redirector=", 13);
    obj_print(self->redirector);
    write(1, " \e[1;0m", 7);

}

/**
 * @brief Fonction qui pour une chaine de caractère (donnée sous-forme d'objet String) 
 * donnée, va découper la chaine de caractère afin de récupérer les nom de variables 
 * qui y figurent.
 * 
 * L'objet `_String_` donné n'est pas modifié.<br>
 * La fonction va renvoyer une `_ArrayList_` contenant deux `_ArrayList_`, la première 
 * contiendera les chaines de caractères qui délimitaient les variables et l'autre contiendera
 * les noms des variables dans l'ordre où elles ont été déclarées dans la chaine de départ.
 * <br>
 * On peut alors reassembler la chaine avec les valeurs des variables, on assemblant ainsi 
 * `radical_0`+`variable_0`+`radical_1`+`variable_1`+...+`variable_n`+`radical_n+1` 
 * avec `radical_i` un élément de la première liste et `variable_i` un élément de la seconde.
 * Le derinier radical (`radical_n+1`) peut être vide si le mot terminait avec une variable.
 *
 * @param arg La chaine de caractère qui va subir le traitement (l'objet reste intact).
 * @return L'`_ArrayList_` contenant les radicaux et les noms des variables.
 */
static ArrayList getSplit(String arg){

    /**
     * Chaines de caractère qui vont servir à délimiter les mots dans la chaine.
     */
    char* debut;
    char* fin;
    
    /**
     * Récupération de la taille de la chaine afin d'en faire une copie modifiable. 
     * La chaine de départ est donc laissée telle qu'elle est.
     */
    size_t size_of_arg = arg->size;
    // Allocation de la mémoire qui va accueillir la copie.
    char* str = malloc(sizeof(char) * (size_of_arg + 1));
    // Copie de la chaine.
    strcpy(str, obj_to_string(arg));
    
    /**
     * Les deux pointeurs de délimitation de pointeront dans un premier temps 
     * le début de chaine.
     */
    debut = str;
    fin = str;
    
    // Création des listes qui vont accueillir les mots découpés
    ArrayList rad = obj_new(_ArrayList_, 0);
    ArrayList var = obj_new(_ArrayList_, 0);
    
    /**
     * Algorithme : 
     *
     * On parcours la chaine à l'aide des deux pointeurs `debut` et `fin` 
     * `debut` comme son nom l'indique va pointer vers le début du prochain 
     * mot qui sera trouvé, il ne bougera donc pas tant qu'un mot n'est pas trouvé.
     * `fin` comme son nom l'indique pointera vers la fin d'un mot, c'est ce pointeur 
     * qui va servir à ajouter le caractère `\0` sur la chaine `str` afin de créer une 
     * sous-chaine commençant par `debut` obtenant ainsi un mot.
     * 
     * Lorsque `fin` rencontre un caractère `$` (symbole indiquant la présence d'une variable), 
     * il met un caractère de fin (`\0`) et le mot commençant par début est ajouté dans la liste 
     * des radicaux. Ensuite `debut` est décalé à `fin` afin de permettre de récupérer le prochain 
     * mot, qui sera ici le nom de la variable. Tant que `fin` ne lira pas le caractère de fin 
     * ou un caractère non-alphanumérique, il continue d'avancer. Une fois terminé `fin` ajoute un
     * caractère de fin et le mot commençant par `debut` est ajouté dans la liste des variables.
     * `debut` est de nouveau décalé à `fin` et la boucle recommence.
     */
    while (*fin){ // Tant qu'on a pas lu toute la chaine.
        
        // Si fin rencontre un `$` (symbole d'une varaible)
        if (*fin == '$'){

            // Ajout du caractère de fin de chaine pour recépérer le radical
            *fin = '\0'; 
            // Ajout du radical dans la liste.
            ArrayList_add_last(rad, obj_new(_String_, debut), 0);
            // Mise à jour des pointeurs. `fin` et `debut` pointent vers le premier caractère du prochain mot.
            fin++; 
            debut = fin;
            
            // Récupération du nom de la variable.
            while (*fin){ // Tant qu'on a pas fini la chaine.
                
                /**
                 * Si le caractère pointé par `fin` est un caractère alphanumérique ou un undescore 
                 * ('_' On se permet d'autoriser ce caractère comme séparateur) alors il fait parti du 
                 * nom de la variable.
                 */
                if (isalnum(*fin) || *fin == '_'){
                    fin++; // Avancement.
                           
                    // On continue seulement si le prochain caractère n'est pas la fin (il y a une petite 
                    // gestion à faire avant de quitter la boucle).
                    if(*fin) 
                        continue;
                }
                
                if (*fin == '$'){ // Si le caractère est '$' (donc il y a concaténation de deux variables)
                    *fin = '\0'; // Alors on a trouvé le nom de la variable.
                    // On ajoute la variable.
                    ArrayList_add_last(var, obj_new(_String_, debut), 0);

                    // On ajoute également une chaine de caractère (vide) qui va servir de séparateur entre 
                    // les deux. Il est important d'en ajouter une car l'algorithme par du principe 
                    // qu'on a toujours à la fin `radical`+`variable`+`radical`+`variable`+... Le cas `variable`+`variable`
                    // ne satisferait pas cet invariant d'où le fait de rajouter une chaine vide dans la liste 
                    // des radicaux.
                    ArrayList_add_last(rad, obj_new(_String_, ""), 0);
                    
                    // Mise à jour des pointeurs. `fin` et `debut` pointent vers le premier caractère du prochain mot.
                    fin++;
                    debut = fin;
                    continue;
                }

                /**
                 * Si on arrive à la fin de la chaine princiapal alors on 
                 * ajoute le nom de la variable avant de quitter.
                 */
                if (!*fin) {
                    ArrayList_add_last(var, obj_new(_String_, debut), 0);
                    // Mise à jour des pointeurs. `fin` et `debut` pointent vers le premier caractère du prochain mot 
                    // (qui est vide mais c'est fait exprès pour préserver l'invariant).
                    debut = fin;
                    goto continuer;
                }
                
                // Si on est ici c'est qu'on a un caractère qui n'est pas un caractère pour une variable
                // Alors on s'arrête là, on récupère le mot comme nom de la variable.
                char tmp = *fin;
                    
                *fin = '\0';
                ArrayList_add_last(var, obj_new(_String_, debut), 0);
                // On remet le caractère précédent pour ne pas modifier le prochain mot;
                *fin = tmp;
                
                // Mise à jour des pointeurs. `fin` et `debut` pointent vers le premier caractère du prochain mot.
                debut = fin++;
                goto continuer;
            }
        }

        // Si on est ici c'est que le caractère pointé n'est pas '$' donc fait parti du mot racine.
        fin++;
        continuer : // Label
        continue; // Alors on continue.
    }

    // Une fois en dehors de la boucle on a ajoute le deriner mot (qui n' a pas eut le temps d'être ajouté à cause de l'arrêt de la boucle).
    ArrayList_add_last(rad, obj_new(_String_, debut), 0);

    // Création d'un ArrayList qui va accueillir le deux listes.
    ArrayList res_split = obj_new(_ArrayList_, 0);
    
    // Ajout des deux listes.
    ArrayList_add_last(res_split, rad, 0);
    ArrayList_add_last(res_split, var, 0);

    // Libération la chaine alloué pour l'exécution de l'algorithme.
    free(str);

    // Renvoie de la valeur.
    return res_split;
}

/**
 * @internal
 * @brief Fonction qui pour un mot `_String_` donné (pouvant contenir des déclarations 
 * de variables) va renvoyer la chaine de caractère où on a remplacé toutes les 
 * variables par leur valeur.
 *
 * La fonction va renvoyer NULL si mot donné a une variable qui n'existe pas.<br>
 * L'ordre de déclaration des variables est gardé.<br>
 * Si le mot ne contient pas de variable, il est renvoyé tel qu'il est. (Attention 
 * le mot qui sera renvoyé sera le même mot mais pas la même référence au mot donné 
 * en argument. Garder cela à l'esprit pour éviter les fuites de méomire, les deux mots 
 * doivent être détruit !)
 *
 * @param str La chaine de caractère à transformer. (Cette chaine n'est ni modifiée ni détruite)
 * @return NULL si la chaine possède une déclaration de variables inexistantes ou le mot dont 
 * le nom de chaque variable déclarée est remplacé par sa valeur.
 */
String replaceVar( String str ){
        // Récupération de l'argument.
        // Récupération des deux listes où on a extrait les noms des variables de l'argument.
        ArrayList split = getSplit(str);
        ArrayList rad = ArrayList_get_first(split, 0);
        ArrayList var = ArrayList_get_last(split, 0);
        
        // Vérification que l'on a bien que la liste des radicaux et celle des variables diffèrent de 1.
        assert(ArrayList_size(rad) == ArrayList_size(var)+1);
        
        // Récupération du premier radical.
        str = ArrayList_get_first(rad, 1);
        
        // Parcours de la liste des variables.
        for (size_t j = 0; j < ArrayList_size(var); j++){ 
            // Récupération du nom de la variable.
            String s1 = ArrayList_get(var, j, 1);
            // Récupération du radical.
            String s2 = ArrayList_get(rad, j + 1, 1);
            // Récupération de la valeur de la variable.
            const char* value = Shell_get_val(obj_to_string(s1));
            
            if (value){ // Si cette valeur existe.
                obj_destroy(s1); // On détruit s1 (il n'est plus utile)
                s1 = obj_new(_String_, value); // On le remplace par la valeur de la variable.

            }else{ // Sinon on regarde parmi les variables d'environement si la valeur existe.
                value = getenv(obj_to_string(s1));
                if (value){ // Si elle existe.
                    obj_destroy(s1); // On détruit s1 (il n'est plus utile)
                    s1 = obj_new(_String_, value); // On le remplace par la valeur de la variable.
                }else{ 
                    // Sinon on détruit tout le monde et l'argument n'est plus valide (il contient une variable qui n'existe pas).
                    obj_destroy(s1);
                    obj_destroy(s2);
                    obj_destroy(str);
                    str = NULL;
                    break; // On quitte la boucle.
                }
            }

            // arg = String_concat(arg, s1);
            // Concaténation du radical et de la valeur de la variable.
            String tmp = String_concat(str, s1);
            obj_destroy(str);
            str = tmp;
            tmp = String_concat(str, s2);
            obj_destroy(str);
            str = tmp;
            obj_destroy(s1);
            obj_destroy(s2);
        }

        // Une fois terminé on ajoute la vraie valeur de l'argument. 
        obj_destroy(split);
        return str;                                           
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
        // Récupération de l'argument avec les noms de variables remplacés.
        String arg = replaceVar( ArrayList_get(self->args, i, 0) );
        ArrayList_add_last(new_args, arg, 0);
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

    // Application de la redirection.
    int ret = Redirector_apply_redirection(self->redirector);
    if (!ret) // S'il y a une erreur d'ourture de fichier.
        return 1;

    // Parcours des arguments pour vérifier s'il n'y a pas de variable $...
    Command_remplace_variable(self, self->args_tmp);
    
    // On garde la liste originale des arguments pour ne pas perdre sa référence.
    ArrayList origin_args = self->args;
    
    // Remplacement de la liste des arguments par la nouvelle.
    self->args = self->args_tmp;

    // Sinon on éxécute la commande.
    int ret_val = self->execute(self);
    
    // Redirection multiple des sorties si activée (stdin est remis si possible)
    Redirector_multiple_redirection(self->redirector);

    // Changement de la liste des arguments, remise de l'original.
    self->args = origin_args;
    // suppression de tous les éléments de la liste temporaire.
    ArrayList_clear(self->args_tmp);
    

    return ret_val;
}






















