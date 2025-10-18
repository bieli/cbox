#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cbox.h"
#include "cbox_array.h"

// Helper to check if a value appears in JSON
int json_has_value(const char* json, int value) {
    char pattern[16];
    snprintf(pattern, sizeof(pattern), "%d", value);
    return strstr(json, pattern) != NULL;
}

int test_box_intarray() {
    IntArray* arr = malloc(sizeof(IntArray));
    arr->length = 3;
    arr->values = malloc(sizeof(int) * arr->length);
    arr->values[0] = 10;
    arr->values[1] = 20;
    arr->values[2] = 30;

    CBox* box = CBOX_WRAP_INTARRAY(arr);
    if (!box || box->size != sizeof(IntArray)) return 1;

    IntArray* data = (IntArray*)box->data;
    if (data->length != 3 || data->values[1] != 20) return 2;

    cbox_free(box);
    return 0;
}

int test_clone_intarray() {
    IntArray* arr = malloc(sizeof(IntArray));
    arr->length = 2;
    arr->values = malloc(sizeof(int) * arr->length);
    arr->values[0] = 5;
    arr->values[1] = 15;

    CBox* box = CBOX_WRAP_INTARRAY(arr);
    CBox* clone = cbox_clone(box);
    if (!clone || clone->parent != box) return 1;

    IntArray* cp = (IntArray*)clone->data;
    if (cp->length != 2 || cp->values[1] != 15) return 2;

    cbox_free(box);
    cbox_free(clone);
    return 0;
}

int test_print_intarray() {
    IntArray* arr = malloc(sizeof(IntArray));
    arr->length = 3;
    arr->values = malloc(sizeof(int) * arr->length);
    arr->values[0] = 1;
    arr->values[1] = 2;
    arr->values[2] = 3;

    CBox* box = CBOX_WRAP_INTARRAY(arr);
    printf("Expected: IntArray[3]: 1 2 3\nActual: ");
    cbox_print(box);
    cbox_free(box);
    return 0;
}

int test_json_intarray() {
    IntArray* arr = malloc(sizeof(IntArray));
    arr->length = 2;
    arr->values = malloc(sizeof(int) * arr->length);
    arr->values[0] = 100;
    arr->values[1] = 200;

    CBox* box = CBOX_WRAP_INTARRAY(arr);
    char json[256];
    cbox_to_json(box, json, sizeof(json));
    if (!json_has_value(json, 100) || !json_has_value(json, 200)) return 1;

    cbox_free(box);
    return 0;
}

int test_destroy_intarray() {
    IntArray* arr = malloc(sizeof(IntArray));
    arr->length = 2;
    arr->values = malloc(sizeof(int) * arr->length);
    arr->values[0] = 7;
    arr->values[1] = 14;

    CBox* box = cbox_new_with_trait(arr, sizeof(IntArray), CBOX_ARRAY, &CBOX_INTARRAY_TRAIT);
    cbox_free(box); // should free both struct and values
    return 0;
}

int main() {
    int failed = 0;

    printf("Running test_box_intarray...\n");
    failed += test_box_intarray();

    printf("Running test_clone_intarray...\n");
    failed += test_clone_intarray();

    printf("Running test_print_intarray...\n");
    failed += test_print_intarray();

    printf("Running test_json_intarray...\n");
    failed += test_json_intarray();

    printf("Running test_destroy_intarray...\n");
    failed += test_destroy_intarray();

    if (failed == 0) {
        printf("[   OK   ] All intarray tests passed.\n");
        return 0;
    } else {
        printf("[ FAILED ] %d test(s) failed.\n", failed);
        return 1;
    }
}
