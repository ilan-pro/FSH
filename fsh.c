#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "base/LinkedList.h"
#include "base/new.h"
#include "base/String.h"

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
    return 0; 
}
