#include "cbox_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* intarray_clone(const void* data) {
    const IntArray* arr = (const IntArray*)data;
    IntArray* copy = malloc(sizeof(IntArray));
    if (!copy) return NULL;

    copy->length = arr->length;
    copy->values = malloc(sizeof(int) * arr->length);
    if (!copy->values) {
        free(copy);
        return NULL;
    }

    memcpy(copy->values, arr->values, sizeof(int) * arr->length);
    return copy;
}

void intarray_destroy(void* data) {
    IntArray* arr = (IntArray*)data;
    free(arr->values);
    free(arr);
}

void intarray_print(const void* data) {
    const IntArray* arr = (const IntArray*)data;
    printf("IntArray[%zu]: ", arr->length);
    for (size_t i = 0; i < arr->length; ++i)
        printf("%d ", arr->values[i]);
    printf("\n");
}

void intarray_serialize_json(const void* data, char* out, size_t maxlen) {
    const IntArray* arr = (const IntArray*)data;
    size_t pos = 0;
    pos += snprintf(out + pos, maxlen - pos, "{\"values\":[");
    for (size_t i = 0; i < arr->length && pos < maxlen - 1; ++i) {
        pos += snprintf(out + pos, maxlen - pos, "%d%s", arr->values[i], (i < arr->length - 1) ? "," : "");
    }
    snprintf(out + pos, maxlen - pos, "]}");
}

CBoxTrait CBOX_INTARRAY_TRAIT = {
    .clone = intarray_clone,
    .destroy = intarray_destroy,
    .print = intarray_print
    // TODO: add .serialize = intarray_serialize_json to extends CBoxTrait
};

