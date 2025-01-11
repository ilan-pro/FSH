/**
 * @file shell.c
 * @brief Fichier contenant l'implémentation des fonctions utilisé par le shell pour le traitement et construction des commandes.
 * 
 * On y retrouve notament la fonction `Shell_start()` qui va démarrer le shell et l'intéraction avec l'utilisateur.
 */
#include "shell.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
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

/**
 * Fonction qui va initialiser la tables des variables qui seront utilisées 
 * dans le Shell.
 * 
 * Les boucles for par exemple viendront stocker les différents identificateurs dans la table.
 * 
 * La table n'est pas vraiment une table (pas une table de Hachage mais c'est envisageable, si 
 * le temps le premet).
 * 
 * Il s'agit de deux listes dont l'une contient les indentificateurs et l'autre les valeurs.
 * 
 * Les fonctions qui gère ces deux listes vont garentir que les indices des valeurs et des identificateurs 
 * seront les mêmes.
 * L'encapsulation de ces deux listes permet d'éviter qu'un élément extérieur vienne casser cet invariant. 
 */
static void init_var_table();

/**
 * Fonction qui va détruire la tables des variables qui seront utilisées 
 * dans le Shell.
 * 
 * Les boucles for par exemple viendront stocker les différents identificateurs dans la liste.
 */
static void destroy_var_table();

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

/** 
 * @brief Il s'agit de la liste qui va contenir les identificateurs des différentes variables 
 * utilisées dans le shell.
 *
 * L'indice de l'identificateur est le même que sa valeur  contenu dans 
 * l'autre liste qui contient les valeurs (`Ids_values_table`) (une HashMap serait plus intérressant mais pas le temps).
 */
static ArrayList Ids_tables;

/**
 * @brief Il s'agit de la liste contenant les valeurs des différentes variables dont les identificateurs sont 
 * dans la liste : `Ids_tables`.
 * 
 * L'indice de la vlaeur est le même que celui de son identificateur contenu dans 
 * l'autre liste qui continent les identificateurs (`Ids_values_table`) (une HashMap serait plus intérressant mais pas le temps).
 */
static ArrayList Ids_values_table;


//****************** Fonctions **************** */
/********************************************** */

/**
 * Fonction qui va écire dans le shell tout le prompt suivi de la 
 * d'une ligne rempli de '~' jusqu'à la dernière position pointé 
 * par `usr_prompt` (position qui a déclanché l'erreur de syntaxe).
 * 
 * Cette fonction permet de signaler une erreur de syntaxe dans 
 * la saisie de l'utilisateur.
 *
 * @param usr_prompt Le pointeur original de la saisie de l'utilisateur (celui 
 * qui a été parsé donc décalé jusqu'au caractère déclanchant l'erreur).
 *
 * @param cp_prompt Une copie de la saisie de l'utilisateur (permettant d'indiquer 
 * sur la chaine le caractère qui a déclancher l'erreur.)
 */
static void Shell_print_syntaxe_error(char* usr_prompt, char* cp_prompt){
    
    Command_perror();
    write(2, "\e[1;35m\n\t", 9);
    write(2, cp_prompt, strlen(cp_prompt));
    write(2, "\e[1;36m\n\t", 9);
    // Calcul de la position où a eut lieu l'erreur.
    int pos = usr_prompt - cp_prompt;

    for (int i = 0; i<pos-1; i++)
        write(1, "~", 1);
    write(1, "^\n\e[1;0m", 8);

}

// /**
//  * @internal
//  * Fonction qui démarre le shell "FSH".
//  */
// int Shell_start(){

//     // Initialisation du chemin absolue vers le réprtoire de travail.
//     if (init_working_dir())
//         return 1;

//     // initialisation de la liste des variables qui seront utilisées dans le shell.
//     init_var_table();

//     // Redirection vers stderr
//     rl_outstream = stderr;

//     while(1){
        
//         // Mise à jour du prompt.
//         update_prompt();
//         // Récupération de la saisie de l'utilisateur.
//         char* usr_prompt = readline(prompt);
//         // Copie du pointeur pour sa destruction.
//         char *cp_prompt = usr_prompt;
//         if (usr_prompt){ // Si le pointeur vers la saisie n'est pas NULL (Pas de EOF)
//             ArrayList file_principal = Shell_parse(&usr_prompt); // Parsing de la saisie, pour en extraire les commandes.
                
//             if (!file_principal) {// Si la file renvoyée est NULL alors il y a une erreur de syntaxe.
//                 /** Shell_print_syntaxe_error(usr_prompt, cp_prompt); */
//                 previous_cmd_finish = 1;
//                 free(cp_prompt); // Destruction du pointeur alloué par readline.
//             }
//             else {
//                 /** obj_print(file_principal); printf("\n"); // debug */
//                 previous_cmd_finish = Shell_execute_all(file_principal); // Exécution de la file principale.
//                 obj_destroy(file_principal); // Destruction de la file principale.
//                 free(cp_prompt); // Destruction du pointeur alloué par readline.
//             }
//         }
//         else{ // Si on est ici c'est qu'il y reception de EOF
//             break; // Alors on arrête le shell.
//         }
//         rl_free_line_state();
//     }
    
//     // Destruction de la liste des variables utilisées dans le shell.
//     destroy_var_table();
//     // On vide l'historique.
//     rl_clear_history();
//     return previous_cmd_finish; // On revoie le résultat de la dernière commande.

// }

/**
 * @internal
 * Fonction qui démarre le shell "FSH".
 */
int Shell_start() {
    // Initialisation du chemin absolue vers le répertoire de travail.
    if (init_working_dir())
        return 1;

    // Initialisation de la liste des variables qui seront utilisées dans le shell.
    init_var_table();

    // Redirection vers stderr
    rl_outstream = stderr;

    while (1) {
        // Mise à jour du prompt.
        update_prompt();
        // Récupération de la saisie de l'utilisateur.
        char *usr_prompt = readline(prompt);
        // Copie du pointeur pour sa destruction.
        char *cp_prompt = usr_prompt;
        if (usr_prompt) {  // Si le pointeur vers la saisie n'est pas NULL (Pas de EOF)
            // Ajouter la commande à l'historique si elle n'est pas vide, on en à besoin pour que l'historique se construise
            if (*usr_prompt) {
                add_history(usr_prompt);
            }

            // Parsing de la saisie, pour en extraire les commandes.
            ArrayList file_principal = Shell_parse(&usr_prompt);

            if (!file_principal) {  // Si la file renvoyée est NULL alors il y a une erreur de syntaxe.
                /** Shell_print_syntaxe_error(usr_prompt, cp_prompt); */
                previous_cmd_finish = 1;
                free(cp_prompt);  // Destruction du pointeur alloué par readline.
            } else {
                /** obj_print(file_principal); printf("\n"); // debug */
                previous_cmd_finish = Shell_execute_all(file_principal);  // Exécution de la file principale.
                obj_destroy(file_principal);  // Destruction de la file principale.
                free(cp_prompt);  // Destruction du pointeur alloué par readline.
            }
        } else {  // Si on est ici c'est qu'il y a réception de EOF
            break;  // Alors on arrête le shell.
        }
        rl_free_line_state();
    }

    // Destruction de la liste des variables utilisées dans le shell.
    destroy_var_table();
    // On vide l'historique, il n'est pas persistant (entre deux sessions nous n'aurons pas de liens entre les historiques)
    rl_clear_history();
    return previous_cmd_finish;  // On renvoie le résultat de la dernière commande.
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
            if(lu1lettre > 0) // Si on a lu au moins une lettre alors il y a une commande à ajouter avant
                if (!apply_cmd(char_lu, file_secondaire, prompt)) // Si la commande n'est pas créée il y a une erreur de syntaxe.
                    goto error;

            // Ajout de la pipeline (si et seulement si elle n'est pas vide)
            if (ArrayList_size(file_secondaire) > 0)
                ArrayList_add_last(file_principale, file_secondaire, 0);
            else // Si elle est vide on la détruit
                obj_destroy(file_secondaire);
                
            break;
        }
        
        if (res == '{') // L'ouverture d'une accolade marque la fin d'un parsing.
            goto fin;   
        
        if(res == '|'){
            if (lu1lettre){ // Si on a lu au moins une lettre alors il y a une commande à ajouter avant
                if (!apply_cmd(char_lu, file_secondaire, prompt)) // Si la commande n'est pas créée il y a une erreur de syntaxe.
                    goto error;

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
                goto error;
            }
        }


        // on check si res est arrivé à la fin d'une PipeLine
        if(res == ';'){
            if (lu1lettre){ // Si on a lu au moins une lettre alors il y a une commande à ajouter avant
                if (!apply_cmd(char_lu, file_secondaire, prompt)) // Si la commande n'est pas créée il y a une erreur de syntaxe.
                    goto error;

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
            if (!apply_cmd(char_lu, file_secondaire, prompt)) // Si la commande n'est pas créée il y a une erreur de syntaxe.
                goto error;

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

error:
    // Annulation du parsing;
    obj_destroy(file_principale); // Destruction de la file principale.
    obj_destroy(file_secondaire); // Destruction de la file secondaire en cours de création.
    free(char_lu);
    return NULL; // On ne créé pas de file, on renvoie NULL

}

/**
 * Fonction qui va échanger les descripteurs de deux tubes.
 */

static void swap_pipes(int* const pipe1, int* const pipe2){
    
    int tmp[2];
    tmp[0] = pipe1[0];
    tmp[1] = pipe1[1];

    pipe1[0] = pipe2[0];
    pipe1[1] = pipe2[1];

    pipe2[0] = tmp[0];
    pipe2[1] = tmp[1];

}   
   
/**
 * Fonction qui va s'occuper de faire les redirections des entrées 
 * et sorties standards sur les tubes (si elles sont possibles).
 *
 * @param pipe1 Les descripteurs du premier tube (tube entrant).
 * @param pipe2 Les descripteurs du second tube (tube sortant).
 * @param stdout_def La duplication de la sortie standard (permet de remettre la sortie 
 * de départ).
 *
 * @param command_index La position de la commande dans la pipeline (pour pourvoir savoir quelles redirections lui appliquer).
 * @param pipleline_size La taille de la pipeline.
 */
static void pipe_redirection(const int* pipe1, const int* pipe2, const int stdout_def, const int command_index, const size_t pipeline_size ){
    
    /**
     * Remplacement de la sortie standard par le coté 'écriture' 
     * du tube `pipe2`. (le second tube, tube de sortie)
     *
     * Sauf la dernière commande qui est redirigée vers le `stdout` 
     * par défaut.
     */
    if( command_index < pipeline_size - 1 ){
        if( dup2(pipe2[1], 1) == -1 )
            perror("dup2 - stdout");
    }
    else{ // Ce cas conserne la dernière commande
        if ( dup2(stdout_def, 1) == -1) // Redirection vers `stdout`
            perror("dup2 - Redirection vers `stdout` de la dernière commande");
    }
    
    /**
     * Remplacement de l'entrée standard par le coté 'lecture' 
     * du tube `pipe1`. (le premier tube, tube d'entrée).
     *
     * La première commande ne subit aucune redirection.
     */
    if ( command_index > 0 ){ // Première commande ignorée.
        if ( dup2(pipe1[0], 0) == -1 )
            perror("dup2 - stdin");
    }

    /**
     * L'écrivain de `pipe1` doit impérativement être fermé sinon 
     * la commande ne pourra jamais recevoir EOF dans au moment 
     * de la lecture du tube, qui est son `stdin` (en général 
     * ce n'est qu'à partir de la seconde commande du pipleline
     * que pipe1[0] est le `stdin`, voir ci-dessus).
     */
    close(pipe1[1]); // Peut retourner -1 s'il est déjà fermé (Ex: redirection au sein de Command_execute())
    
    
}

/**
 * @brief Fonction qui va afficher des informations sur l'endroit 
 * où sont redirigées la sortie et l'entrée standard.
 *
 * Cette fonction peut servir pour débuger.
 * 
 * @param cmd La commande actuelle.
 * @param Le numéro du terminal (tout est écrit sur le terminal directement et non dans stdout)
 */
void show_std_new_location(Command cmd, int term_num){
    
    char ter[20];
    snprintf(ter, 20, "/dev/pts/%d", term_num);

    int fd = open(ter, O_RDWR);
    char tmp[400];
    char target_stdin[128];
    char target_stdout[128];
    ssize_t len1 = readlink("/proc/self/fd/0", target_stdin, sizeof(target_stdin) - 1);
    ssize_t len2 = readlink("/proc/self/fd/1", target_stdout, sizeof(target_stdout) - 1);
    target_stdin[len1] = '\0';
    target_stdout[len2] = '\0';
    snprintf(tmp, 400, "\e[1;36mCommande : %s\nDescripteur stdin : %s | stdout : %s\n", cmd->name, target_stdin , target_stdout);;
    write(fd, tmp, strlen(tmp));
    write(fd, "\e[1;0m\n", 7);
    close(fd);

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
    
    // S'il n'y a rien à exécuter alors on ne fait rien, on renvoie le résultat de la commande précédente.
    if (ArrayList_size(struct_cmds) == 0)
        return previous_cmd_finish;

    // Sert à stocker le status des processus enfant afin de retourner le dernier éxecuter.
    int last_status = 0;
    
    /**
     * Création de deux tubes, entrant et sortant dont l'un (pipe1) sera celui qui 
     * remplacera `stdin` (tube entrant, en lecture) et l'autre (pipe2) remplacera 
     * `stdout` (tube sortant, en écriture).
     */
    int pipe1[2];
    int pipe2[2];
    
    pipe(pipe1);
    pipe(pipe2);


    /**
     * On fait une copie des descripteurs 
     * de stdin et stdout actuels.
     *
     * La copie est faite ici car `stdin` et `stdout` peuvent être redirigées
     * avant l'exécution, 
     *      Ex: pwd | for i in . {ftype $i|wc} | sort.
     * Dans cet exemple lors de l'éxécution de {ftype $i|wc} stdin et stdout 
     * seront des tubes.
     *
     * Mais ça peut tout simplement être une reidrection vers des fichiers 
     * donc il est important de remettre les descripteurs à leur place après chaque pipeline.
     */
    int stdin_def = dup(0);
    int stdout_def = dup(1);
    int stderr_def = dup(2);

    // Parcours de la liste des pipeline (toute récupération se fait en O(1))
    for (size_t i = 0; i<ArrayList_size(struct_cmds); i++){
        
        // on récupère la list chaîné de chaque pipeline
        ArrayList pipeline = ArrayList_get(struct_cmds, i, 0);
        
        for (size_t j = 0; j<ArrayList_size(pipeline); j++){
            
            // Redirection des entrées et sortie des commande la pipeline.
            pipe_redirection(pipe1, pipe2, stdout_def, j, ArrayList_size(pipeline));

            // Récupération de la commande.
            Command cmd = ArrayList_get(pipeline, j, 0); 

            //Debug
            // show_std_new_location(cmd, 2);
            assert(fcntl(pipe1[1], F_GETFD)==-1); // pipe1[1] doit toujours être fermé !

            // Exécution de la commande.
            last_status = Command_execute(cmd);
            if(last_status != 0){
                Command_perror();
                /** goto exit; */
            }
            
            // Pipe1 est inutilisable car son écrivain est fermé (donc il en faut un nouveau)
            close(pipe1[0]); 
            
            // Pas besoin de créer un Pipe pour le dernier (c'est juste une optimisation)
            if (j < ArrayList_size(pipeline)-1) 
                pipe(pipe1);
            
            // échange de pipe1 et pipe2.
            swap_pipes(pipe1, pipe2);

        }
        
        // Remise de stdin comme entrée standard (stdout est toujours remis après la dernière commande chaque pipeline)
        dup2(stdin_def, 0);
        dup2(stderr_def, 2);
    }
    
    /** exit: */
    dup2(stdin_def, 0);
    dup2(stdout_def, 1);
    dup2(stderr_def, 2);
    // Fermeture des tube utilisés
    close(pipe1[0]);
    close(pipe2[0]);
    close(pipe1[1]);
    close(pipe2[1]);

    // fermeture des copie des descFic de stdin et stdout copiés (surtout si c'était des tubes, à fermer absolument !)
    close(stdin_def);
    close(stdout_def);
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
    for(int i=0; i<ALL_COMMAND_SIZE; i++){
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

    char val_ret[10]; // Chaine qui va stocker le code de retour.
    snprintf(val_ret, 10, "%d", previous_cmd_finish);

    if (len>PROMPT_CHAR_LIMITS){
        // On pointe vers les 30 derniers caracètres (7 = 3 sont pour les ... + '[' + ']' + '$' + ' ' )
        tr = working_dir + (len-PROMPT_CHAR_LIMITS+7+strlen(val_ret));
        snprintf(path, 30, "...%s", tr);
    }
    else
        // Sinon on écrit le chemin tel qu'il est.
        snprintf(path, 30, "%s", tr);
    

    if (!previous_cmd_finish) // Si la précédente commande s'est bien terminée.
        snprintf(prompt, 50, "\033[32m[%d]\033[36m%s\033[00m$ ", previous_cmd_finish, path); // On le met en vert.
    else
        snprintf(prompt, 50, "\033[91m[%d]\033[00m%s$ ", previous_cmd_finish, path); // On le met en rouge.

}


/**
 * Fonction qui va initialiser la tables des variables qui seront utilisées 
 * dans le Shell.
 * 
 * Les boucles for par exemple viendront stocker les différents identificateurs dans la table.
 * 
 * La table n'est pas vraiment une table (pas une table de Hachage mais c'est envisageable, si 
 * le temps le premet).
 * 
 * Il s'agit de deux listes dont l'une contient les indentificateurs et l'autre les valeurs.
 * 
 * Les fonctions qui gère ces deux listes vont garentir que les indices des valeurs et des identificateurs 
 * seront les mêmes.
 * L'encapsulation de ces deux listes permet d'éviter qu'un élément extérieur vienne casser cet invariant. 
 */
static void init_var_table(){

    // Initialisation de la liste des identificateurs des variables
    Ids_tables = obj_new(_ArrayList_, 0);

    // Initialisation de la liste des valeurs des variables.
    Ids_values_table = obj_new(_ArrayList_, 0);

}

/**
 * Fonction qui va détruire la tables des variables qui seront utilisées 
 * dans le Shell.
 * 
 * Les boucles for par exemple viendront stocker les différents identificateurs dans la liste.
 */
static void destroy_var_table(){

    // Destruction de la liste des identificateurs des variables
    obj_destroy(Ids_tables);

    // Destruction de la liste des valeurs des variables.
    obj_destroy(Ids_values_table);

}



/**
 * @internal
 * @brief Fonction qui va ajouter une variable dans la table des 
 * variables du shell.
 * 
 * Si la variable existe déjà dans la table, elle ne sera pas écrasée, 
 * la fonction va juste renvoyer 0. <br>
 * Pour écraser une variable déjà existante dans la table, utiliser 
 * plutôt `Shell_update_var()`.
 * 
 * NB : Le liste des variables est détruite à la sortie du shell.
 * @param id_ L'identificateur (nom) de la variable.
 * @param value_ La valeur à stocker dans la variable.
 * 
 * @return 1 Si l'ajout à réussi. 0 Sinon (La virable existe probablement 
 * dans la table).
 */
int Shell_add_var(const char* id_, const char* value_){

    // Vérifications.
    assert(id_);
    assert(value_);
    assert(ArrayList_size(Ids_tables) == ArrayList_size(Ids_values_table));

    // Création des objets String qui vont stocker les chaines.
    String id = obj_new(_String_, id_);


    // Vérification de l'existance de l'identificateur de la variable.
    if (ArrayList_contains(Ids_tables, id)){
        // Destruction de l'objet String créé.
        obj_destroy(id);
        return 0;
    }

    // Création du String de la valeur.
    String value = obj_new(_String_, value_);
    
    ArrayList_add_last(Ids_tables, id, 0);
    ArrayList_add_last(Ids_values_table, value, 0);

    // Vérification de la taille (c'est suffisant).
    assert(ArrayList_size(Ids_tables) == ArrayList_size(Ids_values_table));
    return 1; 

}


/**
 * @internal
 * @brief Fonction qui va retirer une variable dans la table des 
 * variables du shell.
 * 
 * Si la variable n'existe pas déjà dans la table la fonction va renvoyer 0.
 * La fonction ne renvoie pas la valeur supprimée, utiliser `Shell_get_val()` 
 * si on souhaite la récupérer avant.
 * 
 * NB : Le liste des variables est détruite à la sortie du shell.
 * 
 * @param id L'identificateur (nom) de la variable.
 * 
 * @return 1 Si la suppression a réussi. 0 sinon (la variable n'existe probablement pas 
 * dans la table).
 */
int Shell_remove_var(const char* id_){

    // Vérifications.
    assert(id_);
    assert(ArrayList_size(Ids_tables) == ArrayList_size(Ids_values_table));

    
    // Création de l'objet String qui va contenir l'identificateur.
    String id = obj_new(_String_, id_);

    // Récupération de l'indice dans la liste des identificateurs.
    long long int index = ArrayList_index_of(Ids_tables, id);

    // Destruction de l'objet String créé (il existe déjà dans la table).
    obj_destroy(id);
    if (index == -1){ // Si l'indice est -1 alors l'ID n'existe pas.

        return 0; // On revoie 0

    }

    // Suppression de l'identificateur.
    ArrayList_remove(Ids_tables, index, 1);
    // Suppression de la valeur dans la liste table 
    ArrayList_remove(Ids_values_table, index, 1);

    // Vérification de la taille (c'est suffisant).
    assert(ArrayList_size(Ids_tables) == ArrayList_size(Ids_values_table));

    return 1;
}



/**
 * @internal
 * @brief Fonction qui va mettre à jour la valeur d'une variable dans la table des 
 * variables du shell.
 * 
 * Si la variable n'existe pas déjà dans la table la fonction va renvoyer O.
 * La fonction ne renvoie pas la valeur supprimée (celle qui était là avant), 
 * utiliser `Shell_get_val()` si on souhaite la récupérer avant.
 * 
 * NB : Le liste des variables est détruite à la sortie du shell.
 * 
 * @param id_ L'identificateur (nom) de la variable.
 * @param new_value_ La nouvelle valeur à stocker dans la variable.
 * 
 * @return 1 Si la mise à jour a réussie. 0 sinon (la variable n'existe probablement pas 
 * dans la table).
 */
int Shell_update_var(const char* id_, const char* new_value_){

    // Vérifications.
    assert(id_);
    assert(new_value_);
    assert(ArrayList_size(Ids_tables) == ArrayList_size(Ids_values_table));


    // Création de l'objet String qui va contenir l'identificateur.
    String id = obj_new(_String_, id_);

    // Récupération de l'indice de l'identificateur dans la liste des identificateurs.
    long long int index = ArrayList_index_of(Ids_tables, id);

    // Destruction de l'objet String créé (il existe déjà dans la table).
    obj_destroy(id);
    // Vérification de l'existance de l'identificateur de la variable.
    if (index == -1) // Si l'indice est -1 alors l'ID n'existe pas.
        return 0; // On renvoie 0.
    
    // printf("AE ? %s\n", new_value_);
    // Création du String de la nouvelle valeur.
    String new_value = obj_new(_String_, new_value_); 

    // Suppréssion de la précédente valeur (on a son indice pour placer la nouvelle)
    ArrayList_remove(Ids_values_table, index, 1);
    ArrayList_add(Ids_values_table, new_value, index, 0);


    // Vérification de la taille (c'est suffisant).
    assert(ArrayList_size(Ids_tables) == ArrayList_size(Ids_values_table));
    return 1; 

}



/**
 * @internal
 * @brief Fonction qui va renvoyer la valeur contenu dans une variable de 
 * la table des variables du shell.
 * 
 * Si la variable n'existe pas déjà dans la table la fonction va renvoyer NULL.
 * **Attention** : La chaine de caractère renvoyée ne peut pas être modifiée, 
 * la copier si on souhaite l'utiliser.
 * 
 * NB : Le liste des variables est détruite à la sortie du shell.
 * 
 * @param id_ L'identificateur (nom) de la variable.
 * 
 * @return La valeur stockée dans la variable. NULL sinon (la variable n'existe probablement pas 
 * dans la table).
 */
const char*  Shell_get_val(const char* id_){

    // Vérifications.
    assert(id_);
    assert(ArrayList_size(Ids_tables) == ArrayList_size(Ids_values_table));

    // Création de l'objet String qui va contenir l'identificateur.
    String id = obj_new(_String_, id_);

    // Récupération de l'indice de l'identificateur dans la liste des identificateurs.
    long long int index = ArrayList_index_of(Ids_tables, id);

    // Destruction de l'objet String créé (il existe déjà dans la table).
    obj_destroy(id);
    // Vérification de l'existance de l'identificateur de la variable.
    if (index == -1) // Si l'indice est -1 alors l'ID n'existe pas.
        return NULL; // On renvoie NULL.
    
    String value = ArrayList_get(Ids_values_table, index, 0); // On ne le clone il est immuable de toute façon.
    // pas besoin de détruire l'objet ce n'est pas un clone.

    return obj_to_string(value);
    
}
