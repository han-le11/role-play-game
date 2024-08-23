#ifndef PROJECT_H
#define PROJECT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Implement function strdup() if it is not supported by the compiler
#ifndef _POSIX_C_SOURCE
char* strdup(const char *str) {
    char *dup = malloc(strlen(str) + 1);  // Allocate memory for the copy
    if (dup != NULL) {
        strcpy(dup, str);  // Copy the string into the new memory
    }
    return dup;
}
#endif

// Structure for a character in the game
typedef struct Character { 
    char *name;             // Character's name
    int hit_points;         // Character's hit points
    int experience;         // Character's experience points
    char *weapon;           // Character's weapon name
    int damage;             // Weapon damage
    struct Character *next; // Pointer to the next character in the list
} Character;

// Function declarations
void add_character(const char *name, int hit_points, const char *weapon, int damage);
void attack(const char *attacker_name, const char *target_name);
void print_game(void);
void quit_game(void);
void save_to_file(const char *filename);
void load_from_file(const char *filename);
Character *find_character(const char *name);

#endif
