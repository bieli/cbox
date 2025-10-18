#ifndef CBOX_REGISTRY_H
#define CBOX_REGISTRY_H

#include "cbox.h"

#define MAX_CBOX_TYPES 32

typedef struct {
    CBoxType type;
    CBoxTrait* trait;
} CBoxRegistryEntry;

typedef struct {
    CBoxRegistryEntry entries[MAX_CBOX_TYPES];
    size_t count;
} CBoxRegistry;

void cbox_registry_init(CBoxRegistry* registry);
int  cbox_registry_register(CBoxRegistry* registry, CBoxType type, CBoxTrait* trait);
CBoxTrait* cbox_registry_get(CBoxRegistry* registry, CBoxType type);

#endif

