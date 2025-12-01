// debug.h
#ifndef DEBUG_H // Si DEBUG_H n'est pas défini
#define DEBUG_H // Alors on le définit maintenant

#include <stdio.h>

extern int debug_level;

// Macro
#define DEBUG(level, fmt, ...) \
    do { \
        if (debug_level >= level) { \
            fprintf(stderr, "[DEBUG:%d] " fmt "\n", level, ##__VA_ARGS__); \
        } \
    } while (0)

#endif
