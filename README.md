Systèmes L3 2024-2025
=================

**Conseil :** cloner ce dépôt et travailler directement dans le répertoire cloné.

```bash
git clone git@moule.informatique.univ-paris-diderot.fr:poulalho/sy5-2024-2025.git
...
git pull
```
(ce mode de clonage nécessite une authentification par le protocole `ssh`
et non `http`, cf TP nº1)

Autre option, plus compliquée mais ouvrant plus de perspectives :
commencer par faire un `fork` de ce dépôt depuis l'interface `gitlab`, ce
qui va créer un dépôt dont vous serez propriétaire, puis cloner ce
nouveau dépôt. Vous pourrez alors modifier votre dépôt comme bon vous
semble (notamment y ajouter votre travail personnel), tout en gardant la
possibilité d'y intégrer les évolutions futures du dépôt du cours par un
simple clic sur le bouton `update fork` qui apparaîtra le cas échéant.


# Équipe pédagogique

| | | |
|---:|---|---|
| amphi + TP groupe MI | Dominique Poulalhon | dominique.poulalhon@irif.fr |
| TP groupes INFO 1/BIO et 2/JAP | Guillaume Geoffroy |  guillaume.geoffroy@irif.fr |
| TP groupe INFO 3 | Astyax Nourel | astyax@irif.fr |
| TP groupe INFO 4 | Isabelle Fagnot | fagnot@irif.fr |
| TP groupe INFO 5 | Anne Micheli | anne.micheli@irif.fr |


# Suggestions pour réviser le C

* _Modern C_, de Jens Gustedt, 2020 (Manning Publications), [disponible en
  ligne](https://gustedt.gitlabpages.inria.fr/modern-c/)
* [un tutoriel qui a une bonne
  tête](https://zestedesavoir.com/tutoriels/755/le-langage-c-1/); voir la
  section II en particulier.

# Suggestions de ressources externes 

Attention, ces ressources traitent beaucoup plus que le contenu du cours,
il est facile de s'y perdre...

* _UNIX, Programmation et communication_, J.-M. Rifflet et J.-B. Yunès, Dunod
* _Modern Operating Systems_, A. Tanenbaum, Pearson; les 2e et 3e éditions ont été traduites en français (_Systèmes d'exploitation_)
* [le manuel de POSIX](https://pubs.opengroup.org/onlinepubs/9699919799/)


# Modalités d'évaluation

La note finale tiendra compte des éléments suivants :
* assiduité (vérification systématique de la présence en TD/TP)
* 2 QCMs (sans doute fin octobre et courant décembre)
* projet (énoncé donné fin octobre, échéancier à respecter, rendu et
  soutenance)
* examen écrit


### QCM nº1 

Le premier QCM aura lieu durant le CM nº7 (23 octobre 2024). Il portera
sur l'intégralité des cours 1 à 5.

Répartition dans les deux amphis à venir.


# Cours

* [cours nº1](Cours/cours_1.pdf) : introduction, rôle d'un système
  d'exploitation, notions de processus, de fichier, d'appel système

* cours nº2 : `open` et `close`;
  tables des inoeuds (virtuels), des fichiers ouverts, des descripteurs;
  rôle des compteurs;
  flags et opérateurs logiques binaires;
  pointeur de position courante;
  `read`, `write` (à suivre au prochain cours);
  [slides](Cours/cours_2.pdf) + [programmes](Cours/code-cours-2) présentés en cours

* cours nº3 : retour sur `read` et `write`; `lseek`; les entrés/sorties
  via la bibliothèque standard;
  [slides](Cours/cours_3.pdf) +
  [programmes](Cours/code-cours-3) présentés en cours

* cours nº4 : duplication de descripteurs et redirections; 
  différentes stratégies pour stocker les fichiers; inoeuds (physiques); 
  consultation avec `stat` et dérivés;
  [slides](Cours/cours_4.pdf) +
  [programmes](Cours/code-cours-4) présentés en cours

* cours nº5 : organisation du SGF; ouverture et parcours de répertoires;
  [slides](Cours/cours_5.pdf) +
  [programmes](Cours/code-cours-5) présentés en cours

* cours nº6 : organisation du SGF, suite et fin; début du cours sur les
  processus; [slides](Cours/cours_6.pdf) +
  [programmes](Cours/code-cours-6) présentés en cours

* cours nº7 : processus, suite; [slides](Cours/cours_7.pdf) +
  [programmes](Cours/code-cours-7) présentés en cours (+ un exemple
  encore plus simple de `fork`)


# TD et TP

* [énoncé de TP nº1](TP/TP1/tp1.pdf) : `ssh` et rappels
* [énoncé de TP nº2](TP/TP2/tp2.md) : entrées-sorties
* [énoncé de TP nº3](TP/TP3/tp3.md) : entrées-sorties, encore
* [énoncé de TP nº4](TP/TP4/tp4.md) : manipulation d'archives `tar`
* [énoncé de TP nº5](TP/TP5/tp5.md) : manipulation d'archives `tar`, suite


# Projet

Le [sujet du projet](Projet/projet.md) est disponible.

Par ailleurs, nous avons ouvert un [serveur
discord](https://discord.gg/DsAWzppbCG) avec en particulier un salon
dédié aux questions concernant le projet, et un autre pour faciliter les
prises de contact si vous avez du mal à constituer une équipe.

# Sujets d'examen

* [échantillon de questions de QCM](echantillon-qcm.pdf)

