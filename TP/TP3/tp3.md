TP nº3 : filtres UNIX
=====================

**L3 Informatique - Système**


Filtres
-------

On rappelle qu'un **filtre** est un programme permettant d'effectuer des
transformations sur les flux de données. Dans le contexte de systèmes
d'exploitation de type *UNIX*, les filtres sont typiquement des
programmes qui utilisent principalement l'entrée standard (`stdin`) comme
source de données en entrée et la sortie standard (`stdout`) comme
destination des données en sortie.

Exemples de filtres UNIX classiques :

  - `cat` : appelé sans argument, copie `stdin` sur `stdout`
  - `dd` : appelé sans argument, copie `stdin` sur `stdout` par blocs de
    512 octets
  - `wc` : appelé sans argument, affiche sur `stdout` le nombre de
    caractères, mots et lignes lus dans `stdin`
  - `tr` : copie `stdin` sur `stdout` en remplaçant ou supprimant
    certains caractères. 
  - `grep PATTERN` : `cat` sélectif, *i.e.* seules les lignes de `stdin`
    qui correspondent au motif `PATTERN` (une expression régulière) sont
    copiées sur `stdout`

L'objectif de ce TP est d'implémenter (des versions simplifiées de) ces
filtres. Vous êtes invités à systématiquement consulter leurs pages de
manuel pour vérifier leur sémantique, et à les tester dans un shell.

L'implémentation des filtres doit être faite en utilisant comme
fonctionnalités d'entrée/sortie les appels systèmes `open`, `read`,
`write`, etc. Il est interdit d'utiliser les fonctions de plus haut
niveau de la bibliothèque standard (`fopen`, `fread`, `fwrite`, etc).


### Instructions complémentaires

Pour chacun des exercices 1 à 3, vous trouverez dans le sous-répertoire
`ex-N-blabla` une partie du code, à compléter, ainsi qu'un Makefile qui vous
permettra :

 - de compiler votre code avec `make`,
 - de tester votre code avec `make test`.

À la fin de chaque exercice, avant de passer au suivant, assurez-vous que
votre programme passe *tous* les tests : lorsque vous exécutez `make
test`, toutes les lignes doivent dire `OK` !


### Exercice 1 : `my-wc`

Le répertoire correspondant est `TP/TP3/work/ex-1-my-wc`. 
Le seul fichier à modifier est `my-wc.c`.

L'objectif est d'écrire un programme `my-wc` qui satisfasse la
spécification suivante (adaptée de `man wc`) :

```
SYNOPSIS
    ./my-wc [-cl] [fichier]
  
DESCRIPTION
    ./my-wc compte le nombre de retours à la ligne ('\n') et d'octets
    dans le fichier indiqué.

    Si aucun fichier n'est fourni, ou si le nom de fichier est "-", la
    lecture se fait depuis l'entrée standard.

    Chaque statistique occupe une colonne. Les colonnes sont séparées par
    une tabulation ('\t') et sont affichées dans l'ordre suivant : nombre
    de retours à la ligne, puis nombre d'octets.
    
    Si un nom de fichier est fourni, ce nom est affiché après les
    statistiques, dans une nouvelle colonne.

    Par défaut wc affiche les deux valeurs. Les options permettent de
    n'en afficher que l'une d'entre elles. Par exemple, ./my-wc -c
    affiche le nombre d'octets.

    En cas de succès, ./my-wc doit terminer avec le code 0, et en cas
    d'échec, avec le code 1.
    
OPTIONS
    -c
        Afficher uniquement le nombre d'octets.
    -l
        Afficher uniquement le nombre de retours à la ligne.
```

Pour convertir un entier en chaîne de caractères, vous pourrez inclure
`<stdio.h>` et utiliser la fonction `snprintf()`. En revanche, les
entrées et sorties doivent toujours se faire avec `read()` et `write()`.

Complétez le code de `my-wc.c`, et testez votre programme avec `make-test`.


### Exercice 2 : `my-tr`

Le répertoire correspondant est `TP/TP3/work/ex-2-my-tr`. 
Le seul fichier à modifier est `my-tr.c`.

Le but de cet exercice est d'écrire un programme `my-tr` qui soit une
version simplifiée de la commande `tr`, et satisfasse les spécifications 
suivantes :

```
SYNOPSIS
     my-tr car1 car2
     my-tr -d car1
	 
DESCRIPTION
     La commande tr copie l'entrée standard sur la sortie standard et :
	 - appelée sans option, remplace chaque occurrence du caractère car1
	   par le caractère car2;
	 - appelée avec l'option -d, supprime chaque occurrence du
	   caractère car1.
```

Complétez le code de `my-tr.c`, et testez votre programme avec `make-test`.

Attention, les lectures et écritures doivent se faire par blocs. Il sera
donc nécessaire d'utiliser un tampon de lecture. Il est recommandé
d'utiliser un autre tampon pour l'écriture.

### Exercice 3 :  `my-sed`

Le répertoire correspondant est `TP/TP3/work/ex-3-my-sed`. 
Le seul fichier à modifier est `my-sed.c`.

On veut maintenant enrichir la commande précédente afin qu'elle se
comporte un peu comme la version en place de la commande `sed` (option
`-i`), c'est-à-dire qu'elle puisse remplacer ou supprimer directement
dans un fichier toutes les occurrences d'un caractère. Le synopsis pour
cette commande sera le suivant :

```
SYNOPSIS
     my-sed car1 car2 filename
     my-sed -d car1 filename
```

Complétez le code de `my-sed.c`, et testez votre programme avec `make-test`.

La fonction `ftruncate()` sera utile. Allez voir le `man` pour
comprendre ce qu'elle fait et comment l'utiliser.

### Exercice hors-série : lecture ligne par ligne et tampon d'entrée

Cet exercice est à faire uniquement si vous avez terminé tous les autres et
que vous voulez aller plus loin. Il n'y a pas de correction automatique.

L'objectif est d'écrire un programme `my-grep` qui prend un argument, lit
l'entrée standard, et affiche sur la sortie standard *uniquement les lignes
qui contiennent l'argument*. Par exemple :
```
$ echo "12345\n34567\n56789\n789AB" | ./my-grep 567
34567
56789
```

Vous pouvez dans un premier temps écrire une version « naïve » qui lise
l'entrée caractère par caractère. Cependant, celle-ci aura le même
problème que `garfield` du TP2. Vous devrez donc à terme lire l'entrée
par blocs. Mais comme vous ne connaissez pas à l'avance la taille de
chaque ligne (rappelons que vous devez obligatoirement utiliser l'appel
système `read()`, et non pas des fonctions de la bibliothèque standard du
C telles que `fgets()` ou `getline()`), il arrivera à votre programme de
lire le début de la ligne suivante, voire de lire plusieurs lignes d'un
coup. Pour ne pas perdre toutes les données lues « en trop », il faudra
donc stocker celles-ci dans un *tampon d'entrée*.

