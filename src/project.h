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

// Structure for the weapon
typedef struct Weapon {
    char *name;
    int damage;
} Weapon;

// Structure for the character
typedef struct Character {
    char *name;
    int hit_points;
    int experience;
    Weapon weapon;
} Character;

// Function Declarations
void add_character(const char *name, int hit_points, const char *weapon_name, int weapon_damage);
void attack(const char *attacker_name, const char *target_name);
void print_game(void);
void save_game(const char *filename);
void load_game(const char *filename);
void quit_game(void);
int find_character(const char *name);

#endif //! _PROJECT__H_
