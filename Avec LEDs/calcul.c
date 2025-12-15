// calcul.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calcul.h"
#include "debug.h"
#include "led.h"

//Solution pour la suite de Conway pour un certain nombre d'itérations en partant d'une valeur
char *solve(const char *value, const int iterations) {
        size_t len_current_value = strlen(value);

	// Buffer
        char *current_value = malloc(len_current_value + 1);
        if (current_value == NULL) {
                DEBUG(0, "Error: malloc failed");
                return NULL;
        }
        strcpy(current_value, value);

	char *new_value = NULL;
        size_t new_value_alloc = 0; // Taille allouée

        for (int i = 0; i < iterations; i++) {

                if (i % 2 == 0) {
                        setCyanLed();
                } else {
                        setBlueLed();
                }

                size_t j = 0; // Index de lecture
                size_t len = 0; // Index d'écriture

		size_t needed_alloc = len_current_value * 2 + 32; // On prévoit assez de place avec une marge de 32

		// On augmente la taille du buffer si trop petit
		if (new_value_alloc < needed_alloc) {
			char *tmp = realloc(new_value, needed_alloc);
			if (tmp == NULL) {
				free(current_value);
				free(new_value);
				return NULL;
			}
			new_value = tmp;
			new_value_alloc = needed_alloc;
		}

		// Boucle de lecture
                while (j < len_current_value) {
                        char c = current_value[j];
                        size_t count = 0;

                        while (j < len_current_value && current_value[j] == c) {
                                j++;
				count++;
                        }

			if (count < 10) {
				new_value[len++] = count + '0'; // Si on met '0' + 1 on obtient le caractère '1'
			} else {
				len += sprintf(new_value + len, "%zu", count);
			}

			new_value[len++] = c;
                }

                new_value[len] = '\0';

		// Swap des pointeurs
                char *tmp = current_value;
                current_value = new_value;
                new_value = tmp;

                new_value_alloc = len_current_value;
		len_current_value = len;
        }

        free(new_value);
        return current_value;
}

//Cette fonction compte le nombre de chiffre différents lorsqu'on lui donne un "string"
int count_digits(const char *number) {
        int digits[10] = {0};
        int count = 0;
        for (size_t i = 0; number[i] != '\0'; i++) {
                int digit = number[i] - '0';
                if (!digits[digit]) {
                        digits[digit] = 1;
                        count++;
                }
        }
        return count;
}
