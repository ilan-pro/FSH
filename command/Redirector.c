/**
 * @file Redirector.c
 * @brief 
 * 
 */


/** @cond HIDE_THIS*/
#include <unistd.h>
#define REDIRECTOR_CLASS "Redirector"
#include "Redirector.h"
#include "../base/String.h"
#include "../base/Integer.h"
#include <fcntl.h>
/**@endcond*/


/*****************************************************/
/************ Déclaration des fonctions **************/

/**
 * Constructeur d'un objet Redirector.
 * 
 * @param self_ L'objet alloué par calloc().
 * @param args La liste des arguements (Nomalement elle doit être vide).
 * @return Un objet de type Redirector.
 */
static void* Redirector_constructor(void* self_, va_list* args);

/**
 * Destructeur d'un objet de type commande Redirector.
 *
 * Le destructeur va juste appeller le destructeur du parent, 
 * qui va désallouer les ressources qui sont héritées.
 * 
 * @param self_ L'objet à désallouer.
 * @return Un objet dont tout l'intértieur desalloué.
 */
static void* Redirector_destroy(void* self_);

/**
 * Fonction qui va afficher les informations sur l'objet de 
 * redirection.
 *
 * @param self_ L'objet à désallouer.
 */
static void Redirector_print(const void* self_);


/*****************************************************/
/************* Structure de la classe ****************/


/**
 * @brief Classe `Redirector` 
 */
typedef struct _Redirector_ {
    
    Class _Redirector_;
    REDIRECTOR_FIELDS

}* Redirector;

static struct _Class_ __Redirector__ = {
    
    NULL,
    REDIRECTOR_CLASS,
    sizeof(struct _Redirector_),
    Redirector_constructor,
    Redirector_destroy,
    0,
    0,
    0,
    Redirector_print
};

/**
 */
const Class _Redirector_ = &__Redirector__;

/***************************************************/
/************** Corps des fonctions ****************/


static void print_syntax_err( const char* msg ){

    char tmp[] = "\e[1;31m[Erreur] Redirection : ";
    write(2,tmp, strlen(tmp));
    write(2, msg, strlen(msg));
    write(2, "\n\e[1;0m", 7);

}

/**
 * Fonction qui vide la liste de descripteurs et les ferme. 
 *
 * @param descriptors_list La liste des descripteurs à fermer.
 */
 static void close_descriptor( ArrayList descriptors_list ){

    for (int i = 0; i<ArrayList_size(descriptors_list); i++) // Fermeture de tous les descripteurs
        close(((Integer)ArrayList_get(descriptors_list, i, 0))->val);
    // On vide la liste des descripteurs
    ArrayList_clear(descriptors_list); 

}

// Constructeur.
static void* Redirector_constructor(void* self_, va_list* args){
    
    // Vérification des arguments.
    assert(self_ && args);    
    
    // Conversion en objet de type Redirector.
    Redirector self = (Redirector) self_;
    
    /**
     * Création des listes de descripteurs.
     */
    self->err_descriptors = obj_new(_ArrayList_, 0);
    self->out_descriptors = obj_new(_ArrayList_, 0);
    self->in_descriptors = obj_new(_ArrayList_, 0);
        

    // Activation de la redirection multiple.
    self->red_err = 1;
    self->red_out = 1;
    self->red_in = 1;

    return self;
}

// Destructeur.
static void* Redirector_destroy(void* self_){
    
    // Vérification des arguments.
    assert(self_);    
    assert(obj_instance_of(self_, _Redirector_));
    
    // Conversion en objet de type Redirector.
    Redirector self = (Redirector) self_;
    
    // Fermeture des descripteurs ouverts
    close_descriptor(self->err_descriptors);
    close_descriptor(self->in_descriptors);
    close_descriptor(self->out_descriptors);

    // Destruction des liste des descripteurs.
    obj_destroy(self->err_descriptors);
    obj_destroy(self->out_descriptors);
    obj_destroy(self->in_descriptors);

    return self;
}   

/**
 * @brief Fonction qui renvoie quelle sortie ou entrée est redirigée.
 *
 *
 * S'il ne s'agit pas de l'entrée standard la fonction va regarder le nombre 
 * qui précède le '>' si ce nombre est 1 alors il s'agit de la sortie standard, 
 * s'il vaut 2 alors c'est la sortie erreur, sinon par défaut ce sera la sortie 
 * standard.
 *
 * @param prompt le prompt.
 * @return Le descripteur de la sortie.
 */
static int get_wich_std( char ** prompt ){
    
    // Si c'est l'entrée standard.
    if (**prompt == '<')
        return 0;
    
    // Récupération du caractère précédent.
    char c = *((*prompt)-1);
    
    switch (c) {
        case '1':
        default :
            return 1;
        case '2':
            return 2;
    }

}

/**
 * Fonction qui renvoie le fichier dans lequel la 
 * sortie ou l'entrée est redirigée.
 *
 * @param prompt Le prompt sur lequel la fonction va travailler.
 * @return Le fichier dans lequel la sortie ou 
 * l'entrée est redirigée.
 */
static String get_file(char** prompt){

    // Création d'une ArrayList temporaire.
    ArrayList tmp = obj_new(_ArrayList_, 0);
        
    // Ajout du nom du fichier (la prochaine sous chaine délimité par des ' ').
    if (!Command_parse_all_sub(tmp, prompt, 1, NULL))
        return NULL;
  
    // Vérification de si on a bien une seule chaine de caractère.
    assert(ArrayList_size(tmp) == 1);
    
    // Suppression du dernier élément de la liste
    String s = ArrayList_remove_last(tmp, 0);
    
    // Destruction de la liste.
    obj_destroy(tmp);

    return s;
}

/**
 * Fonction qui va rediriger la sortie vers un fichier.
 *
 * @param self L'objet `Redirector` s'occupant des redirections.
 * @param prompt Le prompt de l'utilisateur.
 * @param fd Le descripteur de la sortie à rediriger (permet de savoir si c'est std_out ou std_err).
 * @param overwrite Booléen indiquant si oui ou non le fichier doit être écrasé.
 * @param mult_redirect Booléen indiquant si oui ou non une redirection multiple est spécifié 
 * (si non alors la liste des redirection est vidée seul ce dernier fichier contiendera la sortie).
 * @return 1 Si la redirection est enregistrée, 0 sinon.
 */
static int out_redirection(Redirector self, char** prompt, int fd, int overwrite, int mult_redirect, int append){
    
    // Incrémentation du pointeur (caractère suivant).
    (*prompt)++;
    printf("Test '%c'\n", **prompt);
    // Si on est à la fin alors il y a une erreur de syntaxe.
    if (!(**prompt)){
        print_syntax_err("Fichier non renseigné !");
        assert(0);
        return 0;
    }
    
    // Récupération du caractère pointé.
    char c =  **prompt;

    switch (c){
        case '|' : // écrasement
            return out_redirection(self, prompt, fd, 1, mult_redirect, 0);
        case '!' : // activation multiple redirection
            return out_redirection(self, prompt, fd, overwrite, 1, append);
        case '>' : // Ajout à la fin (Append)
            printf("??\n");
            return out_redirection(self, prompt, fd, 0, mult_redirect, 1);
        default : // Maintenant on redirige
            {
                // On active O_TRUNC si la valeur `overwrite` est activé sinon 0 (ne change rien dans le | ).
                int trunc = (overwrite ? O_TRUNC : 0);

                // On active O_EXCL si la valeur `overwrite` n'est pas activée sinon 0. (ne change rien dans le | ).
                int excl = (overwrite ? 0 : O_EXCL);

                // On active O_APPEND si la valeur `append` n'est pas activée sinon 0. (ne change rien dans le | ).
                int apnd = (append ? 0 : O_APPEND);
                
                // Récupération du nom du fichier.
                String file = get_file(prompt);
                
                printf("Trunc ? %s. Excl ? %s. Append ? %s. \n", (trunc ? "Oui" : "Non"), (excl ? "Oui" : "Non"), (append ? "Oui" : "Non")); 

                // On peut vérifier ici si le fichier n'est pas une variable (si le temps le permet)
                
                /////////////////////////////////////////////////////////////////////////////////////

                // Ouverture du fichier
                int file_d = open(obj_to_string(file), O_CREAT | trunc | excl | apnd | O_WRONLY, 0666);
                            
                // Destruction du nom du fichier
                obj_destroy(file);

                // Vérification
                if (file_d == -1) // Si l'ouverture a échouée on quitte 
                    return 0;  /// S'il y a des chose à ajouter avant de quitter c'est ici !
                
                // Ajout de la sortie
                if (fd == 2){
                    
                    // Si la redirection multiple n'est pas activée alors on desactive la redirection multiple définitivement.
                    if (!mult_redirect)
                        self->red_err = 0;

                    if (self->red_err) // Si la redirection multiple est activée pour la commande
                        ArrayList_add_last(self->err_descriptors, obj_new(_Integer_, file_d), 0); // On ajoute seulement à la fin.
                    else { // Sinon
                        close_descriptor(self->err_descriptors); // Fermeture des descripteur et supresion des éléments de la liste.
                        ArrayList_add_last(self->err_descriptors, obj_new(_Integer_, file_d), 0); // Ajout du descripteur actuel.
                    }
                }
                else{

                    // Si la redirection multiple n'est pas activée alors on desactive la redirection multiple définitivement.
                    if (!mult_redirect)
                        self->red_out = 0;

                    if (self->red_out) // Si la redirection multiple est activée pour la commande
                        ArrayList_add_last(self->out_descriptors, obj_new(_Integer_, file_d), 0); // On ajoute seulement à la fin.
                    else { // Sinon
                        printf("ça ? %d\n", file_d);
                        close_descriptor(self->out_descriptors); // Fermeture des descripteur et supresion des éléments de la liste.
                        ArrayList_add_last(self->out_descriptors, obj_new(_Integer_, file_d), 0); // Ajout du descripteur actuel.
                    }
                }

            }
            break;

    }

    return 1;

}



/**
 * Fonction qui va rediriger l'entrée sur un fichier.
 *
 * @param self L'objet `Redirector` s'occupant des redirections.
 * @param prompt Le prompt de l'utilisateur.
 * @param mult_redirect Booléen indiquant si oui ou non une redirection multiple est spécifié 
 * (si non alors la liste des redirection est vidée seul ce dernier fichier contiendera la sortie).
 * @return 1 Si la redirection est enregistrée, 0 sinon.
 */
static int in_redirection(Redirector self, char** prompt, int mult_redirect ){
    
    // Incrémentation du pointeur (caractère suivant).
    (*prompt)++;
    
    // Si on est à la fin alors il y a une erreur de syntaxe.
    if (!(**prompt)){
        print_syntax_err("Fichier non renseigné !");
        return 0;
    }
    
    // Récupération du caractère pointé.
    char c =  **prompt;

    switch (c){
        case '<': // Here-Document mode (si le temps le permet)
            return in_redirection(self, prompt, mult_redirect);

        default : // Maintenant on redirige
            {
                // Récupération du nom du fichier.
                String file = get_file(prompt);
                if (!file) // Si la fonction a échoué (renvoie NULL)
                    return 0; // On quitte.

                // On peut vérifier ici si le fichier n'est pas une variable (si le temps le permet)

                /////////////////////////////////////////////////////////////////////////////////////

                // Ouverture du fichier
                int file_d = open(obj_to_string(file), O_RDONLY);
                            
                // Destruction du nom du fichier
                obj_destroy(file);
                            
                // Vérification
                if (file_d == -1) // Si l'ouverture a échouée on quitte 
                    return 0;  /// S'il y a des chose à ajouter avant de quitter c'est ici !
                
                
                // Si la redirection multiple n'est pas activée alors on desactive la redirection multiple définitivement.
                if (!mult_redirect)
                    self->red_in = 0;

                if (self->red_in) // Si la redirection multiple est activée pour la commande
                    ArrayList_add_last(self->in_descriptors, obj_new(_Integer_, file_d), 0); // On ajoute seulement à la fin.
                else { // Sinon
                    close_descriptor(self->in_descriptors); // Fermeture des descripteur et supresion des éléments de la liste.
                    ArrayList_add_last(self->in_descriptors, obj_new(_Integer_, file_d), 0); // Ajout du descripteur actuel.
                }
                    
                break;

            }

    }

    return 1;
    
}

/**
 * @internal
 * @brief Fonction qui va récupérer les fichiers dans lesquels on souhaite faire une redirection 
 * et va les mettre dans la liste des descripteur de redirection.
 *
 * Le Shell `FSH` est conçu pour permettre de faire des redirections multiples, 
 * c'est à dire que l'on peut rediriger la sortie d'une commande dans plusieurs fichiers, 
 * en plus de la sortie standard (seulement si on le spécifie avec `!`, 
 * Ex : `ls >! achirafi` va rediriger la commande vers le fichier 
 * `./achirafi` en plus de l'afficher sur la sortie standard).
 * <p>
 * Si l'un des fichiers (au moins) n'a pas '!' dans la redirection alors, la sortie standard sera 
 * redirigée vers le dernier fichier de redirection déclaré. <br>Exemple : `ls >!a >!b >c >!d`. 
 * L'abscence de '!' dans la redirection implique que stdout sera redirigé uniquement dans le 
 * fichier './d'.
 * <p>
 * Cela concerne également `stdin` et `stderr`.
 *
 * @param self L'objet `Redirector` s'occupant des redirections.
 * @param prompt Le prompt de l'utilisateur.
 * @return 1 Si la redirection est enregistrée, 0 sinon.
 */
int Redirector_add_redirection( Redirector self, char** prompt ){
    
    // Vérifications
    assert( self );
    assert( obj_instance_of(self, _Redirector_) );

    // Entier qui va contenir le descripteur à rediriger.
    int std_fd = get_wich_std(prompt);

    if (std_fd) // S'il s'agit d'une sortie.
        return out_redirection(self, prompt, std_fd, 0, 0, 0);
    else{
        int mult_redirect = 0;
        if (*((*prompt)-1)=='!')
            mult_redirect = 1;
        return in_redirection(self, prompt, mult_redirect);
    }
    
}


/**
 * @brief Fonction qui rediriger les entrée/sortie de la commande avant son exécution.
 * 
 * 
 * @param self L'objet `Redirector` s'occupant des redirections.
 */
void Redirector_redirect( Redirector self ){

    // Vérifications
    assert( self );
    assert( obj_instance_of(self, _Redirector_) );

    // Pour l'instant, pas d'implémentation.

    return ;    

}

static void Redirector_print( const void* self_ ){
    
    // Vérifications
    assert( self_ );
    assert( obj_instance_of(self_, _Redirector_) );
    
    // Conversion en objet de type redirector.
    Redirector self = (Redirector) self_;

    write(1, " In:", 4);
    obj_print(self->in_descriptors);

    write(1, "| Out:", 6);
    obj_print(self->out_descriptors);

    write(1, "| Err:", 6);
    obj_print(self->err_descriptors);

}








