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
ArrayList Shell_parse(char** prompt);

/**
 * @brief Il s'agit de la valeur de retour de la dernière commande exécutée.
 * 
 * Il peut aussi stocker le numéro du signale qui a intérrompu la commande 
 * pendant son processus.
 */
extern int previous_cmd_finish;

/**
 * @brief Fonction qui va ajouter une variable dans la table des 
 * variables du shell.
 * 
 * Si la variable existe déjà dans la table, elle ne sera pas écrasée. 
 * La fonction va renvoyer 0. 
 * Pour écraser une variable déjà existante dans la table, utiliser 
 * plutôt `Shell_update_var()`.
 * 
 * NB : La liste des variables est détruite à la sortie du shell.
 * @param id_ L'identificateur (nom) de la variable.
 * @param value_ La valeur à stocker dans la variable.
 * 
 * @return 1 Si l'ajout à réussi. 0 Sinon (La variable existe probablement 
 * dans la table).
 */
int Shell_add_var(const char* id_, const char* value_);

/**
 * @brief Fonction qui va retirer une variable dans la table des 
 * variables du shell.
 * 
 * Si la variable n'existe pas déjà dans la table la fonction va renvoyer O.
 * La fonction ne renvoie pas la valeur supprimée, utiliser `Shell_get_val()` 
 * si on souhaite la récupérer avant.
 * 
 * NB : Le liste des variables est détruite à la sortie du shell.
 * 
 * @param id_ L'identificateur (nom) de la variable.
 * 
 * @return 1 Si la suppression a réussi. 0 sinon (la variable n'existe probablement pas 
 * dans la table).
 */
int Shell_remove_var(const char* id_);

/**
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
int Shell_update_var(const char* id_, const char* new_value_);

/**
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
const char*  Shell_get_val(const char* id_);

#endif
