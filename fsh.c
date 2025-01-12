/**
 * @file fsh.c
 * @brief Fichier contenant la fonction main de l'application.
 * 
 * C'est cette fonction qui va initialiser les commandes et démarrer le shell.
 */
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "base/String.h"
#include "base/new.h"
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
    cmd[5][1] = _If_;

}


int main(){ 
    initialize_cmd();
    
    /** char* prompt = " -l"; */
    /** ExternalCommand e = obj_new(_ExternalCommand_, "ls", &prompt); */

    /** Command_execute((Command)e); */

    /** obj_destroy(e); */
    /** return Shell_start();  */
    String s1 = obj_new(_String_, "Hello");
    String s2 = obj_new(_String_, "World");
    
    String c = String_concat(s1, s2);
    
    char* str = String_to_allocated_str(c);
    printf("La chaine est là : %s\n", str);
    obj_print(c);
    printf("\n");

    free(str);
    obj_destroy(s1);
    obj_destroy(s2);
    obj_destroy(c);
}
