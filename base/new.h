/**
 * @file new.h
 * @brief Fichier d'en-tête contenant les déclarations de toutes les méthodes génériques agissant 
 * sur des objets.
 *
 * Ce fichier contient les signatures publiques des fonctions générique agissant sur des objets, 
 * par exemple `obj_new()` qui est une fonction qui initialise un nouvel objet, à partir d'une 
 * classe.
 */

#include "Class.h"
#ifndef NEW_H // protection contre les multiples inclusions.
#define NEW_H 

/**
 * @brief Fonction qui crée un objet à partir de sa classe.
 *
 * La fonction alloue dynamiquement la mémoire l'objet avec 
 * calloc(), et appelle le constructeur définie dans la classe 
 * (Liaison dynamique).
 * Si un constructeur n'est pas définie alors la fonction `obj_new()`
 * renvoie l'objet avec tous les attribut à 0 (à cause calloc()).
 *
 * @param \_class\_  La classe de l'objet à construire.
 * @param ... La liste des arguments que prend le constructeur.
 *
 * @return L'objet renvoyé par le constructeur définie 
 * dans sa classe ou la structure telle qu'elle a été alloué avec `calloc()` 
 * (si il n'y avait pas de constructeur).
 * 
 */
void* obj_new(void* _class_, ...);

/**
 * @brief Fonction qui détruit et desalloue l'espace mémoire alloué pour l'objet. 
 *
 * La fonction va appeler la fonction `dtor()` définie dans la classe de 
 * l'objet, qui va désallouer tous les éléments utilisés dans l'objet, puis 
 * après l'appel, va désallouer l'objet avec `free()`. 
 *
 * NB : La desallocation du pointeur vers l'objet `self_` ne doit pas se faire 
 * dans `dtor()`, cela va générer une erreur car au retour de l'appel cette 
 * fonction le fait également (il y aura donc deux `free()` sur une référence)).
 *
 * @param self_ L'objet à détruire.
 * @throw Abort Le programme est interrompu si une fonction de destruction n'est pas définie 
 * dans la classe.
 */
void obj_destroy(void* self_);

/**
 * @brief Fonction qui renvoie une chaine de caractère représentative de 
 * l'objet.
 *
 * La fonction appelle la fonction `to_string()` définie dans la 
 * classe de l'objet qui doit renvoyer une chaine de caractère.
 *
 * Si la fonction n'est pas définie, `obj_to_string()` renvoie le nom 
 * de la classe suivi de l'adresse du pointeur de l'objet.
 * 
 * ATTENTION : NE JAMAIS RENVOYER UNE CHAINE DE CARACTERE ALLOUEE DYNAMIQUEMENT !<br> 
 * Il est impératif d'écrire directement dans le buffer `to_string_buffer` situé 
 * en dessous et de laisser au code appelant la responsabilité de décider s'il 
 * souhaite en faire une copie ou non.
 * 
 * @param self_ L'objet à interroger.
 * @return La chaine de caractère renvoyé par `to_string`, qui est généralement 
 * un pointeur vers `to_string_buffer` ci-dessous.
 *
 */
char* obj_to_string(const void* self_);

/**
 * @brief Mémoire tampon qui doit être utilisée dans les fonctions `to_string` des objets. 
 * Cette mémmoire est généralement statique, afin d'éviter les allocations dynamiques. 
 *
 * De cette façon on réduit les risques de fuites mémoire et on confie au code 
 * appelant la responsabilité de gérer, si nécessaire, une copie de la chaîne résultante.
 */
extern char to_string_buffer[1024];

/**
 * @brief Fonction qui renvoie si oui ou non les deux objets 
 * sont egaux.
 *
 * La fonction appelle la fonction `equals()` définie 
 * dans la classe de l'objet. 
 * Si la fonction n'est pas définie elle compare bit à bit les 
 * deux objets en mémoire. (avec la fonction `memcmp()`)
 * 
 * @param self_ L'objet 1.
 * @param object_ L'objet 2.
 * @return Si oui (1) ou non (0) objet1 et objet2 sont égaux.
 */
int obj_equals(const void* self_, const void* object_);

/**
 *
 * @brief Fonction qui permet de vérifier si un objet est l'instance d'une classe 
 * donnée.
 *
 * Cette fonction fonctionne également sur les objets dont la classe hérite d'une autre (
 * elle vérifie récursivement avec les ancètres).
 *
 * @param self_ L'objet dont on souhaite vérifier l'appartenance à la classe.
 * @param \_class\_ La classe.
 * @return Si oui (1) ou non (0) l'objet est une instance de la classe.
 *
 */
int obj_instance_of(const void* self, const Class _class_);

#endif
