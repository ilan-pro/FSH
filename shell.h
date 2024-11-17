#ifndef SHELL_H
#define SHELL_H
#include <linux/limits.h>
/**
 * @brief Il s'agit du nombre de caractère maximal que doit avoir le prompt.
 * 
 * 
 */
#define PROMPT_CHAR_LIMITS 30

/**
 * @brief Fonction qui va démarrer l'interpreteur de commande.
 * 
 * @return Le code de retour au moment de la fermture du terminal.
 */
int Shell_start();

/**
 * @brief Il s'agit du chemin absolue vers le repertoire courant.
 */
extern char working_dir[PATH_MAX];

#endif