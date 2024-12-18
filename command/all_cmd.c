#include "all_cmd.h"

/**
 * @internal
 * @brief L'ensemble de toutes les commandes internes du shell.
 * 
 * Ce tableau est un tableau à deux dimension dont chaque ligne contient deux éléments 
 * le premier étant le nom de la commande (char*) est le second, la classe où elle appartient.
 */
const void* cmd[][2] = {
    {"pwd", NULL},
    {"exit", NULL},
    {"cd", NULL},
    {"ftype", NULL},
    {"for", NULL},
    {"if", NULL}
};
