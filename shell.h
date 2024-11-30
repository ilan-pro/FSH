/**
 * @file shell.h
 * @brief Fichier contenant la déclration des variables globales du shell ainsi que les fonctions utilisées par le shell pour 
 * le traitement, la construction et l'exécution des commandes.
 */
#ifndef SHELL_H
#define SHELL_H
#include "base/ArrayList.h"

/**
 * @brief Il s'agit du nombre de caractère maximal que doit avoir le prompt.
 */
#define PROMPT_CHAR_LIMITS 30

/**
 * @brief Fonction qui va démarrer l'interpreteur de commande.
 * 
 * @return Le code de retour au moment de la fermture du terminal.
 */
int Shell_start();

  
/**
 * @brief Fonction qui va exécuter toutes les pipelines d'une 
 * commande structurée (par appel de la méthode execute() de chaque commande).
 * 
 * La fonction va créer un nouveau processus pour chaque éxécution de commande et 
 * va attendre que l'exécution de la commande soit terminée avant de passer à la commande 
 * suivante. La fonction s'occupera également de faire les redirections de sorties 
 * si jamais il y en a.
 *
 * @param struct_cmds La commande structurée. Une commande structurée est une liste de listes 
 * @return La valeur de retour de la dernière exécution (si elle échoue alors ce sera la valeur de 
 * son code d'erreur).
 */
int Shell_execute_all(ArrayList struct_cmds);

/**
 * @brief Il s'agit du chemin absolue vers le repertoire courant.
 */
extern char working_dir[4096];

// commentaire
ArrayList Shell_parse();

extern int previous_cmd_finish;

#endif
