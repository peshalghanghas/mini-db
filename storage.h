#ifndef STORAGE_H
#define STORAGE_H

void db_open(const char* filename);
void db_close();
void db_insert(const char* key, const char* value);
char* db_select(const char* key);
void db_delete(const char* key);
void db_rebuild_index();

#endif
