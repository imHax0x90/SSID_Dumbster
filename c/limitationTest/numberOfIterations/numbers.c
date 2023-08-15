/*
This file is designed to try a certain amount of attempts for connecting to a known network.
possible passwords :
    "AZerty000"
    "ABcdef000"
    "MeTooISuck"
    "IAintAPassword@!?.:"
    "azeRTy!*"

Network name : "PlsDontTalk"

So for each password we try the follwing command line : 
    sudo iwconfig wlp0s20f3 essid "PlsDontTalk" key s:"password"
    sudo dhclient wlp0s20f3

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 256

int main() {
    const char *ssid = "PlsDontTalk";
    const char *passwords[] = {
        "AZerty000",
        "ABcdef000",
        "MeTooISuck",
        "azeRTy!*000?/*",
        "IAintAPassword@!?.:"
    };
    int numPasswords = sizeof(passwords) / sizeof(passwords[0]);

    char command[BUFFER_SIZE];
    int connected = 0;

    for (int i = 0; i < numPasswords && !connected; i++) {
        snprintf(command, BUFFER_SIZE, "sudo iw dev wlp0s20f3 connect \"%s\" key 0:%s", ssid, passwords[i]);

        int system_result = system(command);

        if (WIFEXITED(system_result) && WEXITSTATUS(system_result) == 0) {
            printf("Successfully connected to \"%s\" with password \"%s\".\n", ssid, passwords[i]);
            connected = 1;
        } else {
            perror("Connection attempt failed");
        }

        // If not connected, wait a bit before the next attempt
        if (!connected && i < numPasswords - 1) {
            usleep(1000000); // 1 second
        }
    }

    if (!connected) {
        printf("Unable to connect to \"%s\" with any of the provided passwords.\n", ssid);
    }

    return 0;
}

