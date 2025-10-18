#include <stdio.h>
#include "cbox.h"
#include "cbox_registry.h"
#include "cbox_debug.h"

int main() {
    CBoxRegistry registry;
    cbox_registry_init(&registry);
    cbox_registry_register(&registry, CBOX_INT, &CBOX_INT_TRAIT);

    int x = 42;
    CBoxTrait* trait = cbox_registry_get(&registry, CBOX_INT);
    CBox* box = cbox_new_with_trait(&x, sizeof(int), CBOX_INT, trait);

    char json[128];
    cbox_to_json(box, json, sizeof(json));
    printf("JSON: %s\n", json);

    cbox_free(box);
    return 0;
}

