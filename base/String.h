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
        const char* str; 

    

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
        const char* const str;
    } __string_private;
    
    /**
     * @brief Attribut privés des objets de type String.
     * 
     * Il s'agit des attribut de la structure contenant les champs de l'objet
     * ces attribut seront visible pour tous ceux qui "include" cette en-tête.
     */
    #define STRING_H_FIELD \
        const char _[sizeof(__string_private)]; // Les champs privés rendus opaques par un pointeur vers une zone "vide".
    
    /** endcond */
    
    /**
     * @brief Classe `String` des chaines de caractères.
     *
     * Il s'agit d'un boxing du type `char*` en object de 
     * type String.
     * <p>Il permet nottement de les utiliser dans des structure de donnée génériques comme 
     * les listes.
     * 
     * Construction : obj_new( _String_, <chaine_de_caractere> );
     */
    typedef struct _String_ {
        /**
         * Classe String
         */
        Class _String_;
        STRING_H_FIELD
    }* String;

#endif
    
    extern Class _String_;

#endif
