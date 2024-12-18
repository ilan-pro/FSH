/**
 * @file SimpleCmd.h
 * @brief  Fichier d'en-tête contenant les déclarations des variables publiques et de toutes les méthodes public de `_SimpleCmd_`.
 * 
 * Un objet de type `_SimpleCmd_` est une commande dont les arguments et les paramètre sont récupéré directement (avec un parsing)
 * dans le construteur. Ces commandes sont donc tous de la forme `Cmd_name -param1 -param2 arg1 -parm3 ...`. Les arguments et les 
 * paramètres pouvant êtres déclarés dans un ordre quelconque et les paramètre peuvent être 
 * concaténés (Ex: `cmd -AbCd` = `cmd -A -b -C -d`).
 * <p>
 * Cette classe ne fourni pas de méthode `execute()`, elle doit donc être héritée par une classe qui implémentera la méthode 
 * sinon l'exécution déclanchera une erreur.
 * <p>
 * NB : Les commandes ayant une définition complexe comme les commandes avec des paramètres 
 * non-booléens (Ex : `cmd --param=oiseau -param1 Wazo`  ou  `for i in ... {}` ). Ne peuvent pas être construit directement 
 * avec le constructeur, alors il est conseillé d'hériter directement de la classe `_Command_` au lieu de celle-ci et 
 * de définir un constructeur qui gère la construction de la commande.
 * <p>
 * Construction : <code><strong> obj_new( `_SimpleCmd_`, char* cmd_name, char** prompt ); </strong></code>
 * où : <ul>    
 *         <li> `_SimpleCmd_` désigne la classe des objets de type `SimpleCmd`. </li>
 *         <li> `char* cmd_name` Le nom de la commande.</li>
 *         <li> `char** prompt` Le reste du prompt qui va servir à construire la comande.</li>
 *      </ul>
 */

#ifndef SIMPLE_CMD_H 
/**@cond HIDE_THIS */
#define SIMPLE_CMD_H
/**@endcond */

#ifdef SIMPLE_CMD_CLASS
    #define COMMAND_CLASS
    #include "Command.h"

    // Attributs d'une commande.
    #define SIMPLE_CMD_FIELDS \
        COMMAND_FIELDS

    typedef struct _SimpleCmd_* SimpleCmd;
        
#else
#include "Command.h"
    
    /** @cond HIDE_THIS*/
    // Champs privées des commandes.
    typedef struct __simplecmd_private_fields{
    } __simple_cmd_priv_fields;
    
    // Champs visible depuis l'extérieur, les champs privés sont cachés par un pointeur opaque
    #define SIMPLE_CMD_H_FIELDS \
        COMMAND_H_FIELDS \
        const char _simple_cmd_prv[sizeof(__simple_cmd_priv_fields)];
    /**@endcond*/
    
    /**
     * @brief Classe de toutes les commandes simples.
     * 
     * Un objet de type `_SimpleCmd_` est une commande dont les arguments et les paramètre sont récupéré directement (avec un parsing)
     * dans le construteur. Ces commandes sont donc tous de la forme `Cmd_name -param1 -param2 arg1 -parm3 ...`. Les arguments et les 
     * paramètres pouvant êtres déclarés dans un ordre quelconque et les paramètre peuvent être 
     * concaténés (Ex: `cmd -AbCd` = `cmd -A -b -C -d`).
     * <p>
     * Construction : <code><strong> obj_new( `_SimpleCmd_`, char* cmd_name, char** prompt ); </strong></code>
     * où : <ul>
     *         <li> `_SimpleCmd_` désigne la classe des objets de type `SimpleCmd`. </li>
     *         <li> `char* cmd_name` Le nom de la commande.</li>
     *         <li> `char** prompt` Le reste du prompt qui va servir à construire la comande.</li>
     * </ul>
     */
    typedef struct _SimpleCmd_ {
        
        /**
         * @brief Classe de toutes les commandes simples.
         * 
         * Un objet de type `_SimpleCmd_` est une commande dont les arguments et les paramètre sont récupéré directement (avec un parsing)
         * dans le construteur. Ces commandes sont donc tous de la forme `Cmd_name -param1 -param2 arg1 -parm3 ...`. Les arguments et les 
         * paramètres pouvant êtres déclarés dans un ordre quelconque et les paramètre peuvent être 
         * concaténés (Ex: `cmd -AbCd` = `cmd -A -b -C -d`).
         * <p>
         * Construction : <code><strong> obj_new( `_SimpleCmd_`, char* cmd_name, char** prompt ); </strong></code>
         * où : <ul>
         *         <li> `_SimpleCmd_` désigne la classe des objets de type `SimpleCmd`. </li>
         *         <li> `char* cmd_name` Le nom de la commande.</li>
         *         <li> `char** prompt` Le reste du prompt qui va servir à construire la comande.</li>
         * </ul>
         */
        Class _SimpleCmd_;
        SIMPLE_CMD_H_FIELDS
    }* SimpleCmd;
        
#endif
    
    
    /**
     * @brief Classe de toutes les commandes simples.
     * 
     * Un objet de type `_SimpleCmd_` est une commande dont les arguments et les paramètre sont récupéré directement (avec un parsing)
     * dans le construteur. Ces commandes sont donc tous de la forme `Cmd_name -param1 -param2 arg1 -parm3 ...`. Les arguments et les 
     * paramètres pouvant êtres déclarés dans un ordre quelconque et les paramètre peuvent être 
     * concaténés (Ex: `cmd -AbCd` = `cmd -A -b -C -d`).
     * <p>
     * Construction : <code><strong> obj_new( `_SimpleCmd_`, char* cmd_name, char** prompt ); </strong></code>
     * où : <ul>
     *         <li> `_SimpleCmd_` désigne la classe des objets de type `SimpleCmd`. </li>
     *         <li> `char* cmd_name` Le nom de la commande.</li>
     *         <li> `char** prompt` Le reste du prompt qui va servir à construire la comande.</li>
     * </ul>
     */
    extern const Class _SimpleCmd_;

    /**
     * @brief Fonction qui vérifie si oui ou non le caractère actuellement pointé par 
     * le pointeur vers le prompte de l'utilisateur est valide, c'est à dire 
     * si on doit oui ou non arrêter la parsing de la commande.
     * 
     * Un caractère est valide si et seulement si : 
     *  - Ce n'est pas le caracètre null (fin de chaine).
     *  - Ce n'est pas l'un des caracètres '|', ';' ou '}'.
     * 
     * Si le caractère n'est pas null (fin de chaine) et qu'il est entre guillemets (simple ou double) ou échapé 
     * alors il est autamatiquement valide ('|', ';' et '}' seront considérés comme des caracètres normaux).
     * 
     * @param quoted Booléen indiquant si le caracètre est entre guillemets simple.
     * @param double_quoted Booléen indiquant si le caracètre est entre guillemets doubles.
     * @param esc Booléen indiquant si le caracètre est echapé par \ .
     * @param prompt Le prompt en cours de paring.
     * @return Si oui ou non le caractère actuellement pointé par 
     * le pointeur vers le prompte de l'utilisateur est valide.
     */
    int SimpleCmd_actual_char_is_legal(int quoted, int double_quoted, int esc, char** prompt);
#endif
