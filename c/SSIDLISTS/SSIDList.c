#include <stdio.h>
#include <stdlib.h>

int main() {

    FILE *file = fopen("../networkWithoutPasswords.txt", "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }


    FILE *nmcli_output = popen("nmcli -t -f SSID device wifi list", "r");
    if (nmcli_output == NULL) {
        perror("Erreur lors de l'exécution de nmcli");
        fclose(file);
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), nmcli_output)) {
        fprintf(file, "%s", line); 
    }

 
    pclose(nmcli_output);
    fclose(file);

    printf("Liste des SSID enregistrée dans ../networkWithoutPasswords.txt\n");

    return 0;
}
