/**
 * @file If.c
 * @brief Fichier contenant l'implémentation de la classe `_If_`.
 *
 * La classe `_If_` est une classe de commande permettant de contrôler l'exécution 
 * d'un bloc de commandes en l'exécutant sous une certaine condition (un ensemble de commande Test).
 * 
 * Construction <code><strong> obj_new( `_If_`, char** prompt ); </strong></code>
 * où : <ul>
 *          <li>`_If_` désigne de la classe des objets `Command` de type `If`. </li>
 *          <li><code><strong>char** prompt</strong></code> La suite du prompt (qui permet 
 *          d'initialiser le reste de la commande).</li>
 * </ul>
 *
 */

#include "../shell.h"

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
 * @cond HIDE_THIS
 * @internal
 * @brief Structure de la classe `_If_`.
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
     * @brief Classe des objets de type `_If_`.
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
/**@endcond*/

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
 * @internal
 * @brief Classe des objets de type `_If_`.
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

static void If_print_syntax_err( const char* msg ){

    char tmp[] = "\e[1;31m[Erreur] Instruction `If` mal écrite : ";
    write(2,tmp, strlen(tmp));
    write(2, msg, strlen(msg));
    write(2, "\n\e[1;0m", 7);

}

static int add_test_block( If self, char** prompt ){
    
    //Parsing du TestCmd
    if (!(self->test_command = Shell_parse(prompt))) // Si la création du bloc a échouée on renvoie NULL
        return 0;
    
    // Si aucun bloc de commande n'est lu.
    if (ArrayList_size( self->test_command ) == 0){
        If_print_syntax_err("Bloc de test abscent !");
        return 0;
    }
    // Si le prochain caractère n'est pas pas `{`
    // alors c'est la fin du prompt qui a arrêté `Shell_parse()`
    if (**prompt != '{'){
        If_print_syntax_err("Bloc de commande '{...}' abscent");
        return 0;
    }
    (*prompt)++; // Avance d'un caractère.
    opened_brackets ++; // Augmentation du nombre d'accorlade ouverts.

    return 1;

}

static int add_if_block( If self, char** prompt ){
    
    // Récupération du bloc du if.
    if (!(self->if_block = Shell_parse(prompt))) // Si la création du bloc a échouée on renvoie NULL.
        return 0; 

    // Si le prochain caractère n'est pas pas `}`
    // alors c'est la fin du prompt qui a arrêté `Shell_parse()`
    if (**prompt != '}'){
        If_print_syntax_err("Accolade fermante abscente !");
        return 0;
    }
    (*prompt)++; // Avance d'un caractère.
    opened_brackets --; // Décrémentation du nombre d'accorlade ouverts.

    return 1;
}

/**
 * Constructeur d'un objet de type `If`.
 */
static void* If_constructor( void* self_, va_list* args ){
    
    /// Mise à jour de _Cd_ en tant que héritier de _Command_ (si ce n'est pas déjà le cas)
    if(!_If_->_super_)
        *(Class*)& (_If_->_super_) = _Command_;

    // Vérifications
    assert( self_ );
    assert( args );
    
    // Conversion en objet de type If
    If self = (If) self_;

    // Appel du constructeur de la classe Command ( parent )
    _Command_->ctor(self, args);   
    
    // Ajout de la méthode Exécute.
    *( int (**) (Command) )& (self->execute) = If_execute;

    // Récupération du prompt.
    char** prompt = va_arg(*args, char**);

    // Si l'ajout du bloc de test a échoué on renvoie NULL
    if (!add_test_block(self, prompt)){
        obj_destroy(self);
        return NULL;
    }

    // Si l'ajout du block d'instruction du 'if' a échoué
    if (!add_if_block(self, prompt)){
        obj_destroy(self);
        return NULL; // On revnoie NULL
    }
    

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
    obj_destroy( self->if_block );
    obj_destroy( self->test_command );
    obj_destroy( self->else_block );

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

    int ret_val = 0;

    // Exécution du bloc de test et récupération de la valeur de retour
    int ret = Shell_execute_all(self->test_command); 
    
    if (!ret)  // Si l'exécution du block test a réussi (valeur de retour 0)
        ret_val = Shell_execute_all(self->if_block);  // Alors on éxécute le bloc du if et on recupère sa vlaeur de retour. 
    else { // Sinon 
        if (self->else_block) // Si le bloc else exist .
            ret_val = Shell_execute_all(self->else_block); // On exécute le bloc du else.
    }

    return ret_val; 
}




