#include <stdio.h>
#include <kv.h>
#include <string.h>
#include <assert.h>

int main() {
    kv_t *db = kv_init(16);

    kv_put(db, "name", "alice");
    kv_put(db, "city", "berlin");

    // printf("%d) %s\n", alice_idx, kv_get(db, "name"));
    // printf("%d\n", kv_delete(db, "name"));
    assert(kv_delete(db, "name") == 0);
    assert(kv_get(db, "name") == NULL);
    assert(db->count == 1);

    assert(kv_delete(db, "missing") == -1);

    kv_free(db);
}