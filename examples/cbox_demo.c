#include <stdio.h>
#include "cbox.h"
#include "cbox_struct.h"
#include "cbox_debug.h"

int main() {
    // Example 1: Wrapping an int
    int x = 42;
    CBox* int_box = CBOX_WRAP_INT(x);
    printf("== INT BOX ==\n");
    CBOX_DEBUG_ALL(int_box);

    // Clone and print
    CBox* int_clone = cbox_clone(int_box);
    printf("== INT CLONE ==\n");
    CBOX_DEBUG_ALL(int_clone);

    // Example 2: Wrapping a float
    float f = 3.14f;
    CBox* float_box = CBOX_WRAP_FLOAT(f);
    printf("== FLOAT BOX ==\n");
    CBOX_DEBUG_ALL(float_box);

    // Example 3: Wrapping a custom struct (Person)
    Person p = { "Alice", 30 };
    CBox* person_box = CBOX_WRAP_PERSON(p);
    printf("== PERSON BOX ==\n");
    CBOX_DEBUG_ALL(person_box);

    // Cleanup
    cbox_free(int_box);
    cbox_free(int_clone);
    cbox_free(float_box);
    cbox_free(person_box);

    return 0;
}

