#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "storage.h"

#define MAX_INPUT 1024

int main() {
    db_open("database.db");

    char input[MAX_INPUT];

    while (1) {
        printf("db> ");
        fgets(input, MAX_INPUT, stdin);

        input[strcspn(input, "\n")] = 0;

        if (strncmp(input, "insert ", 7) == 0) {
            char* key = strtok(input + 7, " ");
            char* value = strtok(NULL, "");

            if (key && value)
                db_insert(key, value);
            else
                printf("Usage: insert <key> <value>\n");
        }

        else if (strncmp(input, "select ", 7) == 0) {
            char* key = input + 7;
            char* value = db_select(key);

            if (value) {
                printf("%s\n", value);
                free(value);
            } else {
                printf("Key not found\n");
            }
        }

        else if (strncmp(input, "delete ", 7) == 0) {
            char* key = input + 7;
            db_delete(key);
        }

        else if (strcmp(input, "exit") == 0) {
            break;
        }

        else {
            printf("Unknown command\n");
        }
    }

    db_close();
    return 0;
}
