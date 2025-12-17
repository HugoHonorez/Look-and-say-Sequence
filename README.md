# Projet Systèmes Informatiques

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

## Prérequis

* **libgpiod** : Nécessaire pour le contrôle des GPIOs sur Raspberry Pi avec le code C.
* **Python 3** : Nécessaire pour exécuter `main.py` et le script de `benchmark.py`.

## Compilation (Code C)

Pour compiler le projet et générer l'exécutable `main`, exécutez la commande suivante à la racine du projet :

```bash
make
