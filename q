[1mdiff --git a/.vscode/settings.json b/.vscode/settings.json[m
[1mindex 689d7e0..90a2c07 100644[m
[1m--- a/.vscode/settings.json[m
[1m+++ b/.vscode/settings.json[m
[36m@@ -3,5 +3,13 @@[m
         "history.h": "c",[m
         "cstdlib": "cpp",[m
         "class.h": "c"[m
[31m-    }[m
[32m+[m[32m    },[m
[32m+[m[32m    "spellright.language": [[m
[32m+[m[32m        "fr"[m
[32m+[m[32m    ],[m
[32m+[m[32m    "spellright.documentTypes": [[m
[32m+[m[32m        "markdown",[m
[32m+[m[32m        "latex",[m
[32m+[m[32m        "plaintext"[m
[32m+[m[32m    ][m
 }[m
\ No newline at end of file[m
[1mdiff --git a/base/LinkedList.c b/base/LinkedList.c[m
[1mindex d631c44..9b3972e 100644[m
[1m--- a/base/LinkedList.c[m
[1m+++ b/base/LinkedList.c[m
[36m@@ -107,7 +107,7 @@[m [mstatic struct _Class_ __LinkedList__ = {[m
  * <p>[m
  * NB : Tous les éléments de la liste peuvent être de plusieurs type différents[m
  */[m
[31m-Class _LinkedList_ = &__LinkedList__;[m
[32m+[m[32mconst Class _LinkedList_ = &__LinkedList__;[m
 [m
 /***************************************************/[m
 /************** Corps des fonctions ****************/[m
[1mdiff --git a/base/LinkedList.h b/base/LinkedList.h[m
[1mindex 1718331..9dff4a0 100644[m
[1m--- a/base/LinkedList.h[m
[1m+++ b/base/LinkedList.h[m
[36m@@ -94,7 +94,7 @@[m
      *[m
      * Elle est définie dans `LinkedList.c`[m
      */[m
[31m-    extern Class _LinkedList_;[m
[32m+[m[32m    extern const Class _LinkedList_;[m
 [m
         [m
     /**[m
[1mdiff --git a/base/ListCell.c b/base/ListCell.c[m
[1mindex 89ed2c3..bf3b570 100644[m
[1m--- a/base/ListCell.c[m
[1m+++ b/base/ListCell.c[m
[36m@@ -95,7 +95,7 @@[m [mstatic struct _Class_ __ListCell__ = {[m
 /**[m
  * Classe de tous les objets de type `_ListCell_`.[m
  */[m
[31m-Class _ListCell_ = & __ListCell__;[m
[32m+[m[32mconst Class _ListCell_ = & __ListCell__;[m
 [m
 /***************************************************/[m
 /************** Corps des fonctions ****************/[m
[1mdiff --git a/base/ListCell.h b/base/ListCell.h[m
[1mindex 899cf60..088dced 100644[m
[1m--- a/base/ListCell.h[m
[1m+++ b/base/ListCell.h[m
[36m@@ -72,7 +72,7 @@[m
      *[m
      * Elle est définie dans `ListCell.c`[m
      */[m
[31m-    extern Class _ListCell_;[m
[32m+[m[32m    extern const Class _ListCell_;[m
 [m
     /**[m
      * @brief Fonction qui ajoute un objet à une position donnée dans la liste, à partir de cette cellule.[m
[1mdiff --git a/base/String.c b/base/String.c[m
[1mindex 3faaae8..153f658 100644[m
[1m--- a/base/String.c[m
[1m+++ b/base/String.c[m
[36m@@ -106,7 +106,7 @@[m [mstatic struct _Class_ __String__ = {[m
 /**[m
  * Classe de tous les objets de type String.[m
  */[m
[31m-Class _String_ = &__String__;[m
[32m+[m[32mconst Class _String_ = &__String__;[m
 [m
 [m
 /***************************************************/[m
[1mdiff --git a/base/String.h b/base/String.h[m
[1mindex 92e3c57..bbf909d 100644[m
[1m--- a/base/String.h[m
[1m+++ b/base/String.h[m
[36m@@ -85,6 +85,6 @@[m
      * [m
      * Elle est défini dans le fichier `String.c`[m
      */[m
[31m-    extern Class _String_;[m
[32m+[m[32m    extern const Class _String_;[m
 [m
 #endif[m
[1mdiff --git a/command/Cd.c b/command/Cd.c[m
[1mindex 0f7365e..f16a5e3 100644[m
[1m--- a/command/Cd.c[m
[1m+++ b/command/Cd.c[m
[36m@@ -108,7 +108,7 @@[m [mstatic struct _Class_ __Cd__ = {[m
  * Contruction : <code><strong>obj_new(_Cd_, "cd");</strong></code>[m
  * `_Cd_` désigne la classe des commandes cd. et la chaine "cd" est le nom de la commande.[m
  */[m
[31m-Class _Cd_ = &__Cd__;[m
[32m+[m[32mconst Class _Cd_ = &__Cd__;[m
 [m
 /***************************************************/[m
 /************** Corps des fonctions ****************/[m
[1mdiff --git a/command/Cd.h b/command/Cd.h[m
[1mindex e079f8a..f046342 100644[m
[1m--- a/command/Cd.h[m
[1m+++ b/command/Cd.h[m
[36m@@ -56,6 +56,6 @@[m
      * Contruction : <code><strong>obj_new(_Cd_, "cd");</strong></code><br>   [m
      * `_Cd_` désigne la classe des commandes cd. et la chaine "cd" est le nom de la commande.[m
      */[m
[31m-    extern Class _Cd_;[m
[32m+[m[32m    extern const Class _Cd_;[m
 [m
 #endif[m
[1mdiff --git a/command/Command.c b/command/Command.c[m
[1mindex c8b9e65..e1a836d 100644[m
[1m--- a/command/Command.c[m
[1m+++ b/command/Command.c[m
[36m@@ -113,11 +113,35 @@[m [mstatic struct _Class_ __Command__ = {[m
  * [m
  * Construction : `obj_new(_Command_, <Nom_de_la_commande> )`[m
  */[m
[31m-Class _Command_ = &__Command__;[m
[32m+[m[32mconst Class _Command_ = &__Command__;[m
 [m
 /***************************************************/[m
 /************** Corps des fonctions ****************/[m
 [m
[32m+[m[32m/**[m
[32m+[m[32m * @internal[m
[32m+[m[32m * @brief Fonction qui vérifie si oui ou non le caractère actuellement pointé par[m[41m [m
[32m+[m[32m * le pointeur vers le prompte de l'utilisateur est valide, c'est à dire[m[41m [m
[32m+[m[32m * si on doit oui ou non arrêter la parsing de la commande.[m
[32m+[m[32m *[m[41m [m
[32m+[m[32m * Un caractère est valide si et seulement si :[m[41m [m
[32m+[m[32m *  - Ce n'est pas le caracètre null (fin de chaine).[m
[32m+[m[32m *  - Ce n'est pas l'un des caracètres '|', ';' ou '}'.[m
[32m+[m[32m *[m[41m [m
[32m+[m[32m * Si le caractère n'est pas null (fin de chaine) et qu'il est entre guillemets (simple ou double) ou échapé[m[41m [m
[32m+[m[32m * alors il est autamatiquement valide ('|', ';' et '}' seront considérés comme des caracètres normaux).[m
[32m+[m[32m *[m[41m [m
[32m+[m[32m * @param quoted Booléen indiquant si le caracètre est entre guillemets simple.[m
[32m+[m[32m * @param double_quoted Booléen indiquant si le caracètre est entre guillemets doubles.[m
[32m+[m[32m * @param esc Booléen indiquant si le caracètre est echapé par \ .[m
[32m+[m[32m * @return Si oui ou non le caractère actuellement pointé par[m[41m [m
[32m+[m[32m * le pointeur vers le prompte de l'utilisateur est valide.[m
[32m+[m[32m */[m
[32m+[m[32mint Command_actual_char_is_legal(int quoted, int double_quoted, int esc){[m
[32m+[m
[32m+[m[32m    return *usr_prompt && (quoted || double_quoted || esc || ((*usr_prompt) != '|' && (*usr_prompt) != ';' && (*usr_prompt) != '}'));[m
[32m+[m
[32m+[m[32m}[m
 [m
 /**[m
  * Fonction qui va lire les paramètres d'une commande.[m
[36m@@ -189,10 +213,10 @@[m [mstatic void Command_parse_usr_prompt(Command self){[m
     int esc = 0; // Booléean indiquant si oui ou non le caractère suivant est echappé.[m
 [m
     /** [m
[31m-     * Lecture du prompt dans la boucle tant que l'on a pas fini le prompt [m
[31m-     * ou qu'on a pas rencontré l'un des caractères `;`, `}` ou `|` sauf s'ils apparaissent sont échapés par quelque chose.[m
[32m+[m[32m     * Lecture du prompt dans la boucle tant que le caractère pointé est[m[41m [m
[32m+[m[32m     * valide (voir documentation de la fonction actual_char_is_legal()).[m
      */[m
[31m-    while(*usr_prompt && (quoted || double_quoted || esc || ((*usr_prompt) != '|' && (*usr_prompt) != ';' && (*usr_prompt) != '}')) ){[m
[32m+[m[32m    while(Command_actual_char_is_legal(quoted, double_quoted, esc)){[m
         [m
         // Récupération du caractère pointé par le pointeur, et incémentation de la posisiton.[m
         char c = *(usr_prompt++);[m
[36m@@ -209,9 +233,9 @@[m [mstatic void Command_parse_usr_prompt(Command self){[m
                 mot[char_compteur - 1] = c;[m
                 break; [m
 [m
[31m-            case ' ':[m
[32m+[m[32m            case ' ': // Le cas où le caractère lu est un espace.[m
     [m
[31m-                if (quoted || double_quoted || esc) // Si on est dans une expression entre guillemets[m
[32m+[m[32m                if (quoted || double_quoted || esc) // Si on est dans une expression entre guillemets ou que ' ' est echapé[m
                     goto h_default; // On va dans default[m
 [m
                 if (char_compteur){ // Si il y a au moins une lettre lue.[m
[36m@@ -243,6 +267,17 @@[m [mstatic void Command_parse_usr_prompt(Command self){[m
                      */[m
                     goto h_default;[m
                 }[m
[32m+[m[32m                else if (!Command_actual_char_is_legal(quoted, double_quoted, esc) || *(usr_prompt)==' '){[m
[32m+[m[32m                    /**[m
[32m+[m[32m                     * Si le prochain caractère (car on a fait usr_prompt++) n'est pas valide[m
[32m+[m[32m                     * (et donc qu'il n'y aura pas de prochaine itération) ou que le prochain[m[41m [m
[32m+[m[32m                     * caractère est un espace, alors il n'y pas de paramètre à chercher '-'[m[41m [m
[32m+[m[32m                     * est considéré comme un argument (ex : cd - ou cat - ).[m
[32m+[m[32m                     */[m
[32m+[m[32m                    goto h_default;[m
[32m+[m[32m                }[m
[32m+[m
[32m+[m
                 // Sinon on récupère les paramètres.[m
                 Command_parse_usr_prompt_params(self);[m
                 break;[m
[1mdiff --git a/command/Command.h b/command/Command.h[m
[1mindex 56ddcad..3b73d53 100644[m
[1m--- a/command/Command.h[m
[1m+++ b/command/Command.h[m
[36m@@ -65,7 +65,7 @@[m
      * [m
      * Construction : `obj_new(_Command_, <Nom_de_la_commande> )`[m
      */[m
[31m-    extern Class _Command_;[m
[32m+[m[32m    extern const Class _Command_;[m
     [m
     /**[m
      * @brief Fonction qui va exécuter la commande.[m
[36m@@ -80,4 +80,24 @@[m
      */[m
     int Command_execute(Command self);[m
 [m
[32m+[m[32m    /**[m
[32m+[m[32m     * @brief Fonction qui vérifie si oui ou non le caractère actuellement pointé par[m[41m [m
[32m+[m[32m     * le pointeur vers le prompte de l'utilisateur est valide, c'est à dire[m[41m [m
[32m+[m[32m     * si on doit oui ou non arrêter la parsing de la commande.[m
[32m+[m[32m     *[m[41m [m
[32m+[m[32m     * Un caractère est valide si et seulement si :[m[41m [m
[32m+[m[32m     *  - Ce n'est pas le caracètre null (fin de chaine).[m
[32m+[m[32m     *  - Ce n'est pas l'un des caracètres '|', ';' ou '}'.[m
[32m+[m[32m     *[m[41m [m
[32m+[m[32m     * Si le caractère n'est pas null (fin de chaine) et qu'il est entre guillemets (simple ou double) ou échapé[m[41m [m
[32m+[m[32m     * alors il est autamatiquement valide ('|', ';' et '}' seront considérés comme des caracètres normaux).[m
[32m+[m[32m     *[m[41m [m
[32m+[m[32m     * @param quoted Booléen indiquant si le caracètre est entre guillemets simple.[m
[32m+[m[32m     * @param double_quoted Booléen indiquant si le caracètre est entre guillemets doubles.[m
[32m+[m[32m     * @param esc Booléen indiquant si le caracètre est echapé par \ .[m
[32m+[m[32m     * @return Si oui ou non le caractère actuellement pointé par[m[41m [m
[32m+[m[32m     * le pointeur vers le prompte de l'utilisateur est valide.[m
[32m+[m[32m     */[m
[32m+[m[32m    int Command_actual_char_is_legal(int quoted, int double_quoted, int esc);[m
[32m+[m[41m    [m
 #endif[m
[1mdiff --git a/command/Exit.c b/command/Exit.c[m
[1mindex 5bf1a53..65f409c 100644[m
[1m--- a/command/Exit.c[m
[1m+++ b/command/Exit.c[m
[36m@@ -5,7 +5,7 @@[m
  * [m
  * `exit` est la commande qui permet de quitter le terminal.[m
  * [m
[31m- * Contruction : <code><strong>obj_new(_Exit_, "exit");</strong></code><br>[m
[32m+[m[32m * Construction : <code><strong>obj_new(_Exit_, "exit");</strong></code><br>[m
  * `_Exit_` désigne la classe des commandes exit. et la chaine "exit" est le nom de la commande.[m
  */[m
 [m
[36m@@ -72,7 +72,7 @@[m [mtypedef struct _Exit_ {[m
      *[m
      * `exit` est la commande qui permet de quitter le terminal.[m
      * [m
[31m-     * Contruction : <code><strong>obj_new(_Exit_, "exit");</strong></code><br>   [m
[32m+[m[32m     * Construction : <code><strong>obj_new(_Exit_, "exit");</strong></code><br>[m[41m   [m
      * `_Exit_` désigne la classe des commandes exit. et la chaine "exit" est le nom de la commande.[m
      */[m
     Class _Exit_;[m
[36m@@ -100,10 +100,10 @@[m [mstatic struct _Class_ __Exit__ = {[m
  *[m
  * `exit` est la commande qui permet de quitter le terminal.[m
  *[m
[31m- * Contruction : <code><strong>obj_new(_Exit_, "exit");</strong></code>[m
[32m+[m[32m * Construction : <code><strong>obj_new(_Exit_, "exit");</strong></code>[m
  * `_Exit_` désigne la classe des commandes exit. et la chaine "exit" est le nom de la commande.[m
  */[m
[31m-Class _Exit_ = &__Exit__;[m
[32m+[m[32mconst Class _Exit_ = &__Exit__;[m
 [m
 /***************************************************/[m
 /************** Corps des fonctions ****************/[m
[1mdiff --git a/command/Exit.h b/command/Exit.h[m
[1mindex 915a6e2..705aa34 100644[m
[1m--- a/command/Exit.h[m
[1m+++ b/command/Exit.h[m
[36m@@ -4,7 +4,7 @@[m
  * @file Exit.h[m
  * @brief Fichier d'en-tête contenant les déclarations des variables publiques et de toutes les méthodes public de Exit.[m
  * [m
[31m- * Contruction : <code><strong>obj_new(_Exit_, "exit");</strong></code>[m
[32m+[m[32m * Construction : <code><strong>obj_new(_Exit_, "exit");</strong></code>[m
  * `_Exit_` désigne la classe des commandes exit. et la chaine "exit" est le nom de la commande.[m
  */[m
 [m
[36m@@ -53,9 +53,9 @@[m
      *[m
      * `exit` est la commande qui permet de quitter le terminal.[m
      *[m
[31m-     * Contruction : <code><strong>obj_new(_Exit_, "exit");</strong></code><br>   [m
[32m+[m[32m     * Construction : <code><strong>obj_new(_Exit_, "exit");</strong></code><br>[m[41m   [m
      * `_Exit_` désigne la classe des commandes exit. et la chaine "exit" est le nom de la commande.[m
      */[m
[31m-    extern Class _Exit_;[m
[32m+[m[32m    extern const Class _Exit_;[m
 [m
 #endif[m
[1mdiff --git a/command/ExternalCommand.c b/command/ExternalCommand.c[m
[1mindex 322e754..d773067 100644[m
[1m--- a/command/ExternalCommand.c[m
[1m+++ b/command/ExternalCommand.c[m
[36m@@ -5,13 +5,19 @@[m
  * [m
  * ExternalCommand est la classe des commandes externes.[m
  * [m
[31m- * Contruction : <code><strong>obj_new(_ExternalCommand_, "nom_de_la_commande_externe");</strong></code><br>[m
[32m+[m[32m * Construction : <code><strong>obj_new(_ExternalCommand_, "nom_de_la_commande_externe");</strong></code><br>[m
  * `_ExternalCommand_` désigne la classe des commandes de type ExternalCommand (commandes externes). et la [m
  * chaine "nom_de_la_commande_externe" doit contenir le nom de la commande.[m
  */[m
 [m
 [m
 #include <assert.h>[m
[32m+[m[32m#include <stdlib.h>[m
[32m+[m[32m#include <stdio.h>[m
[32m+[m[32m#include <unistd.h>[m
[32m+[m[32m#include <string.h>[m
[32m+[m[32m#include "../base/new.h"[m
[32m+[m[32m#include "../shell.h"[m
 [m
 /** @cond HIDE_THIS*/[m
 #define EXTERNAL_COMMAND_CLASS "ExternalCommand"[m
[36m@@ -56,7 +62,7 @@[m [mstatic int ExternalCommand_execute(Command self_);[m
 /**[m
  * Fonction qui va afficher la commande externe.[m
  */[m
[31m-static void ExternalCommand_print(void* self_);[m
[32m+[m[32mstatic void ExternalCommand_print(const void* self_);[m
 [m
 /*****************************************************/[m
 /************* Structure de la classe ****************/[m
[36m@@ -66,7 +72,7 @@[m [mstatic void ExternalCommand_print(void* self_);[m
  * @brief Classe `ExternalCommand` de la commande ExternalCommand qui affiche [m
  * le chemin absolu du répertoire courant.[m
  *[m
[31m- * Contruction : <code><strong>obj_new(_ExternalCommand_, "nom_de_la_commande_externe");</strong></code><br>[m
[32m+[m[32m * Construction : <code><strong>obj_new(_ExternalCommand_, "nom_de_la_commande_externe");</strong></code><br>[m
  * `_ExternalCommand_` désigne la classe des commandes de type ExternalCommand (commandes externes). et la [m
  * chaine "nom_de_la_commande_externe" doit contenir le nom de la commande.[m
  */[m
[36m@@ -77,7 +83,7 @@[m [mtypedef struct _ExternalCommand_ {[m
      *[m
      * ExternalCommand est la classe des commandes externes.[m
      * [m
[31m-     * Contruction : <code><strong>obj_new(_ExternalCommand_, "nom_de_la_commande_externe");</strong></code><br>[m
[32m+[m[32m     * Construction : <code><strong>obj_new(_ExternalCommand_, "nom_de_la_commande_externe");</strong></code><br>[m
      * `_ExternalCommand_` désigne la classe des commandes de type ExternalCommand (commandes externes). et la [m
      * chaine "nom_de_la_commande_externe" doit contenir le nom de la commande.[m
      */[m
[36m@@ -96,7 +102,7 @@[m [mstatic struct _Class_ __ExternalCommand__ = {[m
     0,[m
     0,[m
     0,[m
[31m-    0 // Sera celui du parent.[m
[32m+[m[32m    ExternalCommand_print[m
 [m
 [m
 };[m
[36m@@ -106,16 +112,15 @@[m [mstatic struct _Class_ __ExternalCommand__ = {[m
  *[m
  * ExternalCommand est la classe des commandes externes.[m
  * [m
[31m- * Contruction : <code><strong>obj_new(_ExternalCommand_, "nom_de_la_commande_externe");</strong></code><br>[m
[32m+[m[32m * Construction : <code><strong>obj_new(_ExternalCommand_, "nom_de_la_commande_externe");</strong></code><br>[m
  * `_ExternalCommand_` désigne la classe des commandes de type ExternalCommand (commandes externes). et la [m
  * chaine "nom_de_la_commande_externe" doit contenir le nom de la commande.[m
  */[m
[31m-Class _ExternalCommand_ = &__ExternalCommand__;[m
[32m+[m[32mconst Class _ExternalCommand_ = &__ExternalCommand__;[m
 [m
 /***************************************************/[m
 /************** Corps des fonctions ****************/[m
 [m
[31m-[m
 // Constructeur.[m
 static void* ExternalCommand_constructor(void* self_, va_list* args){[m
     [m
[36m@@ -125,15 +130,85 @@[m [mstatic void* ExternalCommand_constructor(void* self_, va_list* args){[m
     // Conversion en objet de type ExternalCommand.[m
     ExternalCommand self = (ExternalCommand) self_;[m
 [m
[31m-    // On ajoute le parent à EXTERNAL_COMMAND (en controunant la contrainte sur const).[m
[32m+[m[32m    // On ajoute le parent à EXTERNAL_COMMAND (en contournant la contrainte sur const).[m
     *(Class*)&(self->_ExternalCommand_->_super_) = _Command_; [m
 [m
         [m
     // Ajout de la fonction execute de la classe fille.[m
     *((int (**) (Command)) &(self->execute)) = ExternalCommand_execute;[m
 [m
[31m-    return _Command_->ctor(self, args);[m
[32m+[m[32m    // Les champs arguments et paramètres ( qui sont des listes chainées ) ne seront pas initialisés.[m
[32m+[m[41m    [m
[32m+[m[32m    // Récupération du nom de la commande.[m
[32m+[m[32m    char* cmd_name = va_arg(*args, char*);[m
[32m+[m[32m    *(char**)&self->name = cmd_name; // Mise à jour du nom de la commande.[m
     [m
[32m+[m[32m    int cmd_length = strlen(cmd_name);[m
[32m+[m[32m    char* command = malloc((2 + cmd_length) * sizeof(char));[m
[32m+[m[32m    assert(command);[m
[32m+[m[32m    strcpy(command, cmd_name);[m
[32m+[m[41m    [m
[32m+[m[32m    command[cmd_length] = ' ';[m
[32m+[m[32m    command[cmd_length+1] = '\0';[m
[32m+[m[32m    cmd_length ++;[m
[32m+[m
[32m+[m[32m    int quoted = 0; // Booléean indiquant si oui ou non on se trouve dans des guillemets.[m
[32m+[m[32m    int double_quoted = 0; // Booléean indiquant si oui ou non on se trouve dans des guillemets double.[m
[32m+[m[32m    int esc = 0; // Booléean indiquant si oui ou non le caractère suivant est echappé.[m
[32m+[m
[32m+[m[32m    /**[m[41m [m
[32m+[m[32m     * Lecture du prompt dans la boucle tant que le caractère pointé est[m[41m [m
[32m+[m[32m     * valide (voir documentation de la fonction actual_char_is_legal()).[m
[32m+[m[32m     */[m
[32m+[m[32m    while(Command_actual_char_is_legal(quoted, double_quoted, esc)){[m
[32m+[m[41m        [m
[32m+[m[32m        // Récupération du caractère pointé par le pointeur, et incémentation de la posisiton.[m
[32m+[m[32m        char c = *(usr_prompt++);[m
[32m+[m[32m        printf("%c\n", c);[m
[32m+[m[32m        switch (c){[m
[32m+[m[32m            default :[m[41m [m
[32m+[m[32m                h_default: // label[m[41m [m
[32m+[m
[32m+[m[32m                esc = 0; // On met l'echappement à 0 (si jamais le caractère était échappé)[m
[32m+[m[32m                // Incrémentation du compteur.[m
[32m+[m[32m                cmd_length++;[m
[32m+[m[32m                // Réallocation pour accueillir le nouveau caractère.[m
[32m+[m[32m                command = realloc(command, (1 + cmd_length) * sizeof(char));[m
[32m+[m[32m                assert(command);[m
[32m+[m[32m                command[cmd_length] = '\0';[m
[32m+[m[32m                command[cmd_length- 1] = c;[m
[32m+[m[32m                break;[m[41m [m
[32m+[m
[32m+[m
[32m+[m[32m            case '"': // Le cas où on a des guillemets doubles[m
[32m+[m[32m                if (!quoted && !esc) // S'ils sont dans des guillemets simples ou explicitement echapés alors ils sont échapés[m
[32m+[m[32m                    double_quoted = !double_quoted;[m
[32m+[m
[32m+[m[32m                goto h_default;[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case '\'': // Le cas où on a des guillemets simples[m
[32m+[m[32m                if (!double_quoted && !esc) // S'ils sont dans des guillemets doubles ou explicitement echapés alors ils sont échapés[m
[32m+[m[32m                    quoted = !quoted;[m
[32m+[m
[32m+[m[32m                goto h_default;[m
[32m+[m[32m                break;[m
[32m+[m[41m            [m
[32m+[m[32m            case '\\': // Les echapement peuvent exister dans les commandes externes ex: find ... -exec rm {} \; ici ; est echapé[m
[32m+[m[41m            [m
[32m+[m[32m                if (esc) // Si il est echappé[m
[32m+[m[32m                    goto h_default;[m
[32m+[m[41m                [m
[32m+[m[32m                esc = 1;[m
[32m+[m[32m                break;[m
[32m+[m[32m        }[m
[32m+[m[32m    }[m
[32m+[m[41m    [m
[32m+[m[32m    // Mise à jour de la commande[m
[32m+[m[32m    self->command = command;[m
[32m+[m
[32m+[m[32m    // La commande externe est correctement initialisée.[m
[32m+[m[32m    return self;[m
 }[m
 [m
 // Destructeur.[m
[36m@@ -141,8 +216,34 @@[m [mstatic void* ExternalCommand_destroy(void* self_){[m
     [m
     // Vérification des arguments.[m
     assert(self_);    [m
[32m+[m[32m    assert(obj_instance_of(self_, _ExternalCommand_));[m
[32m+[m
[32m+[m[32m    ExternalCommand self = (ExternalCommand) self_;[m
     [m
[31m-    return _Command_->dtor(self_);[m
[32m+[m[32m    // Libération de la mémoire pour la chaine de la commande.[m
[32m+[m[32m    free(self->command);[m
[32m+[m[41m    [m
[32m+[m[32m    // Aucun autre champ n'est à libérer il n'y a pas de liste d'argument ni de liste de paramètre.[m
[32m+[m
[32m+[m[32m    return self;[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32m/**[m
[32m+[m[32m * Fonction qui va afficher la commande externe.[m
[32m+[m[32m */[m
[32m+[m[32mstatic void ExternalCommand_print(const void* self_){[m
[32m+[m[41m    [m
[32m+[m[32m    // Vérification de l'argument.[m
[32m+[m[32m    assert(self_);[m[41m    [m
[32m+[m[32m    assert(obj_instance_of(self_, _ExternalCommand_));[m
[32m+[m[41m    [m
[32m+[m[32m    // Conversion en objet de type ExternalCommand.[m
[32m+[m[32m    ExternalCommand self = (ExternalCommand) self_;[m
[32m+[m[41m    [m
[32m+[m[32m    write(1, "Commande externe : ", 19);[m
[32m+[m[32m    write(1, self->name, strlen(self->name));[m
[32m+[m[32m    write(1, " | expr : ", 10);[m
[32m+[m[32m    write(1, self->command, strlen(self->command));[m
     [m
 }[m
 [m
[36m@@ -164,3 +265,4 @@[m [mstatic int ExternalCommand_execute(Command self_){[m
 [m
 [m
 [m
[41m+[m
[1mdiff --git a/command/ExternalCommand.h b/command/ExternalCommand.h[m
[1mindex 7aa5735..f0e77de 100644[m
[1m--- a/command/ExternalCommand.h[m
[1m+++ b/command/ExternalCommand.h[m
[36m@@ -4,7 +4,7 @@[m
  * [m
  * ExternalCommand est la classe des commandes externes.[m
  * [m
[31m- * Contruction : <code><strong>obj_new(_ExternalCommand_, "nom_de_la_commande_externe");</strong></code><br>[m
[32m+[m[32m * Construction : <code><strong>obj_new(_ExternalCommand_, "nom_de_la_commande_externe");</strong></code><br>[m
  * `_ExternalCommand_` désigne la classe des commandes de type ExternalCommand (commandes externes). et la [m
  * chaine "nom_de_la_commande_externe" doit contenir le nom de la commande.[m
  */[m
[36m@@ -16,8 +16,9 @@[m
 #include "Command.h"[m
     [m
     #define EXTERNAL_COMMAND_FILEDS \[m
[31m-        COMMAND_FIELDS /* Doit toujours être déclaré en premier pour pouvoir réduire l'objet à un objet de type Command pour le polymorphisme */\ [m
[31m-    [m
[32m+[m[32m        COMMAND_FIELDS /* Doit toujours être déclaré en premier pour pouvoir réduire l'objet à un objet de type Command pour le polymorphisme */\[m
[32m+[m[32m        char* command; /** Il s'agit la commande entière à donner (avec argument et paramètres) */\[m[41m [m
[32m+[m
     typedef struct _ExternalCommand_* ExternalCommand;[m
     [m
 #else[m
[36m@@ -25,7 +26,7 @@[m
     [m
     /** @cond HIDE_THIS*/[m
     typedef struct __external_command_private_fields{[m
[31m-       // Attribut qu'on veut rendre privée ici. [m
[32m+[m[32m        char* command;[m
     } __external_command_private_fields;[m
 [m
     #define EXTERNAL_COMMAND_H_FIELDS \[m
[36m@@ -58,9 +59,9 @@[m
      * absolue du vers le répertoire dans lequel on se [m
      * trouve.[m
      *[m
[31m-     * Contruction : <code><strong>obj_new(_ExternalCommand_, "external_command");</strong></code><br>   [m
[32m+[m[32m     * Construction : <code><strong>obj_new(_ExternalCommand_, "external_command");</strong></code><br>[m[41m   [m
      * `_ExternalCommand_` désigne la classe des commandes external_command. et la chaine "external_command" est le nom de la commande.[m
      */[m
[31m-    extern Class _ExternalCommand_;[m
[32m+[m[32m    extern const Class _ExternalCommand_;[m
 [m
 #endif[m
[1mdiff --git a/command/Ftype.c b/command/Ftype.c[m
[1mindex bfc4420..7adcfd8 100644[m
[1m--- a/command/Ftype.c[m
[1m+++ b/command/Ftype.c[m
[36m@@ -5,7 +5,7 @@[m
  * [m
  * `ftype` est la commande qui donne le type d'un fichier.[m
  * [m
[31m- * Contruction : <code><strong>obj_new(_Ftype_, "ftype");</strong></code><br>[m
[32m+[m[32m * Construction : <code><strong>obj_new(_Ftype_, "ftype");</strong></code><br>[m
  * `_Ftype_` désigne la classe des commandes ftype. et la chaine "ftype" est le nom de la commande.[m
  */[m
 [m
[36m@@ -72,7 +72,7 @@[m [mtypedef struct _Ftype_ {[m
      *[m
      * `ftype` est la commande qui donne le type d'un fichier.[m
      *[m
[31m-     * Contruction : <code><strong>obj_new(_Ftype_, "ftype");</strong></code><br>   [m
[32m+[m[32m     * Construction : <code><strong>obj_new(_Ftype_, "ftype");</strong></code><br>[m[41m   [m
      * `_Ftype_` désigne la classe des commandes ftype. et la chaine "ftype" est le nom de la commande.[m
      */[m
     Class _Ftype_;[m
[36m@@ -100,10 +100,10 @@[m [mstatic struct _Class_ __Ftype__ = {[m
  *[m
  * `ftype` est la commande qui donne le type d'un fichier.[m
  *[m
[31m- * Contruction : <code><strong>obj_new(_Ftype_, "ftype");</strong></code>[m
[32m+[m[32m * Construction : <code><strong>obj_new(_Ftype_, "ftype");</strong></code>[m
  * `_Ftype_` désigne la classe des commandes ftype. et la chaine "ftype" est le nom de la commande.[m
  */[m
[31m-Class _Ftype_ = &__Ftype__;[m
[32m+[m[32mconst Class _Ftype_ = &__Ftype__;[m
 [m
 /***************************************************/[m
 /************** Corps des fonctions ****************/[m
[1mdiff --git a/command/Ftype.h b/command/Ftype.h[m
[1mindex 9d2036d..67af4ff 100644[m
[1m--- a/command/Ftype.h[m
[1m+++ b/command/Ftype.h[m
[36m@@ -4,7 +4,7 @@[m
  * @file Ftype.h[m
  * @brief Fichier d'en-tête contenant les déclarations des variables publiques et de toutes les méthodes public de Ftype.[m
  * [m
[31m- * Contruction : <code><strong>obj_new(_Ftype_, "ftype");</strong></code>[m
[32m+[m[32m * Construction : <code><strong>obj_new(_Ftype_, "ftype");</strong></code>[m
  * `_Ftype_` désigne la classe des commandes ftype. et la chaine "ftype" est le nom de la commande.[m
  */[m
 [m
[36m@@ -53,9 +53,9 @@[m
      *[m
      * `ftype` est la commande qui donne le type d'un fichier.[m
      *[m
[31m-     * Contruction : <code><strong>obj_new(_Ftype_, "ftype");</strong></code><br>   [m
[32m+[m[32m     * Construction : <code><strong>obj_new(_Ftype_, "ftype");</strong></code><br>[m[41m   [m
      * `_Ftype_` désigne la classe des commandes ftype. et la chaine "ftype" est le nom de la commande.[m
      */[m
[31m-    extern Class _Ftype_;[m
[32m+[m[32m    extern const Class _Ftype_;[m
 [m
 #endif[m
[1mdiff --git a/command/Pwd.c b/command/Pwd.c[m
[1mindex 9be4715..5137849 100644[m
[1m--- a/command/Pwd.c[m
[1m+++ b/command/Pwd.c[m
[36m@@ -5,7 +5,7 @@[m
  * [m
  * Pwd est une commande qui va afficher le chemin absolue vers le répertoire courant.[m
  * [m
[31m- * Contruction : <code><strong>obj_new(_Pwd_, "pwd");</strong></code><br>[m
[32m+[m[32m * Construction : <code><strong>obj_new(_Pwd_, "pwd");</strong></code><br>[m
  * `_Pwd_` désigne la classe des commandes pwd. et la chaine "pwd" est le nom de la commande.[m
  */[m
 [m
[36m@@ -74,7 +74,7 @@[m [mtypedef struct _Pwd_ {[m
      * absolue du vers le répertoire dans lequel on se [m
      * trouve.[m
      *[m
[31m-     * Contruction : <code><strong>obj_new(_Pwd_, "pwd");</strong></code><br>   [m
[32m+[m[32m     * Construction : <code><strong>obj_new(_Pwd_, "pwd");</strong></code><br>[m[41m   [m
      * `_Pwd_` désigne la classe des commandes pwd. et la chaine "pwd" est le nom de la commande.[m
      */[m
     Class _Pwd_;[m
[36m@@ -104,10 +104,10 @@[m [mstatic struct _Class_ __Pwd__ = {[m
  * absolue du vers le répertoire dans lequel on se [m
  * trouve.[m
  *[m
[31m- * Contruction : <code><strong>obj_new(_Pwd_, "pwd");</strong></code>[m
[32m+[m[32m * Construction : <code><strong>obj_new(_Pwd_, "pwd");</strong></code>[m
  * `_Pwd_` désigne la classe des commandes pwd. et la chaine "pwd" est le nom de la commande.[m
  */[m
[31m-Class _Pwd_ = &__Pwd__;[m
[32m+[m[32mconst Class _Pwd_ = &__Pwd__;[m
 [m
 /***************************************************/[m
 /************** Corps des fonctions ****************/[m
[1mdiff --git a/command/Pwd.h b/command/Pwd.h[m
[1mindex 845c73e..99fcb65 100644[m
[1m--- a/command/Pwd.h[m
[1m+++ b/command/Pwd.h[m
[36m@@ -4,7 +4,7 @@[m
  * @file Pwd.h[m
  * @brief Fichier d'en-tête contenant les déclarations des variables publiques et de toutes les méthodes public de Pwd.[m
  * [m
[31m- * Contruction : <code><strong>obj_new(_Pwd_, "pwd");</strong></code>[m
[32m+[m[32m * Construction : <code><strong>obj_new(_Pwd_, "pwd");</strong></code>[m
  * `_Pwd_` désigne la classe des commandes pwd. et la chaine "pwd" est le nom de la commande.[m
  */[m
 [m
[36m@@ -57,9 +57,9 @@[m
      * absolue du vers le répertoire dans lequel on se [m
      * trouve.[m
      *[m
[31m-     * Contruction : <code><strong>obj_new(_Pwd_, "pwd");</strong></code><br>   [m
[32m+[m[32m     * Construction : <code><strong>obj_new(_Pwd_, "pwd");</strong></code><br>[m[41m   [m
      * `_Pwd_` désigne la classe des commandes pwd. et la chaine "pwd" est le nom de la commande.[m
      */[m
[31m-    extern Class _Pwd_;[m
[32m+[m[32m    extern const Class _Pwd_;[m
 [m
 #endif[m
[1mdiff --git a/command/all_cmd.h b/command/all_cmd.h[m
[1mindex b222b37..df962fd 100644[m
[1m--- a/command/all_cmd.h[m
[1m+++ b/command/all_cmd.h[m
[36m@@ -1,10 +1,17 @@[m
[32m+[m[32m#ifndef ALL_CMD_H[m
[32m+[m[32m#define ALL_CMD_H[m
[32m+[m
 #include "Pwd.h"[m
[31m-#include "exit.h"[m
[31m-#include "cd.h"[m
[31m-#include "ftype.h"[m
[31m-const void* cmd[][2] = {[m
[31m-    {"pwd", _Pwd_},[m
[31m-    {"exit", _Exit_},[m
[31m-    {"cd", _Cd_},[m
[31m-    {"ftype", _Ftype_}[m
[31m-};[m
\ No newline at end of file[m
[32m+[m[32m#include "Exit.h"[m
[32m+[m[32m#include "Cd.h"[m
[32m+[m[32m#include "Ftype.h"[m
[32m+[m
[32m+[m[32m/**[m
[32m+[m[32m * @brief L'ensemble de toutes les commandes internes du shell.[m
[32m+[m[32m *[m[41m [m
[32m+[m[32m * Ce tableau est un tableau à deux dimension dont chaque ligne contient deux éléments[m[41m [m
[32m+[m[32m * le premier étant le nom de la commande (char*) est le second, la classe où elle appartient.[m
[32m+[m[32m */[m
[32m+[m[32mextern const void* cmd[][2];[m
[32m+[m
[32m+[m[32m#endif[m
\ No newline at end of file[m
[1mdiff --git a/fsh b/fsh[m
[1mindex 72be54f..0e722f8 100755[m
Binary files a/fsh and b/fsh differ
[1mdiff --git a/fsh.c b/fsh.c[m
[1mindex b78a9ca..0c83d86 100644[m
[1m--- a/fsh.c[m
[1m+++ b/fsh.c[m
[36m@@ -1,3 +1,9 @@[m
[32m+[m[32m/**[m
[32m+[m[32m * @file fsh.c[m
[32m+[m[32m * @brief Fichier contenant la fonction main de l'application.[m
[32m+[m[32m *[m[41m [m
[32m+[m[32m * C'est cette fonction qui va initialiser les commandes et démarrer le shell.[m
[32m+[m[32m */[m
 #include <readline/readline.h>[m
 #include <readline/history.h>[m
 #include <stdio.h>[m
[36m@@ -9,9 +15,25 @@[m
 [m
 #include "base/LinkedList.h"[m
 #include "base/new.h"[m
[31m-#include "command/Pwd.h"[m
[32m+[m[32m#include "command/all_cmd.h"[m
 #include "shell.h"[m
 [m
[32m+[m[32m/**[m
[32m+[m[32m * @brief Fonction qui va ajouter les classes des commandes internes dans le[m[41m [m
[32m+[m[32m * la table qui contient les commandes internes.[m
[32m+[m[32m *[m[41m [m
[32m+[m[32m * Ajouter directement les classes dans la déclaration faite dans le fichier `all_cmd.h`[m[41m [m
[32m+[m[32m * est impossible car les structures des classes ne sont pas constantes au moment de la compilation.[m[41m [m
[32m+[m[32m */[m
[32m+[m[32mvoid initialize_cmd(){[m
[32m+[m
[32m+[m[32m    cmd[0][1] = _Pwd_;[m
[32m+[m[32m    cmd[1][1] = _Exit_;[m
[32m+[m[32m    cmd[2][1] = _Cd_;[m
[32m+[m[32m    cmd[3][1] = _Ftype_;[m
[32m+[m
[32m+[m[32m}[m
[32m+[m
 int run_commande(){[m
     while(1){[m
         char *rep = readline("Entrez une commande: ");[m
[36m@@ -44,6 +66,7 @@[m [mint run_commande(){[m
 }[m
 [m
 int main(){[m
[32m+[m[32m    initialize_cmd();[m
     Shell_start(); [m
     // Pwd t = obj_new(_Pwd_, "pwd");[m
     // obj_print(t);[m
[1mdiff --git a/shell.c b/shell.c[m
[1mindex 00870d0..ffa7262 100644[m
[1m--- a/shell.c[m
[1m+++ b/shell.c[m
[36m@@ -1,3 +1,9 @@[m
[32m+[m[32m/**[m
[32m+[m[32m * @file shell.c[m
[32m+[m[32m * @brief Fichier contenant l'implémentation des fonctions utilisé par le shell pour le traitement et construction des commandes.[m
[32m+[m[32m *[m[41m [m
[32m+[m[32m * On y retrouve notament la fonction `Shell_start()` qui va démarrer le shell et l'intéraction avec l'utilisateur.[m
[32m+[m[32m */[m
 #include "shell.h"[m
 #include <stddef.h>[m
 #include <string.h>[m
[36m@@ -11,6 +17,7 @@[m
 #include "base/Class.h"[m
 #include "command/all_cmd.h"[m
 #include "command/Command.h"[m
[32m+[m[32m#include "command/ExternalCommand.h"[m
 #include "base/String.h"[m
 #include "base/new.h"[m
 [m
[36m@@ -54,13 +61,13 @@[m [mstatic char path[PROMPT_CHAR_LIMITS];[m
 static char prompt[50];[m
 [m
 /**[m
[32m+[m[32m * @internal[m
  * Chaine de caractère qui va stocker la saisie de l'utilisateur.[m
  */[m
 char *usr_prompt = "";[m
 [m
 /**[m
  * @internal[m
[31m- * [m
  * Fonction qui démarre le shell "FSH".[m
  */[m
 int Shell_start(){[m
[36m@@ -94,7 +101,7 @@[m [mvoid parse_prompt(){[m
     char_lu[0] = '\0';[m
     // tant qu'il y a quelque chose à lire (différent \0)[m
     LinkedList file_principale = obj_new(_LinkedList_);[m
[31m-    LinkedList file_secondaire = obj_new(_LinkedList_);	[m
[32m+[m[32m    LInkedList file_secondaire = obj_new(_LinkedList_);[m[41m	[m
     while(1){[m
         // On récupère le caractère pointé par user_prompt puis on incrémente pour les caractères suivant.[m
  	[m
[36m@@ -115,7 +122,7 @@[m [mvoid parse_prompt(){[m
 				printf("...commande externe");[m
 			}[m
 		}[m
[31m-		LinkedList_add_last(file_principale, file_secondaire, 0);[m
[32m+[m		[32mLinkedLIst_add_last(file_principale, file_secondaire, 0);[m
 		break;[m
 	}[m
 [m
[36m@@ -139,11 +146,9 @@[m [mvoid parse_prompt(){[m
             // on remet lu1lettre à 0 car nous voulons relire une commande[m
             lu1lettre = 0;[m
             printf("'%s' \n", char_lu);[m
[31m-            Command c = obj_new(_Command_,char_lu);[m
[31m-            obj_print(c);[m
 [m
             // à mettre dans une fonction a par[m
[31m-	    Class tmp = get_internal_cmd(char_lu);[m
[32m+[m	[32m    Class tmp = get_interal_cmd(char_lu);[m
             if(tmp){[m
 		// je cré l'objet associé à ma commande[m
                	Command cmd = obj_new(tmp, char_lu);[m
[1mdiff --git a/shell.h b/shell.h[m
[1mindex 388849a..b86cffd 100644[m
[1m--- a/shell.h[m
[1m+++ b/shell.h[m
[36m@@ -1,9 +1,12 @@[m
[32m+[m[32m/**[m
[32m+[m[32m * @file shell.h[m
[32m+[m[32m * @brief Fichier contenant la déclration des variables globales du shell ainsi que les fonctions utilisées par le shell pour[m[41m [m
[32m+[m[32m * le traitement, la construction et l'exécution des commandes.[m
[32m+[m[32m */[m
 #ifndef SHELL_H[m
 #define SHELL_H[m
 /**[m
  * @brief Il s'agit du nombre de caractère maximal que doit avoir le prompt.[m
[31m- * [m
[31m- * [m
  */[m
 #define PROMPT_CHAR_LIMITS 30[m
 [m
[36m@@ -20,7 +23,6 @@[m [mint Shell_start();[m
 extern char working_dir[4096];[m
 [m
 /**[m
[31m- * @internal.[m
  * Il s'agit de la chaine de caractère saisie par l'utilisateur.[m
  */[m
 extern char *usr_prompt;[m
