/**
 * @file String.h 
 * @brief Fichier d'en-tête contenant les déclarations des variables publiques et de toutes les méthodes public de String.
 */


#include "Class.h"
#ifndef STRING_H
#define STRING_H

#ifdef STRING_CLASS
    
    /**
     * Il s'agit des champs d'un objet de type String.
     * <p>
     * Cette macro n'est visible que sur des sources dont la définition STRING_CLASS est faite.
     * cela permet d'avoir une encasulation des attributs en les rendant privés dans toute implémentation 
     * ne possèdant pas la décrlatation STRING_CLASS.
     * <p>
     * Cela permet également pour l'héritage de classe, de récrire PARENT_FIELD (Ici STRING_FIELD)
     * pour inclure les arrtibuts du parents dans ceux de l'enfant.
     */
    #define STRING_FIELD \
        const int size; \
        const char* str; 

    typedef struct _String_* String;

#else

    /**
     * @cond HIDE_THIS
     *
     * Attribut privés des objets de type String.
     *
     * On les déclrare ici pour permettre de reserver un espace mémoire vide (qui ne contient aucune information utile)
     * afin que le deux structures contenant les attributs de classe (celle visible publiquement ici et celle qui 
     * contient les champs privés visibles uniquement dans le fichiers source) aient la même taille.
     */
    typedef struct string_private {
        const char* str;
    } __string_private;
    
    /**
     * Attribut des objets de type String.
     * 
     * Il s'agit des attributs de la structure contenant les champs de l'objet.
     * Ces attributs seront visibles pour tous ceux qui "include" cette en-tête.
     *
     * Le tableau `const char _[sizeof(__string_private)];` contiendera tous les champs 
     * que l'objet aura rendu privé, ce pointeur n'est là que pour que les deux structures 
     * contenant les attributs de classe (celle visible publiquement ici et celle qui
     * contient les champs privés visibles uniquement dans le fichiers source) aient la même taille.
     */
    #define STRING_H_FIELD  \
        const int size; \
        \
        const char _[sizeof(__string_private)]; // Les champs privés rendus opaques par un pointeur vers une zone "vide".
    
    /**@endcond */
    
    /**
     * @internal
     * 
     * Classe `String` des chaines de caractères.
     *
     * Il s'agit d'un boxing du type `char*` en object de 
     * type String.
     * <p>Il permet nottement de les utiliser dans des structure de donnée génériques comme 
     * les `_LinkedList_`.
     * 
     * Construction : obj_new( _String_, <chaine_de_caractere> );
     */
    typedef struct _String_ {
        const Class _String_;
        STRING_H_FIELD
    }* String;

#endif
    
    /**
     * @internal
     * Classe de tous les objets de type String.
     * 
     * Elle est défini dans le fichier `String.c`
     */
    extern const Class _String_;
    
    /**
     * @brief Fonction qui peremet de faire la concaténation de deux chaines de caractères.
     *
     * La chaine résultante est un objet qu'il faudra détruire. <br>
     * Les deux chaines donnée en arguments ne sont pas modifiées.
     *
     * @param s1_ La première chaine.
     * @param s2_ La seconde chaine.
     *
     * @return La chaine issue de la concaténation des deux chaine (chaine qu'il faudra détruire après utilisation.)
     */
    String String_concat(String s1_, String s2_);

    /**
     * @brief Fonction qui va vérifier si oui ou non le caractère donné 
     * en argument à la fonction se trouve dans la chaine.
     * 
     * @param self La chaine à parcourir.
     * @param c Le caractère à rechercher.
     *
     * @return 0 si la caractère n'est pas trouvé, 1 sinon. 
     */
    int String_contains( String self, char c);        


#endif
