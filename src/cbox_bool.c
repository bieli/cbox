#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cbox.h"

void* bool_clone(const void* data) {
    bool* copy = malloc(sizeof(bool));
    *copy = *(bool*)data;
    return copy;
}

void bool_destroy(void* data) {
    free(data);
}

void bool_print(const void* data) {
    printf("bool: %s", *(bool*)data ? "true" : "false");
}

void bool_serialize_json(const void* data, char* out, size_t size) {
    snprintf(out, size, "{\"type\":\"bool\",\"value\":%s}", *(bool*)data ? "true" : "false");
}

CBoxTrait CBOX_BOOL_TRAIT = {
    .clone = bool_clone,
    .destroy = bool_destroy,
    .print = bool_print,
    .serialize_json = bool_serialize_json
};
