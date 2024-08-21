#include "project.h"

// Dynamic array of characters
Character *characters = NULL;
int num_characters = 0;

// Function to add a character
void add_character(const char *name, int hit_points, const char *weapon_name, int weapon_damage) {
    // Check if hit points are valid
    if (hit_points < 1) {
        printf("HP cannot be lower than 1.\n");
        return;
    }

    // Check if character already exists
    for (int i = 0; i < num_characters; i++) {
        if (strcmp(characters[i].name, name) == 0) {
            printf("Fighter \"%s\" is already in the database.", characters[i].name);
            return;
        }
    }

    // Reallocate space for a new character
    characters = realloc(characters, sizeof(Character) * (num_characters + 1));
    if (!characters) {
        printf("Error: Memory allocation failed\n");
        return;
    }

    // Dynamically allocate memory for name and weapon name
    characters[num_characters].name = strdup(name);
    characters[num_characters].hit_points = hit_points;
    characters[num_characters].experience = 0;
    characters[num_characters].weapon.name = strdup(weapon_name);
    characters[num_characters].weapon.damage = weapon_damage;

    num_characters++;
    printf("SUCCESS\n");
}

// My helper function to find a character by name
int find_character(const char *name) {
    for (int i = 0; i < num_characters; i++) {
        if (strcmp(characters[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Function to handle attack. The character cannot attack itself,
void attack(const char *attacker_name, const char *target_name) {
    // Check if attacker is attacking themselves
    if (strcmp(attacker_name, target_name) == 0) {
        printf("Attacker \"%s\" cannot attack to itself.", attacker_name);
        return;
    }

    int attacker_index = find_character(attacker_name);
    int target_index = find_character(target_name);

    if (attacker_index == -1 || target_index == -1) {
        printf("Error: Attacker or target does not exist\n");
        return;
    }

    Character *attacker = &characters[attacker_index];
    Character *target = &characters[target_index];

    // Apply damage and update experience
    target->hit_points -= attacker->weapon.damage;
    attacker->experience += attacker->weapon.damage;

    // Print attack results
    printf("%s attacked %s with %s by %d damage.\n", attacker->name, target->name, attacker->weapon.name, attacker->weapon.damage);
    printf("%s has %d hit points remaining.\n", target->name, target->hit_points > 0 ? target->hit_points : 0);
    printf("%s gained %d experience points.\n", attacker->name, attacker->weapon.damage);

    // if (target->hit_points <= 0) {
    //     printf("%s has died.\n", target->name);
    // }

    printf("SUCCESS\n");
}

// Function to print the game status
void print_game(void) {
    // Sort by experience (descending order)
    for (int i = 0; i < num_characters - 1; i++) {
        for (int j = i + 1; j < num_characters; j++) {
            if (characters[i].experience < characters[j].experience) {
                Character temp = characters[i];
                characters[i] = characters[j];
                characters[j] = temp;
            }
        }
    }

    // Print the characters
    for (int i = 0; i < num_characters; i++) {
        printf("%s %d %d %s %d\n", characters[i].name, characters[i].hit_points, characters[i].experience,
               characters[i].weapon.name, characters[i].weapon.damage);
    }
    printf("SUCCESS\n");
}

// Function to save the game to a file
void save_game(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: Cannot open file\n");
        return;
    }

    for (int i = 0; i < num_characters; i++) {
        fprintf(file, "%s %d %d %s %d\n", characters[i].name, characters[i].hit_points,
                characters[i].experience, characters[i].weapon.name, characters[i].weapon.damage);
    }

    fclose(file);
    printf("SUCCESS\n");
}

// Function to load the game from a file
void load_game(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file\n");
        return;
    }

    // Free existing characters
    for (int i = 0; i < num_characters; i++) {
        free(characters[i].name);
        free(characters[i].weapon.name);
    }
    free(characters);
    characters = NULL;
    num_characters = 0;

    // Load characters from the file
    char name[100], weapon_name[100];
    int hit_points, experience, weapon_damage;
    while (fscanf(file, "%s %d %d %s %d", name, &hit_points, &experience, weapon_name, &weapon_damage) == 5) {
        add_character(name, hit_points, weapon_name, weapon_damage);
        characters[num_characters - 1].experience = experience; // Set experience directly
    }

    fclose(file);
    printf("SUCCESS\n");
}

// Function to quit the game
void quit_game(void) {
    for (int i = 0; i < num_characters; i++) {
        free(characters[i].name);           // Free dynamically allocated memory for name
        free(characters[i].weapon.name);    // Free dynamically allocated memory for weapon name
    }
    free(characters);  // Free the dynamic array
    printf("SUCCESS\n");
}

#include "project.h"

int main(void) {
    char input[1000];  // Buffer to store user input

    // Infinite loop for processing commands
    while (1) {
        // Read user input
        if (!fgets(input, sizeof(input), stdin)) {
            break;  // Exit if input is not valid (EOF or error)
        }

        // Extract the command letter
        char command;
        sscanf(input, " %c", &command);

        // Process commands based on the command letter
        if (command == 'A') {
            char name[100], weapon_name[100];
            int hit_points, weapon_damage;

            // Parse and add a character
            if (sscanf(input, "A %s %d %s %d", name, &hit_points, weapon_name, &weapon_damage) == 4) {
                add_character(name, hit_points, weapon_name, weapon_damage);
            } else {
                printf("Error: Invalid command arguments\n");
            }

        } else if (command == 'H') {
            char attacker[100], target[100];

            // Parse and execute an attack
            if (sscanf(input, "H %s %s", attacker, target) == 2) {
                attack(attacker, target);
            } else {
                printf("Error: Invalid command arguments\n");
            }

        } else if (command == 'L') {
            // Print the game status
            print_game();

        } else if (command == 'W') {
            char filename[100];

            // Parse and save the game to a file
            if (sscanf(input, "W %s", filename) == 1) {
                save_game(filename);
            } else {
                printf("Error: Invalid command arguments\n");
            }

        } else if (command == 'O') {
            char filename[100];

            // Parse and load the game from a file
            if (sscanf(input, "O %s", filename) == 1) {
                load_game(filename);
            } else {
                printf("Error: Invalid command arguments\n");
            }

        } else if (command == 'Q') {
            // Quit the game and release resources
            quit_game();
            break;  // Exit the loop when 'Q' is received

        } else {
            // Handle invalid commands
            printf("Invalid command %c\n", command);
        }
    }

    return 0;  // Program ends after quitting
}

