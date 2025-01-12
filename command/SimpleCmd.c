/**
 * @file SimpleCmd.c
 * @brief  Fichier contenant l'implémentation de la classe `_SimpleCmd_`.
 * 
 * Un objet de type `_SimpleCmd_` est une commande dont les arguments et les paramètre sont récupéré directement (avec un parsing)
 * dans le construteur. Ces commandes sont donc tous de la forme `Cmd_name -param1 -param2 arg1 -parm3 ...`. Les arguments et les 
 * paramètres pouvant êtres déclarés dans un ordre quelconque et les paramètre peuvent être 
 * concaténés (Ex: `cmd -AbCd` = `cmd -A -b -C -d`).
 * <p>
 * Cette classe ne fourni pas de méthode `execute()`, elle doit donc être héritée par une classe qui implémentera la méthode 
 * sinon l'exécution déclanchera une erreur.
 * <p>
 * NB : Les commandes ayant une définition complexe comme les commandes avec des paramètres 
 * non-booléens (Ex : `cmd --param=oiseau -param1 Wazo`  ou  `for i in ... {}` ). Ne peuvent pas être construit directement 
 * avec le constructeur, alors il est conseillé d'hériter directement de la classe `_Command_` au lieu de celle-ci et 
 * de définir un constructeur qui gère la construction de la commande.
 * <p>
 * Construction : <code><strong> obj_new( `_SimpleCmd_`, char* cmd_name, char** prompt ); </strong></code>
 * où : <ul>
 *         <li> `_SimpleCmd_` désigne la classe des objets de type `SimpleCmd`. </li>
 *         <li> `char* cmd_name` Le nom de la commande.</li>
 *         <li> `char** prompt` Le reste du prompt qui va servir à construire la comande.</li>
 * </ul>
 */
/**@cond HIDE_THIS*/
#define SIMPLE_CMD_CLASS "SimpleCommand"  
#include "SimpleCmd.h"
/**@endcond*/

#include <time.h>
#include "../base/String.h"

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
static void* SimpleCmd_constructor(void* self_, va_list*);
    
/**
 * Destructeur d'une commande cette fonction va désallouer et détruire les 
 * listes d'arguments et de paramètres.
 *
 * @param self_ La commande à désallouer.
 * @return La commandes avec les ressources désalloués, pour qu'elle puisse être désallouée 
 * à son tour.
 */
static void* SimpleCmd_destroy(void* self_);    
    

/*****************************************************/
/************* Structure de la classe ****************/

/**
 * @internal
 * @brief Classe de toutes les commandes simples.
 * 
 * Un objet de type `_SimpleCmd_` est une commande dont les arguments et les paramètre sont récupéré directement (avec un parsing)
 * dans le construteur. Ces commandes sont donc tous de la forme `Cmd_name -param1 -param2 arg1 -parm3 ...`. Les arguments et les 
 * paramètres pouvant êtres déclarés dans un ordre quelconque et les paramètre peuvent être 
 * concaténés (Ex: `cmd -AbCd` = `cmd -A -b -C -d`).
 * <p>
 * Construction : <code><strong> obj_new( `_SimpleCmd_`, char* cmd_name, char** prompt ); </strong></code>
 * où : <ul>
 *         <li> `_SimpleCmd_` désigne la classe des objets de type `SimpleCmd`. </li>
 *         <li> `char* cmd_name` Le nom de la commande.</li>
 *         <li> `char** prompt` Le reste du prompt qui va servir à construire la comande.</li>
 * </ul>
 */
typedef struct _SimpleCmd_ {

    /**
     * @internal
     * @brief Classe de toutes les commandes simples.
     * 
     * Un objet de type `_SimpleCmd_` est une commande dont les arguments et les paramètre sont récupéré directement (avec un parsing)
     * dans le construteur. Ces commandes sont donc tous de la forme `Cmd_name -param1 -param2 arg1 -parm3 ...`. Les arguments et les 
     * paramètres pouvant êtres déclarés dans un ordre quelconque et les paramètre peuvent être 
     * concaténés (Ex: `cmd -AbCd` = `cmd -A -b -C -d`).
     * <p>
     * Construction : <code><strong> obj_new( `_SimpleCmd_`, char* cmd_name, char** prompt ); </strong></code>
     * où : <ul>
     *         <li> `_SimpleCmd_` désigne la classe des objets de type `SimpleCmd`. </li>
     *         <li> `char* cmd_name` Le nom de la commande.</li>
     *         <li> `char** prompt` Le reste du prompt qui va servir à construire la comande.</li>
     * </ul>
     */
    Class _SimpleCmd_;
    SIMPLE_CMD_FIELDS
}* SimpleCmd;

static struct _Class_ __SimpleCmd__ = {
    NULL,
    SIMPLE_CMD_CLASS,
    sizeof(struct _SimpleCmd_),
    SimpleCmd_constructor,
    SimpleCmd_destroy,
    0,
    0,
    0,
    0
};

/**
 * @internal
 * @brief Classe de toutes les commandes simples.
 * 
 * Un objet de type `_SimpleCmd_` est une commande dont les arguments et les paramètre sont récupéré directement (avec un parsing)
 * dans le construteur. Ces commandes sont donc tous de la forme `Cmd_name -param1 -param2 arg1 -parm3 ...`. Les arguments et les 
 * paramètres pouvant êtres déclarés dans un ordre quelconque et les paramètre peuvent être 
 * concaténés (Ex: `cmd -AbCd` = `cmd -A -b -C -d`).
 * <p>
 * Construction : <code><strong> obj_new( `_SimpleCmd_`, char* cmd_name, char** prompt ); </strong></code>
 * où : <ul>
 *         <li> `_SimpleCmd_` désigne la classe des objets de type `SimpleCmd`. </li>
 *         <li> `char* cmd_name` Le nom de la commande.</li>
 *         <li> `char** prompt` Le reste du prompt qui va servir à construire la comande.</li>
 * </ul>
 */
const Class _SimpleCmd_ = &__SimpleCmd__;

/***************************************************/
/************** Corps des fonctions ****************/

/**
 * Fonction qui va lire les paramètres d'une commande.
 *
 * Cette fonction est appelée par la fonction `SimpleCmd_parse_usr_prompt()` 
 * au moment elle rencontre un caractère '-'. 
 * La fonction va lire toutes les lettres qu'elle croisera comme étant un 
 * paramètre, jusqu'à ce qu'elle rencontre `;`, `}` ou `|`, ou bien que la 
 * chaine soit terminé (la chaine est toujours le prompt de l'utilisateur).
 * 
 * @param self La commande en cours de construction.
 * @param prompt Le prompt en cours de paring.
 */
static void SimpleCmd_parse_usr_prompt_params(SimpleCmd self, char** prompt){


    /** 
     * Lecture du prompt dans la boucle tant que l'on a pas fini le prompt 
     * ou qu'on a pas rencontré l'un des caractères `;`, `}` ou `|`
     */
    while( **prompt && *(*prompt) != '|' && *(*prompt) != '>' && *(*prompt) != '<' && *(*prompt) != ';' && *(*prompt) != '}' ){
        
        if (**prompt== ' ') // Si on tombe sur un espace alors on a fini de lire.
            break;

        // On crée une chaine à un seul caractère pour le paramètre.
        // TODO : Penser à créer une classe Character (si le temps le permet)
        char param[2];
        param[0] = *((*prompt)++);
        param[1] = '\0';
        
        // Ajout du paramètre dans la liste.
        ArrayList_add_last(self->params, obj_new(_String_, param), 0);

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
 * @param prompt Le prompt en cours de paring.
 * @return Si oui ou non le parsing a réussi.
 */
static int SimpleCmd_parse_usr_prompt(SimpleCmd self, char** prompt){
    
    int ret = 1;

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
    while(Command_actual_char_is_legal(quoted, double_quoted, esc, prompt)){
        // Récupération du caractère pointé par le pointeur, et incémentation de la posisiton.
        char c = *((*prompt)++);
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
                
                add_mot:
                if (char_compteur){ // Si il y a au moins une lettre lue.
                    
                    // Ajout du mot dans la liste des arguments de la commande.
                    // L'ajout se fait à la fin pour préserver l'ordre de déclaration des arguments (sait-on jamais).
                    ArrayList_add_last(self->args, obj_new(_String_, mot), 0);
                    
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
                else if (!Command_actual_char_is_legal(quoted, double_quoted, esc, prompt) || *(*prompt)==' '){
                    /**
                     * Si le prochain caractère (car on a fait usr_prompt++) n'est pas valide
                     * (et donc qu'il n'y aura pas de prochaine itération) ou que le prochain 
                     * caractère est un espace, alors il n'y pas de paramètre à chercher '-' 
                     * est considéré comme un argument (ex : cd - ou cat - ).
                     */
                    goto h_default;
                }


                // Sinon on récupère les paramètres.
                SimpleCmd_parse_usr_prompt_params(self, prompt);
                break;

            case '>': // Redirection
            case '<':
                // Si on est dans une expression entre guillemets, que '>' est echapé ou redirector est NULL
                if (quoted || double_quoted || esc ) 
                    goto h_default; // On va dans default
                
                (*prompt)--; // On repointe sur '>'.
                assert(**prompt=='>' || **prompt=='<');

                if (!char_compteur){ // Si on a rien lu avant alors il n'y a rien à faire on redirige
                    if (!Redirector_add_redirection(self->redirector, prompt)){
                        ret = 0;
                        goto exit;
                    }
                    goto add_mot;
                }
                
                // On vérifie si la lettre précédent '>' est 1 ou 2
                char prec = *((*prompt)-1);
                if (prec=='1' || prec=='2'){
                    // On diminue le compteur de 1 (pour ignorer le nombre)
                    char_compteur--;

                    // Troncage du mot.
                    mot = realloc(mot, (1 + char_compteur) * sizeof(char));
                    assert(mot);
                    mot[char_compteur] = '\0';

                    // Redirection
                    if (!Redirector_add_redirection(self->redirector, prompt)){
                        ret = 0;
                        goto exit;
                    }
                    goto add_mot;
                }
                else{ 
                    // Redirection
                    if (!Redirector_add_redirection(self->redirector, prompt)){
                        ret = 0;
                        goto exit;
                    }
                    goto add_mot;
                }
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
    
    if(char_compteur)  // Si on a lu au moins un mot.
        ArrayList_add_last(self->args,obj_new(_String_, mot), 0); // On ajoute le mot

exit :
    // Mise à jour de la commande
    free(mot);
    return ret;

}


// Constructeur
static void* SimpleCmd_constructor(void* self_, va_list* args){

    /// Mise à jour de _SimpleCmd_ en tant que héritier de _Command_ (si ce n'est pas déjà le cas)
    if(!_SimpleCmd_->_super_)
        *(Class*)& (_SimpleCmd_->_super_) = _Command_;

    // Vérification de self.
    assert(self_);
    // Conversion en objet SimpleCmd
    SimpleCmd self = (SimpleCmd) self_;


    // Appel du constructeur de la classe Command ( parent )
    _Command_->ctor(self, args);

    // On met la fonction print du parent comme fonction d'affichage de l'enfant.
    *((void (**)) &(self->_SimpleCmd_->print)) = _Command_->print;

    // Récupération du prompt
    char** prompt = va_arg(*args, char**);

    // La fonction execute() est mise à 0 pour déclancher une erreur si la classe fille n'a pas fourni une implémentation.
    *(int(**))&self->execute = 0;

    // Lecture du prompt de l'utilisateur
    if (!SimpleCmd_parse_usr_prompt(self, prompt)){
        Command_fail_with_message((Command)self, Command_FAIL_CODE);
        obj_destroy(self);
        return NULL;
    }

    return self;
}

// Destructeur
static void* SimpleCmd_destroy(void* self_){
        
    // vérification de self_.
    assert(self_);
    assert( obj_instance_of(self_, _SimpleCmd_) );
    return _Command_->dtor(self_);

}


















