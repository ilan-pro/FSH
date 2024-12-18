/**
 * @file ListCell.c
 * @brief Fichier contenant l'implémentation des variables et de toutes les méthodes déclarés dans `ListCell.h`.
 * 
 * C'est dans ce fichier que sont implémentés les constructeurs, destructeurs, etc, des objets de type `_ListCell_`,
 * En plus des méthodes publique de `ListCell.h`.
 */

#include "new.h"

/**@cond HIDE_THIS*/
#define LISTCELL_CLASS "ListCell"
#include "ListCell.h"
/*@endcond*/

/*****************************************************/
/************ Déclaration des fonctions **************/
    
/**
 * Constructeur qui va créer une cellule pour une liste doublement chainée.
 * 
 * Le constructeur doit revoir un objet (sous forme de void*)
 * qui sera la valeur contenu dans la cellule.
 *
 * @param self_ La cellule dans laquelle stocker l'élément.
 * @param args Une liste (va_list) qui contient la valeur à insérer.
 *
 * @return Un objet de type ListCell dans lequel figure l'élément.
 */
static void* ListCell_constructor(void* self_, va_list* args);
    
/**
 * Cette fonction doit généralement être appelée par `obj_destroy()`
 * à moins de veiller soie-même à libérer la référence après son appel.
 *
 * Fonction qui va libérer l'espace mémoire reservée à la cellule 
 * et détruire également la valeur qui y est stockée.
 * La fonction va ensuite continuer récursivement sur la cellule 
 * suivante détuisant ainsi toute la sous-liste commençant par 
 * cette cellule.
 * 
 * @param self_ La celle à détruire.
 * @return Une référence de la cellule pour qu'elle soit libérée.
 */
static void* ListCell_destroy(void* self_);
    
/**
 * Fonction qui va faire une copie de la cellule et continuer 
 * recursivement sur le la cellule suivante, créant ainsi 
 * une sous-liste dont la copie de cette cellule sera le premier élément.
 *
 * @param self_ La cellule à cloner.
 * @return La copie de la cellule.
 */
static void* ListCell_clone(const void* self_);
    
/**
 * Fonction qui va afficher le contenue du noeud et qui va continuer 
 * recursivement l'affichage jusqu'au dernier élément de la liste.
 * 
 * @param self_ La cellule à afficher.
 */
static void ListCell_print(const void* self_);

/*****************************************************/
/************* Structure de la classe ****************/

/**
 * @brief Class `ListCell` des cellules d'une liste doublement 
 * chainée.
 *
 * Construction : obj_new ( \_ListCell\_, void* value )
 */
typedef struct _ListCell_{
    Class _ListCell_;
    LISTCELL_FIELDS
}* ListCell;

static struct _Class_ __ListCell__ = {
    NULL,
    LISTCELL_CLASS,
    sizeof(struct _ListCell_),
    ListCell_constructor,
    ListCell_destroy,
    0,
    0,
    ListCell_clone,
    ListCell_print
};


/**
 * Classe de tous les objets de type `_ListCell_`.
 */
const Class _ListCell_ = & __ListCell__;

/***************************************************/
/************** Corps des fonctions ****************/

// Constructeur.
static void* ListCell_constructor(void* self_, va_list* args){
    
    // On vérifie si c'est pas NULL.
    assert(self_ && *args);
    // Conversion en ListCell
    ListCell self = (ListCell) self_;
    
    // Récupération de la valeur à donner
    void* value = va_arg(*args, void*);
    
    // Mise à jour de la valeur.
    self->value = value;
    
    // Le reste va rester à NULL
    
    // On retourne self.
    return self;

}

// Destructeur
static void* ListCell_destroy(void* self_){
        
    assert(self_ && obj_instance_of(self_, _ListCell_));
    // Conversion en ListCell.
    ListCell self = (ListCell) self_;
    
    if (self->next) // Si le suivant n'est pas NULL
        obj_destroy(self->next); // Alors on détruit le suivant.
    
    // On désalloue ensuite la valeur.
    obj_destroy(self->value);

    // Le précédent sera désalloué après cet appel alors on ne fait rien pour lui ici
    
    // On return self_ pour qu'il soit désalloué à son tour.
    return self_;

}

// Clone
static void* ListCell_clone(const void* self_){
    
    assert(self_ && obj_instance_of(self_, _ListCell_));
    // Conversion en ListCell.
    ListCell self = (ListCell) self_;
    
    // Le clone de la cellule suivante.
    ListCell next_clone = NULL;

    if (self->next) // Si le suivant n'est pas NULL
        next_clone = (ListCell) obj_clone(self->next); // Alors on clone le suivant.
    
    // On crée un clone à partir du clone de la valeur de cette cellule.
    ListCell clone = (ListCell) obj_new(_ListCell_, obj_clone(self->value));
    clone->next = next_clone; // On met à jour le suivant du clone.
    if (next_clone) // Si next_clone n'est pas NULL
        next_clone->previous = clone; // On met à jour le précédent du clone du suivant.
    
    // Le précédent de cette cellule sera cloné après cet appel ou laissé à NULL 
    // alors on ne fait rien pour lui ici
    
    // On return le clone.
    return clone;
    
}

//Print
static void ListCell_print(const void* self_){
   
    //Vérfication de self
    assert(self_); 
    // Vérification de l'appartenance de self à la classe ListCell
    assert(obj_instance_of(self_, _ListCell_));
    
    ListCell self = (ListCell) self_;

    // Affichage de la valeur.
    obj_print(self->value);
    
    // Si il y a un suivant alors on ajoute un virgule et on avance.
    if (self->next){
        write(1, ", ", 2);
        ListCell_print(self->next);
    }
    

}


/**
 * @internal
 * Fonction qui ajouter un object à une position donnée.
 *
 * @param self La cellule où la recherche de la bonne position va débuter.
 * @param object L'objet à ajouter dans la liste.
 * @param index la position où l'on doit ajoute l'objet.
 * @return La dernière cellule de la chaine.
 */
int ListCell_add(ListCell self, void* object, int index){

    // Vérification de self.
    assert(self);
    assert(index >=0);

    if (index == 0){
        
        // On ajoute la nouvelle cellule.
        ListCell new_cell = obj_new(_ListCell_, object);
        new_cell->previous=self->previous;
        new_cell->next = self;
        
        // Le precédent n'est jamais NULL mais on ne sait jamais.
        if (self->previous != NULL)
            self->previous->next = new_cell;

        self->previous = new_cell;
        
        return 1;
    }

    // Le suivant n'est jamais NULL (car index n'aurait pas passé le test dans LinkedList_add()) mais on ne sait jamais.
    if (self->next == NULL)
        return 1;
    
    else
        return ListCell_add(self->next, object, index-1);

}


/**
 * @internal
 * @brief Fonction qui va retirer un object à une position donnée.
 * 
 * @param self La cellule où la recherche de la bonne position àr retirer va débuter.
 * @param index la position où l'on doit ajoute l'objet.
 * @param clone Booléen dont 1 permet d'indiquer à la fonction qu'on souhaite qu'elle nous renvoie 
 * une copie de l'objet après suppression. Pour rappel, l'objet sera détruit après sa suppression 
 * de la liste.
 * @return La dernière cellule de la chaine.
 */
void* ListCell_remove(ListCell self, int index, int clone){

    
    // Vérification de self.
    assert(self);
    assert(index >=0);

    if (index == 0){
            
    
        self->previous->next = self->next;
        // Le precédent n'est jamais NULL (car index n'aurait pas passé le test dans LinkedList_add()) mais on ne sait jamais.
        if (self->next != NULL)
            self->next->previous = self->previous;
        
        self->previous = NULL;
        self->next = NULL;
        void* ret_val = (clone) ? obj_clone(self->value) : NULL;
        obj_destroy(self);
        return ret_val;
    }

    // Le suivant n'est jamais NULL (car index n'aurait pas passé le test dans LinkedList_add()) mais on ne sait jamais.
    if (self->next == NULL)
        return NULL;
    
    else
        return ListCell_remove(self->next, index-1, clone);

}


/**
 * @internal
 * @brief Fonction qui va rechercher une valeur à l'aide de l'indice de la cellule 
 * qui la contient. 
 *
 * @param self_ La cellule où la recherche va débuter.
 * @param clone Booléen dont 1 permet d'indiquer à la fonction qu'on souhaite qu'elle renvoie 
 * une copie de l'objet après au lieux de la vraie référence.
 * 
 * @return La dernière cellule de la chaine.
 */
void* ListCell_get(const ListCell self, int index, int clone){

    // Vérification de self.
    assert(self);
    assert(index >=0);

    if (index == 0){

        void* ret_val = (clone) ? obj_clone(self->value) : self->value;
        return ret_val;

    }

    // Le suivant n'est jamais NULL (car index n'aurait pas passé le test dans LinkedList_add()) mais on ne sait jamais.
    if (self->next == NULL){
        return NULL;
    }
    
    else
        return ListCell_get(self->next, index-1, clone);

}


/**
 * @internal
 *
 * Fonction qui renvoie la dernière cellule de la chaine.
 */
ListCell ListCell_get_last(const ListCell self_){
    
    // Vérification si y a pas de NULL
    assert(self_);
    assert(obj_instance_of(self_, _ListCell_));

    // Si le suivant n'est pas NULL
    if( self_->next )
        // Appel recursif sur le suivant.
        return ListCell_get_last(self_->next);

    else 
        return self_; // Sinon on retourn self_

}

/**
 * @internal 
 * Fonction qui renvoie la cellule précédant `self`
 */
ListCell ListCell_get_previous(const ListCell self){

    assert(self && obj_instance_of(self, _ListCell_));
    return self->previous;

}

/**
 * @internal 
 * Fonction qui renvoie la valeur contenue dans la cellule `self`
 */
void* ListCell_get_value(const ListCell self){

    assert(obj_instance_of(self, _ListCell_));
    return self->value;

}

/**
 * @internal
 * Fonction qui renvoie la cellule suivant `self`
 */
ListCell ListCell_get_next(const ListCell self){

    assert(self && obj_instance_of(self, _ListCell_));
    return self->next;

}

/**
 * @internal
 * Fonction qui change la cellule précédant self.
 */
void* ListCell_replace_previous(const ListCell self, const ListCell new_prev, int clone){

    assert(self && obj_instance_of(self, _ListCell_));
    if(new_prev) // Si le précédent n'est pas NULL
        assert(obj_instance_of(new_prev, _ListCell_)); // On vérifie si c'est un ListCell
                                                                      
    ListCell prev = self->previous;
    void* self_prev_value = NULL; // La valeur qui se trouve dans la cellule prev (NULL pour l'instant)

    if (prev){ // Si prev n'était pas NULL

        if (prev->previous) // Si son précendent n'est pas NULL
            prev->previous->next = new_prev; // On met à jour son suivant.
        
        if (new_prev) // Si new prev n'est pas null.
            new_prev->previous = prev->previous; // Mise à jour du précédent du nouveau précédent. (peut être NULL ce n'est pas grave)

        if (clone) // Si la fonction doit faire une copie.
            self_prev_value = obj_clone(prev->value);

        prev->next = NULL; // On le coupe du reste de la liste.
        prev->previous = NULL; 
        obj_destroy(prev); // Et on le détruit.

    }
    // Sinon le nouveau précédent garde son ancien précédent

    // On change le précdent de self.
    self->previous = new_prev; 
    
    if (new_prev) // Si new prev n'est pas null.
        new_prev->next = self;

    return self_prev_value;

}

/**
 * @internal
 * Fonction qui remplace la cellule suivant `self`
 */
void* ListCell_replace_next(const ListCell self, const ListCell new_next, int clone){

    assert(self && obj_instance_of(self, _ListCell_));
    
    if(new_next) // Si le suivant n'est pas NULL
        assert(obj_instance_of(new_next, _ListCell_)); // On vérifie si c'est un ListCell
                                
    ListCell next = self->next; // On récupère le suivant actuelle de self.
    void* self_next_value = NULL; // La valeur qui se trouve dans la cellule next (NULL pour l'instant)

    if (next){ // Si next n'était pas NULL

        if (next->next) // Si son suivant n'est pas NULL
            next->next->previous = new_next; // On met à jour son précédent.

        if (new_next)// Si new_next n'est pas null.
            new_next->next = next->next; // Mise à jour du suivant du nouveau suivant. (peut être NULL ce n'est pas grave)

        if (clone) // Si la fonction doit faire une copie.
            self_next_value = obj_clone(next->value);

        next->next = NULL; // On le coupe du reste de la liste.
        next->previous = NULL; 
        obj_destroy(next); // Et on le détruit.

    }
    // Sinon le nouveau précédent garde son ancien précédent

    // On change le next de self.
    self->next = new_next; 
    
    if (new_next)// Si new_next n'est pas null.
        new_next->previous = self;

    return self_next_value;
}



    
/** 
 * @internal 
 * @brief Fonction qui modifie la cellule qui précède `self`.
 * 
 * **ATTENTION** Risque de fuite de mémoire, rien ne sera désalloué. 
 * La fonction va seulement remplacer la référence à la cellule précédente 
 * par la nouvelle. 
 * 
 * @param self La cellule dont on veut changer celle qui la suit.
 * @param new_prev La nouvelle cellule suivant `self`.
 * @return L'ancienne cellule précédante de `self`.
 */
ListCell ListCell_set_previous(const ListCell self, const ListCell new_prev){

    assert(self && obj_instance_of(self, _ListCell_));
    if(new_prev) // Si le précédent n'est pas NULL
        assert(obj_instance_of(new_prev, _ListCell_)); // On vérifie si c'est un ListCell
                                                                      
    ListCell prev = self->previous;
    self->previous = new_prev;

    return prev;
}

/**
 * @internal
 * @brief Fonction qui modifie la cellule qui suit `self`.
 * 
 * **ATTENTION** Risque de fuite de mémoire, rien ne sera désalloué.
 * La fonction va seulement remplacer la référence à la cellule suivante 
 * par la nouvelle. 
 * 
 * @param self La cellule dont on veut changer celle qui la précède.
 * @param new_prev La nouvelle cellule précédant `self`.
 * @return L'ancienne cellule suivante de `self`.
 */
ListCell ListCell_set_next(const ListCell self, const ListCell new_next){

    assert(self && obj_instance_of(self, _ListCell_));
    
    if(new_next) // Si le suivant n'est pas NULL
        assert(obj_instance_of(new_next, _ListCell_)); // On vérifie si c'est un ListCell

    ListCell next = self->previous;
    self->next = new_next;

    return next;
}

/**
 * @internal
 * @brief Fonction qui compare la valeur contenue dans la cellule avec `value`.
 * 
 * La fonction va comparer les deux valeurs avec la méthode `obj_equals()`.
 * Si les deux valeurs sont différentes alors la fonction continue sur la cellule suivante.
 * 
 * @param self La cellule courante.
 * @param value La valeur à comparer.
 * @return 0 Si aucune cellule de la chaine ne contient `value`
 */
int ListCell_contains(ListCell self, void* value){

    assert(self && obj_instance_of(self, _ListCell_));

    // Si la valeur de la cellule est l'objet sont les mêmes.
    if (obj_equals(self->value, value))
        return 1; // On renvoie vrai.
    
    if (self->next) // Si la cellule admet un suivant.
        return ListCell_contains(self->next, value); // On demande au suivant.

    return 0; // Sinon on renvoie faux.

}



/**
 * @internal
 * @brief Fonction qui renvoie l'indice de la première occurence d'un élément 
 * qui se trouve dans la liste à partir de la cellule courrante.
 * 
 * La fonction va comparer les deux valeurs avec la méthode `obj_equals()`.
 * Si les deux valeurs sont différentes alors la fonction continue sur la cellule suivante.
 * 
 * @param self La cellule courante.
 * @param object L'élément à rechercher.
 * @param index L'indice de la cellule actuelle.
 * @return L'indice dans la liste de l'élément. -1 Si l'élément n'est pas dans la liste.
 * @see obj_equals()
 */
long long int ListCell_index_of( ListCell self, void* value, long long int index ){

    assert(self && obj_instance_of(self, _ListCell_));

    // Si la valeur de la cellule est l'objet sont les mêmes.
    if (obj_equals(self->value, value))
        return index; // On renvoie l'indice.
    
    
    if (self->next) // Si la cellule admet un suivant.
        return ListCell_index_of(self->next, value, index + 1); // On demande au suivant.

    return -1;

}















