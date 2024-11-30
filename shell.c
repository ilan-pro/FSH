/**
 * @file shell.c
 * @brief Fichier contenant l'implémentation des fonctions utilisé par le shell pour le traitement et construction des commandes.
 * 
 * On y retrouve notament la fonction `Shell_start()` qui va démarrer le shell et l'intéraction avec l'utilisateur.
 */
#include "shell.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <assert.h>
#include "base/ArrayList.h"
#include "command/For.h"
#include "command/all_cmd.h"
#include "command/ExternalCommand.h"
#include "base/String.h"
#include "shell.h"

#include "base/new.h"


/**********************************************/
/********* Déclaration des fonctions **********/

/**
 * Fonction qui va récupérer le chemin absolue vers le répertoir courant.
 */
static int init_working_dir();


/**
 * Fonction qui cré le texte du prompt
 */
static void update_prompt();
static Class get_internal_cmd(char *char_lu);
static Command apply_cmd(char *char_lu, ArrayList file_secondaire, char** prompt);

/**********************************************/

/**
 * Le résultat de la dernière commande.
 */
int previous_cmd_finish = 0;

/**
 * @internal
 * @brief Il s'agit du chemin absolue vers le repertoire courant.
 */
char working_dir[4096]; // PATH_MAX va éviter d'avoir des débordement.

/**
 * Le chemin vers le répertoire courrant (tronqué dans le prompt).
 */
static char path[PROMPT_CHAR_LIMITS];

/**
 * La chaine qui va contenir le prompt en entier.
 */
static char prompt[50];


static void Shell_print_syntaxe_error(char* usr_prompt, char* cp_prompt){

    char tmp[] = "\nErreur de syntaxe :\n\n\t\e[1;35m";
    write(2, tmp, strlen(tmp));
    write(2, cp_prompt, strlen(cp_prompt));
    write(2, "\n\t", 2);
    // Calcul de la position où a eut lieu l'erreur.
    int pos = usr_prompt - cp_prompt;

    for (int i = 0; i<pos-1; i++)
        write(1, "~", 1);
    write(1, "^\n\e[1;0m", 8);

}

/**
 * @internal
 * Fonction qui démarre le shell "FSH".
 */
int Shell_start(){

    // Initialisation du chemin absolue vers le réprtoire de travail.
    if (init_working_dir())
        return 1;

    // Redirection vers stderr
    rl_outstream = stderr;

    while(1){

        // On vide les identificateurs des boucles for
        ArrayList_clear(For_files);
        ArrayList_clear(For_ids);
        
        // Mise à jour du prompt.
        update_prompt();
        // Récupération de la saisie de l'utilisateur.
        char* usr_prompt = readline(prompt);
        // Copie du pointeur pour sa destruction.
        char *cp_prompt = usr_prompt;
        if (usr_prompt){ // Si le pointeur vers la saisie n'est pas NULL (Pas de EOF)
            ArrayList file_principal = Shell_parse(&usr_prompt); // Parsing de la saisie, pour en extraire les commandes.
                
            if (!file_principal) {// Si la file renvoyée est NULL alors il y a une erreur de syntaxe.
                Shell_print_syntaxe_error(usr_prompt, cp_prompt);
                previous_cmd_finish = 1;
                free(cp_prompt); continue;
            }

            obj_print(file_principal); printf("\n"); // debug
            previous_cmd_finish = Shell_execute_all(file_principal); // Exécution de la file principale.
            obj_destroy(file_principal); // Destruction de la file principale.
            free(cp_prompt); // Destruction du pointeur alloué par readline.
        }
        else{ // Si on est ici c'est qu'il y reception de EOF
            break; // Alors on arrête le shell.
        }
        rl_free_line_state();
    }
    
    // On vide l'historique.
    rl_clear_history();
    return 0;

}

/**
 * @internal
 * @brief Fonction qui va se charger d'initialiser la liste des commandes à exécuter par le shell.
 * La fonction va avoir une file principale qui va contenir toutes les différentes pipelines que l'utilisateur va taper.
 * Nous allons stocker les commandes de chaque pipeline dans les file secondaire qui seront initialisées dans le while à chaque rencontre 
 * de charactère ';'
 * Nous allons stocker le nom de chaque commande dans char_lu qu'on va redimensionner dynamiquement au fur et à mesure
 * de l'avancement dans la pipeline (et maj à 0 quand nous avons fini de lire une commande).
 * Nous allons aussi utiliser une variable qui stock le nombre de charactère lu (utile pour le redimensionnement dynamique et
 * avoir l'information que nous avons déjà lu quelque chose ou non).
 * Quand le programme rencontre un charactère ';' ou un '\0', il ajoute la pipeline à la liste de nos pipelines car
 * cette dernière à finit d'être parsé.
 * Nous avons différents traitements dans notre while. Le traitement de la piepline qui va juste continuer le while
 * (car nous ne le traitons pas encore).
 * Le '}' pour signifier que nous sommes arriver à la fin des instructions d'une boucle for.
 * Le ' ', va se charger de continuer simplement le while si nous n'avons toujours pas lu de commande et va ajouter la commande dans le cas
 * ou nous avons déjà lu une commande puis continue pour continuer de parser.
 * @return La file principale qui va contenir l'ensemble des pipelines tapés par l'utilisateur (Si une erreur de syntaxe est identifié,
 * alors le programme renvoie la liste NULL)
 */
ArrayList Shell_parse(char **prompt){
    // Variable qui stock le nombre de caractères que nous avons lu.
    int lu1lettre = 0;
    // Espace mémoire qui va servir à stocker le nom de la commande
    char *char_lu = malloc(sizeof(char));
    assert(char_lu);
    char_lu[0] = '\0';
    // tant qu'il y a quelque chose à lire (différent \0)
    ArrayList file_principale = obj_new(_ArrayList_, 0);
    ArrayList file_secondaire = obj_new(_ArrayList_, 0);	
    while(1){

        // On récupère le caractère pointé par user_prompt puis on incrémente pour les caractères suivant.
        char res = **prompt;
        
        if(res == '\0'){
            fin:
            //en faire une fonction
            if(lu1lettre > 0){ // Si on a lu au moins une lettre alors il y a une commande à ajouter avant
                if (!apply_cmd(char_lu, file_secondaire, prompt)) {// Si la commande n'est pas créée il y a une erreur de syntaxe.
                    // Annulation du parsing;
                    obj_destroy(file_principale); // Destruction de la file principale.
                    obj_destroy(file_secondaire); // Destruction de la file secondaire en cours de création.
                    free(char_lu);
                    return NULL; // On ne créé pas de file, on renvoie NULL
                }
            }

            // Ajout de la pipeline (si et seulement si elle n'est pas vide)
            if (ArrayList_size(file_secondaire) > 0)
                ArrayList_add_last(file_principale, file_secondaire, 0);
            else // Si elle est vide on la détruit
                obj_destroy(file_secondaire);
                
            break;
        }
        

        
        if(res == '|'){
            if (lu1lettre){ // Si on a lu au moins une lettre alors il y a une commande à ajouter avant
                if (!apply_cmd(char_lu, file_secondaire, prompt)) {// Si la commande n'est pas créée il y a une erreur de syntaxe.
                    // Annulation du parsing;
                    obj_destroy(file_principale); // Destruction de la file principale.
                    obj_destroy(file_secondaire); // Destruction de la file secondaire en cours de création.
                    free(char_lu);
                    return NULL; // On ne créé pas de file, on renvoie NULL
                }
                lu1lettre = 0;
            }

            (*prompt)++; // Décalage vers le caractère suivant
            continue;
        }

        if(res == '}'){
            if (opened_brackets)
                // Si une accolade est ouverte.
                goto fin;
            else { // Sinon erreur de syntaxe
                char tmp[] = "\e[1;31m[Erreur] Il y a une accolade en trop ! \e[1;0m\n";
                write(1,tmp, strlen(tmp));
                obj_destroy(file_principale); // Destruction de la file principale.
                obj_destroy(file_secondaire); // Destruction de la file secondaire en cours de création.
                free(char_lu);
                return NULL; // On ne créé pas de file, on renvoie NULL
            }
        }


        // on check si res est arrivé à la fin d'une PipeLine
        if(res == ';'){
            if (lu1lettre){ // Si on a lu au moins une lettre alors il y a une commande à ajouter avant
                if (!apply_cmd(char_lu, file_secondaire, prompt)) {// Si la commande n'est pas créée il y a une erreur de syntaxe.
                    // Annulation du parsing;
                    obj_destroy(file_principale); // Destruction de la file principale.
                    obj_destroy(file_secondaire); // Destruction de la file secondaire en cours de création.
                    free(char_lu);
                    return NULL; // On ne créé pas de file, on renvoie NULL
                }
                lu1lettre = 0;
            }
            
            // Si nous ne rencontrons un ';' c'est que nous avons fini notre PipeLine et que nous pouvons stocker dans la liste, le résultat de cette dernière.
            // Ajout de la pipeline (si et seulement si elle n'est pas vide)
            if (ArrayList_size(file_secondaire) > 0){
                ArrayList_add_last(file_principale, file_secondaire, 0);
                // Nous pouvons maintenant commencer une nouvelle file afin de stocker la futur PipeLine
                file_secondaire = obj_new(_ArrayList_, 0);	
            }
                

            (*prompt)++; // Décalage vers le caractère suivant
            continue;
        }

        // on vérifie que ce caractère est un espace
        if(res == ' '){
            // si c'est un espace et que nous n'avons pas lu de caratère ...
            if(!lu1lettre){
                // ...alors on continue
            
                (*prompt)++; // Décalage vers le caractère suivant
                continue;
            }
            // si on arrive ici, c'est qu'il y a un espace et qu'on à déjà lu un caractère
            // donc char_lu contient le nom de la commande
            // on remet lu1lettre à 0 car nous voulons relire une commande
            lu1lettre = 0;
            if (!apply_cmd(char_lu, file_secondaire, prompt)) {// Si la commande n'est pas créée il y a une erreur de syntaxe.
                // Annulation du parsing;
                obj_destroy(file_principale); // Destruction de la file principale.
                obj_destroy(file_secondaire); // Destruction de la file secondaire en cours de création.
                free(char_lu);
                return NULL; // On ne créé pas de file, on renvoie NULL
            }

            // on trunk l'espace alloué par l'ancienne commande afin de pouvoir stocker le nom de la nouvelle commande sans l'avoir à la concatener avec l'ancienne
            char_lu = realloc(char_lu, sizeof(char));
            char_lu[0] = '\0';
            // pour éviter d'avoir des espaces si il y a une autre commande qui suit
            // continue pour revenir au debut du whilevoir des espaces si 
            continue;
        }
        
        // ajout du charactère res à char_lu
        lu1lettre++;
        char_lu = realloc(char_lu, (sizeof(char) * (lu1lettre+1)));
        assert(char_lu);
        char_lu[lu1lettre-1] = res;
        char_lu[lu1lettre] = '\0';
        (*prompt)++; // Décalage vers le caractère suivant
    }
    free(char_lu);
    return file_principale;
}
    
/**
 * @internal
 * @brief Fonction qui va exécuter toutes les pipelines d'une 
 * commande structurée (par appel de la méthode execute() de chaque commande).
 * 
 * La fonction s'occupera également de faire les redirections de sorties 
 * si jamais il y en a.
 *
 * @param struct_cmds La commande structurée. Une commande structurée est une liste de listes.
 * @return La valeur de retour de la dernière exécution (si elle échoue alors ce sera la valeur de 
 * son code d'erreur).
 */
int Shell_execute_all(ArrayList struct_cmds){
    // Sert à stocker le status des processus enfant afin de retourner le dernier éxecuter.
    int last_status = 0;

    // Parcours de la liste des pipeline (toute récupération se fait en O(1))
    for (size_t i = 0; i<ArrayList_size(struct_cmds); i++){

        // on récupère la list chaîné de chaque pipeline
        ArrayList pipeline = ArrayList_get(struct_cmds, i, 0);
        
        for (size_t j = 0; j<ArrayList_size(pipeline); j++){
            
            // Récupération de la commande.
            Command cmd = ArrayList_get(pipeline, j, 0); 
            
            // Exécution de la commande.
            last_status = Command_execute(cmd);
            if(last_status != 0)
                return last_status;
        }
    }

    return last_status;
}

/** 
 * Fonction qui va retourner la commande associé 
 * à ce que l'utilisateur à tapé sur le prompt.
 * Et va l'ajouter dans la file de la pipleline qu'on parse actuellement
 */
static Command apply_cmd(char *char_lu, ArrayList file_secondaire, char** prompt){
	Class tmp = get_internal_cmd(char_lu);
	Command cmd;
	if(tmp){

		// je cré l'objet associé à ma commande
		cmd = obj_new(tmp, char_lu, prompt);
        
        // Si la commande interne n'est pas créée alors il y a une erreur de syntaxe, on renvoie NULL
        if(!cmd)
            return NULL;

		//j'ajoute la commande interne à ma file
		ArrayList_add_last(file_secondaire, cmd, 0);	

	}else{

        // donc c'est une commande externe
		cmd = obj_new(_ExternalCommand_, char_lu, prompt);
        ArrayList_add_last(file_secondaire, cmd, 0);

	}
	
    return cmd;
}

/**
 * Fonction qui récupère la commande interne correspondante.
 */
static Class get_internal_cmd(char *char_lu){
    for(int i=0; i<5; i++){
        if(strcmp(char_lu, cmd[i][0]) == 0){
            return (Class)cmd[i][1];
        }
    }
    // si aucune commande externe n'a été trouvé return NULL
    return NULL;
}


/**
 * Fonction qui va allouer l'espace mémoire pour stocker le chemin
 * absolue vers le répertoire courant.
 */
static int init_working_dir(){

    if (!getcwd(working_dir, 4092)){
        perror("Récupération du chemin absolue.");
        return 1;
    }

    return 0;

}
/**
 * Fonction qui créer le texte du prompt
 */
static void update_prompt(){

    // Calcul de la longueur de la chaine (chemin absolue vers le répertoire courant)
    int len = strlen(working_dir);
    // pointeur vers la chaine de caractère. 
    char* tr = working_dir; 

    if (len>PROMPT_CHAR_LIMITS){
        // On pointe vers les 33 derniers caracètres (Les 3 sont pour les  ...)
        tr = working_dir + (len-PROMPT_CHAR_LIMITS+8);
        snprintf(path, 30, "...%s", tr);
    }
    else
        // Sinon on écrit le chemin tel qu'il est.
        snprintf(path, 30, "%s", tr);
    

    if (!previous_cmd_finish) // Si la précédente commande s'est bien terminée.
        snprintf(prompt, 50, "\033[32m[%d]\033[36m%s\033[00m$ ", previous_cmd_finish, path); // On le met en vert.
    else
        snprintf(prompt, 50, "\033[91m[%d]\e[1;33m%s\033[00m$ ", previous_cmd_finish, path); // On le met en rouge.

}




