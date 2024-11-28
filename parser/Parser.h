/**
 * @file Parser.h
 * @brief Fichier contenant les déclarations des fonctions de l'interface de la classe `_Parser_`.
 *
 * Un objet Parser est un objet qui va analyser une chaine de caractère et va appeler une va appeler 
 * une certaine fonction (handler) à chaque caractère lu, afin qu'elle l'interprète à sa guise.
 * 
 * <br>
 * NB : Lorsqu'un parsing est terminé. L'objet peut ne plus être utilisable (tout appel à `Parser_parse()` finira aussitôt), car le parseur 
 * aura atteint la fin de la chaine à lire (à moins qu'il ne soit stopé avant).
 * <br>
 * 
 *
 * La classe possède un certains nombres de fonction permettant de manipuler la chaine en cours de 
 * lecture et le tampon qui stock les caractères déjà lus.
 *
 * Construction : <code><strong> obj_new(`_Parser_`, char** string, ParseHandler handler) </strong></code><br>
 * où : <br>
 * <ul>
 *  <li> `_Parser_` désigne la classe des objets de type `Parser`</li> 
 *  <li> <strong>`string`</strong> est un pointeur vers une chaine de caractère (Attention la chaine de 
 *  caractère est modifiée pendant la parsing, alors penser à faire une copie).</li>
 *  <li> <strong>`handler`</strong> un objet de type `_ParseHandler_` qui va contenir la fonction qui va gérer chaque caractère lu. </li>
 * </ul>
 */

#ifndef PARSER_H
#define PARSER_H
#include "ParseHandler.h"

#ifdef PARSER_CLASS
    
    #define PARSER_FIELDS \
        char** str; /** Il s'agit de la chaine de caractère à lire ( /!\ elle est modifiée pendant le parsing) */\
        char* buffer; /**Il s'agit des caractères qui sont déjà lus (mémoire tampon)*/\
        int buff_size; /**Il s'agit de la taille du buffer (ça évite de la calculer tout le temps).*/ \
        unsigned char parse; /** Boolean indiquant si oui ou non il faut continuer le parsing (sur la lettre suivante) ou arrêter. */\
        ParseHandler handler; /** Il s'agit du getionnaire qui va gérer chaque caractère lu par le parser */
    
    typedef struct _Parser_* Parser;    

#else

    /**@cond HIDE_THIS*/
    
    typedef struct __parse_prv_f {
        char* const* str;
        char* buffer; 
        int buff_size;
        unsigned char parse;
        ParseHandler handler; 
    } __parser_private_fields;    

    #define PARSER_H_FIELDS \
        const char _parser [ sizeof(__parser_private_fields) ];
    
    /**@endcond*/


    /**
     * @brief Classe des objets de type `Parser`.
     *
     * Un objet Parser est un objet qui va analyser une chaine de caractère et va appeler une va appeler 
     * une certaine fonction (handler) à chaque caractère lu, afin qu'elle l'interprète à sa guise.
     * 
     * La classe possède un certains nombres de fonction permettant de manipuler la chaine en cours de 
     * lecture et le tampon qui stock les caractères déjà lus.
     * 
     * <br>
     * NB : Lorsqu'un parsing est terminé. L'objet peut ne plus être utilisable (tout appel à `Parser_parse()` finira aussitôt), car le parseur 
     * aura atteint la fin de la chaine à lire (à moins qu'il ne soit stopé avant).
     * <br>
     * 
     * Construction : <code><strong> obj_new(`_Parser_`, char** string, ParseHandler handler) </strong></code><br>
     * où : <br>
     * <ul>
     *  <li> `_Parser_` désigne la classe des objets de type `Parser`</li> 
     *  <li> <strong>`string`</strong> est un pointeur vers une chaine de caractère (Attention la chaine de 
     *  caractère est modifiée pendant la parsing, alors penser à faire une copie).</li>
     *  <li> <strong>`handler`</strong> un objet de type `_ParseHandler_` qui va contenir la fonction qui va gérer chaque caractère lu. </li>
     * </ul>
     * 
     * Il faut donc faire attention à la chaine qu'on donne au constructeur (surtout vérifier qu'elle ne sera pas désallouée pendant le parsing). 
     * Toujours faire une copie de la chaine si on prévoit de l'utiliser entierèment ailleurs.
     */
    typedef struct _Parser_{
        /**
         * @brief Classe des objets de type `Parser`.
         *
         * Un objet Parser est un objet qui va analyser une chaine de caractère et va appeler une va appeler 
         * une certaine fonction (handler) à chaque caractère lu, afin qu'elle l'interprète à sa guise.
         * 
         * Construction : <code><strong> obj_new(`_Parser_`, char** string, ParseHandler handler) </strong></code><br>
         * où : <br>
         * <ul>
         *  <li> `_Parser_` désigne la classe des objets de type `Parser`</li> 
         *  <li> <strong>`string`</strong> est un pointeur vers une chaine de caractère (Attention la chaine de 
         *  caractère est modifiée pendant la parsing, alors penser à faire une copie).</li>
         *  <li> <strong>`handler`</strong> un objet de type `_ParseHandler_` qui va contenir la fonction qui va gérer chaque caractère lu. </li>
         * </ul>
         */ 
        Class _Parser_;
        PARSER_H_FIELDS
    }* Parser;

#endif
    // Méthodes publics
    
    /**
     * @brief Classe des objets de type `Parser`.
     *
     * Un objet Parser est un objet qui va analyser une chaine de caractère et va appeler une va appeler 
     * une certaine fonction (handler) à chaque caractère lu, afin qu'elle l'interprète à sa guise.
     * 
     * Construction : <code><strong> obj_new(`_Parser_`, char** string, ParseHandler handler) </strong></code><br>
     * où : <br>
     * <ul>
     *  <li> `_Parser_` désigne la classe des objets de type `Parser`</li> 
     *  <li> <strong>`string`</strong> est un pointeur vers une chaine de caractère (Attention la chaine de 
     *  caractère est modifiée pendant la parsing, alors penser à faire une copie).</li>
     *  <li> <strong>`handler`</strong> un objet de type `_ParseHandler_` qui va contenir la fonction qui va gérer chaque caractère lu. </li>
     * </ul>
     */ 
    extern Class _Parser_;
        
    /**
     * @brief Fonction qui efface tous les caractères lu par le parser.
     *
     * Cette fonction remet la chaine de caractère contenant les caractères lus (mémoire tampon) par le 
     * parseur à une chaine vide.
     *
     * @param self Le perseur dont on veut vider la mémoire tampon.
     */
    void Parser_clear_buf(Parser self);
    
    /**
     * @brief Fonction qui va indiquer au parseur d'ajouter le caractère en tête de lecture dans 
     * la mémoire tampon.
     *
     * Cette fonction va décaler la pointeur de la chaine de caractère de 1, et cela va modifier 
     * le premier caractère pointé par la chaine (la chaine de caractère n'est pas une copie de la chaine 
     * donnée au constructeur, alors penser à la copier avant).
     *
     * @param self Le perseur dont on veut ajouter le caractère en tête de lecture dans la mémoire tampon.
     */
    void Parser_accept(Parser self);
        
    /**
     * @brief Fonction qui indiquer au parseur d'ignorer le caractère en tête de lecture à d'aller au suivant.
     *
     * Cette fonction va décaler la pointeur de la chaine de caractère de 1, et cela va modifier 
     * le premier caractère pointé par la chaine (la chaine de caractère n'est pas une copie de la chaine 
     * donnée au constructeur, alors penser à la copier avant).
     *
     * @param self Le perseur dont on veut ignorer le caractère en tête de lecture.
     */
    void Parser_pass(Parser self);
    
    /**
     * @brief Fonction qui va renvoyer le nombre de caractère lu (qui la taille de la chaine du buffer).
     * 
     * La fonction permet de savoir aussi si on a oui ou non lu un caractère dans `self->str`.
     * 
     * @param self Le parseur dont on souhaite connaitre la taille du buffer.
     * @return >=1 Si le buffer n'est pas vide (il y a un caracètre lu), 0 sinon.
     */
    int Parser_read(Parser self);

    
    /**
     * @brief Fonction qui renvoie la chaine de caractère contenant les caractères lus.
     *
     * Cette fonction ne renvoie pas une copie de la mémoire tampon (donc ne pas désallouer la chaine).
     *
     * @param self Le parseur dont on souhaite récupérer le buffer (la chaine de caractère contenant les caractères lus).
     * @return Le buffer (la chaine de caractère contenant les caractères lus) du paseur.
     */
    const char* Parser_get_buff(Parser self);
            
    /**
     * @brief Fonction qui va commencer le parsing de la chaine du parseur.
     *
     * La fonction va appeller, pour chaque caractère lu, la fonction `ParseHandler_interpret()`.
     *
     * @param self Le parseur dont on souhaite démarrer le parsing.
     */
    void Parser_parse(Parser self);

    /**
     * @brief Fonction qui va mettre un terme au parsing.

     *
     * @param self Le parseur dont on souhaite terminer le parsing.
     */
    void Parser_stop_parse(Parser self);
#endif
