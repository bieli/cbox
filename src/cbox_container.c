#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cbox_container.h"

CBoxContainer* cbox_container_create(size_t initial_capacity) {
    CBoxContainer* container = malloc(sizeof(CBoxContainer));
    if (!container) return NULL;
    container->count = 0;
    container->capacity = initial_capacity;
    container->items = malloc(sizeof(CBox*) * initial_capacity);
    return container;
}

int cbox_container_add(CBoxContainer* container, CBox* item) {
    if (container->count >= container->capacity) {
        size_t new_capacity = container->capacity * 2;
        CBox** new_items = realloc(container->items, sizeof(CBox*) * new_capacity);
        if (!new_items) return -1;
        container->items = new_items;
        container->capacity = new_capacity;
    }
    container->items[container->count++] = item;
    return 0;
}

void* container_clone(const void* data) {
    const CBoxContainer* original = (const CBoxContainer*)data;
    CBoxContainer* copy = cbox_container_create(original->capacity);
    for (size_t i = 0; i < original->count; ++i) {
        copy->items[i] = cbox_clone(original->items[i]);
    }
    copy->count = original->count;
    return copy;
}

void container_destroy(void* data) {
    CBoxContainer* container = (CBoxContainer*)data;
    for (size_t i = 0; i < container->count; ++i) {
        cbox_free(container->items[i]);
    }
    free(container->items);
    free(container);
}

void container_print(const void* data) {
    const CBoxContainer* container = (const CBoxContainer*)data;
    printf("CBoxContainer[%zu]:\n", container->count);
    for (size_t i = 0; i < container->count; ++i) {
        printf("  [%zu] ", i);
        cbox_print(container->items[i]);
    }
}

void container_serialize_json(const void* data, char* out, size_t size) {
    const CBoxContainer* container = (const CBoxContainer*)data;
    char* cursor = out;
    size_t remaining = size;

    int written = snprintf(cursor, remaining, "{\"type\":\"CBoxContainer\",\"count\":%zu,\"items\":[", container->count);
    cursor += written;
    remaining -= written;

    for (size_t i = 0; i < container->count; ++i) {
        char item_json[CBOX_CONTAINER_JSON_MAX_LEN];
        if (container->items[i] && container->items[i]->trait && container->items[i]->trait->serialize_json) {
            container->items[i]->trait->serialize_json(container->items[i]->data, item_json, sizeof(item_json));
        } else {
            snprintf(item_json, sizeof(item_json), "\"<unknown>\"");
        }

        written = snprintf(cursor, remaining, "%s%s", i > 0 ? "," : "", item_json);
        cursor += written;
        remaining -= written;
    }

    snprintf(cursor, remaining, "]}");
}

CBoxTrait CBOX_CONTAINER_TRAIT = {
    .clone = container_clone,
    .destroy = container_destroy,
    .print = container_print,
    .serialize_json = container_serialize_json
};
