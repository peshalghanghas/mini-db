#ifndef HASH_H
#define HASH_H

void hash_init();
void hash_put(const char* key, long offset);
long hash_get(const char* key);
void hash_remove(const char* key);
void hash_free();

#endif
