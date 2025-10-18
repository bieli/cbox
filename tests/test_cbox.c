#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cbox.h"
#include "cbox_struct.h"
#include "test_utils.h"

int test_cbox_int() {
    int value = 42;
    CBox* box = cbox_new_with_trait(&value, sizeof(int), CBOX_INT, &CBOX_INT_TRAIT);
    if (!box || *(int*)box->data != 42) return 1;

    CBox* clone = cbox_clone(box);
    if (!clone || *(int*)clone->data != 42) return 2;
    if (clone->parent != box) return 3;

    cbox_free(box);
    cbox_free(clone);
    return 0;
}

int test_cbox_json() {
    int value = 7;
    CBox* box = cbox_new_with_trait(&value, sizeof(int), CBOX_INT, &CBOX_INT_TRAIT);
    char json[256];
    cbox_to_json(box, json, sizeof(json));
    if (strstr(json, "\"value\":7") == NULL) return 1;
    cbox_free(box);
    return 0;
}

int test_cbox_int_print() {
    int value = 123;
    CBox* box = cbox_new_with_trait(&value, sizeof(int), CBOX_INT, &CBOX_INT_TRAIT);
    if (!box || !box->trait || !box->trait->print) return 1;

    printf("Expected output: CBox<int>: 123\nActual output: ");
    box->trait->print(box->data);  // Should print: CBox<int>: 123

    cbox_free(box);
    return 0;
}

int main() {
    int failed = 0;

    printf("Running test_cbox_int...\n");
    failed += test_cbox_int();

    printf("Running test_cbox_json...\n");
    failed += test_cbox_json();

    printf("Running test_cbox_int_print...\n");
    failed += test_cbox_int_print();

    PRINT_TEST_RESULT("cbox tests", "cbox", failed);
    return failed;
}
