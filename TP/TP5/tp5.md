TP nº5 : manipulation d'archives tar(suite) - le système de gestion des fichiers (SGF)
=====================

#### Pour commencer
Faites `make init`. Cela vous donnera, pour l'exo 1, les fichiers qui changent par rapport au TP 4 et installera dans le répertoire `/tmp/votrelogin/test` une arborescence qui vous premettra de tester la commande `mktar`.
Pour l'exercice 2, rien n'est fourni.

#### Exercice 1 : 
Dans cet exercice, nous allons reprendre la commande `mktar` en la rendant plus générale.


La façon d'appeler est toujours la même:

```bash
mktar arc.tar f1 f2 ....    # archive f1, f2, ... dans arc.tar

```

Mais les fichiers `f1`, `f2`, ... ne sont plus forcèment des fichiers réguliers:

* pour un fichier régulier, on l'archive comme dans le TP4.
* pour un lien symbolique, on devra aussi  mettre dans l'entête le nom du fichier pointé (champ `linkname`). Il faudra utiliser la fonction `readlink` pour le trouver.
    Attention le type qu'il faut mettre dans `typeflag` est `SYMTYPE` et pas `LNKTYPE`. (Dans le nouveau fichier `tarutils.h`, les commentaires ont été modifiés pour que ce soit clair.)
      Il n'y aura que le bloc d'entête à écrire.
* pour un tube (fifo), là aussi on ne met que le fichier d'en-tête. (Un tube est un fichier de taille limité qui sert à la communication entre processus, nous verrons cela plus tard en cours.)
* pour un répertoire, on listera les fichiers, liens symboliques et tubes  contenus dans le répertoire.
      Il n'y aura pas de récursivité, i.e. les sous-répertoires dans  le répertoire sont ignorés.


Si on rencontre un fichier qui ne peut être archivé, on se contente d'un commentaire en sortie erreur et on continue l'archivage.


 Pour ne pas suivre les liens symboliques pour récupérer la structure `stat`, il faut utiliser `lstat`. Il faut compiler sans `std=c99` pour pouvoir l'utiliser.



#### Exercice 2 : pwd

Le but final de cet exercice est d'écrire un programme affichant la
référence absolue du répertoire courant à l'instar de la commande
`pwd -P` (consulter `man pwd` si nécessaire), sans utiliser la fonction
`getcwd()`.

On va procéder par étapes :

* Écrivez la fonction `char *nom_du_repertoire()`  qui retourne le nom du
  répertoire courant dans une arborescence stockée sur un unique
  disque.

* Modifier la fonction précédente pour que qu'elle fonctionne aussi dans le cas général (c'est-à-dire avec plusieurs disques).

* Dans une arborescence complexe, i.e. avec des points de montage, tester si deux chemins référencent le même fichier nécessite de comparer à la fois les numéros d'inœud **et ceux de device**, en utilisant impérativement le numéro d'inœud fourni par `stat()` et non celui fourni par la structure `dirent`. 

   Corrigez la fonction précédente pour qu'elle marche même dans une arborescence complexe. Testez  sur lulu dans le répertoire /run; vous pouvez en profiter pour vérifier que dans ce cas-là, les numéros d'inœud fournis par `stat` et par `dirent` sont différents.

* On veut maintenant la référence absolue du répertoire courant.
Le fichier `mon_pwd.c` vous donne un squelette du programme.
Pour cela, on appliquera l'algorithme suivant :

	* initialiser un chemin `courant` à `"."` et un chemin `pwd` à `"/"`
	* chercher le nom du répertoire correspondant à `courant`
	* concaténer ce nom avec le chemin `pwd`
	* recommencer récursivement sur le répertoire parent du répertoire
      `courant`, jusqu'à ce que `courant` corresponde à la racine `/` de l'arborescence.

Une caractérisation de la racine  est qu'elle est son propre parent, c'est-à-dire 
que `.` et `..` ont donc les mêmes numéros d'inœud et de disque.

Pour simplifier, toutes les chaînes de caractères seront de taille `PATH_MAX`. Il est possible que cela occasionne quelques warnings à la compilation qu'on ignorera. On pourra bien sûr améliorer le programme dans un deuxième temps.

Testez votre programme et comparer avec le résultat de `pwd -P`. En
particulier, vérifiez qu'il fonctionne correctement sur lulu, dont
l'arborescence est constituée de plusieurs disques.

