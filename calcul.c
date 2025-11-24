#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calcul.h"

//Solution pour la suite de Conway pour un certain nombre d'itérations en partant d'une valeur
char *solve(const char *value, const int iterations) {

	size_t len_value = strlen(value);
	char *current_value = malloc(len_value + 1);
	if (current_value == NULL) {
		printf("Error: malloc failed\n");
		return NULL;
	}

	char *new_value = NULL;

	strcpy(current_value, value);

	size_t len_current_value = len_value;

	for (size_t i = 0; i < iterations; i++) {

		size_t j = 0;
		size_t len = 0;

		new_value = realloc(new_value, 2 * len_current_value + 1);
        	if (new_value == NULL) {
            		printf("Error: realloc failed\n");
            		free(current_value);
            		return NULL;
        	}

		while (j < len_current_value) {

			size_t orig_j = j;
			char c = current_value[j];
			j++;

			while (j < len_current_value && current_value[j] == c) {

				j++;

			}

			size_t count = j - orig_j;

			len += sprintf(new_value + len, "%zu%c", count, c);

		}

		new_value[len] = '\0';

		char *tmp = current_value;
		current_value = new_value;
		new_value = tmp;

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
