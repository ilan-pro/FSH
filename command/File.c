
/** @cond HIDE_THIS */
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "../base/new.h"
#define FILE_CLASS "File"
#include "File.h"
/** @endcond */


/*****************************************************/
/************ Déclaration des fonctions **************/
    
    /**
     * Fonction qui va construire l'objet du fichier.
     * 
     * Le constructeur ne prend pas d'arguement. Le chemin ainsi 
     * le descripteur du fichier doivent être donné à l'objet (par 
     * le biais des setters) après construction. Si le chemin est donné sur le setter 
     * le fichier est ouvert et le champs `fd` est mis également à jour. (Le réciproque
     * n'est pas vraie.)
     *
     * @param self_ Le référence de l'objet créé par `obj_new` ou autre;
     * @param args La liste des arguements (censé être vide).
     */
   static void* File_constructor( void* self_, va_list* args ); 
    
    /**
     * Fonction qui va détruire l'objet du fichier.
     *  
     * Le descripteur du fichier est fermé (si une fermeture échoue, un 
     * avertissement suivi du message d'erreur de la fermeture sont 
     * affichés sur la sortie standard) et les objets sont détruits.
     */
   static void* File_destructor( void* self_ ); 
      
/*****************************************************/
/************* Structure de la classe ****************/

  typedef struct _File_ {
     Class _File_;
     FILE_FIELDS
  }* File;  

   static struct _Class_ __File__ = {
      NULL, 
      FILE_CLASS,
      sizeof(struct _File_),
      File_constructor,
      File_destructor,
      0,
      0,
      0,
      0
   };

   Class _File_ = &__File__;

/***************************************************/
/************** Corps des fonctions ****************/

    /**
     * Fonction qui va construire l'objet du fichier.
     * 
     * Le constructeur ne prend pas d'arguement. Le chemin ainsi 
     * le descripteur du fichier doivent être donné à l'objet (par 
     * le biais des setters) après construction. Si le chemin est donné sur le setter 
     * le fichier est ouvert et le champs `fd` est mis également à jour. (Le réciproque
     * n'est pas vraie.)
     *
     * @param self_ Le référence de l'objet créé par `obj_new` ou autre;
     * @param args La liste des arguements (censé être vide).
     */
   static void* File_constructor( void* self_, va_list* args ){
      
      // Vérifications.
      assert(self_);
      assert(args);

      // Conversion en objet de type File 
      File self = (File) self_;
      self->path = (String) obj_new(_String_, ""); // Création d'un chemin vide pour le nouveau fichier plutôt que NULL.
      self->fd = -1; // On met le descripteur à -1 pour indiquer qu'il y en a pas.

      return self;

   }

    /**
     * Fonction qui va détruire l'objet du fichier.
     *  
     * Le descripteur du fichier est fermé (si une fermeture échoue, un 
     * avertissement suivi du message d'erreur de la fermeture sont 
     * affichés sur la sortie standard) et les objets sont détruits.
     */
   static void* File_destructor( void* self_ ){

      // Vérifications.
      assert(self_);

      // Conversion en objet de type File 
      File self = (File) self_;
      
      // destruction du chemin.
      obj_destroy(self->path);
      // Fermeture du fichier s'il y en a un d'ouvert.
      if (self->fd >= 0){
         int ret = close(self->fd);

         /** // Si la fermeture échoue. */
         /** if (ret == -1){ */
         /**    char tmp[100]; */
         /**    printf("%d Après\n", self->fd); */
         /**    snprintf(tmp, 100, "[Attention] : La fermeture d'un fichier ouvert a échouée.\n"); */
         /**    write(2, tmp, strlen(tmp)); */
         /**    perror("Fermeture de fichier"); */
         /** }  */
      }

      return self;

   }


    /**
     * @internal
     * @brief Fonction qui va mettre à jour le chemin vers le fichier ouvert.
     *
     * Cette fonction va égalament fermer le descripteur actuel du fichier et 
     * va en ouvrir un autre.
     *
     * @param self Fichier dont on veut modifier le chemin.
     * @param path Le nouveau chemin à donner au fichier.
     * @return Si oui ou non la mise à jour s'est fait sans problème, sinon 
     * penser à regarder le valeur de ERRNO
     */
    int File_setPath( File self, const char* path ){
   
      // Vérifications
      assert( self );
      assert( obj_instance_of(self, _File_) ); 
      assert( path );

      // Destruction de l'ancien chemin vers le fichier.
      obj_destroy(self -> path);

      // Mise à jour du chemin vers le fichier.
      self->path = obj_new(_String_, path);
      
      // On indique que le fichier n'est pas ouvert.
      self->opened = 0;

       /** write(2, "Un setPath ici !\n",17 ); */
      // Tout s'est bien terminé.
      return 1;
    }

   
    /**
     * @internal
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
    int File_setOpenTag( File self, int tag ){

      // Vérifications
      assert( self );
      assert( obj_instance_of(self, _File_) ); 
      
      // Mise à jour du flag d'ouverture.
      self -> openFlag = tag;

      // On indique que le fichier n'est pas ouvert.
      self->opened = 0;

      return 1;

    }


    /**
     * @internal
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
    int File_setFd( File self, int fd, int duplicate){
      
      // Vérifications
      assert( self );
      assert( obj_instance_of(self, _File_) ); 
      
      // Nouveau descripteur (peut être une duplication).
      int new_fd = (duplicate) ? dup(fd) : fd;
      
      // Fermeture de l'ancien descripteur (S'il y en avait un).
      if ( self->fd >= 0 && new_fd != self -> fd){
         // Fermeture du fichier.
         int ret = close (self->fd);
         
         // Gestion en cas d'erreur de fermeture.
         if (ret == -1){
            char tmp[100]; 
            snprintf(tmp, 100, "Echec de la fermeture du l'ancien descripteur du fichier \"%s\" \n", obj_to_string(self->path));
            write(2, tmp, strlen(tmp));
            return 0;
         }
      }
   
      // Mise à jour du descripteur .
      self->fd = new_fd;
      
      // On indique que le fichier n'est pas ouvert.
      self->opened = 0;

      // Tout s'est bien passé.
      return 1;
   
    }


    /**
     * @internal
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
    const char* File_getPath( File self ){
      
      // Vérifications
      assert( self );
      assert( obj_instance_of(self, _File_) ); 
      
      // On retourne la chaine.
      return obj_to_string( self->path );

    }

   
    /**
     * @internal
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
    int File_getFd( File self, int duplicate){
      
       
      // Vérifications
      assert( self );
      assert( obj_instance_of(self, _File_) ); 
      
      /** fprintf(stderr, "Ici %d\n", self->fd); */
      /** fflush(stderr); */

      // On retourne le descripteur.
      return duplicate ? dup( self->fd ) : self->fd;

    }



    /**
     * @internal
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
    int File_open( File self ){

      // Vérifications
      assert( self );
      assert( obj_instance_of(self, _File_) ); 
   
      // Si il n'y a aucun chemin ou si le fichier est ouvert (mais que le chemin n'avait pas de variable) alors on a terminé.
      if (strcmp(obj_to_string(self->path), "") == 0 || (self->opened && !self->hasVariable))
         return 1; 

       /** write(2, "Un open ici !\n",14 ); */
      // Ouverture du fichier.
      int new_fd = open(obj_to_string(self->path), self->openFlag, 0664);
      if (new_fd == -1){ // Gestion de l'échec d'ouverture.
         char tmp[100]; 
         snprintf(tmp, 100, "Echec de l'ouverture du fichier \"%s\" \n", obj_to_string(self->path));
         write(2, tmp, strlen(tmp));
         return 0;
      }

      // Fermeture de l'ancien descripteur (S'il y en avait un).
      if ( self->fd >= 0 && self->fd != new_fd){
         // Fermeture du fichier.
         int ret = close (self->fd);
         
         // Gestion en cas d'erreur de fermeture.
         // Fermeture du nouveau fichier ouvert.
         if (ret == -1){
            char tmp[100]; 
            snprintf(tmp, 100, "Echec de la fermeture du l'ancien descripteur du fichier \"%s\" \n", obj_to_string(self->path));
            write(2, tmp, strlen(tmp));
            return 0;
         }

      }

      // Si on est ici c'est que tout s'est bien passé.
      // Mise à jour alors.
      
      // Mise à jour du descripteur.
      self->fd = new_fd;
      /** printf("FD : %d\n\n", self->fd); */
      /** fflush(stdout); */
   
      // On indique que le fichier est ouvert
      self->opened = 1;

      // Tout s'est bien terminé.
      return 1;
      
    }









