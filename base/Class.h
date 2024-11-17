/**
 * @file Class.h
 * @brief Fichier d'en-tête contenant la définition du type Class.
 */

#ifndef CLASS_H
#define CLASS_H

#include <stdarg.h>
#include <stddef.h>

/**
 *
 * @brief Structure représentant la classe dans laquelle appratient un objet.
 *
 * Il s'agit d'une structure fournissant des informations sur la classe d'un objet créé. 
 * On y retrouve par exemple, la taille de mémoire à allouer (avec `calloc()`) lors de 
 * la construction de l'objet par le constructeur générique du fichier d'en-tête `new.h`. 
 * 
 * Elle contient égalment des détails sur la classe, tels que son nom, la référence à la 
 * classe parente (en cas d'héritage), ainsi que les fonctions de construction et de 
 * destruction associées. Ces informations permettent au constructeur générique d'invoquer 
 * ces fonctions dynamiquement (liaison dyn.).
 * 
 * Cette structure peut également jouer le rôle d'une sorte d'empreinte unique pour les 
 * objets, car étant donné que chaque classe d'objets possède sa propre structure (partagée 
 * par tous les objets créés à partir de cette classe). Cela permet, par exemple, de 
 * déterminer si deux objets appartiennent à la même classe (de manière similaire au 
 * mécanisme instanceof en Java).
 *
 *              ********************** IMPORTANT ***********************
 *
 * **Important** : Lors de la création d'une classe, le pointeur vers la classe des objets (le champ 
 * de type Class) doit impérativement être le premier champ déclaré dans la structure. Ce 
 * choix est essentiel, car il garantit que les fonctions génériques opérant sur les objets 
 * des classes puissent traiter n'importe quel objet comme un pointeur vers son premier élément. 
 *
 * Cette approche repose sur le fait qu'une structure est stockée en mémoire de manière contiguë, 
 * comme un tableau, cela permet d'accéder directement à la classe de l'objet en considérant l'objet
 * comme un pointeur `Class*` car le champs `Class` est son premier élément.
 *
 * En revanche, ce mécanisme ne fonctionne pas pour les autres champs de la structure, même le 
 * deuxième (pas comme les tableaux où on peut rajouter +1 pour aller au suivant), car ils 
 * peuvent avoir des types différents, et le compilateur peut insérer des zones de padding pour 
 * l'alignement en mémoire. 
 *
 * Par conséquent, cet invariant doit impérativement être respecté pour assurer le bon fonctionnement 
 * du projet.
 *
 *             *********************************************************
 */
typedef struct _Class_ {

    /**
     * @brief Classe du parent. NULL si pas de parent.
     */
    const struct _Class_* _super_;                    
    /**
     * @brief Le nom de la classe de l'objet.
     */
    const char* name;                           
    /**
     * @brief La taille d'allocation à la construction d'un objet.
     */
    const size_t size;
    /**
     * @brief Référence vers le constructeur d'une instance de la classe.
     * 
     * Cette fonction sera appelée dynamiquement par la fonction `obj_new()` définie 
     * dans `new.h`
     */
    void* (* const ctor) (void*, va_list*);            
    /**
     * @brief Référence vers le destructeur d'une instance de la classe.
     *
     * Cette fonction sera appelée dynamiquement par la fonction `obj_destroy()` définie 
     * dans `new.h`
     */
    void* (* const dtor) (void*);                       
    /**
     * @brief Référence vers la fonction `to_string` de la classe.
     *
     * Cette fonction sera appelée dynamiquement par la fonction `obj_to_string()` définie 
     * dans `new.h`
     */
    char* (* const to_string) (const void*);            
    /**
     * @brief Référence vers la fonction `equals` de la classe 
     *
     * Cette fonction sera appelée dynamiquement par la fonction `obj_equals()` définie 
     * dans `new.h`
     */
    int (* const equals) (const void*, const void*);    
        
    /*
     * @brief Référence vers la fonction clone de la classe.
     *
     * Cette fonction est appellée dynamiquement par la fonction `obj_clone()` définie 
     * dans `new.h`
     */
    void* (*const clone) (const void*);
    /*
     * @brief Référence vers la fonction print de la classe.
     *
     * Cette fonction est appellée dynamiquement par la fonction `obj_print()` définie 
     * dans `new.h`
     */
    void (*const print) (const void*);


}* Class;                                        // le type `Class` est pointeur vers cette structure.

#endif 

