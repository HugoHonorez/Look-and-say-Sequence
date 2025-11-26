#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "led.h"
#include "calcul.h"
#include <string.h>

char *run(FILE *input_file, FILE *output_file, int only_longest) {

        char buffer[4000];
        int nbr_digits = 0;
        int diff_digits =0;
        char *cont = NULL;


        while (fgets(buffer, sizeof(buffer), input_file) != NULL) {
                buffer[strcspn(buffer, "\n")] = 0;

                char *separator = strchr(buffer, ' ');
                if (separator != NULL) {
                        *separator = '\0';

                        char *value = buffer;
                        char *iter_str = separator + 1;

                        if (strlen(value) == 0) {
                                fprintf(stderr, "Error: The initial value is empty.\n");
                                continue;
                        }

                        if (strlen(iter_str) == 0) {
                                fprintf(stderr, "Error: The number of iterations is empty.\n");
                                continue;
                        }

                        int iterations = atoi(iter_str);

                        if (iterations < 0) {
                                fprintf(stderr, "Error: The number of iterations is invalid.\n");
                                continue;
                        }

                        char *result = solve(value, iterations);
                        if ( only_longest == 0 ) {
                                if (result != NULL) {
                                        fprintf(output_file, "%s\n", result);
                                        free(result);
                                } else {
                                        fprintf(stderr, "Error: The calculation did not work.");
                                }
                        } else {

                                if ( count_digits(result)==diff_digits && strlen(result) == nbr_digits ) {
                                        char *tempo;
                                        tempo=realloc(cont,strlen(cont)+strlen(value)+strlen(iter_str)+3 );
                                        if ( tempo==NULL ){
                                                fprintf(stderr,"Erreur mémoire");
                                                return NULL;
                                        }
                                        cont=tempo;
                                        sprintf(cont+strlen(cont), "\n%s %s",value,iter_str);
                                } else if (count_digits(result)==diff_digits && strlen(result) > nbr_digits ) {
                                        free(cont);
                                        cont = NULL;
                                        cont=realloc(cont,strlen(value)+strlen(iter_str)+2);
                                        if ( cont==NULL ){
                                                fprintf(stderr,"Erreur mémoire");
                                                return NULL;
                                        }
                                        sprintf(cont, "%s %s",value,iter_str);
                                        nbr_digits=strlen(result);
                                } else if ( count_digits(result)>diff_digits ){
                                        free(cont);
                                        cont = NULL;
                                        cont=realloc(cont,strlen(value)+strlen(iter_str)+2);
                                        if ( cont==NULL ){
                                                fprintf(stderr,"Erreur mémoire");
                                                return NULL;
                                        }
                                        sprintf(cont, "%s %s",value,iter_str);
                                        nbr_digits=strlen(result);
                                        diff_digits=count_digits(result);
                                }
                                free(result);
                                //printf("%d",nbr_digits);
                        }
                } else {
                        fprintf(stderr, "Error: Invalid format\n");
                        continue;
                }
        }
        return cont;
}


int main(int argc, char *argv[]) {

        char *input_path = NULL;
        char *output_path = NULL;
        int only_longest = 0;
        int debug = 0;

        for (int i = 1; i < argc; i++) {
                if (strcmp(argv[i], "--input") == 0) {
                        if (i + 1 < argc) {
                                input_path = argv[++i];
                        } else {
                                fprintf(stderr, "Error: The --input option expects a file path.\n");
                                return 1;
                        }
                } else if (strcmp(argv[i], "--output") == 0) {
                        if (i + 1 < argc) {
                                output_path = argv[++i];
                        } else {
                                fprintf(stderr, "Error: The --output option expects a file path.\n");
                                return 1;
                        }
                } else if (strcmp(argv[i], "--only-longest") == 0) {

                        only_longest = 1;

                } else if (strcmp(argv[i], "--debug") == 0) {
                        if (i + 1 < argc) {
                                debug = atoi(argv[++i]);
                        } else {
                                fprintf(stderr, "Error: The --debug option expects a number.\n");
                                return 1;
                        }
                }
        }

        FILE *input_file = stdin;
        if (input_path != NULL) {
                input_file = fopen(input_path, "r");
                if (input_file == NULL) {
                        fprintf(stderr, "Error: Unable to open the input file %s.\n", input_path);
                        return 1;
                }
        }

        FILE *output_file = stdout;
        if (output_path != NULL) {
                output_file = fopen(output_path, "w");
                if (output_file == NULL) {
                        fprintf(stderr, "Error: Unable to open the output file %s.\n", output_path);
                        if (input_path != NULL) {
                                fclose(input_file);
                        }
                        return 1;
                }
        }

        char *second_input=run(input_file, output_file,only_longest);
        if (only_longest == 1) {
                if (second_input != NULL) {
                        FILE *flux_memoire = fmemopen(second_input, strlen(second_input), "r");

                        if (flux_memoire != NULL) {
                                char *second_output = run(flux_memoire, output_file, 0);
                                free(second_output);
                                fclose(flux_memoire);

                        } else {
                                fprintf(stderr, "Erreur: Echec de fmemopen\n");
                        }
                }
        }

        free(second_input);
        if (input_path != NULL) {

                fclose(input_file);

        }

        if (output_path != NULL) {

                fclose(output_file);

        }

        return 0;

}
