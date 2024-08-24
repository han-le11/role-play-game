#include "project.h"

// Global variable for the character database (linked list)
Character *game_database = NULL;

// Helper function to find a character by name
Character *find_character(const char *name) {
    Character *current = game_database;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// My function to compare two characters by experience (for sorting)
int compare_characters(const void *a, const void *b) {
    Character *char_a = *(Character **)a;
    Character *char_b = *(Character **)b;
    return char_b->experience - char_a->experience;  // descending order
}

// Function to add a new character
void add_character(const char *name, int hit_points, const char *weapon, int damage) {
    // Check if the character already exists
    if (find_character(name) != NULL) {
        printf("Fighter \"%s\" is already in the database.\n", name); 
        return;
    }

    // Check if the hit points and damage are valid
    if (hit_points < 1) {
        printf("HP cannot be lower than 1.\n");
        return;
    }

    // Allocate memory for the new character
    Character *new_character = (Character *)malloc(sizeof(Character));
    if (!new_character) {
        printf("ERROR: Memory allocation failed.\n");
        return;
    }

    // Allocate memory for the character's name and weapon
    new_character->name = strdup(name);
    new_character->weapon = strdup(weapon);
    new_character->hit_points = hit_points;
    new_character->experience = 0;          // Initial experience is 0
    new_character->damage = damage;         // Weapon damage
    new_character->next = game_database;    // Add to the start of the list
    game_database = new_character;
    printf("SUCCESS\n");
}

// Function to make a character attack another character
void attack(const char *attacker_name, const char *target_name) {
    Character *attacker = find_character(attacker_name);
    Character *target = find_character(target_name);

    // Error checks
    if (!attacker) {
        printf("Attacker \"%s\" does not exist.\n", attacker_name);
        return;
    }
    if (!target) {
        printf("Target \"%s\" does not exist.\n", target_name);
        return;
    }
    if (attacker->hit_points == 0) {
        printf("Attacker \"%s\" is already dead.\n", attacker_name);
        return;
    }
    if (target->hit_points == 0) {
        printf("Target \"%s\" is already dead.\n", attacker_name);
        return;
    }
    if (strcmp(attacker_name, target_name) == 0) {
        printf("Attacker \"%s\" cannot attack to itself.\n", attacker_name);
        return;
    }

    // Attack process
    printf("%s attacked %s with %s by %d damage.\n", attacker->name, target->name, attacker->weapon, attacker->damage);
    target->hit_points -= attacker->damage;
    attacker->experience += attacker->damage;

    // Make sure hit points don't go below 0
    if (target->hit_points < 0) {
        target->hit_points = 0;
    }

    // Print attack result
    printf("%s has %d hit points remaining.\n", target->name, target->hit_points);
    printf("%s gained %d experience points.\n", attacker->name, attacker->damage);
    printf("SUCCESS\n");
}

// Function to print the current game state (sorted by experience)
void print_game(void) {
    if (!game_database) {
        printf("ERROR: No characters in the game.\n");
        return;
    }

    // Count characters
    int count = 0;
    Character *current = game_database;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    // Store characters in an array for sorting
    Character **characters = (Character **)malloc(count * sizeof(Character *));
    if (!characters) {
        printf("ERROR: Memory allocation failed.\n");
        return;
    }

    current = game_database;
    for (int i = 0; i < count; i++) {
        characters[i] = current;
        current = current->next;
    }

    // Sort characters by experience (descending)
    qsort(characters, count, sizeof(Character *), compare_characters);

    // Print sorted characters
    for (int i = 0; i < count; i++) {
        printf("%s %d %d %s %d\n",
               characters[i]->name,
               characters[i]->hit_points,
               characters[i]->experience,
               characters[i]->weapon,
               characters[i]->damage);
    }

    free(characters);
    printf("SUCCESS\n");
}

// Function to save the game to a file (sorted by experience)
void save_to_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Cannot open file %s for writing.\n", filename);
        return;
    }

    // Count characters
    int count = 0;
    Character *current = game_database;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    // Store characters in an array for sorting
    Character **characters = (Character **)malloc(count * sizeof(Character *));
    if (!characters) {
        printf("ERROR: Memory allocation failed.\n");
        fclose(file);
        return;
    }

    current = game_database;
    for (int i = 0; i < count; i++) {
        characters[i] = current;
        current = current->next;
    }

    // Sort characters by experience (descending)
    qsort(characters, count, sizeof(Character *), compare_characters);

    // Write sorted characters to file
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %d %d %s %d\n",
                characters[i]->name,
                characters[i]->hit_points,
                characters[i]->experience,
                characters[i]->weapon,
                characters[i]->damage);
    }

    free(characters);
    fclose(file);
    printf("SUCCESS\n");
}

// Function to load the game from a file
void load_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open file %s for reading.\n", filename);
        return;
    }
    // Clear the existing game data
    quit_game();

    char name[100];
    int hit_points, experience, damage;
    char weapon[100];
    while (fscanf(file, "%s %d %d %s %d", name, &hit_points, &experience, weapon, &damage) == 5) {
        add_character(name, hit_points, weapon, damage);
        Character *character = find_character(name);
        if (character) {
            character->experience = experience;  // Restore experience points
        }
    }
    fclose(file);
}


// Function to free all allocated memory
void quit_game(void) {
    Character *current = game_database;
    while (current != NULL) {
        Character *next = current->next;
        free(current->name);
        free(current->weapon);
        free(current);
        current = next;
    }
    game_database = NULL;
    printf("SUCCESS\n");
}

// Function prototypes
void handle_add_character(char *input);
void handle_attack(char *input);
void handle_save_game(char *input);
void handle_load_game(char *input);

int main(void) {
    char *line = NULL;  // To store user input
    size_t len = 0;     // Size of the allocated buffer
    size_t read;        // Number of characters read

    // Main loop for the game
    while (1) {
        // Get a line of input from the user
        read = getline(&line, &len, stdin);

        // Check if the input was valid
        if (read ==  (size_t)-1) {
            break; // Exit the loop on EOF
        }

        // Trim newline character from input if it exists
        if (line[read - 1] == '\n') {
            line[read - 1] = '\0';
        }

        // Command processing based on the first character in the input
        char command = line[0];

        switch (command) {
            case 'A': // Add character
                handle_add_character(line + 1); // Pass the remaining input
                break;
            case 'H': // Attack
                handle_attack(line + 1); // Pass the remaining input
                break;
            case 'L': // Print game
                print_game();
                break;
            case 'W': // Save to file
                handle_save_game(line + 1); // Pass the remaining input
                break;
            case 'O': // Load from file
                handle_load_game(line + 1); // Pass the remaining input
                break;
            case 'Q': // Quit game
                quit_game();
                free(line); // Free the getline buffer
                return 0;
            default:
                printf("Invalid command.\n");
        }
    }

    free(line); // Free the getline buffer
    return 0;
}

// Helper function to handle the Add Character command
void handle_add_character(char *input) {
    char *name = strtok(input, " ");
    char *hp_str = strtok(NULL, " ");
    char *weapon = strtok(NULL, " ");
    char *damage_str = strtok(NULL, " ");

    if (!name || !hp_str || !weapon || !damage_str) {
        printf("Invalid add character command format.\n");
        return;
    }

    int hit_points = atoi(hp_str);
    int weapon_damage = atoi(damage_str);

    add_character(name, hit_points, weapon, weapon_damage);
}

// Helper function to handle the Attack command
void handle_attack(char *input) {
    char *attacker = strtok(input, " ");
    char *target = strtok(NULL, " ");

    if (!attacker || !target) {
        printf("Invalid attack command format.\n");
        return;
    }

    attack(attacker, target);
}

// Helper function to handle the Save to File command
void handle_save_game(char *input) {
    char *filename = strtok(input, " ");

    if (!filename) {
        printf("Invalid save game command format.\n");
        return;
    }

    save_to_file(filename);
}

// Helper function to handle the Load from File command
void handle_load_game(char *input) {
    char *filename = strtok(input, " ");

    if (!filename) {
        printf("Invalid load game command format.\n");
        return;
    }

    load_from_file(filename);
}

