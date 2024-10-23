# TP nº6 : A Long Walk along The Filesystem

**L3 Informatique - Système**

Ce TP permet d'approfondir les parcours d'arborescences. Pour cela, nous
allons programmer une commande pour trouver un fichier à partir d'un
emplacement donné.
Le parcours d'arborescence à faire est un parcours récursif descendant.

#### Instructions pour faire le TP

1. Mettez à jour le dépôt `git` du cours. Placez-vous dans le
   répertoire du cours et lancez la commande `git pull`.
2. Déplacez-vous dans le répertoire `TP/TP6/`.
3. Lancez la commande `make init` pour initialiser le répertoire de
   travail.
4. Déplacez-vous dans le répertoire `work` pour travailler.

#### Exercice 1 : `mystring`, mini-bibliothèque de chaînes de caractères

Pour l'exercice 2, vous aurez besoin d'effectuer les deux opérations
suivantes sur des chaînes de caractères :

 * Ajout de caractères supplémentaires à la fin,
 * Troncature, c'est-à-dire suppression d'un certain nombre de caractères à la fin.

La mini-bibliothèque de manipulation de chaînes de caractères est à compléter.

`TODO:` Déplacez-vous dans le répertoire `work/findp`. Lisez le fichier
`mystring.h` pour regarder brièvement les fonctions à utiliser.
Dans le fichier `mystring.c`, écrivez le code de la fonction `string_append`.


#### Exercice 2 : `findp`, trouver l'emplacement d'un fichier

Nous allons programmer une version simplifiée de la commande `find`, qui
se contentera d'une recherche selon le critère du nom (_basename_) : on
souhaite que `findp dir_name target_name` soit un équivalent de `find
dir_name -name target_name`.

Le principe de l'algorithme est simple. Intuitivement, il faut lister
toutes les entrées d'un répertoire, et pour chaque entrée, vérifier si
elle porte le nom cherché; par ailleurs, s'il s'agit d'un répertoire, il
faut poursuivre la recherche dedans.

Déplacez-vous dans le répertoire `work/findp`.

1. Complétez la fonction `main` de `findp.c`. Votre programme doit retourner
   une erreur si l'argument donné en paramètre n'existe pas et si ce n'est pas
   un répertoire. Dans le cas contraire, le programme doit appeler la fonction
   `process_dir` qui recherche les fichiers `target_name` récursivement dans
   le répertoire `path` (`TODO[1]`).

2. Complétez la fonction `process_dir` de `findp.c`. Cette fonction doit
   parcourir le répertoire `path` en argument. Dans un premier temps,
   constituez le corps de la fonction qui parcourt les entrées du
   répertoire (`TODO[2]`).

3. Pour chaque entrée du répertoire, tester si le fichier coïncide avec
   celui qui est recherché (`TODO[3]`) et effectuez l'affichage de `findp`.

4. Il faut maintenant implémenter la recherche récursive; pour cela, nous
   avons besoin de construire les chemins des sous-répertoires à
   explorer. Pour chercher dans un sous-répertoire, il faut concaténer
   son nom au chemin en cours d'inspection et le supprimer quand nous avons
   fini de le traiter. Sans cela, nous n'aurons pas le bon chemin pour
   inspecter les fichiers du sous-répertoire. Implémentez la recherche
   dans les sous-répertoires (`TODO[4]`). Attention, la fonction `readdir`
   ne conserve pas sa position si le répertoire a été fermé avec `closedir`.

5. Assurez-vous que votre programme termine avec un code de retour de `0`
   s'il a trouvé des fichiers portant le nom cherché, et `1` sinon (`TODO[5]`).

6. Testez votre code avec la commande `make test`.

#### Exercice 3 : `mktar`, archiver une arborescence.

Modifier le programme `mktar` du TP5 pour permettre l'archivage d'arborescences
complètes.

#### Exercice 4 : `untar`, reproduire une arborescence

Modifier le programme `untar` du TP3 pour permettre le désarchivage
d'arborescences.
