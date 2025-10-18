#ifndef CBOX_ARRAY_H
#define CBOX_ARRAY_H

#include "cbox.h"

typedef struct {
    int* values;
    size_t length;
} IntArray;

void* intarray_clone(const void* data);
void  intarray_destroy(void* data);
void  intarray_print(const void* data);
void  intarray_serialize_json(const void* data, char* out, size_t maxlen);

extern CBoxTrait CBOX_INTARRAY_TRAIT;

#define CBOX_WRAP_INTARRAY(ptr) cbox_new_with_trait((ptr), sizeof(IntArray), CBOX_ARRAY, &CBOX_INTARRAY_TRAIT)

#endif // CBOX_ARRAY_H

