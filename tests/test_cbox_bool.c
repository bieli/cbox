#include <stdio.h>
#include "cbox.h"
#include "cbox_bool.h"
#include "test_utils.h"

int test_cbox_bool() {
    bool* val_true = cbox_bool_alloc(true);

    CBox* box_true = cbox_new_with_trait(val_true, sizeof(bool), CBOX_BOOL, &CBOX_BOOL_TRAIT);
    CBox* box_false = CBOX_WRAP_BOOL(false);  // using macro

    // Print test
    printf("🔍 Printing CBoxBool values:\n");
    cbox_print(box_true);   printf("\n");
    cbox_print(box_false);  printf("\n");

    // JSON test
    char json_true[128], json_false[128];
    cbox_to_json(box_true, json_true, sizeof(json_true));
    cbox_to_json(box_false, json_false, sizeof(json_false));

    printf("JSON true: %s\n", json_true);
    printf("JSON false: %s\n", json_false);

    // Check correctness
    if (!strstr(json_true, "\"value\":true")) return 1;
    if (!strstr(json_false, "\"value\":false")) return 2;

    cbox_free(box_true);
    cbox_free(box_false);
    return 0;
}

int main() {
    int failed = 0;

    printf("Running test_cbox_bool...\n");
    failed += test_cbox_bool();

    PRINT_TEST_RESULT("bool tests", "cbox_bool", failed);
    return failed;
}
