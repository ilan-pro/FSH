/**
 * @file fsh.c
 * @brief Fichier contenant la fonction main de l'application.
 * 
 * C'est cette fonction qui va initialiser les commandes et démarrer le shell.
 */
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "base/ArrayList.h"
#include "base/String.h"
#include "base/new.h"
#include "command/ExternalCommand.h"
#include "command/For.h"
#include "command/all_cmd.h"
#include "shell.h"

/**
 * @brief Fonction qui va ajouter les classes des commandes internes dans le 
 * la table qui contient les commandes internes.
 * 
 * Ajouter directement les classes dans la déclaration faite dans le fichier `all_cmd.h` 
 * est impossible car les structures des classes ne sont pas constantes au moment de la compilation. 
 */
void initialize_cmd(){

    cmd[0][1] = _Pwd_;
    cmd[1][1] = _Exit_;
    cmd[2][1] = _Cd_;
    cmd[3][1] = _Ftype_;
    cmd[4][1] = _For_;

}


/**
 * @brief Fonction qui va initialiser les listes chainées utilsées pour stocker 
 * les identificateurs et les noms des fichiers en cours de traitement.
 */
void initialize_for_lists(){
        
   For_ids = obj_new(_LinkedList_);   
   For_files = obj_new(_LinkedList_);   

}

/**
 * @brief Fonction qui va détruire les listes chainées utilsées pour stocker 
 * les identificateurs et les noms des fichiers en cours de traitement.
 */
void destroy_for_lists(){
        
    obj_destroy(For_ids);
    obj_destroy(For_files);

}

int run_commande(){
    while(1){
        char *rep = readline("Entrez une commande: ");
        char **cmd_splitted = (char **) malloc(sizeof(char*));
        int cp=0;
        // On découpe la chaîne en sous-parties
        char *cut = strtok(rep, " ");
        do {
            char *buf = (char *)malloc(strlen(cut) * sizeof(char));
            strcpy(buf,cut);
            cmd_splitted[cp++] = buf;
            cmd_splitted = realloc(cmd_splitted, (cp+2) * sizeof(char*));
            cut = strtok(NULL, " ");
        }while(cut);
        
        cmd_splitted[cp] = NULL;
        pid_t son_processus = fork();
        if(son_processus == 0){
            execvp(cmd_splitted[0],cmd_splitted);
        }
        wait(NULL);
        free(rep);
        for(int i=0;i<cp;i++){
            // on libère tous les bufs alloué dans le do while
            free(cmd_splitted[i]);
        }
        free(cmd_splitted); 
    }
    return 0;
}

int main(){ 
    initialize_cmd();
    initialize_for_lists();
    
    /** char* prompt = " -l"; */
    /** ExternalCommand e = obj_new(_ExternalCommand_, "ls", &prompt); */

    /** Command_execute((Command)e); */

    /** obj_destroy(e); */
    //Shell_start();
    destroy_for_lists();
    return 0; 
}
