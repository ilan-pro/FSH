/**@cond HIDE_THIS*/
#define COMMAND_CLASS "Command"  
#include "Command.h"
/**@endcond*/

/*****************************************************/
/************ Déclaration des fonctions **************/
    
/**
 * Il s'agit du constructeur de la commande.
 *
 * La fonction va répérer le pointeur vers la saisie de l'utilisateur et va 
 * continuer la lecture (en incrémentant le pointeur) jusqu'à ce qu'il tombe sur 
 * le caractère `;`, `|`, `}`.
 *
 * pendant la lecture la fonction va mettre à jour les champs `params` s'il 
 * tombe sur des paramètres de commande (- suivi d'une séquence de lettres) 
 * et `args` si elle tombe sur des arguments (tout ce qui ne commence pas 
 * par un -). Plus tard (si ce n'est pas déjà fait), la fonction va également 
 * reconnaitre les redirections s'il y en a ( `>` ).
 *
 * @param self_ La référence vers la commande.
 * @return La commande avec tous les éléments initialisés.
 */
static void* Command_constructor(void* self_);
    
/**
 * Destructeur d'une commande cette fonction va désallouer et détruire les 
 * listes d'arguments et de paramètres.
 *
 * @param self_ La commande à désallouer.
 * @return La commandes avec les ressources désalloués, pour qu'elle puisse être désallouée 
 * à son tour.
 */
static void* Command_destroy(void* self_);    
    
/**
 * Fonction qui va afficher la commande à l'écran.
 *
 * La fonction va afficher une chaine du genre.
 *
 * Command : <Nom_de_la_command>, Params=[...], Args=[...] 
 */
static char* Command_print(const void* self_);
    
/**
 * @internal
 * @brief Fonction qui va exécuter la commande.
 *
 * Cette fonction est abstraite (déclanche une erreur si aucune redéfinition
 * n'est trouvée), à chaque appel la fonction va appeller la méthode `execute()` 
 * de l'objet (liason dynamique). Cela va permettre plus tard de créer une 
 * file de commandes toutes distinctes et de les éxécuter.
 * 
 * @param self La commande à faire exécuter.
 * @return La valeur de retour de la commande après exécution.
 */
int Command_execute();

/*****************************************************/
/************* Structure de la classe ****************/



/***************************************************/
/************** Corps des fonctions ****************/



