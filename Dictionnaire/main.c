#include <stdio.h>
#include <string.h>

void generateCombinations(FILE* outputFile, char* prefix, int prefixLength, int remainingLength) {
    if (remainingLength == 0) {
        fprintf(outputFile, "%s\n", prefix);
        return;
    }

    char possibleCharacters[] = "abcdefghijklmnopqrstuvwxyz0123456789@$+?.;:_=&#!-/*ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int numPossibleCharacters = sizeof(possibleCharacters) - 1;

    for (int i = 0; i < numPossibleCharacters; ++i) {
        prefix[prefixLength] = possibleCharacters[i];
        generateCombinations(outputFile, prefix, prefixLength + 1, remainingLength - 1);
    }
}

int main() {
    FILE* outputFile = fopen("dico.txt", "w");
    if (outputFile == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    for (int length = 4; length <= 30; ++length) {
        char prefix[length + 1];
        memset(prefix, 0, sizeof(prefix)); 

        generateCombinations(outputFile, prefix, 0, length);
    }

    fclose(outputFile);

    return 0;
}
