#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"
#include "hash.h"

#define OP_INSERT 1
#define OP_DELETE 2

static FILE* db_file = NULL;

void db_open(const char* filename) {
    db_file = fopen(filename, "ab+");
    if (!db_file) {
        perror("Failed to open database file");
        exit(1);
    }

    hash_init();
    db_rebuild_index();
}

void db_close() {
    if (db_file)
        fclose(db_file);

    hash_free();
}

void db_insert(const char* key, const char* value) {
    fseek(db_file, 0, SEEK_END);
    long offset = ftell(db_file);

    int key_size = strlen(key);
    int value_size = strlen(value);

    char op = OP_INSERT;

    fwrite(&op, 1, 1, db_file);
    fwrite(&key_size, sizeof(int), 1, db_file);
    fwrite(&value_size, sizeof(int), 1, db_file);
    fwrite(key, 1, key_size, db_file);
    fwrite(value, 1, value_size, db_file);

    fflush(db_file);

    hash_put(key, offset);
}

char* db_select(const char* key) {
    long offset = hash_get(key);
    if (offset == -1)
        return NULL;

    fseek(db_file, offset, SEEK_SET);

    char op;
    int key_size, value_size;

    fread(&op, 1, 1, db_file);
    fread(&key_size, sizeof(int), 1, db_file);
    fread(&value_size, sizeof(int), 1, db_file);

    char* key_buf = malloc(key_size + 1);
    fread(key_buf, 1, key_size, db_file);
    key_buf[key_size] = '\0';

    char* value_buf = malloc(value_size + 1);
    fread(value_buf, 1, value_size, db_file);
    value_buf[value_size] = '\0';

    free(key_buf);
    return value_buf;
}

void db_delete(const char* key) {
    fseek(db_file, 0, SEEK_END);

    char op = OP_DELETE;
    int key_size = strlen(key);
    int value_size = 0;

    fwrite(&op, 1, 1, db_file);
    fwrite(&key_size, sizeof(int), 1, db_file);
    fwrite(&value_size, sizeof(int), 1, db_file);
    fwrite(key, 1, key_size, db_file);

    fflush(db_file);

    hash_remove(key);
}

void db_rebuild_index() {
    fseek(db_file, 0, SEEK_SET);

    while (1) {
        long offset = ftell(db_file);

        char op;
        if (fread(&op, 1, 1, db_file) != 1)
            break;

        int key_size, value_size;
        fread(&key_size, sizeof(int), 1, db_file);
        fread(&value_size, sizeof(int), 1, db_file);

        char* key = malloc(key_size + 1);
        fread(key, 1, key_size, db_file);
        key[key_size] = '\0';

        if (op == OP_INSERT) {
            fseek(db_file, value_size, SEEK_CUR);
            hash_put(key, offset);
        } else if (op == OP_DELETE) {
            hash_remove(key);
        }

        free(key);
    }
}
