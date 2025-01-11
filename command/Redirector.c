/**
 * @file Redirector.c
 * @brief 
 * 
 */


/** @cond HIDE_THIS*/
#include "File.h"
#include <stdio.h>
#include <time.h>
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

// Constructeur.
static void* Redirector_constructor(void* self_, va_list* args){
    
    // Vérification des arguments.
    assert(self_ && args);    
    
    // Conversion en objet de type Redirector.
    Redirector self = (Redirector) self_;
    
    /**
     * Création des listes de descripteurs.
     */
    self->err_files = obj_new(_ArrayList_, 0);
    self->out_files = obj_new(_ArrayList_, 0);
    self->in_files = obj_new(_ArrayList_, 0);
        

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
    
    // Destruction des liste des descripteurs.
    obj_destroy(self->err_files);
    obj_destroy(self->out_files);
    obj_destroy(self->in_files);

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
    if (!Command_parse_all_sub(tmp, prompt, 1, NULL)){
        obj_destroy(tmp);
        return NULL;
    }
  
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
    /** printf("Test '%c'\n", **prompt); */
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
        case '!' : // Redirection exclusive
            (*prompt)++;
            if (fd == 2){ // Si c'est stderr
                if (!ArrayList_size(self->err_files)){ // Si aucun fichier n'est redirigé 
                    Command_fail(2);
                    print_syntax_err("Une redirection exclusive a été déclarée pour `stderr` mais aucun fichier n'est redirigé !\nIl faut au moins indiquer une redirection avant `>!`.");
                    return 0;
                }
                if (self->red_err == 0) // S'il n'y a pas de redirection multiple alors on ne fait rien
                    return 1; // On a fini.
                
                self->red_err = 2;
            }
            else{
                if (!ArrayList_size(self->out_files)){ // Si aucun fichier n'est redirigé 
                    (*prompt)++;
                    print_syntax_err("Une redirection exclusive a été déclarée pour `stdout` mais aucun fichier n'est redirigé !\nIl faut au moins indiquer une redirection avant `>!`.");
                    Command_fail(2);
                    return 0;
                }
                if (self->red_out == 0) // S'il n'y a pas de redirection multiple alors on ne fait rien
                    return 1; // On a fini.
                
                self->red_out = 2;
            }
            break;
                

        case '+' : // activation multiple redirection
            return out_redirection(self, prompt, fd, overwrite, 1, append);
        case '>' : // Ajout à la fin (Append)
            return out_redirection(self, prompt, fd, 0, mult_redirect, 1);
        default : // Maintenant on redirige
            {
                // On active O_TRUNC si la valeur `overwrite` est activé sinon 0 (ne change rien dans le | ).
                int trunc = (overwrite ? O_TRUNC : 0);

                // On active O_EXCL si la valeur `overwrite` n'est pas activée sinon 0. (ne change rien dans le | ).
                int excl = (overwrite || append ? 0 : O_EXCL);

                // On active O_APPEND si la valeur `append` n'est pas activée sinon 0. (ne change rien dans le | ).
                int apnd = (append ? O_APPEND : 0);
                
                // Récupération du nom du fichier.
                String file = get_file(prompt);
                
                /** printf("\nTrunc ? %s [%d]. Excl ? %s[%d]. Append ? %s[%d]. \nMult-Redir ? %s[out=%d, err=%d].\n\n",  */
                        /** (trunc ? "Oui" : "Non"),trunc, (excl ? "Oui" : "Non"), excl, (apnd ? "Oui" : "Non"),apnd, */
                        /** (mult_redirect ? "Oui" : "Non"), self->red_out, self->red_err); */

                // On peut vérifier ici si le fichier n'est pas une variable (si le temps le permet)
                
                /////////////////////////////////////////////////////////////////////////////////////

                // Ouverture du fichier (On ouvre en lecture et écriture car pour 
                // les redirections multiples on aura besoin de lire le fichier
                // où la sortie a été écrite pour copier vers tous les autres).
                
                // Le fichier n'est pas ouvert directement mais seulement à l'exécution.
                File new_out_file = obj_new(_File_);

                // Mise à jour des options d'ouverture.
                File_setOpenTag(new_out_file, O_CREAT | trunc | excl | apnd | O_RDWR);

                // Mise à jour du chemin vers le fichier.
                File_setPath(new_out_file, obj_to_string(file));
                            
                // Destruction du nom du fichier
                obj_destroy(file);

                // Ajout de la sortie
                if (fd == 2){
                    
                    // Si la redirection multiple n'est pas activée alors on desactive la redirection multiple définitivement.
                    if (!mult_redirect)
                        self->red_err = 0;

                    if (self->red_err) // Si la redirection multiple est activée pour la commande
                        ArrayList_add_last(self->err_files, new_out_file, 0); // On ajoute seulement à la fin.
                    else { // Sinon
                        ArrayList_clear(self->err_files);
                        ArrayList_add_last(self->err_files, new_out_file, 0); // On ajoute à la fin.
                    }
                }
                else{

                    // Si la redirection multiple n'est pas activée alors on desactive la redirection multiple définitivement.
                    if (!mult_redirect)
                        self->red_out = 0;

                    if (self->red_out) // Si la redirection multiple est activée pour la commande
                        ArrayList_add_first(self->out_files, new_out_file, 0); // On ajoute seulement à la fin.
                    else { // Sinon
                        ArrayList_clear(self->out_files);
                        ArrayList_add_first(self->out_files, new_out_file, 0); // On ajoute à la fin.
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
 * Pour `stdin` les documents sont lu dans l'ordre de leur insertion, 
 * c'est à dire que le dernier document n'est pas le premier lu comme pour les 
 * redirections de sortie où c'est le dernier document qui est écrit et 
 * copié sur tous les autres par la suite.
 *
 * Donc dans la commande `sort <+a <+b <+c <!` `a` est lu puis `b` ensuite `c` (et `stdin` 
 * n'est pas lu à cause de `<!`).
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
        Command_fail(1);
        return 0;
    }
    
    // Récupération du caractère pointé.
    char c =  **prompt;

    switch (c){
        case '!' : // Redirection exclusive
            (*prompt)++;
            if (!ArrayList_size(self->in_files)){ // Si aucun fichier n'est redirigé 
                Command_fail(2);
                print_syntax_err("Une redirection exclusive a été déclarée pour `stdin` mais aucun fichier n'est redirigé !\nIl faut au moins indiquer une redirection avant `<!`.");
                return 0;
            }
            if (self->red_in == 0) // S'il n'y a pas de redirection multiple alors on ne fait rien
                return 1; // On a fini.
            
            self->red_in = 2;
            break;

        case '+' : // activation multiple redirection
            return in_redirection(self, prompt, 1);
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

                File new_in_file = obj_new(_File_);

                // Mise à jour des options d'ouverture.
                File_setOpenTag(new_in_file, O_RDONLY);

                // Mise à jour du chemin vers le fichier.
                File_setPath(new_in_file, obj_to_string(file));

                // Destruction du nom du fichier
                obj_destroy(file);
                            
                
                // Si la redirection multiple n'est pas activée alors on desactive la redirection multiple définitivement.
                if (!mult_redirect)
                    self->red_in = 0;

                if (self->red_in) // Si la redirection multiple est activée pour la commande
                    ArrayList_add_last(self->in_files, new_in_file, 0); // On ajoute seulement à la fin.
                else { // Sinon
                    ArrayList_clear(self->in_files);
                    ArrayList_add_last(self->in_files, new_in_file, 0); // Ajout du descripteur actuel.
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
 * Fonction qui rediriger stdout sur le descripteur du premier fichier
 * de la liste des fichiers de redirection. 
 *
 * Si la reidrection multiple est activée, alors le shell fait une 
 * copie de stdout avant la redirection  vers le fichier.
 *
 * @param self L'objet de redirection.
 */
static int stdout_redirect( Redirector self ){

    // Si la liste des descripteurs est vide.
    if (ArrayList_size(self->out_files)==0)
        return 1; // Alors on a fini.

    // Si la redirection multiple est active.
    if (self->red_out)
        // On garde une copie de stdout.
        self->old_out = dup(STDOUT_FILENO);
          
    // Récupération du descripteur du fichier à rediriger.
    int file_d = File_getFd((File)ArrayList_get_first(self->out_files, 0), 0);

    // Fermeture de stdout et écriture dans le fichier.
    int fd = dup2( file_d, STDOUT_FILENO );
    if ( fd == -1 ){ // Si la redirection a échoueé.
        Command_fail(2);
        return 0;    
    }

    return 1;

}


/**
 * Fonction qui rediriger stderr sur le descripteur du premier fichier
 * de la liste des fichiers de redirection. 
 *
 * Si la reidrection multiple est activée, alors le shell fait une 
 * copie de stderr avant la redirection  vers le fichier.
 *
 * @param self L'objet de redirection.
 */
static int stderr_redirect( Redirector self ){

    // Si la liste des descripteurs est vide.
    if (ArrayList_size(self->err_files)==0)
        return 1; // Alors on a fini.

    // Si la redirection multiple est active.
    if (self->red_err)
        // On souvegarde une copie de stderr
        self->old_err = dup(STDERR_FILENO);
          
    // Récupération du descripteur du fichier à rediriger.
    int file_d = File_getFd((File)ArrayList_get_first(self->err_files, 0), 0);

    // Fermeture de stdout et écriture dans le fichier.
    int fd = dup2( file_d, STDERR_FILENO );
    if ( fd == -1 ){ // Si la redirection a échoueé.
        Command_fail(2);
        return 0;    
    }
        
    return 1;

}

/**
 * Il s'agit d'une fonction qui va écrire sur un tube annonyme 
 * en parallèle pendant que le processus va lire les données écrites 
 * par l'enfant.
 *
 * @param self L'objet de redirection qui se charge de la redirection 
 * de stdin.
 * @param _pipe_ le tube partagé par le père et le fils.
 */
static void write_on_pipe( Redirector self, int _pipe_[2] ){
    
    // Fermeture du tube coté lecture (seul le père lit)
    close(_pipe_[0]);

    // Buffer qui va contenir la lecture (assez grand pour ne pas ralentir).
    char buff[8196];

    // Parcours des descripteurs
    for (int i = 0; i<ArrayList_size(self->in_files); i++){ 
            
        // Récupération du descripteur du fichier.
        int fd = File_getFd((File)ArrayList_get(self->in_files, i, 0), 0);
        int nb;
        
        // Remise du curseur au début.
        int seek = lseek( fd, 0, SEEK_SET );
    
        // Vérification
        if (seek == -1) goto failure; 

        // Lecture du fichier et écriture dans le pipe.
        while( (nb=read(fd, buff, 8192)) > 0)
            // Ecriture dans le tube.
            write(_pipe_[1], buff, nb);
        
    }

    // Lecture de stdin 
    if (self->red_in == 1){ // Si la redirection mupltiple n'est pas exclusive.
        int nb;
        // Lecture de stdin et écriture dans le pipe.
        while( (nb=read(self->old_in, buff, 8192)) > 0)
            // Ecriture dans le tube.
            write(_pipe_[1], buff, nb);
    }

    // Fermeture du coté écrivain du tube.
    // pour permettre une terminaison de la lecture.
    close( _pipe_[1] );

    failure : 
    Command_fail(2);
    return;

}


/**
 * Fonction qui rediriger stderr sur le descripteur du premier fichier
 * de la liste des fichiers de redirection. 
 *
 * Si la reidrection multiple est activée, alors le shell va 
 * copier `stdin` ainsi que tous les fichiers dans un tube annonyme
 * dont le coté écriture sera fermé (Une fois que tous les données
 * lu dans `stdin`) pour permettre à la commande de lire le contenu 
 * en tant qu'entrée.
 *
 * @param self L'objet de redirection.
 * copie de stderr avant la redirection  vers le fichier.
 */
static int stdin_redirect( Redirector self ){

    // Si la liste des descripteurs est vide.
    if (ArrayList_size(self->in_files)==0)
        return 1; // Alors on a fini.

    // Si la redirection multiple est active.
    if (self->red_in)
        // On garde une copie de stdin.
        self->old_in = dup(STDIN_FILENO);
        
    else{ // Sinon on redirige stdout et on continue le programme
        // Récupération du descripteur du fichier à rediriger.
        int file_d = File_getFd((File)ArrayList_get_first(self->in_files, 0), 0);

        // Fermeture de stdin et lecture dans le fichier.
        int fd = dup2( file_d, STDIN_FILENO );
        if ( fd == -1 ) // Si la redirection a échoueé.
            goto failure; 

        return 1;
    }

    /**
     * Dans le code qui suit, un tube annonyme sera créé pour accueillir 
     * le contenu de tous les fichier qui doivent être lu.
     */
    // tableau des descripteurs d'entrée et sortie du tube.
    int _pipe_[2];
    
    // Création du tube
    pipe(_pipe_);

    /**
     * La taille des pipes étant limitée (65,536 octets d'après la manpage 
     * de Ubuntu), lorsqu'il y a un débordement le pipe attend alors qu'un 
     * lecteur lise et consome le contenue du tube. 
     *
     * Cela pose problème car étant donnée que l'exécution de se fait sur 
     * un seul processus, ce dernier va se bloquer en attente d'un lecteur.
     *
     * L'idée est donc de faire le replissage du pipe en parallèle, au 
     * moment d'un débordement, le programme va attendre que le processus 
     * principale lise l'entrée pour continuer (Et si le processus principale 
     * est en avance alors la lecture va attendre).
     */
    
    // Division
    pid_t pip = fork();
    
    switch (pip) {
    
        // fils
        case 0 :
            
            //Ecriture dans le pipe.
            write_on_pipe(self, _pipe_);
            exit(0); // On quitte le sous-programme.
        
        // Erreur
        case -1: 
            perror("fork");
            goto failure;
        
        // Père
        default :
    
            // Fermeture du coté écrivain du tube, 
            // seul le fils écrit dans le tube.
            close( _pipe_[1] );
            break;
    }

    // Redirection de stdin sur le tube.
    int dp = dup2( _pipe_[0], STDIN_FILENO );
    if (dp == -1)  goto failure; // En cas d'echec de redirection.

    //Fermeture du coté lecteur du tube (seul stdin maintenant est lecteur)
    close( _pipe_[0] );

    return 1;

    failure : 
    Command_fail(2);
    return 0;
}


/**
 * @internal
 * 
 * @brief Fonction qui rediriger les entrée/sortie de la commande avant son exécution.
 * 
 * @param self L'objet `Redirector` s'occupant des redirections.
 */
int Redirector_apply_redirection (Redirector self ){

    // Vérifications
    assert( self );
    assert( obj_instance_of(self, _Redirector_) );
            
    /**
     * Ouverture de tous les fichiers.
     */
  // Pour chaque file on l'ouvre.
    for (int i = 0; i<ArrayList_size(self->out_files); i++){
        int ret = File_open(ArrayList_get(self->out_files, i, 0)); // Ouverture du fichier

        if (!ret){ // Echec d'ouverture.
            perror("Pipeline_run");
            return 0;
        }
    }
            
    // Pour chaque file on l'ouvre.
    for (int i = 0; i<ArrayList_size(self->err_files); i++){
        int ret = File_open(ArrayList_get(self->err_files, i, 0)); // Ouverture du fichier

        if (!ret){ // Echec d'ouverture.
            perror("Pipeline_run");
            return 0;
        }
    }
            
    // Pour chaque file on l'ouvre.
    for (int i = 0; i<ArrayList_size(self->in_files); i++){
        int ret = File_open(ArrayList_get(self->in_files, i, 0)); // Ouverture du fichier

        if (!ret){ // Echec d'ouverture.
            perror("Pipeline_run");
            return 0;
        }
    }
    
    // Redirection de stdout.
    stdout_redirect(self);

    // Redirection de stderr.
    stderr_redirect(self);

    // Redirection de stdin.
    stdin_redirect(self);

    return 1;    

}


static void Redirector_print( const void* self_ ){
    
    // Vérifications
    assert( self_ );
    assert( obj_instance_of(self_, _Redirector_) );
    
    // Conversion en objet de type redirector.
    Redirector self = (Redirector) self_;

    write(1, " In:", 4);
    obj_print(self->in_files);

    write(1, "| Out:", 6);
    obj_print(self->out_files);

    write(1, "| Err:", 6);
    obj_print(self->err_files);

}


/**
 * @brief il s'agit d'une fonction qui va écrire sur tous les fichiers à 
 * vers lesquels la redirection multiple de `stderr` doit être faite.
 *
 * La fonction va également remettre le descripteur `stderr`
 * source (qui était là au départ) si '>!' (Redirection exclusive) n'était 
 * pas déclaré dans le prompte.
 * 
 * @param self L'objet `Redirector` s'occupant des redirections.
 * @param fd entier indiquant quelle sortie est traitée (err ou out).
 */
static void out_err_multiple_redirection(Redirector self, int fd){
        
    // S'il y a pas de multiple redirection ou si la liste est vide.
    if (fd == 1 && (!self->red_out || !ArrayList_size(self->out_files)))
        return;
    else if (fd == 2 && (!self->red_err || !ArrayList_size(self->err_files))) 
        return;
   
    // Récupération de la liste des descripteurs
    ArrayList list = (fd == 1) ? self->out_files : self->err_files;

    // Le premier descripteur est celui dans lequel le résultat (erreur ou sortie) de la commande a été écrit.
    int file_d = File_getFd((File) ArrayList_get_first(list, 0), 0);
    
    // Parcours de tous descripteurs 
    for (size_t i = 0; i< ArrayList_size(list); i++){ 
        // Récupération de leur FD
        int fd = File_getFd((File) ArrayList_get(list, i, 0), 0);
         
        // Remise de leur curseur au début.
        // Cela ne va pas gêner ceux ouvert avec APPEND car tout 
        // écriture se ferra à la fin quoi qu'il arrive.
        lseek(fd, 0, SEEK_SET);
    }
    
    // Buffer pour lire le contenu du fichier.
    char buff[4096];
    int nb;

    // Lecture de tout le fichier
    while ((nb=read(file_d, buff, 4096)) >0){
        for (size_t i = 1; i< ArrayList_size(list); i++){ // Parcours de tous descripteurs()
            // Récupération de leur FD
            int fd = File_getFd((File) ArrayList_get(list, i, 0), 0);
            
            // Ecriture dans le fichier.
            int nb2 = write(fd, buff, nb);

            // Vérfication de nb2
            if (nb2 == -1){
                perror("write");
                return;
            }
        }

        // écriture dans le stdout ou err si elle est permise.
        if (fd == 1 && self->red_out == 1){
            // Ecriture dans le fichier.
            int nb2 = write(self->old_out, buff, nb);

            // Vérfication de nb2
            if (nb2 == -1){
                perror("write");
                return;
            }
        }
        if (fd == 2 && self->red_err == 1){
            // Ecriture dans le fichier.
            int nb2 = write(self->old_err, buff, nb);

            // Vérfication de nb2
            if (nb2 == -1){
                perror("write");
                return;
            }
        }
    }

    // Vérification de nb
    if (nb == -1){
        perror("read");
        return;
    }

    if (fd == 2){
        // Si c'est la sortie erreur qui est traitée
        if (self->red_err == 1){ // Si la redirection n'est pas exclusive
            // On remet stderr à sa place.
            int fd = dup2(self->old_err, STDERR_FILENO);
            if (fd==-1){
                perror("dup2");
                return;
            }
        }

        // On ferme le duplication de sterr
        close(self->old_err);

    }
    else if (fd == 1){
    
        // Si c'est la sortie erreur qui est traitée
        if (self->red_out == 1){ // Si la redirection n'est pas exclusive
            // On remet stderr à sa place.
            int fd = dup2(self->old_out, STDOUT_FILENO);
            if (fd==-1){
                perror("dup2");
                return;
            }
        }

        // On ferme le duplication de sterr
        close(self->old_out);
        
    }

}

/**
 * @brief Fonction qui va remettre stdin.
 *
 * La fonction va seulement remttre l'entrée standard ( car la 
 * redirection est faite avant exécution et non après) si la redirection 
 * n'est pas exlusive.
 * 
 * @param self L'objet `Redirector` s'occupant des redirections.
 */
static void in_multiple_redirection (Redirector self){

    if (!self->red_in || !ArrayList_size(self->in_files)) 
        return;
        
    // Si la redirection est exclusive, on ne fait rien.
    if(self->red_in == 2) return;
    assert(self->red_in == 1);
    
    // Redirection de stdin vers son ancien descripteur (le tube est complètement fermé)
    dup2(self->old_in, STDIN_FILENO);

}

/**
 * @brief il s'agit d'une fonction qui va écrire sur tous les fichiers à 
 * vers lesquels la redirection multiple doit être faite.
 *
 * La fonction va également remettre les descripteurs d'entrée et sortie 
 * source (qui était là au départ) si '>!' (Redirection exclusive) n'était 
 * pas déclaré dans le prompte.
 * 
 * <p>
 * Pour stdin la fonction va seulement remttre l'entrée standard ( car la 
 * redirection est faite avant exécution et non après) si la redirection 
 * n'est pas exlisive.
 *
 * @param self L'objet `Redirector` s'occupant des redirections.
 */
void Redirector_multiple_redirection( Redirector self ){
    // Vérifications
    assert( self );
    assert( obj_instance_of(self, _Redirector_) );
    
    //Redirection multiple pour stdout.
    out_err_multiple_redirection(self, 1);
    
    //Redirection multiple pour stderr.
    out_err_multiple_redirection(self, 2);
    
    // Cette fonction va juste remettre `stdin` si la redirection 
    // d'entrée n'était pas exclusive.
    in_multiple_redirection(self);
}






