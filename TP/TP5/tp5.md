TP nº5 : manipulation d'archives tar (suite) - le système de gestion des fichiers (SGF)
=====================

#### Pour commencer

Exécuter `make init` depuis le répertoire `TP/TP5/`. Cela vous permettra,
pour l'exercice 1, de récupérer une version actualisée des fichiers
`tarutils.[ch]` du TP 4, et installera dans le répertoire
`/tmp/<votrelogin>/test` (où `<votrelogin>` est votre nom de login) une
arborescence contenant des fichiers de types variés sur laquelle vous
pourrez tester la commande `mktar`.  Pour l'exercice 2, rien n'est
fourni.


#### Exercice 1 : `mktar` plus complet

Dans cet exercice, nous allons reprendre la commande `mktar` pour
permettre l'archivage d'arborescences plus générales.

La forme de la ligne de commande reste la même :

```bash
mktar arc.tar f1 f2 ....    # archive f1, f2, ... dans arc.tar
```

mais les fichiers `f1`, `f2`, ... ne sont plus nécessairement des fichiers
ordinaires :

* chaque fichier ordinaire est toujours archivé comme au TP4;

* pour chaque lien symbolique, il n'y a pas de bloc de contenu, seul le
  bloc d'entête est à écrire; utiliser la fonction `readlink()` pour
  renseigner le champ `linkname`; et attention au champ `typeflag` qu'il
  faut mettre à `SYMTYPE` et non `LNKTYPE` (dans le nouveau fichier
  `tarutils.h`, les commentaires ont été modifiés par souci de
  clarification);

* pour chaque tube nommé (ou _fifo_, un type de fichier dédié à la
  communication entre processus), il n'y a également que le bloc
  d'entête à écrire;

* pour chaque répertoire, un bloc d'entête spécifique est à écrire, puis on
  archivera les fichiers ordinaires, liens symboliques et tubes contenus
  dans le répertoire; en revanche, on ne gèrera (pour le moment du moins) pas 
  la récursivité et on ignorera les éventuels sous-répertoires. 

Lorsqu'on rencontre un fichier qui ne peut être archivé, on se contente
d'un commentaire sur la sortie erreur et on continue l'archivage.

Attention à faire appel à `lstat()` et non `stat()` (et compiler *sans*
`std-c99` pour pouvoir l'utiliser).



#### Exercice 2 : `pwd`

Le but final de cet exercice est d'écrire un programme affichant la
référence absolue du répertoire courant à l'instar de la commande `pwd
-P` (consulter `man pwd` si nécessaire), sans utiliser la fonction
`getcwd()`.

On va procéder par étapes :

* Écrire la fonction `char *nom_du_repertoire()`  qui retourne le nom (de
  base) du répertoire courant dans une arborescence stockée sur un unique
  disque.

* Modifier la fonction précédente pour que qu'elle fonctionne aussi dans
  le cas général (c'est-à-dire avec plusieurs disques) : dans une
  arborescence complexe, i.e. avec des points de montage, tester si deux
  chemins référencent le même fichier nécessite de comparer à la fois les
  numéros d'inœud **et ceux de volume (device)**, en utilisant
  impérativement le numéro d'inœud fourni par `stat()` et non celui
  fourni par la structure `dirent`. 

  Corriger la fonction précédente pour qu'elle marche fonctionne dans une
  arborescence complexe. Testez sur lulu dans le répertoire `/run`; vous
  pouvez en profiter pour vérifier que dans ce cas-là, les numéros
  d'inœud fournis par `stat` et par `dirent` sont différents.

* On veut maintenant la référence absolue du répertoire courant.
  Le fichier `mon_pwd.c` vous donne un squelette du programme.
  Pour cela, on appliquera l'algorithme suivant :

    * initialiser un chemin `courant` à `"."` et un chemin `pwd` à `"/"`;
    * chercher le nom du répertoire correspondant à `courant`;
    * concaténer ce nom avec le chemin `pwd`;
    * recommencer récursivement sur le répertoire parent du répertoire
      `courant`, jusqu'à ce que `courant` corresponde à la racine `/` de
      l'arborescence.

Une caractérisation de la racine est qu'elle est son propre parent,
c'est-à-dire que `.` et `..` ont donc les mêmes numéros d'inœud et de
disque.

Pour simplifier, toutes les chaînes de caractères seront de taille
`PATH_MAX`. Il est possible que cela occasionne quelques warnings à la
compilation qu'on ignorera. On pourra bien sûr améliorer le programme
dans un deuxième temps.

Testez votre programme et comparez avec le résultat de `pwd -P`. En
particulier, vérifiez qu'il fonctionne correctement sur lulu, dont
l'arborescence est constituée de plusieurs disques.

