#include <kv.h>
#include <string.h>
#include <stdlib.h>

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

size_t hash(char *val, int capacity) {
    size_t hash = 0x13371337deadbeef;

    while(*val) {
        hash ^= *val;
        hash = hash << 8;
        hash += *val;

        val++;
    }

    return hash % capacity;
}

// fn kv_put
// params:
//  - db: a pointer to the db
//  - key: a pointer to the value itself
//  - value: a pointer to the value itself
// returns: the index of the key, otherwise on
// error, returns -1, on not found, returns -2
int kv_put(kv_t *db, char *key, char *value) {
    if (!db || !key || !value) {
        return -1;
    }

    size_t index = hash(key, db->capacity);

    for(int i = 0; i < db->capacity - 1; i++) {
        size_t real_index = (index + i) % db->capacity;

        kv_entry_t *entry = &db->entries[real_index];
        
        // found the slot, occupied , and the key matches
        if (entry->key 
            && entry->key != (void*)TOMBSTONE
            && !strcmp(entry->key, key)
        ) {
            char *new_val = strdup(value);
            if (!new_val) return -1;
            free(entry->value);
            entry->value = new_val;
            return 0;
        }

        // found the slot, and its empty or tombstone
        if (!entry->key || entry->value == (void*)TOMBSTONE) {
            char *new_val = strdup(value);
            char *new_key = strdup(key);
            if (!new_val || !new_key) { 
                free(new_val);
                free(new_key);
                return -1;
            }
            entry->value = new_val;
            entry->key = new_key;
            db->count++;
            return 0;
        }
    }
    
    // the db is occupied
    return -2;
}

// fn kv_get
// params:
//  - db: a pointer to the db
//  - key: a pointer to the key value
// returns: the pointer to the key NULL if not found
char *kv_get(kv_t *db, char *key) {
    if (!db || !key) return NULL;

    size_t idx = hash(key, db->capacity);

    for (int i = 0; i < db->capacity - 1; i++) {
        size_t real_idx = (idx + i) % db->capacity;

        kv_entry_t *entry = &db->entries[real_idx];

        // is no key, therefore return nothing
        if (entry->key == NULL) {
            return NULL;
        }

        // find an entry and the keys match
        if (entry->key 
            && entry->key != (void*)TOMBSTONE
            && !strcmp(entry->key, key)
        ) {
            return entry->value;
        }
    }
    return NULL;
}

// fn kv_delete
// params:
//  - db: a pointer to the db
//  - key: a pointer to the key value
// returns: 0 if succesful
// -1 if not found
int kv_delete(kv_t *db, char *key) {
    if (!db || !key) return -1;
    
    size_t idx = hash(key, db->capacity);

    for (int i = 0; i < db->capacity - 1; i++) {
        size_t real_idx = (idx + i) % db->capacity;

        kv_entry_t *entry = &db->entries[real_idx];

        if (entry->key == NULL) {
            return -1;
        }

        if (entry->key 
          && entry->key != (void*)TOMBSTONE
          && !strcmp(entry->key, key)
        ) {
            free(entry->key);
            free(entry->value);
            db->count--;
            entry->key = (void*)TOMBSTONE;
            entry->value = NULL;

            return 0;
        }
    }

    return -1;
}

// fn kv_free
// params:
//  - db: a pointer to the db
// returns: 0 if succesful, -1 if failure
int kv_free(kv_t *db) {
    if (!db) return -1;

    for (int i = 0; i < db->capacity - 1; i++) {
        kv_entry_t *e = &db->entries[i];

        if (e->key && e->key != (void*)TOMBSTONE) {
            free(e->key);
            free(e->value);
            e->key = NULL;
            e->value = NULL;
            db->count--;
        }
    }

    free(db->entries);
    free(db);

    return 0;
}