/**
 * @file For.c
 * @brief Fichier contenant l'implémentation des fonctions déclarées dans `For.h`
 * 
 * For est une commande qui exécute un certain nombre de commandes pour chaque fichier d'un répertoire.
 * 
 * Construction : <code><strong>obj_new(_For_, "for");</strong></code><br>
 * `_For_` désigne la classe des commandes for. et la chaine "for" est le nom de la commande.
 */


#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../shell.h"
#include "../base/String.h"

/** @cond HIDE_THIS*/
#define FOR_CLASS "For"
/**@endcond*/
#include "For.h"


/*****************************************************/
/************ Déclaration des fonctions **************/

/**
 * Constructeur d'une commande FOR.
 * 
 * @param self_ L'objet alloué par calloc().
 * @param args La liste des arguements (Nomalement elle ne doit contenir que "For").
 * @return Un objet de type For, initialisé.
 */
static void* For_constructor(void* self_, va_list* args);

/**
 * Destructeur d'un objet de type commande FOR.
 *
 * Le destructeur va juste appeller le destructeur du parent, 
 * qui va désallouer les ressources qui sont héritées.
 * 
 * @param self_ L'objet à désallouer.
 * @return Un objet dont tout l'intérieur desalloué.
 */
static void* For_destroy(void* self_);

/**
 * @brief Fonction qui va exécuter la commande.
 *
 * Cette fonction est une redéfinition de la fonction `Command_execute()` de 
 * la classe `_Command_` (appelée par liaison dynamique).
 *
 * @param self La commande for à faire exécuter.
 * @return La valeur de retour de la commande après exécution.
 */
static int For_execute(Command self_);

static void For_print(const void* self_);
/*****************************************************/
/************* Structure de la classe ****************/


/**
 * @brief Classe `For` de la commande For qui affiche 
 * le chemin absolu du répertoire courant.
 *
 * Construction : <code><strong> obj_new( `_For_`, "for") </strong></code>
 * <br>
 * `_For_` étant la classe de la commande for.
 */
typedef struct _For_ {
    
    /**
     * @brief Classe des commandes for.
     *
     * `for` est la commande qui permet d'afficher le chemin 
     * absolue du vers le répertoire dans lequel on se 
     * trouve.
     *
     * Construction : <code><strong>obj_new(_For_, "for");</strong></code><br>   
     * `_For_` désigne la classe des commandes for. et la chaine "for" est le nom de la commande.
     */
    Class _For_;
    FOR_FILEDS

}* For;

static struct _Class_ __For__ = {
    
    NULL,
    FOR_CLASS,
    sizeof(struct _For_),
    For_constructor,
    For_destroy,
    0,
    0,
    0,
    0 // Sera celui du parent.


};

/**
 * @brief Classe des commandes for.
 *
 * `for` est la commande qui permet d'afficher le chemin 
 * absolue du vers le répertoire dans lequel on se 
 * trouve.
 *
 * Construction : <code><strong>obj_new(_For_, "for");</strong></code>
 * `_For_` désigne la classe des commandes for. et la chaine "for" est le nom de la commande.
 */
const Class _For_ = &__For__;
    
/***************************************************/
/************** Corps des fonctions ****************/


/**
 * Fonction qui va afficher un message d'erreur.
 *
 * @param msg Le message d'erreur qui sera affiché.
 */
static void For_print_syntax_err( const char* msg ){

    char tmp[] = "\e[1;31m[Erreur] Boucle for mal écrite : ";
    write(2, tmp, strlen(tmp));
    write(2, msg, strlen(msg));
    write(2, "\n\e[1;0m", 7);

}

static int next_word(char** mot, char** prompt){

    int break_loop = 0;
    
    // Nombre de caractère lu. (Si ce nombre vaut 0 alors on a encore rien lu 
    // dans le prompt et donc tous les espaces sont ignorés. Le nombre est mis à 0 après chaque lecture d'un mot)
    int char_compteur = 0;

    /** 
     * Lecture du prompt dans la boucle tant que le caractère pointé est 
     * n'est pas \0`(fin de l chaine)
     */
    while(**prompt 
            && **prompt != '{' && 
            !break_loop){
        
        char c = **prompt;
        
        switch (c) {
            default: 
                char_compteur ++;
                // Réallocation pour accueillir le nouveau caractère.
                *mot = realloc(*mot, (1 + char_compteur) * sizeof(char));
                (*mot)[char_compteur] = '\0';
                (*mot)[char_compteur - 1] = c;
                break;
            case ' ': // Le cas où le caractère lu est un espace.
    
                if (char_compteur){ // Si il y a au moins une lettre lue.
                    break_loop = 1;
                    continue;
                }
                break;  
        
            
        }
        
        (*prompt)++;

    }    
    
        if(**mot=='\0') // Si on a rien lu on renvoie 0
            return 0;
        return 1;

}

/**
 * Fonction qui ajoute l'identificateur de variable de boucle (qui va stocker le fichier en 
 * cours de traitement dans la boucle) de la boucle `for`.
 *
 * @return 1 si l'idtentificateur est trouvé. 0 sinon (avec affichage d'une erreur).
 */
static int add_file_id( For self, char** prompt ){
         
    /** 
     * Chaine de caractère qui va contenir le mot en cours de lecture, 
     * cette chaine est troquée après chaque lecture d'un mot (après 
     * avoir rencontré le premier caractère espace depuis la lecture du mot).
     */
    char* mot = (char*)malloc(sizeof(char));
    assert(mot);
    // On ajoute le caractère de fin de chaine au mot.
    mot[0] = '\0';
        
    // Réucpération du prochain mot
    next_word(&mot, prompt);
    if(!**prompt){
        For_print_syntax_err("Syntaxe incomplète !");
        free(mot);
        return 0;
    }
    // Vérification de l'existance de l'identificateur (S'il existe alors il y a une erreur).
    if (Shell_get_val(mot)){ // Si il existe une valeur pour cet identificateur alors on il existe.
        char tmp[50];
        snprintf(tmp, 50, "L'indentificateur '%s' existe déjà !", mot);
        For_print_syntax_err(tmp);
        free(mot);
        return 0;
    }

    // Le mot parsé est l'identificateur des fichiers dans la boucle.
    // Ajout de ce fichier.
    Shell_add_var(mot, "d"); // Pas de valeur pour le moment.
    char* tmp = malloc((1 + strlen(mot) * sizeof(char)));
    assert(tmp);
    strcpy(tmp, mot);
    self->id_var = tmp;
    
    free(mot);
    return 1;
}

/**
 * Fonction qui vérifie si l'opérateur in est bien le prochain mot parsé.
 *
 * @return 1 Si `in` est le prochain mot. 0 sinon (avec affichage d'une erreur).
 */
static int check_in( char** prompt ){

    /** 
     * Chaine de caractère qui va contenir le mot en cours de lecture, 
     * cette chaine est troquée après chaque lecture d'un mot (après 
     * avoir rencontré le premier caractère espace depuis la lecture du mot).
     */
    char* mot = (char*)malloc(sizeof(char));
    assert(mot);
    // On ajoute le caractère de fin de chaine au mot.
    mot[0] = '\0';
    
    // Récupération du prochain mot `in`
    // Si le mot lu n'est pas "in" ou si aucun mot n'est lu (fin)
    if(!next_word(&mot, prompt) || strcmp(mot, "in")){
        For_print_syntax_err("Il manque l'opérateur `in`");
        free(mot);
        return 0;
    }

    // Si le prompt est fini.
    if(!**prompt){
        For_print_syntax_err("Syntaxe incomplète !");
        free(mot);
        return 0;
    }
    
   free(mot);
   return 1;
}
/**
 * Fonction qui ajoute le répertoire de boucle (dans lequel les fichiers seront chargés  
 * pour être traité dans la boucle) de la boucle `for`.
 * 
 * La fonction ne va pas vérifier si le répertoire est valide.
 *
 * @return 1 si le répertoire est saisie. 0 sinon (avec affichage d'une erreur).
 */
static int add_dir( For self, char** prompt ){

    /** 
     * Chaine de caractère qui va contenir le mot en cours de lecture, 
     * cette chaine est troquée après chaque lecture d'un mot (après 
     * avoir rencontré le premier caractère espace depuis la lecture du mot).
     */
    char* mot = (char*)malloc(sizeof(char));
    assert(mot);

    // On ajoute le caractère de fin de chaine au mot.
    mot[0] = '\0';
   
    // Récupération du chemin vers le répertoire.
    if (!next_word(&mot, prompt)){
        For_print_syntax_err("Répertoire non-renseigné !");
        free(mot);
        return 0;
    }

    if(!**prompt){
        For_print_syntax_err("Syntaxe incomplète !");
        free(mot);
        return 0;
    }
    // Ajout du repertoire de recherche.
    size_t index = ArrayList_size(self->args);
    ArrayList_add_last(self->args, obj_new(_String_, mot), 0);
    self->index_dir = index;
    
    free(mot);
    return 1;
}

static int add_param_aux( For self, char ** prompt ){
    
        
    char tmp[20];
    char* mot;

    while (**prompt && **prompt != ' ' && **prompt != '{'){
        char c = *(*prompt)++;
        switch (c) {
            
            case 'A' :

            case 'r' :
                snprintf(tmp, 20, "%c", c); 
                break;

            case 'e' :
                
            case 't' :
            case 'p' :
                
                if (**prompt != ' '){ // Si le prochain caractère n'est pas un espace.
                    goto h_default;
                }

                // Récupération du prochain mot.
                mot = malloc(sizeof(char));
                assert(mot);
                mot[0] = '\0';
                    
                if(!next_word(&mot, prompt)){ // Si aucun mot n'est lu après
                    // Alors la boucle est mal écrite.
                    For_print_syntax_err("Aucun argument n'est trouvé !");
                    return 0;
                }
                
                snprintf(tmp, 20, "%c %s", c, mot);
                
                free(mot);
                break;

            default : 
                h_default :
                
                // Alors la boucle est mal écrite.
                For_print_syntax_err("Paramètre inconnu !");
                return 0;

        }
        
        // Ajout du paramètre ()
        ArrayList_add_last(self->params, obj_new(_String_, tmp), 0);
    }

    return 1;
}

static int add_params( For self,  char ** prompt ){

    while(**prompt!= '{'){
            
        // caractère pointé par le prompt
        char c = *(*prompt);

        // Si on arrive à la fin de la chaine avant d'avoir rencontré un '{'
        if(!**prompt){
            // Alors la boucle est mal écrite.
            For_print_syntax_err("Aucune commande trouvé !");
            return 0;
        }
        
        if(c == '-'){
            (*prompt)++;
            if (!add_param_aux(self, prompt))
                return 0;
            continue;
        }
        
        else if (c == ' '){
            (*prompt)++;
            continue;
        }
        else{ // on est tombé sur un mot qui n'est pas un paramètre.
            (*prompt)++;
            For_print_syntax_err("Argument non-identifié.");
            return 0;
        }
        
    }

    // On décale de 1 pour ignorer le caractère '{'.
    (*prompt)++;
    
    return 1;
}

// Constructeur.
static void* For_constructor(void* self_, va_list* args){
    
    /// Mise à jour de _Cd_ en tant que héritier de _Command_ (si ce n'est pas déjà le cas)
    if(!_For_->_super_)
        *(Class*)& (_For_->_super_) = _Command_;

    // Vérification des arguments.
    assert(self_ && args);    
    
    // Conversion en objet de type For.
    For self = (For) self_;

    // Appel du constructeur de la classe Command ( parent )
    _Command_->ctor(self, args);
    // On met la fonction print du parent comme fonction d'affichage de l'enfant.
    *((void (**)) &(self->_For_->print)) = For_print;

    // Ajout de la fonction execute de la classe fille.
    *((int (**) (Command)) &(self->execute)) = For_execute;

    //Récupération du reste du prompt.
    char** prompt = va_arg(*args, char**);

    // Ajout de l'identificateur de la variable de boucle
    if (!add_file_id(self, prompt)){
        Command_fail_with_message((Command)self,1);
        obj_destroy(self);
        return NULL;
    }

    // Vérification de l'existance de `in`
    if (!check_in(prompt)){
        Command_fail_with_message((Command)self,1);
        obj_destroy(self);
        return NULL;
    }

    // Ajout du répertoire
    if (!add_dir(self, prompt)){
        Command_fail_with_message((Command)self,1);
        obj_destroy(self);
        return NULL;
    }
   

    // Ajout des paramètres.
    if (!add_params(self, prompt)){
        Command_fail_with_message((Command)self,1);
        obj_destroy(self);
        return NULL;
    }
    
    // Augmentation du nombre d'accolades ouvrantes
    opened_brackets ++;

    // Récupération des commandes de for
    // Appel de Shell_parse avec le reste du prompt.
    self->commands = Shell_parse(prompt); 
    if(!self->commands) {// Si self->command = NULL alors il y a eut une erreur de syntaxe quelque part pendant le parsing.
        Command_fail_with_message((Command)self,1);
        obj_destroy(self);
        return NULL; // On renvoie NULL également.
    }

    // Si la caractère pointé par le prompt n'est pas '}'
    if (**prompt != '}'){
        // Alors l'arrêt du parsing de Shell_parse() n'est pas causé par '}'
        // mais par '\0' (il n'y a que ces deux cas qui arrêtent Shell_parse()).
        For_print_syntax_err("Il manque l'accolade fermante !"); // On signale l'erreur de syntaxe.
        Command_fail_with_message((Command)self,1);
        obj_destroy(self);
        return NULL;
    }
    
    // Diminution du nombre d'accolades ouvertantes
    opened_brackets--;
    (*prompt)++;

    return self;

}

// Destructeur.
static void* For_destroy(void* self_){
    
    // Vérification des arguments.
    assert(self_);    
    
    For self = (For) self_;
    obj_destroy(self->commands);
    if (self->id_var)
        Shell_remove_var(self->id_var); // Suppression de la variable de la boucle for.
    free(self->id_var);

    return _Command_->dtor(self_);
    
}


/**
 * Fonction qui va vérifier si l'entrée est '.' ou '..'
 */
static int ent_is_p_or_pp(struct dirent* ent){

    // Récupération du nom.
    char* name = ent->d_name;
    // Si le premier caractère n'est pas .
    if (*name != '.')
        return 0; // Alors ce n'est pas . ni ..
    
    // Si le premier carctère est . mais le second n'est ni \0 ni .
    else if (*(name+1) != '\0' && *(name+1) != '.')
        return 0; // Alors ce n'est pas . ni ..
    
    else if (*(name+1) != '\0' && *(name+2) != '\0')
        return 0;
    
    else 
        return 1;

}

/**
 * Fonction qui va créer le chemin vers le fichier. en cours de traitement à l'aide du 
 * chemin vers le répertoire qui le contient.
 */
static void assemble_path_and_add(char** path, const char* name, ArrayList all_files){

    ///////////////////////////////////////////////////////////
    // Concaténation des deux chaines avec ajout d'une séparation '/'
    char* tmp = (char*) malloc((2+ strlen(*path) + strlen(name)) * sizeof(char));
    assert(strcat(strcat(strcpy(tmp, *path), "/"), name));
    String s = obj_new(_String_, tmp); // Création de la chaine qui va contenir le chemin.
    free(tmp); // libération de tmp, plus besoin.

    ArrayList_add_last(all_files, s, 0);

}

// Fonction pour obtenir l'extension d'un fichier
const char* get_file_extension(const char* filename) {
    // strrchr recup le dernier point contrairement à strchr qui renvoie la première occurence
    const char* dot = strrchr(filename, '.');
    // si le fichier n'a pas d'extension ou si le point est le premier caractère du fichier et qu'il n'a pas d'autre point ".fichier" renverra un pointeur sur le premier point
    if (!dot || dot == filename) return "";
    // retourne l'extension après le point
    return dot + 1;
}

const int get_option(ArrayList options, const char option){
    for (size_t i = 0 ; i < ArrayList_size(options) ; i++){
        String s = (String) ArrayList_get(options, i , 0);
        const char* extension = obj_to_string(s);
        if (extension[0] == option){
            // si le e est trouvé, alors renvoyé le pointeur sur le premier caractère après l'espace
            return i;
        }
    }
    return -1;
}


// Fonction qui va parcourir la liste des elements
const char* get_user_extension(ArrayList options, const char* option ){
    const int indice = get_option(options, option[0]);

    if(indice == -1){
        return NULL;
    }

    String s = (String) ArrayList_get(options, indice , 0);
    const char* extension = obj_to_string(s);

    // si le e est trouvé, alors renvoyé le pointeur sur le premier caractère après l'espace
    return extension + 2;
}


/**
 * Fonction qui de va récupérer les fichiers du répertoire et va également se charger d'appliquer 
 * les filtres donnée
 */
static void For_get_files(For self, DIR* dir, char** path, ArrayList all_files){

    struct dirent* ent;

    while((ent = readdir(dir))){
        
        // Si l'entrée est . ou ..
        if (ent_is_p_or_pp(ent)) 
            continue; // on continue.

        /////////////////////////////////////////////////
        

        // -A
        if(ent->d_name[0] == '.' && get_option(self->params, 'A') == -1){
            continue; 
        }
        
        // -r
        if (get_option(self->params, 'r') != -1){
            // on ouvre pas le père ni lui-meme (risque de boucle infinie)
            if(ent->d_type == DT_DIR){
                char *tmp = malloc(strlen(*path) + strlen(ent->d_name) + 2);
                
                strcpy(tmp, *path);
                // quelque chose de la forme /home/.../dossier
                strcat(strcat(tmp, "/"), ent->d_name);
                tmp[strlen(*path) + strlen(ent->d_name) + 1] = '\0';
                
                DIR* dir_opened = opendir(tmp);
                if(!dir_opened){
                    perror("ouverture du dossier");
                }
                For_get_files(self, dir_opened, &tmp, all_files);

                closedir(dir_opened);
                free(tmp);
            }   
        }

        // -t
        const char *found_option_t;

        if((found_option_t = get_user_extension(self->params, "t"))){
            if (strcmp(found_option_t, "f") == 0 && ent->d_type != DT_REG)
                continue;
            else if (strcmp(found_option_t, "d") == 0 && ent->d_type != DT_DIR)
                continue;
            else if (strcmp(found_option_t, "l") == 0 && ent->d_type != DT_LNK)
                continue;                                                  
            else if (strcmp(found_option_t, "p") == 0 && ent->d_type != DT_FIFO)
                continue;
        }
        
        // -e
        const char *found_option_e;

        if ((found_option_e = get_user_extension(self->params, "e"))){
            const char* extension = get_file_extension(ent->d_name);
            if(strcmp(found_option_e, extension) != 0){
                continue;
            }else{
                // pour les
                // autres fichiers, la variable de boucle F prend pour valeur la
                // référence du fichier amputée de son extension
                char* tmp = (char*) malloc((strlen(ent->d_name) + 1) * sizeof(char));
                strcpy(tmp, ent->d_name);
                // on cherche le dernier point (de l'extension)
                char* extension = strrchr(tmp, '.');
                if (extension)
                    // on remplace le point par un \0 pour supprimer l'extension
                    *extension = '\0';
                // assert(Shell_update_var(self->id_var, tmp));
                assemble_path_and_add(path, tmp, all_files);    
                free(tmp);
                continue;
            }
        }
     
        /////////////////////////////////////////////////

        // Assemblage du chemin vers le fichier.
        assemble_path_and_add(path, ent->d_name, all_files);

    }

}

static int For_execute_for_each_file(For self, ArrayList all_files){

    ////////// La parallèlisation c'est ici (enfin je crois :) ) /////////////////////
    for (size_t i = 0; i<ArrayList_size(all_files); i++){

        char* name = obj_to_string((String)ArrayList_get(all_files, i, 0));

        // On copie au cas où obj_to_string renvoie pointeur vers une mémoire tampon qui risque d'être remplie par autre chaose à tout moment.
        char* tmp = (char*) malloc((1 + strlen(name)) * sizeof(char));
        strcpy(tmp, name);

        // Mise à jour de la variable de boucle avec le i-ème élément de la liste.
        assert(Shell_update_var(self->id_var, tmp));
        free(tmp); // Libération de la copie.

        // Exécution de bloc de commandes.
        int val_ret = Shell_execute_all(self->commands);
        if (val_ret) // Si la valeur de retour n'est pas nulle.
            return val_ret; // On arrête l'exécution.

    }

    return 0; 

}

// Execute.
static int For_execute(Command self_){
        
    int ret = 0;

    // Conversion en objet For
    For self = (For) self_;

    // Ouverture du dossier
        // Récupération du chemin
        char* tmp = obj_to_string((String) ArrayList_get(self->args, self->index_dir, 0));
        char* dir = malloc((1 + strlen(tmp))*sizeof(char));
        assert(strcpy(dir, tmp));

    DIR* dir_opened = opendir(dir);
    if (!dir_opened){ // Si le répertoire ne s'ouvre pas.
        // Affichage de l'erreur
        write(2, "\e[1;31m", 7);
        char tmp[50];
        snprintf(tmp, 50, "Ouverture du répertoire \"%s\"", dir);
        perror(tmp);
        write(2, "\e[1;0m", 6);
        return 1;
    }

    // List qui va contenir tous les fichiers qui seront traités
    ArrayList all_files = obj_new(_ArrayList_, 0);
    // Récupération des fichiers qui seront utilisés.
    For_get_files(self, dir_opened, &dir, all_files);
    
    // Exécution du bloc des commandes pour chaque fichier.
    ret = For_execute_for_each_file(self, all_files);

    // Libération des ressources.
    obj_destroy(all_files);
    closedir(dir_opened);
    free(dir);
    return ret;
}

static void For_print (const void* self_) {

    assert(self_);
    assert(obj_instance_of(self_, _For_));

    For self = (For) self_;

    write(1, "For : ", 6);
    
    // Ecriture de l'ID
    write(1, "ID= \"",5);
    write(1, self->id_var, strlen(self->id_var));
    write(1, "\"  ", 3);

    // Ecriture du répertoire de travail de for
    write(1, "Dir= \"",6);
    char* dir = obj_to_string(ArrayList_get(self->args, self->index_dir, 0));
    write(1, dir, strlen(dir));
    write(1, "\"  ", 3);

    

    write(1, "Params=",7);
    obj_print(self->params);

    write(1, " Commands=",10);
    obj_print(self->commands);  
    write(1, " ", 1);
        
}
