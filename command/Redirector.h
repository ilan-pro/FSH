#ifndef REDIRECTOR_H
#define REDIRECTOR_H
#include "../base/ArrayList.h"

#ifdef REDIRECTOR_CLASS

    #define REDIRECTOR_FIELDS\
        ArrayList out_descriptors; /* liste des descripteur de redirection pour stdout.*/ \
        ArrayList err_descriptors; /* liste des descripteur de redirection pour stderr.*/ \
        ArrayList in_descriptors; /* liste des descripteur de redirection pour stdin.*/ \
        int red_out; /* Il s'agit d'un booléen qui indique si oui ou non il y a une redirection multiple vers stdout */\
        int red_in; /* Il s'agit d'un booléen qui indique si oui ou non il y a une redirection multiple vers stdin */\
        int red_err; /* Il s'agit d'un booléen qui indique si oui ou non il y a une redirection multiple vers stderr */\

    typedef struct _Redirector_* Redirector;

#else
    typedef struct redirectior_prv {
        ArrayList out_descriptors; /* liste des descripteur de redirection pour stdout.*/ 
        ArrayList err_descriptors; /* liste des descripteur de redirection pour stderr.*/ 
        ArrayList in_descriptors; /* liste des descripteur de redirection pour stdin.*/ 
        int red_out; /* Il s'agit d'un booléen qui indique si oui ou non il y a une redirection multiple vers stdout */
        int red_in; /* Il s'agit d'un booléen qui indique si oui ou non il y a une redirection multiple vers stdin */
        int red_err; /* Il s'agit d'un booléen qui indique si oui ou non il y a une redirection multiple vers stderr */
    } redirectior_private;
    
    #define REDIRECTOR_H_FIELDS\
        const char red_prv_f[sizeof(redirectior_private)];

    typedef struct _Redirector_ {
        Class _Redirector_;
        REDIRECTOR_H_FIELDS
    } * Redirector;

#endif
     
    extern const Class _Redirector_;

    /**
     * @cond HIDE_THIS
     * Fonction défini dans le fichier `Command.h`.
     * Elle a été déclarée ici pour éviter les import multiples ( le linker va 
     * charger du reste ).
     */
    int Command_parse_all_sub(ArrayList collect_list, char **prompt, int stop_in_next, Redirector redirector);
    /**@endcond*/

    /**
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
     */
    int Redirector_add_redirection( Redirector self, char** prompt );    
    

    /**
     * @brief Fonction qui rediriger les entrée/sortie de la commande avant son exécution.
     * 
     * 
     * @param self L'objet `Redirector` s'occupant des redirections.
     */
    void Redirector_redirect( Redirector self );

#endif
