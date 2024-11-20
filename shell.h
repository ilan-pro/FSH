#ifndef SHELL_H
#define SHELL_H
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
extern char working_dir[4096];

/**
 * @internal.
 * Il s'agit de la chaine de caractère saisie par l'utilisateur.
 */
extern char *usr_prompt;

#endif