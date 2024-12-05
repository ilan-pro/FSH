#include <unistd.h>


/**@cond HIDE_THIS*/
#define IF_CLASS "If"
#include "If.h"
/**@endcond*/


/*****************************************************/
/************ Déclaration des fonctions **************/

/**
 * Constructeur des objets de type `_If_`.
 *
 * Construction <code><strong> obj_new( `_If_`, char** prompt ); </strong></code>
 * où : <ul>
 *          <li>`_If_` désigne de la classe des objets `Command` de type `If`. </li>
 *          <li><code><strong>char** prompt</strong></code> La suite du prompt (qui permet 
 *          d'initialiser le reste de la commande).</li>
 * </ul>
 */
static void* If_constructor( void*, va_list* );

/**
 * Fonction qui va détruire un objet de type commande.
 */
static void* If_destructor( void* );
    
/**
 * Fonction qui va afficher la commande sur 
 * la sortie standard.
 */
static void If_print( const void* );

/**
 * Fonction qui va exécuter la commande.
 */
static int If_execute( Command );

/*****************************************************/
/************* Structure de la classe ****************/

/**
* Structure de la classe `_If_`.
*
* Construction <code><strong> obj_new( `_If_`, char** prompt ); </strong></code>
* où : <ul>
*          <li>`_If_` désigne de la classe des objets `Command` de type `If`. </li>
*          <li><code><strong>char** prompt</strong></code> La suite du prompt (qui permet 
*          d'initialiser le reste de la commande).</li>
* </ul>
*/
typedef struct _If_ {
    /**
     * Classe des objets de type `_If_`.
     *
     * Construction <code><strong> obj_new( `_If_`, char** prompt ); </strong></code>
     * où : <ul>
     *          <li>`_If_` désigne de la classe des objets `Command` de type `If`. </li>
     *          <li><code><strong>char** prompt</strong></code> La suite du prompt (qui permet 
     *          d'initialiser le reste de la commande).</li>
     * </ul>
     */
    Class _If_;
    IF_FIELDS
}* If;

static struct _Class_ __If__ = {
    NULL,
    IF_CLASS,
    sizeof(struct _If_),
    If_constructor,
    If_destructor,
    0,
    0,
    0,
    If_print
};

/**
 * Classe des objets de type `_If_`.
 *
 * Construction <code><strong> obj_new( `_If_`, char** prompt ); </strong></code>
 * où : <ul>
 *          <li>`_If_` désigne de la classe des objets `Command` de type `If`. </li>
 *          <li><code><strong>char** prompt</strong></code> La suite du prompt (qui permet 
 *          d'initialiser le reste de la commande).</li>
 * </ul>
 */
Class _If_ = &__If__;

/***************************************************/
/************** Corps des fonctions ****************/

/**
 * Constructeur d'un objet de type `If`.
 */
static void* If_constructor( void* self_, va_list* args ){
    
    // Vérifications
    assert( self_ );
    assert( args );
    
    // Conversion en objet de type If
    If self = (If) self_;
    
    printf("Size : %lu\n", self->_If_->size);

    // Ajout du parent
    *(Class*)& (self->_If_->_super_) = _Command_;     
    // Ajout de la méthode Exécute.
    *( int (**) (Command) )& (self->execute) = If_execute;
   
    self->args = obj_new(_ArrayList_, 0);
    self->args_tmp = obj_new(_ArrayList_, 0);
    self->params = obj_new(_ArrayList_, 0);



    return self;
}

/**
 * Destructeur d'un objet de type `_If_`
 */
static void* If_destructor( void* self_ ){
        
    // Assertion
    assert( self_ );

    // Conversion en objet de type `If`
    If self = ( If ) self_;

    // Les éléments hérités de command ne sont pas initialisés.
    

    return self->_If_->_super_->dtor(self);

}

static void If_print( const void* self_ ){
    
    // Assertion
    assert( self_ );

    // Conversion en objet de type `If`
    If self = ( If ) self_;
    
    write( 1, "If : ", 5 );
    write( 1, "TestCmd=", 8 );
    obj_print( self->test_command );
    write( 1, ". IfBlock=", 10 );
    obj_print( self->if_block );
    write( 1, ". ElseBlock=", 12 );
    obj_print(self->else_block);

}


/**
 * Fonction qui va exécuter la commande `_If_`.
 */
static int If_execute( Command self_ ){
    
    // Assertion
    assert( self_ );
        
    // Conversion en objet de type If
    If self  = (If) self_;

    ///// Execute ici !
    
    return 0; 
}




