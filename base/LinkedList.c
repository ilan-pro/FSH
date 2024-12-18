#include "ListCell.h"
#include "new.h"
/**
 * @file LinkedList.c
 * @brief Fichier contenant l'implémentation des variables et de toutes les méthodes déclarés dans `LinkedList.h`.
 * 
 * C'est dans ce fichier que sont implémentés les constructeurs, destructeurs, etc, des objets de type `_LinkedList_`,
 * En plus des méthodes publique de `LinkedList.h`.
 */



/**@cond HIDE_THIS*/
#define LINKEDLIST_CLASS "LinkedList"
#include "LinkedList.h"
/**@endcond*/

/*****************************************************/
/************ Déclaration des fonctions **************/

/**
 * Constructeur d'une liste chainée.
 * 
 * La fonction va créer une liste vide dont le type de valeur 
 * sera donnée en arguement.
 *
 * @param self_ La liste alloué par `obj_new()`.
 * @param args Les arguments donnés au constructeur.
 *
 * @return La liste dont la taille est 0.
 */
static void* LinkedList_constructor(void* self_, va_list* args);

/**
 * Desstructeur d'une liste chainée.
 * 
 * La fonction va détruire la liste.
 * @param self_ La liste à détruire.
 * @return La liste dont tous les éléments sont détruits, pour qu'elle soit détuite elle aussi.
 */
static void* LinkedList_destructor(void* self_);
/**
 * Cloneur d'une liste chainée.
 * 
 * Fonction qui va cloner la liste.
 *
 * @param self_ La liste à cloner.
 * @return Une copie de la liste.
 */
static void* LinkedList_clone (const void* self_);

/**
 * Fonction qui va afficher la liste.
 *
 * La fonction va afficher les objets de la liste avec la méthode `obj_to_string()`.
 * 
 * @param self_ La liste dont on veut afficher les éléments.
 */
static void LinkedList_print (const void* self_);

/*****************************************************/
/************* Structure de la classe ****************/

/**
 * @brief Classe `LinkedList` des listes doublement chainées.
 * 
 * Construction : <code><strong> obj_new( `_LinkedList_`) </strong></code>
 * <br>
 * `_LinkedList_` étant la classe de la liste.
 * <p>
 * NB : Tous les éléments de la liste peuvent être de plusieurs type différents
 */
typedef struct _LinkedList_ {
    /**
     * @brief Classe de tous les objets `_LinkedList_`.
     * Construction : <code><strong> obj_new( `_LinkedList_`) </strong></code>
     * <br>
     * `_LinkedList_` étant la classe de la liste.
     * <p>
     * NB : Tous les éléments de la liste peuvent être de plusieurs type différents
     */
    Class _LinkedList_;
    LINKEDLIST_FIELDS
}* LinkedList;

static struct _Class_ __LinkedList__ = {
    NULL,
    LINKEDLIST_CLASS,
    sizeof(struct _LinkedList_),
    LinkedList_constructor,
    LinkedList_destructor,
    0,
    0,
    LinkedList_clone,
    LinkedList_print
};

/**
 * @brief Classe de tous les objets `_LinkedList_`.
 * Construction : <code><strong> obj_new( `_LinkedList_`) </strong></code>
 * <br>
 * `_LinkedList_` étant la classe de la liste.
 * <p>
 * NB : Tous les éléments de la liste peuvent être de plusieurs type différents
 */
const Class _LinkedList_ = &__LinkedList__;

/***************************************************/
/************** Corps des fonctions ****************/

// Constructeur
static void* LinkedList_constructor(void* self_, va_list* args){
            
    // Vérification si on a NULL ou pas.
    assert(self_ && args);
        
    // Le reste reste à NULL, on ne fait rien dans la liste.
    return self_;

}

// Destructeur
static void* LinkedList_destructor(void* self_){
            
    // Vérification si on a NULL ou pas.
    assert(self_);
    LinkedList self = (LinkedList) self_;
    
    if (self->length >=1) // Si la liste n'est pas vide.
        // Destruction du premier élément (qui va détruire le reste)
        obj_destroy(self->first);
    
    // Le dernier élément est déjà détruit pas l'appel précédent.
    
    return self;

}

// Clone
static void* LinkedList_clone(const void* self_){
        
    // Vérification si on a NULL ou pas.
    assert(self_);
    LinkedList self = (LinkedList) self_;
    
    if (self->length >= 1){ // Si la liste contient au moins un élément.
        // Clonage du premier élément (qui va cloner le reste).
        ListCell clone_first = (ListCell) obj_clone(self->first);
        // Récupération du dernier élément à partir du clone.
        ListCell clone_last = ListCell_get_last(clone_first);
            
        // Création du clone de la liste.
        LinkedList clone = (LinkedList) obj_new(_LinkedList_);
        
        // Mise à jour des éléments du clone.
        clone->first = clone_first; 
        clone->last = clone_last;
        clone->length = self->length;
        
        return clone;
    }
    
    // Sinon on renvoie une nouvelle liste vide.
    return  obj_new(_LinkedList_);
    
}


/**
 * @internal
 *
 * Fonction qui renvoie la taille de la liste.
 * 
 * Voir DOC `LinkedList.h`
 * 
 * @param self_ La référence à la liste.
 * @return La taille de la liste.
 */
size_t LinkedList_size(LinkedList self){
    
    // Vérifiacation de l'appartenance à la classe LinkedList
    assert(self && obj_instance_of(self, _LinkedList_));
    return self->length;
    
}
    
/**
 * @internal
 *
 * Fonction qui ajoute un élément en tête de liste.
 *
 * Voir DOC `LinkedList.h`
 *
 * @param self_  La référence à la liste.
 * @param clone Cloner l'objet ou pas.
 * @return Si oui ou non l'ajout a été fait.
 */
int LinkedList_add_first(LinkedList self_, void *object_, int clone){
    
    // Vérification de la liste.
    assert(self_);
    // Vérifiacation de l'appartenance à la classe LinkedList
    assert(obj_instance_of(self_, _LinkedList_));

    if (clone) // Si on doit cloner l'objet.
        object_ = obj_clone(object_);
    
    // Création de la cellule à rajouter.
    ListCell first = obj_new(_ListCell_, object_);
    

    if (self_->length == 0){
        self_->last = first;
    }
    else
        // On met la première cellule de la liste comme suivant de la nouvelle tête de liste.
        ListCell_replace_next(first, self_->first, 0); // Le précédent de `self_->first` est aussi mis à jour dans la fonction
    

    // Mise à jour du premier élément
    self_->first = first;
    
    // Mise à jour de la taille de la liste.
    ++self_->length;

    return 1;
    
}

/**
 * @internal
 *
 * Fonction qui ajoute un élément en tête de liste.
 *
 * Voir DOC `LinkedList.h`
 *
 * @param self_  La référence à la liste.
 * @param clone Cloner l'objet ou pas.
 * @return Si oui ou non l'ajout a été fait.
 */
int LinkedList_add_last(LinkedList self_, void *object_, int clone){
    
    // Vérification de la liste.
    assert(self_);
    // Vérifiacation de l'appartenance à la classe LinkedList
    assert(obj_instance_of(self_, _LinkedList_));
    
    if (clone) // Si on doit cloner l'objet.
        object_ = obj_clone(object_);
    
    // Création de la cellule à rajouter.
    ListCell last = obj_new(_ListCell_, object_);
    

    if (self_->length == 0){
        self_->first = last;
    }
    else{
        // On met la nouvelle fin de liste comme étant suivant de l'ancienne fin de liste.
        ListCell_replace_next(self_->last, last, 0); // Le précédent de last est aussi mis à jour dans cet appel.
    }

    // Mise à jour du dernier élément
    self_->last = last;
    
    // Mise à jour de la taille de la liste.
    ++self_->length;

    return 1;
    
}

/**
 * @internal
 * Fonction qui supprime le premier élément de la liste.
 */
void* LinkedList_remove_first(LinkedList self, int clone){
    
    // Vérfication de self
    assert(self);
    //vérfication si oui ou non  self est un LinkedList
    assert(obj_instance_of(self, _LinkedList_));
    
    // Si la liste est vide, on renvoie NULL
    if (self->length == 0)
        return NULL;
    
    // Valeur à retourner.
    void* value = (clone) ? (obj_clone(ListCell_get_value(self->first))) : NULL;
    
    // Récupération du prochain first.
    ListCell next_first = ListCell_get_next(self->first);
    ListCell_set_next(self->first, NULL);
    
    if (next_first) // Si le prochain élément n'est pas NULL
        // On supprime la référence du second élément au first.
        ListCell_set_previous(next_first, NULL);
    else  // Sinon last devient NULL aussi et la liste devient vide.
        self->last = NULL;

    // On détruit le premier élément de la liste.
    obj_destroy(self->first);
    // On met à jour le premier élément. 
    self->first = next_first;
    
    // Mise à jour de la taille de la liste
    --self->length;

    // On renvoie la valeur (NULL s'il n'y a pas eut de clonage);
    return value;

}

/**
 * @internal
 * Fonction qui supprime le dernier élément de la liste.
 */
void* LinkedList_remove_last(LinkedList self, int clone){
    
    // Vérfication de self
    assert(self);
    //vérfication si oui ou non  self est un LinkedList
    assert(obj_instance_of(self, _LinkedList_));
    
    // Si la liste est vide, on renvoie NULL
    if (self->length == 0)
        return NULL;
    
    // Valeur à retourner.
    void* value = (clone) ? (obj_clone(ListCell_get_value(self->last))) : NULL;
    
    // Récupération du prochain last.
    ListCell next_last = ListCell_get_previous(self->last);
    if (next_last) // Si l'avant dernier élément n'est pas NULL
        // On supprime last comme next de l'avant dernier élément.
        ListCell_set_next(next_last, NULL);

    else  // Sinon alors le premier aussi devient NULL et la liste devient vide.
        self->first = NULL;

    // On détruit le dernier élément de la liste.
    obj_destroy(self->last);
    // On met à jour le dernier élément. 
    self->last = next_last;
    
    // Mise à jour de la taille de la liste
    --self->length;

    // On renvoie la valeur (NULL s'il n'y a pas eut de clonage);
    return value;

}

static void LinkedList_print(const void* self_){

    // Vérification de la liste et l'objet.
    assert(self_);
    // Vérifiacation de l'appartenance à la classe LinkedList
    assert(obj_instance_of(self_, _LinkedList_));
        
    // Conversion en objet de type LinkedList
    LinkedList self = (LinkedList) self_;

    write(1, "[",1);
    if(self->first){
        
        // Appelle de la fonction print du premier élément (qui va continuer jusqu'au dernier).
        obj_print(self->first);
        
    }
    write(1, "]",1);
    
}




/**
 * @internal
 * @brief Fonction qui renvoie la valeur du dernier élément de la liste.
 * 
* La fonction peut être appelée pour tout objet de type `_LinkedList_` 
* ou sous-type de `_LinkedList_`.
* 
* @param self La liste dont on cherche le dernier élément.
* @param clone Booléen dont 1 permet d'indiquer à la fonction qu'on souhaite qu'elle renvoie 
* une copie de l'objet après au lieux de la vraie référence.
*
* @return La valeur du dernier élément ou NULL si la liste est vide.
*/
void* LinkedList_get_last(LinkedList self, int clone){

    // Vérification de la valeur de self.
    assert(self);
    // Vérifiacation de l'appartenance à la classe LinkedList
    assert(obj_instance_of(self, _LinkedList_));

    if (self->length == 0)
        return NULL;
    else
        return (clone) ? obj_clone(ListCell_get_value(self->last)) : ListCell_get_value(self->last);

}

/**
 * @internal
 * @brief Fonction qui renvoie la valeur du premier élément de la liste.
 * 
* La fonction peut être appelée pour tout objet de type `_LinkedList_` 
* ou sous-type de `_LinkedList_`.
* 
* @param self La liste dont on cherche le premier élément.
* @param clone Booléen dont 1 permet d'indiquer à la fonction qu'on souhaite qu'elle renvoie 
* une copie de l'objet après au lieux de la vraie référence.
*
* @return La valeur du premier élément ou NULL si la liste est vide.
*/

void* LinkedList_get_first(LinkedList self, int clone){

    // Vérification de la valeur de self.
    assert(self);
    // Vérifiacation de l'appartenance à la classe LinkedList
    assert(obj_instance_of(self, _LinkedList_));

    if (self->length == 0)
        return NULL;
    else
        return (clone) ? obj_clone(ListCell_get_value(self->first)) : ListCell_get_value(self->first);

}
/**
 * @internal
 * Fonction qui ajoute un élément à une position précise dans la liste.
 * 
 * Mettre le paramètre `clone` à 1 pour renvoyer une copie au lieu de la 
 * la vraie valeur.
 * 
 * **ATTENTION** : Au moment de la destruction de la liste les valeurs 
 * sont également désallouées. Faire une copie (avec `obj_clone()`) (ou 
 * mettre le paramètre `clone` à 1 pour que la fonction le fasse)
 * de l'objet avant de l'ajouter dans la liste, si on tient à garder l'objet 
 * intact après la destruction de la liste.
 * <p>Si on souhaite insérer 2 fois un élément (avec la même référence) dans la 
 * liste il est important de mettre `clone` à 1 pour que la fonction puisse 
 * clonner l'objet car au moment de la destruction il sera supprimé deux fois 
 * ce qui va générer une erreur.
 * 
 * <p>
 * La fonction peut être appelée pour tout objet de type `_LinkedList_` 
 * ou sous-type de `_LinkedList_`.
 *
 * <p>Chaque ajout ne se fait pas en temps constant O(index).
 * 
 * @param self La liste dont on cherche le dernier élément.
 * @param object_ L'objet à insérer dans la liste (copier l'objet si on 
 * souhaite le garder après la destruction de la liste).
 * @param index Position dans laquelle on souhaite insérer l'objet dans la liste.
 * @param clone Boolean dont 1 permet d'indiquer à la fonction si on souhaite qu'elle 
 * ajoute une copie de l'objet (copié avec `obj_clone()`) plutôt que l'objet lui-même. 
 * Cela va permettre de garentir que l'objet ne sera pas détruit avec la liste. Mettre 
 * à 0 sinon.
 * 
 * @return 0 Si l'ajout a échoué 1 sinon.
 */
int LinkedList_add( LinkedList self, void* object, int index, int clone ){

    // Vérification de la self.
    assert(self);
    // Vérification si self est une liste.
    assert(obj_instance_of(self, _LinkedList_));
    // Vérification si la l'indice est dans la liste.
    assert(index >= 0 && index <= self->length);

    if (index == 0) // Si index est le premier élément
        return LinkedList_add_first(self, object, clone); // Ajout en tête de liste/   
    else if (index == self->length) // Si index est le dernier
        return LinkedList_add_last(self, object, clone); // Ajout en fin de liste.

    if(clone) // Si clone est vrai
        object = obj_clone(object); // On fait la copie de l'objet.


    ++self->length;
    return ListCell_add(self->first, object, index); // Ajout dans les cellules.
    
}

/**
 * @internal 
 * @brief Fonction qui va retirer un élément de la liste qui se trouve à une position donnée.
 *
 * Si la liste est vide alors la fonction va renvoyer NULL.
 *
 * ** Important ** : La cellule qui sera retirée sera immédiatement détruite (appelle à 
 * son destructeur). Ne pas rajouter d'objet dont on souhaite garder la référence après 
 * son retrait dans la liste.
 *<p>
 * La fonction peut être appelée pour tout objet de type `_LinkedList_` 
 * ou sous-type de `_LinkedList_`.
 *
 * <p>Chaque suppression se fait en temps constant O(1).
 * 
 * @param self La liste dont on souhaite retirer le dernier élément.
 * @param index L'indice de l'objet dont on souhaite retirer de la liste.
 * @param clone Booléen dont 1 permet d'indiquer à la fonction qu'on souhaite qu'elle nous renvoie 
 * une copie de l'objet après suppression. Pour rappel, l'objet sera détruit après sa suppression 
 * de la liste.
 * 
 * @return La valeur retirée (ou NULL si on ne souhaite pas la récupérer) Si le dernier
 * élément est bien retiré de la liste, NULL sinon.
*/
void* LinkedList_remove( LinkedList self, int index, int clone ){


    // Vérification de la self.
    assert(self);
    // Vérification si self est une liste.
    assert(obj_instance_of(self, _LinkedList_));
    // Vérification si la l'indice est dans la liste.
    assert(index >= 0 && index < self->length);

    // Si la liste est vide on renvoie NULL (pour éviter de faire l'appel juste après)
    if (self->length == 0)
        return NULL;

    else if (index == 0) // Si index est le premier élément
        return LinkedList_remove_first(self, clone); // suppression de la tête de liste/   
    else if (index == self->length - 1) // Si index est le dernier
        return LinkedList_remove_last(self, clone); // suppression de la fin de la liste.
    

    // Décrémentation de la taille.
    --self->length;
    return ListCell_remove(self->first, index, clone);

}

/**
 * @internal
 * @brief Fonction qui renvoie la valeur d' élément de la liste qui se trouve à une position donnée.
 * 
 * Mettre le paramètre `clone` à 1 pour renvoyer une copie au lieu de la 
 * la vraie valeur.
 * 
 * <p>Une recherche ne se fait pas en temps constant, O(index).
 *<p>
    * La fonction peut être appelée pour tout objet de type `_LinkedList_` 
    * ou sous-type de `_LinkedList_`.
    * 
    * @param self La liste dont on cherche l' élément.
    * @param index La position de l'élément.
    * @param clone Booléen dont 1 permet d'indiquer à la fonction qu'on souhaite qu'elle renvoie 
    * une copie de l'objet après au lieux de la vraie référence.
    * 
    * @return La valeur de l'élément ou NULL si la liste est vide.
    */
void* LinkedList_get(LinkedList self, int index,  int clone){

    // Vérification de self.
    assert(self);
    // Vérification si self est une liste.
    assert(obj_instance_of(self, _LinkedList_));
    // Vérification si la l'indice est dans la liste.
    assert(index >= 0 && index < self->length);

    // Si la liste est vide on renvoie NULL (pour éviter de faire l'appel juste après)
    if (self->length == 0){
        return NULL;
    }

    
    else if (index == 0) // Si index est le premier élément
        return LinkedList_get_first(self, clone); // Renvoie de la valeur de la tête de liste/   
    else if (index == self->length - 1) // Si index est le dernier
        return LinkedList_get_last(self, clone); // Renvoie de la valeur de la fin de la liste.

    
    return ListCell_get(self->first, index, clone);

}

/**
 * @internal
 * @brief Fonction qui vérifie si oui ou non un élément se trouve dans la liste.
 * 
 * La fonction va comparer les éléments avec la méthode `obj_equals()`, qui va appeler 
 * dynamiquement sa redéfinition sur les objets de la liste (si redefinie).
 * 
 * 
 * @param self La liste à parcourir.
 * @param object L'élément à rechercher.
 * @return 1 Si l'élément se trouve dans la liste et 0 sinon.
 * @see obj_equals()
 */
int LinkedList_contains( LinkedList self, void* object ){

    // Vérification de self.
    assert(self);
    // Vérification si self est une liste.
    assert(obj_instance_of(self, _LinkedList_));

    // Si la liste est vide alors il n'y a rien à rechercher.
    if (self->length == 0)
        return 0;

    // Sinon on interroge les cellules.
    return ListCell_contains(self->first, object);

}

/**
 * @brief Fonction qui renvoie l'indice de la première occurence d'un élément 
 * qui se trouve dans la liste.
 * 
 * La fonction va comparer les éléments avec la méthode `obj_equals()`, qui va appeler 
 * dynamiquement sa redéfinition sur les objets de la liste (si redefinie).
 * 
 * 
 * @param self La liste à parcourir.
 * @param object L'élément à rechercher.
 * @return L'indice dans la liste de l'élément. -1 Si l'élément n'est pas dans la liste.
 * @see obj_equals()
 */
long long int LinkedList_index_of( LinkedList self, void* object){

    // Vérification de self.
    assert(self);
    // Vérification si self est une liste.
    assert(obj_instance_of(self, _LinkedList_));

    // Si la liste est vide alors il n'y a rien à rechercher.
    if (self->length == 0)
        return -1;

    // Sinon on interroge les cellules.
    return ListCell_index_of(self->first, object, 0);

}