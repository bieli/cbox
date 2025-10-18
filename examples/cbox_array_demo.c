#include <stdio.h>
#include <stdlib.h>
#include "cbox.h"
#include "cbox_array.h"
#include "cbox_debug.h"

int main() {
    // Allocate IntArray and its values on the heap
    IntArray* arr = malloc(sizeof(IntArray));
    arr->length = 3;
    arr->values = malloc(sizeof(int) * arr->length);
    arr->values[0] = 1;
    arr->values[1] = 2;
    arr->values[2] = 3;

    // Box the heap-allocated array
    CBox* box3 = CBOX_WRAP_INTARRAY(arr);

    CBOX_DEBUG_ALL(box3);

    CBox* clone = cbox_clone(box3);
    CBOX_DEBUG_ALL(clone);

    cbox_free(box3);
    cbox_free(clone);

    return 0;
}
