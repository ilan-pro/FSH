#ifndef FILE_H
#define FILE_H

#include "../base/String.h"
#include "../base/Class.h"

#ifdef FILE_CLASS 
    
    #define FILE_FIELDS \
        String path;        /** Chemin vers le fichier. */\
        int fd;             /** Descripteur du fichier */\
        int hasVariable;    /** Booléen permettant de savoir si oui ou non le nom du fichier contient une variable. */\
        int openFlag;       /** Flag utilisés pour l'ouverture du fichier. */\
        int opened;         /** Flag utilisés pour vérifier si le fichier est déjà ouvert. */\
        size_t size_on_open;         /** Taille du fichier au moment de son ouverture. */\

    typedef struct _File_* File;

#else

    typedef struct _File_priv {
        String path;        /** Chemin vers le fichier. */
        int fd;             /** Descripteur du fichier */
        int hasVariable;    /** Booléen permettant de savoir si oui ou non le nom du fichier contient une variable. */
        int openFlag;       /** Flag utilisés pour l'ouverture du fichier. */\
        int opened;         /** Flag utilisés pour vérifier si le fichier est déjà ouvert. */\
        size_t size_on_open;         /** Taille du fichier au moment de son ouverture. */\
    } __file_priv ;
    
    #define FILE_H_FIELDS \
        const char __file_private[sizeof(__file_priv)];
    
    typedef struct _File_ {
        Class _File_;
        FILE_H_FIELDS
    }* File;
    

#endif
    
    extern Class _File_; 
    
    /**
     * @brief Fonction qui va mettre à jour le chemin vers le fichier ouvert.
     *
     * Cette fonction va égalament fermer le descripteur actuel du fichier et 
     * va en ouvrir un autre.
     *
     * @param self Fichier dont on veut modifier le chemin.
     * @param path Le nouveau chemin à donner au fichier.
     *
     * @return Si oui (1) ou non (0) la mise à jour s'est fait sans problème, sinon 
     * penser à regarder le valeur de ERRNO
     */
    int File_setPath( File self, const char* path );
    
    /**
     * @brief Fonction qui va mettre à jour les options d'ouverture du fichier.
     *
     * [Attention] Cette fonction ne va pas reouvrir le fichier, il faut pour 
     * cela faire appelle à la fonction `File_open()`.
     * <p>
     * Aucune vérification n'est faite sur ce nouveau tag alors faire attention.
     *
     * @param self Fichier dont on veut modifier le chemin.
     * @param tag Le nouveau tag à mettre sur le fichier.
     *
     * @return Si oui (1) ou non (0) la mise à jour s'est fait sans problème, sinon 
     * penser à regarder le valeur de ERRNO
     */
    int File_setOpenTag( File self, int tag );
    
    /**
     * @brief Fonction qui va mettre à jour le descripteur du fichier ouvert.
     *
     * [Attention] : Cette fonction ne va pas changer le chemin vers le fichier. 
     * Utiilser plutôt la fonction `File_setPath()` pour s'assurer de mettre à 
     * jour à la fois le descriteur et le chemin vers le fichier. <br>
     * Cela va éviter d'avoir des incohérences.
     *
     * @param self Fichier dont on veut modifier le descripteur.
     * @param fd Le nouveau descripteur à donner au fichier.
     * @param duplicate Booléen indiquant si oui ou non on souhaite dupliquer le descripteur (cela 
     * va permettre de ne pas le fermer si jamais on a envie de travailler sur le fichier 
     * après la destruction de l'objet).
     *
     * @return Si oui (1) ou non (0) la mise à jour s'est fait sans problème, sinon 
     * penser à regarder le valeur de ERRNO
     */
    int File_setFd( File self, int fd, int duplicate);
    
    /**
     * @brief Fonction qui va renvoyer le chemin vers le fichier. 
     *
     * Cette fonction ne va renvoyer une `char*` qui est un pointeur vers 
     * un buffer, alors copier cette chaine si on a envie de l'utiliser.
     *
     * @param self Fichier dont on veut récupérer le chemin.
     *
     * @return Une chaine de caractère (contenu dans un buffer) qui contient le chemin 
     * vers le fichier.
     */
    const char* File_getPath( File self );

    /**
     * @brief Fonction qui va renvoyer un descripteur associé au fichier (ce descripteur 
     * n'est peut-être pas le seul existant). 
     *
     * [Attention] Si ce descripteur est fermé avant la destruction de l'objet 
     * cela entrainera un avertissement au moment de la destruction de l'objet.
     *
     * @param self Fichier dont on veut récupérer le descripteur. 
     * @param duplicate Booléen indiquant si oui ou non on souhaite dupliquer le 
     * descriteur avant de le renvoyer.
     *
     * @return Le descripteur du fichier ouvert.
     */
    int File_getFd( File self, int duplicate );
    
    /**
     * @brief Fonction qui va renvoyer la taille du fichier au moment de son ouverture.
     *
     * @param self Fichier dont on veut récupérer la taille initiale. 
     * descriteur avant de le renvoyer.
     *
     * @return La taille du fichier au moment de son ouverture.
     */
    size_t File_getSizeOnOpen( File self );
    
    /**
     * @brief Fonction qui va ouvrir le fichier en tenant compte des donnée 
     * de l'objet (chemin, option d'ouverture). 
     *
     * Le mode d'ouverture par défaut pour une création est 0600.
     * <br>
     * La fonction va mettre à jour (et fermer s'il y en avait un) le descripteur actuel 
     * du fichier.
     *
     * @param self Fichier à ouvrir.
     * @return Si oui (1) ou non (0) l'ouverture à réussi. Dans le cas contraire 
     * penser à vérifier la valeur de ERRNO.
     */
    int File_open( File self );

    /*
     * On peut rajouter d'autre fonction mais pas le temps.
     */


#endif
