#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cbox.h"
#include "cbox_container.h"
#include "cbox_decimal.h"
#include "cbox_array.h"

// Helper to create a boxed Decimal
CBox* make_decimal(int whole, int fraction) {
    Decimal d = { whole, fraction };
    return CBOX_WRAP_DECIMAL(d);
}

// Helper to create a boxed IntArray
CBox* make_intarray(const int* values, size_t length) {
    IntArray* arr = intarray_create(length, values);
    return CBOX_WRAP_INTARRAY(arr);
}

int test_container_creation_and_add() {
    CBoxContainer* container = cbox_container_create(2);
    if (!container || container->count != 0 || container->capacity != 2) return 1;

    CBox* d1 = make_decimal(1, 99);
    CBox* d2 = make_decimal(2, 50);
    CBox* d3 = make_decimal(3, 25);

    if (cbox_container_add(container, d1) != 0) return 2;
    if (cbox_container_add(container, d2) != 0) return 3;
    if (cbox_container_add(container, d3) != 0) return 4; // triggers realloc

    if (container->count != 3 || container->capacity < 3) return 5;

    cbox_free(CBOX_WRAP_CONTAINER(container));
    return 0;
}

int test_container_clone() {
    CBoxContainer* container = cbox_container_create(1);
    CBox* arr = make_intarray((int[]){10, 20}, 2);
    CBox* dec = make_decimal(5, 75);
    cbox_container_add(container, arr);
    cbox_container_add(container, dec);

    CBox* box = CBOX_WRAP_CONTAINER(container);
    CBox* clone = cbox_clone(box);
    if (!clone || clone->parent != box) return 1;

    CBoxContainer* cloned_data = (CBoxContainer*)clone->data;
    if (cloned_data->count != 2) return 2;

    cbox_free(box);
    cbox_free(clone);
    return 0;
}

int test_container_print() {
    CBoxContainer* container = cbox_container_create(2);
    cbox_container_add(container, make_decimal(3, 14));
    cbox_container_add(container, make_intarray((int[]){1, 2, 3}, 3));

    CBox* box = CBOX_WRAP_CONTAINER(container);
    printf("Expected: CBoxContainer[2] with nested values\nActual:\n");
    cbox_print(box);
    cbox_free(box);
    return 0;
}

int test_container_json() {
    CBoxContainer* container = cbox_container_create(1);
    cbox_container_add(container, make_decimal(7, 77));
    CBox* box = CBOX_WRAP_CONTAINER(container);

    char json[256];
    cbox_to_json(box, json, sizeof(json));
    if (!strstr(json, "\"type\":\"CBoxContainer\"") || !strstr(json, "\"count\":1")) return 1;

    cbox_free(box);
    return 0;
}

int main() {
    int failed = 0;

    printf("Running test_container_creation_and_add...\n");
    failed += test_container_creation_and_add();

    printf("Running test_container_clone...\n");
    failed += test_container_clone();

    printf("Running test_container_print...\n");
    failed += test_container_print();

    printf("Running test_container_json...\n");
    failed += test_container_json();

    if (failed == 0) {
        printf("[   OK   ] All container tests passed.\n");
    } else {
        printf("[ FAILED ] %d container test(s) failed.\n", failed);
    }

    return failed;
}
