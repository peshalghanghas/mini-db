#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

#define TABLE_SIZE 1024

typedef struct HashNode {
    char* key;
    long offset;
    struct HashNode* next;
} HashNode;

static HashNode* table[TABLE_SIZE];

static unsigned int hash_function(const char* key) {
    unsigned int hash = 5381;
    while (*key) {
        hash = ((hash << 5) + hash) + *key;
        key++;
    }
    return hash % TABLE_SIZE;
}

void hash_init() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = NULL;
    }
}

void hash_put(const char* key, long offset) {
    unsigned int index = hash_function(key);

    HashNode* node = table[index];
    while (node) {
        if (strcmp(node->key, key) == 0) {
            node->offset = offset;
            return;
        }
        node = node->next;
    }

    HashNode* new_node = malloc(sizeof(HashNode));
    new_node->key = strdup(key);
    new_node->offset = offset;
    new_node->next = table[index];
    table[index] = new_node;
}

long hash_get(const char* key) {
    unsigned int index = hash_function(key);
    HashNode* node = table[index];

    while (node) {
        if (strcmp(node->key, key) == 0)
            return node->offset;
        node = node->next;
    }

    return -1;
}

void hash_remove(const char* key) {
    unsigned int index = hash_function(key);
    HashNode* node = table[index];
    HashNode* prev = NULL;

    while (node) {
        if (strcmp(node->key, key) == 0) {
            if (prev)
                prev->next = node->next;
            else
                table[index] = node->next;

            free(node->key);
            free(node);
            return;
        }
        prev = node;
        node = node->next;
    }
}

void hash_free() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* node = table[i];
        while (node) {
            HashNode* temp = node;
            node = node->next;
            free(temp->key);
            free(temp);
        }
    }
}

void hash_list() {
    printf("Stored keys:\n");

    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* node = table[i];
        while (node) {
            printf("- %s\n", node->key);
            node = node->next;
        }
    }
}
