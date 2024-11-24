// 
#ifndef ALL_CMD_H
#define ALL_CMD_H

#include "Pwd.h"
#include "Exit.h"
#include "Cd.h"
#include "Ftype.h"

/**
 * @brief L'ensemble de toutes les commandes internes du shell.
 * 
 * Ce tableau est un tableau à deux dimension dont chaque ligne contient deux éléments 
 * le premier étant le nom de la commande (char*) est le second, la classe où elle appartient.
 */
extern const void* cmd[][2];

// const void* cmd[][1] = {
//     {"pwd"},
//     {"exit"},
//     {"cd"},
//     {"ftype"}
// };

#endif
