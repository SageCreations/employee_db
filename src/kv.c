#include <kv.h>

kv_t *kv_init(size_t capacity) {
    if (capacity == 0) return NULL;

    kv_t *table = malloc(sizeof(kv_t));
    if (table == NULL) {
        return NULL;
    }

    table->capacity = capacity;
    table->count = 0;

    table->entries = calloc(sizeof(kv_entry_t), capacity);
    if (table->entries == NULL) {
        return NULL;
    }    

    return table;
}

/*

int kv_put(kv_t *db, const char *key, const char *value) {
    if (db == NULL || key == NULL || value == NULL) {
        return -1;
    }

    db->entries[db->count].key = key;
    db->entries[db->count].value = value;
    db->count++;

    return 0;
}

char *kv_get(kv_t *db, const char *key) {
    if (db == NULL || key == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < db->count; i++) {
        if (strcmp(db->entries[i].key, key) == 0) {
            return db->entries[i].value;
        }
    }

    return NULL;
}

int kv_delete(kv_t *db, const char *key) {
    if (db == NULL || key == NULL) {
        return -1;
    }

    for (size_t i = 0; i < db->count; i++) {
        if (strcmp(db->entries[i].key, key) == 0) {
            db->entries[i].key = NULL;
            db->entries[i].value = NULL;
            db->count--;
            return 0;
        }
    }

    return -1;
}

void  kv_free(kv_t *db) {
    if (db == NULL) {
        return;
    }

    free(db->entries);
    free(db);
}


*/