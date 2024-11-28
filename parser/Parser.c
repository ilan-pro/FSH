
/**
 * @file Parser.c
 * @brief Fichier contenant l'implémentation de la classe `_Parser_`.
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
 *  caractère est modifiée pendant la parsing, alors penser à faire une copie). cette chaine également n'est pas désallouée après la destruction de l'objet `Parser`</li>
 *  <li> <strong>`handler`</strong> un objet de type `_ParseHandler_` qui va contenir la fonction qui va gérer chaque caractère lu. </li>
 * </ul>
 */
#include "ParseHandler.h"
#include "../base/new.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/** @cond HIDE_THIS */
#define PARSER_CLASS "Parser"
#include "Parser.h"
/** @endcond */



/*****************************************************/
/************ Déclaration des fonctions **************/

/**
 * Constructeur d'un objet Parser.
 *
 * Le constructeur va initialiser les attributs et ne va pas faire 
 * une copie de la chaine donnée en argument. 
 * 
 * 
 * Construction : <code><strong> obj_new(`_Parser_`, char** string, ParseHandler handler) </strong></code><br>
 * où : <br>
 * <ul>
 *  <li> `_Parser_` désigne la classe des objets de type `Parser`</li> 
 *  <li> <strong>`string`</strong> est un pointeur vers une chaine de caractère (Attention la chaine de 
 *  caractère est modifiée pendant la parsing, alors penser à faire une copie). cette chaine également n'est pas désallouée après la destruction de l'objet `Parser`</li>
 *  <li> <strong>`handler`</strong> un objet de type `_ParseHandler_` qui va contenir la fonction qui va gérer chaque caractère lu. </li>
 * </ul>
 */
static void* Parser_constructor (void*, va_list*);
    
/**
 * Fonction qui va détruire l'objet `self` 
 *
 * /!\ La fonction ne desalloue pas la chaine parsée.
 */
static void* Parser_destructor( void* );
    
/**
 * Fonction qui va afficher des informations sur le parseur.
 */
static void Parser_print( const void* );

/*****************************************************/
/************* Structure de la classe ****************/

/**
 * @internal
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
typedef struct _Parser_ {
     Class _Parser_;
     PARSER_FIELDS
}* Parser;

static struct _Class_ __Parser__ = {
    NULL,
    PARSER_CLASS,
    sizeof(struct _Parser_),
    Parser_constructor,
    Parser_destructor,
    0,
    0,
    0,
    Parser_print
};


/**
 * @internal
 * @brief Classe des objets de type `Parser`.
 *
 * Un objet Parser est un objet qui va analyser une chaine de caractère et va appeler une va appeler
 * une certaine fonction (handler) à chaque caractère lu, afin qu'elle l'interprète à sa guise.
 * 
 * <br>
 * NB : Lorsqu'un parsing est terminé. L'objet peut ne plus être utilisable (tout appel à `Parser_parse()` finira aussitôt), car le parseur 
 * aura atteint la fin de la chaine à lire (à moins qu'il ne soit stopé avant).
 * <br>4
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
Class _Parser_ = &__Parser__;

/***************************************************/
/************** Corps des fonctions ****************/

// Constructor
static void* Parser_constructor(void* self_, va_list* args){
        
    // Vérificiation de self et args
    assert(self_);
    assert(args);
        
    // Conversion en objet de type Parser.
    Parser self = (Parser) self_;

    // Récupération des arguments
    char** str = va_arg(*args, char**);
    ParseHandler handler = va_arg(*args, ParseHandler);
        
    // Vérification de str et handler.
    assert(str);
    assert(handler);

    self->str = str; // (Pas de copie faite !)
    self->handler = handler; 
    self->parse = 1;

    self->buffer = malloc(sizeof(char));
    self->buffer[0] = '\0';
    
    return self;
}


// Destructor
static void* Parser_destructor(void* self_){
    
    // Vérfication de self_
    assert(self_);

    Parser self = (Parser) self_;
    
    free(self->buffer);
    obj_destroy(self->handler);
    // self->str n'est pas désalloué.
    
    return self;
}

// Print
static void Parser_print( const void* self_ ){
    
    // Vérfication de self_
    assert(self_);
    // Conversion en objet de type Parser
    Parser self = (Parser) self_;
    
    write(1, self->_Parser_->name, strlen(self->_Parser_->name));
    write(1, " : str=\"", 8);
    write(1, *self->str, strlen(*self->str));
    write(1, "\". buff=\"", 9);
    write(1, self->buffer, self->buff_size);
    write(1, "\".", 2);

}



/**
 * @internal
 * @brief Fonction qui efface tous les caractères lu par le parser.
 *
 * Cette fonction remet la chaine de caractère contenant les caractères lus (mémoire tampon) par le
 * parseur à une chaine vide.
 *
 * @param self Le perseur dont on veut vider la mémoire tampon.
 */
void Parser_clear_buf(Parser self){
    
    // Vérification de self
    assert(self);
    assert(obj_instance_of(self, _Parser_));

    self->buffer = realloc(self->buffer, sizeof(char));
    self->buffer[0] = '\0';
    self->buff_size = 0;

}



/** @internal
 * @brief Fonction qui va indiquer au parseur d'ajouter le caractère en tête de lecture dans 
 * la mémoire tampon.
 *
 * Cette fonction va décaler la pointeur de la chaine de caractère de 1, et cela va modifier 
 * le premier caractère pointé par la chaine (la chaine de caractère n'est pas une copie de la chaine 
 * donnée au constructeur, alors penser à la copier avant).
 *
 * @param self Le perseur dont on veut ajouter le caractère en tête de lecture dans la mémoire tampon.
 */
void Parser_accept(Parser self){

    // Vérification de self
    assert(self);
    assert(obj_instance_of(self, _Parser_));
   
    self->buffer = realloc(self->buffer,1 + (++self->buff_size));
    self->buffer[self->buff_size] = '\0';
    self->buffer[self->buff_size - 1] = *((*self->str)++);

}


/**
 * @internal
 * @brief Fonction qui indiquer au parseur d'ignorer le caractère en tête de lecture à d'aller au suivant.
 *
 * Cette fonction va décaler la pointeur de la chaine de caractère de 1, et cela va modifier
 * le premier caractère pointé par la chaine (la chaine de caractère n'est pas une copie de la chaine
 * donnée au constructeur, alors penser à la copier avant).
 *
 * @param self Le perseur dont on veut ignorer le caractère en tête de lecture.
 */
void Parser_pass(Parser self){

    // Vérification de self
    assert(self);
    assert(obj_instance_of(self, _Parser_));
    
    (*self->str)++;
    
}


/**
 * @internal
 * @brief Fonction qui va renvoyer le nombre de caractère lu (qui la taille de la chaine du buffer).
 * 
 * La fonction permet de savoir aussi si on a oui ou non lu un caractère dans `self->str`.
 * 
 * @param self Le parseur dont on souhaite connaitre la taille du buffer.
 * @return >=1 Si le buffer n'est pas vide (il y a un caracètre lu), 0 sinon.
 */
int Parser_read(Parser self){

    // Vérification de self
    assert(self);
    assert(obj_instance_of(self, _Parser_));
    
    return self->buff_size;
}


/**
 * @internal
 * @brief Fonction qui renvoie la chaine de caractère contenant les caractères lus.
 *
 * Cette fonction ne renvoie pas une copie de la mémoire tampon (donc ne pas désallouer la chaine).
 *
 * @param self Le parseur dont on souhaite récupérer le buffer (la chaine de caractère contenant les caractères lus).
 * @return Le buffer (la chaine de caractère contenant les caractères lus) du paseur.
 */
const char* Parser_get_buff(Parser self){

    // Vérification de self
    assert(self);
    assert(obj_instance_of(self, _Parser_));
    
    return self->buffer; 
}



/**
 * @biref Fonction qui va commencer le parsing de la chaine du parseur.
 *
 * La fonction va appeller, pour chaque caractère lu, la fonction `ParseHandler_interpret()`.
 *
 * @param self Le parseur dont on souhaite démarrer le parsing.
 */
void Parser_parse(Parser self){

    // Vérification de self
    assert(self);
    assert(obj_instance_of(self, _Parser_));
    
    while(*(*self->str) && self->parse) // Tant qu'on est pas tombé sur le caractère de fin '\0'
        ParseHandler_interpret(self->handler, self, *(*self->str)); // Appelle de ParseHandler_interpret() pour la caractère pointé.
    
    // Si on a est ici c'est que le parsing est terminée.
    //
    // Alors on appel la fonction qui gère la fin du parsing (peut être utile pour travailler sur la dernière valeur qui se trouve dans le buffer)
    ParseHandler_action_on_parse_finish(self->handler, self);

}

            

/**
 * @brief Fonction qui va mettre un terme au parsing.
 *
 * @param self Le parseur dont on souhaite terminer le parsing.
 */
void Parser_stop_parse(Parser self){

    // Vérification de self
    assert(self);
    assert(obj_instance_of(self, _Parser_));

    self->parse = 0;

}














