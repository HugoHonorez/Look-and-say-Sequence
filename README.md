# Look-and-say Sequence

Ce projet implémente la suite de Conway ("look-and-say") en langage C, optimisé pour Raspberry Pi. Il gère la mémoire dynamiquement pour supporter une croissance exponentielle et utilise une interface GPIO pour indiquer visuellement l'état du calcul.

Le projet inclut également une implémentation Python pour servir de référence et un outil de benchmark pour comparer les performances entre les deux langages.

## Description des fichiers

* **main.c** : Point d'entrée du programme C. Gère la lecture des arguments, l'ouverture des fichiers et l'orchestration générale.
* **calcul.c / calcul.h** : Cœur algorithmique en C. La fonction `solve` génère les itérations avec une gestion optimisée de la mémoire.
* **led.c / led.h** : Gère le contrôle matériel des LEDs via `libgpiod`.
* **debug.h** : Définit la macro de débogage pour la version C.
* **code.py** : Implémentation de l'algorithme en langage Python.
* **benchmark.py** : Script d'automatisation permettant de réaliser un benchmark comparatif entre le code C et le code Python (mesure du temps d'exécution et consommation mémoire).
* **Makefile** : Script d'automatisation pour la compilation du projet C.

## Prérequis et Installation

Avant l'exécution, installez les paquets nécessaires sur le Raspberry Pi :

1. Pour compiler le code C (libgpiod) :
   sudo apt-get update
   sudo apt-get install libgpiod-dev gcc make time

2. Pour le Benchmark :
   sudo apt-get install python3 python3-pip
   pip3 install matplotlib, pandas, dataframe_image

## Compilation (Code C)

Pour générer l'exécutable "main" :
make

Pour supprimer les fichiers compilés :
make clean

## Options disponibles

Ces options sont valables pour le programme C (./main) et le programme Python (main.py).

* --input <fichier>
  Lit les données depuis un fichier au lieu de l'entrée standard.
  Format obligatoire de chaque ligne du fichier : "chaine_depart nombre_iterations"
  Exemple de ligne : 1 40

* --output <fichier>
  Ecrit le résultat final dans le fichier spécifié au lieu de l'afficher dans la console.

* --debug <niveau>
  Active les messages de débogage sur la sortie d'erreur (stderr) (0, 1, 2).
  Par défaut 0.

* --only-longest
  N'affiche en sortie que le résultat final ayant la plus grande diversité de chiffres et la plus grande longueur parmi toutes les lignes traitées.

## Utilisation

### 1. Programme C

Syntaxe :
./main [options]

Exemples :
* Lancement simple (attente entrée clavier) :
  ./main

* Avec fichiers d'entrée et de sortie :
  ./main --input input.txt --output output.txt

* Avec débogage activé :
  ./main --debug 1

### 2. Programme Python

Syntaxe :
python3 main.py [options]

Exemples :
* Lancement simple :
  python3 main.py

* Avec fichiers d'entrée et de sortie :
  python3 main.py --input input.txt --output output.txt

* Mode "Only Longest" :
  python3 main.py --only-longest

## Benchmark

Ce script exécute séquentiellement la version C et la version Python pour comparer leur vitesse et leur consommation mémoire.

Commande :
python3 benchmark.py

Note : Le code C doit être compilé (make) avant de lancer le benchmark.

## Codes couleur LED

* Cyan / Bleu : Calcul en cours (alternance à chaque itération).
* Vert : Programme terminé avec succès.
* Rouge : Erreur critique (fichier introuvable, erreur mémoire, etc.).
