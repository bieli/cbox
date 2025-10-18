#include "cbox_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* intarray_clone(const void* data) {
    const IntArray* original = (const IntArray*)data;
    IntArray* copy = malloc(sizeof(IntArray));
    if (!copy) return NULL;

    copy->length = original->length;
    copy->values = malloc(sizeof(int) * copy->length);
    if (!copy->values) {
        free(copy);
        return NULL;
    }

    memcpy(copy->values, original->values, sizeof(int) * copy->length);
    return copy;
}

void intarray_destroy(void* data) {
    IntArray* arr = (IntArray*)data;
    if (arr->values) free(arr->values);
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
    size_t offset = snprintf(out, maxlen, "{\"type\":\"IntArray\",\"length\":%zu,\"values\":[", arr->length);
    for (size_t i = 0; i < arr->length && offset < maxlen - 1; ++i) {
        offset += snprintf(out + offset, maxlen - offset, "%d%s", arr->values[i], (i < arr->length - 1) ? "," : "");
    }
    snprintf(out + offset, maxlen - offset, "],\"address\":\"%p\"}", data);
}

CBoxTrait CBOX_INTARRAY_TRAIT = {
    .clone = intarray_clone,
    .destroy = intarray_destroy,
    .print = intarray_print,
    .serialize_json = intarray_serialize_json
};

