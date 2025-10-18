#include "cbox_debug.h"
#include "cbox_struct.h"
#include "cbox_array.h"

int main() {
    int x = 42;
    CBox* box1 = CBOX_WRAP_INT(x);
    CBOX_DEBUG_ALL(box1);

    Person p = { "Alice", 30 };
    CBox* box2 = CBOX_WRAP_PERSON(p);
    CBOX_DEBUG_ALL(box2);

    IntArray arr = { .values = malloc(3 * sizeof(int)), .length = 3 };
    arr.values[0] = 1; arr.values[1] = 2; arr.values[2] = 3;
    CBox* box3 = CBOX_WRAP_INTARRAY(&arr);
    CBOX_DEBUG_ALL(box3);

    cbox_free(box1);
    cbox_free(box2);
    cbox_free(box3);
    return 0;
}

