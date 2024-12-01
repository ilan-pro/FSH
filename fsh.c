/**
 * @file fsh.c
 * @brief Fichier contenant la fonction main de l'application.
 * 
 * C'est cette fonction qui va initialiser les commandes et démarrer le shell.
 */
#include <readline/readline.h>
#include <readline/history.h>
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


int main(){ 
    initialize_cmd();
    
    /** char* prompt = " -l"; */
    /** ExternalCommand e = obj_new(_ExternalCommand_, "ls", &prompt); */

    /** Command_execute((Command)e); */

    /** obj_destroy(e); */
    Shell_start();
    return 0; 
}
