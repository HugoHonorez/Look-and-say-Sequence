// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "calcul.h"
#include "led.h"

int debug_level = 0;
int only_longest = 0;
FILE *input_file;
FILE *output_file;

typedef struct Candidate {
	char *value;
	int iterations;
	struct Candidate *next;
} Candidate;

void free_candidates(Candidate *first) {
	while (first != NULL) {
		Candidate *temp = first;
		first = first -> next;
		free(temp -> value);
		free(temp);
	}
}

int run(void) {
	// Buffer de 4096 puisque maximum 4000 caractères par lignes (on prend une marge)
	char buffer[4096];

	Candidate *candidates_list = NULL;
	int best_len = -1;
	int best_digits = -1;

	DEBUG(1, "Demarrage du traitement des lignes");

	while (fgets(buffer, sizeof(buffer), input_file) != NULL) {
		size_t len = strlen(buffer);
		if (len > 0 && buffer[len - 1] == '\n') {
			buffer[len - 1] = '\0';
			len--;
		}

		if (len == 0) {
			DEBUG(2, "Erreur: Ligne vide");
			free_candidates(candidates_list);
			return 1;
		}

		DEBUG(2, "Traitement d'une ligne: lecture buffer");

		// On cherche où se trouve l'espace vide dans le buffer pour séparer les deux chaînes
		char *separator = strchr(buffer, ' ');

		if (separator == NULL) {
			DEBUG(0, "Erreur ligne: Format invalide, pas d'espace trouvé: '%s'", buffer);
			free_candidates(candidates_list);
			return 1;
		}

		*separator = '\0';

		char *value = buffer;

		if (*value == '\0') {  // value est une chaîne vide
			DEBUG(0, "Erreur ligne: La valeur est vide: '%s'", buffer);
			free_candidates(candidates_list);
			return 1;
		}

		char *iterations_str = separator + 1;
		if (*iterations_str == '\0') {
			DEBUG(0, "Erreur ligne: Nombre d'itérations manquant: '%s'", buffer);
			free_candidates(candidates_list);
			return 1;
		}
		int iterations = atoi(iterations_str);

		char *result = solve(value, iterations);

		if (result == NULL) {
			DEBUG(0, "Erreur: solve() a retourné NULL pour la ligne: '%s'", buffer);
			free_candidates(candidates_list);
			return 1;
		}

		if (!only_longest) {
			size_t len = strlen(result);

                        size_t written = fwrite(result, 1, len, output_file);

                        if (written < len) {
                                DEBUG(0, "Erreur: impossible d'écrire le resultat");
                                free(result);
                                return 1;
                        }

                        fputc('\n', output_file);

                        free(result);
		} else {
			size_t value_len = separator - buffer;

			int current_len = strlen(result);
			int current_digits = count_digits(result);

			DEBUG(2, "Ligne: '%s' -> Len: %d, Digits: %d", buffer, current_len, current_digits);

			free(result);

			if (current_digits > best_digits || (current_digits == best_digits && current_len > best_len)) {
				DEBUG(2, "Nouveau meilleur candidat trouvé ! (Ancien best_digits: %d)", best_digits);

				free_candidates(candidates_list);
				candidates_list = NULL;

				best_len = current_len;
				best_digits = current_digits;

				Candidate *candidate = malloc(sizeof(Candidate));
				if (candidate == NULL) {
					DEBUG(0, "Erreur: malloc échoué pour Candidate");
					return 1;
				}

				candidate->value = malloc((value_len + 1) * sizeof(char));
				if (candidate->value == NULL) {
					DEBUG(0, "Erreur: malloc échoué pour Candidate->value");
					free(candidate);
					return 1;
				}
				strcpy(candidate->value, value);
				candidate -> iterations = iterations;
				candidate -> next = NULL;
				candidates_list = candidate;

			} else if (current_digits == best_digits && current_len == best_len) {
				DEBUG(2, "Candidat ex-aequo");

				Candidate *candidate = malloc(sizeof(Candidate));
				if (candidate == NULL) {
					DEBUG(0, "Erreur: malloc échoué pour Candidate (ex-aequo)");
					free_candidates(candidates_list);
					return 1;
				}

				candidate->value = malloc((value_len + 1) * sizeof(char));
				if (candidate->value == NULL) {
					DEBUG(0, "Erreur: malloc échoué pour Candidate->value (ex-aequo)");
					free(candidate);
					free_candidates(candidates_list);
					return 1;
				}

				strcpy(candidate->value, value);
				candidate -> iterations = iterations;
				candidate -> next = candidates_list;
				candidates_list = candidate;
			}
		}
	}
	if (only_longest && candidates_list != NULL) {
		DEBUG(1, "Fin de lecture. Traitement des meilleurs...");

		if (candidates_list == NULL) {
			DEBUG(1, "Aucun candidat valide trouvé.");
		} else {

			Candidate *current = candidates_list;
			while (current != NULL) {
				DEBUG(2, "Recalcul du vainqueur : %s (%d itérations)", current->value, current->iterations);

				char *result = solve(current -> value, current -> iterations);
				if (result != NULL) {
					size_t len = strlen(result);

                        		size_t written = fwrite(result, 1, len, output_file);

                        		if (written < len) {
                                		DEBUG(0, "Erreur: impossible d'écrire le résultat");
                                		free(result);
                                		return 1;
                        		}

                        		fputc('\n', output_file);

                        		free(result);
				} else {
					DEBUG(0, "Erreur: Impossible de recalculer le résultat final");
					free_candidates(candidates_list);
					return 1;
				}
				current = current -> next;
			}
			free_candidates(candidates_list);
		}
	}
	DEBUG(1, "Traitement terminé avec succès");
	return 0;
}

int main(int argc, char *argv[]) {

	DEBUG(1, "Lancement...");

	DEBUG(1, "Initialisation des LEDs...");
	if (initLed() != 0) {
		DEBUG(0, "Erreur: Impossible d'initialiser les LEDs");
		return 1;
	}

	setBlueLed();

	char *input_path = NULL;
	char *output_path = NULL;

	DEBUG(1, "Traitement des options");
	// On démarre à i=1 pour sauter le nom du programme
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--input") == 0) {
			if (i + 1 < argc) input_path = argv[++i];
			else {
				DEBUG(0, "Erreur: Arugment de l'option --input manquante");
				setRedLed();
				return 1;
			}
		} else if (strcmp(argv[i], "--output") == 0) {
			if (i + 1 < argc) output_path = argv[++i];
			else {
				DEBUG(0, "Erreur: Arugment de l'option --output manquante");
				setRedLed();
				return 1;
			}
		} else if (strcmp(argv[i], "--only-longest") == 0) {
			only_longest = 1;
		} else if (strcmp(argv[i], "--debug") == 0) {
			if (i + 1 < argc) debug_level = atoi(argv[++i]);
			else {
				DEBUG(0, "Erreur: Arugment de l'option --debug manquante");
				setRedLed();
				return 1;
			}
		} else {
			DEBUG(0, "Erreur: Options inconnues");
			setRedLed();
			return 1;
		}
	}

	input_file = stdin;
	output_file = stdout;

	if (input_path != NULL) {
		input_file = fopen(input_path, "r");
		if (input_file == NULL) {
			DEBUG(0, "Erreur: Impossible d'ouvrir le fichier d'input");
			setRedLed();
			return 1;
		}
	}

	if (output_path != NULL) {
		output_file = fopen(output_path, "w");
		if (output_file == NULL) {
			if (input_path) fclose(input_file);
			DEBUG(0, "Erreur: Impossible d'ouvrir le fichier d'output");
			setRedLed();
			return 1;
		}
	}

	// On lance le coeur du projet
	DEBUG(1, "Lancement du programme...");
	int ret = run();

	// On ferme bien les fichiers
	if (input_path != NULL) {
		fclose(input_file);
		DEBUG(2, "Fermeture fichier input");
	}
	if (output_path != NULL) {
		fclose(output_file);
		DEBUG(2, "Fermeture fichier output");
	}

	if (ret != 0) {
        	DEBUG(0, "Le programme s'est terminé avec des erreurs");
			setRedLed();
    } else {
			setGreenLed():
	}
	
	clearLed();

	return ret;
}
