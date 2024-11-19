#include "shell.h"
#include <complex.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <assert.h>

/**********************************************/
/********* Déclaration des fonctions **********/

/**
 * Fonction qui va récupérer le chemin absolue vers le répertoir courant.
 */
static int init_working_dir();


/**
 * Fonction qui créer le texte du prompt
 */
static void update_prompt();
void parse_prompt();

/**********************************************/

/**
 * Le résultat de la dernière commande.
 */
static int previous_cmd_finish = 0;

/**
 * @internal
 * @brief Il s'agit du chemin absolue vers le repertoire courant.
 */
char working_dir[PATH_MAX]; // PATH_MAX va éviter d'avoir des débordement.

/**
 * Le chemin vers le répertoire courrant (tronqué dans le prompt).
 */
static char path[PROMPT_CHAR_LIMITS];

/**
 * La chaine qui va contenir le prompt en entier.
 */
static char prompt[50];

char *usr_prompt;

/**
 * @internal
 * 
 * Fonction qui démarre le shell "FSH".
 */
int Shell_start(){

    // Initialisation du chemin absolue vers le réprtoire de travail.
    if (init_working_dir())
        return 1;

    while(1){
        
        // Mise à jour du prompt.
        update_prompt();
        usr_prompt = readline(prompt);
        char *cp_prompt = usr_prompt;
        parse_prompt();
        free(cp_prompt);
        rl_free_line_state();
        rl_clear_history();
        break;
    }
    return 0;

}

void parse_prompt(){
    int lu1lettre = 0;
    char *char_lu = malloc(sizeof(char));
    assert(char_lu);
    char_lu[0] = '\0';
    while(*usr_prompt){
        char res = *(usr_prompt++);
        if(res == ' '){
           if(!lu1lettre){
            continue;
           }else{
            printf("%s\n", char_lu);
            break;
           }
        }
        lu1lettre++;
        char_lu = realloc(char_lu, (sizeof(char) * (lu1lettre+1)));
        assert(char_lu);
        char_lu[lu1lettre-1] = res;
        char_lu[lu1lettre] = '\0';
    }
    free(char_lu);
}


/**
 * Fonction qui va allouer l'espace mémoire pour stocker le chemin
 * obsolue vers le répertoire courant.
 */
static int init_working_dir(){

    if (!getcwd(working_dir, PATH_MAX)){
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
        tr = working_dir + (len-PROMPT_CHAR_LIMITS+4);
        snprintf(path, 30, "...%s", tr);
    }
    else
        // Sinon on écrit le chemin tel qu'il est.
        snprintf(path, 30, "%s", tr);
    

    if (!previous_cmd_finish) // Si la précédente commande s'est bien terminée.
        snprintf(prompt, 50, "\033[32m[%d]\033[36m%s\033[00m$ ", previous_cmd_finish, path); // On le met en vert.
    else
        snprintf(prompt, 50, "\033[91m[%d]\033[00m%s\033[00m$ ", previous_cmd_finish, path); // On le met en rouge.

}




