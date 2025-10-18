#include "cbox_registry.h"

void cbox_registry_init(CBoxRegistry* registry) {
    registry->count = 0;
}

int cbox_registry_register(CBoxRegistry* registry, CBoxType type, CBoxTrait* trait) {
    if (registry->count >= MAX_CBOX_TYPES) return -1;
    registry->entries[registry->count++] = (CBoxRegistryEntry){ type, trait };
    return 0;
}

CBoxTrait* cbox_registry_get(CBoxRegistry* registry, CBoxType type) {
    for (size_t i = 0; i < registry->count; ++i) {
        if (registry->entries[i].type == type)
            return registry->entries[i].trait;
    }
    return NULL;
}

