#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void solve(const char *value, const int iterations) {

	size_t len_value = strlen(value);
	char *current_value = malloc(len_value + 1);
	if (current_value == NULL) {
		printf("Error: malloc failed\n");
		return;
	}

	char *new_value = NULL;

	strcpy(current_value, value);

	for (int i = 0; i < iterations; i++) {

		int j = 0;
		int len = 0;

		size_t len_current_value = strlen(current_value);
		new_value = realloc(new_value, 2 * len_current_value + 1);

		while (j < len_current_value) {

			int orig_j = j;
			char c = current_value[j];
			j++;

			while (j < len_current_value && current_value[j] == c) {

				j++;

			}

			int count = j - orig_j;

			len += sprintf(new_value + len, "%d%c", count, c);

		}

		char *tmp = current_value;
		current_value = new_value;
		new_value = tmp;

	}

	printf("%s\n", current_value);
	free(current_value);
	free(new_value);
	return;

}

int main(int argc, char *argv[]) {

	if (argc != 3) {
		printf("Error: 3 args required!\n");
		return 1;
	}

	char *value = argv[1];
	int iterations = atoi(argv[2]);
	solve(value, iterations);
	return 0;

}
