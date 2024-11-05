#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

run_commande(){
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
        execvp(cmd_splitted[0],cmd_splitted);
        free(rep);
        for(int i=0;i<cp;i++){
            // on libère tous les bufs alloué dans le do while
            free(cmd_splitted[i]);
        }
        free(cmd_splitted); 
    }   
}

int main(){
    run_commande();
    return 0; 
}
