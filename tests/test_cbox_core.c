#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cbox.h"
#include "cbox_struct.h"
#include "test_utils.h"

// Helper for JSON validation
int json_contains(const char* json, const char* key, const char* value) {
    char pattern[128];
    snprintf(pattern, sizeof(pattern), "\"%s\":%s", key, value);
    return strstr(json, pattern) != NULL;
}

// int_clone, int_destroy
int test_int_clone_destroy() {
    int value = 99;
    void* clone = CBOX_INT_TRAIT.clone(&value);
    if (!clone || *(int*)clone != 99) return 1;
    CBOX_INT_TRAIT.destroy(clone);
    return 0;
}

// float_clone, float_destroy, float_print
int test_float_trait() {
    float value = 3.14f;
    void* clone = CBOX_FLOAT_TRAIT.clone(&value);
    if (!clone || *(float*)clone != 3.14f) return 1;

    printf("Expected float print: CBox<float>: 3.14\nActual: ");
    CBOX_FLOAT_TRAIT.print(clone);

    CBOX_FLOAT_TRAIT.destroy(clone);
    return 0;
}

// double_clone, double_destroy, double_print
int test_double_trait() {
    double value = 2.718;
    void* clone = CBOX_DOUBLE_TRAIT.clone(&value);
    if (!clone || *(double*)clone != 2.718) return 1;

    printf("Expected double print: CBox<double>: 2.718\nActual: ");
    CBOX_DOUBLE_TRAIT.print(clone);

    CBOX_DOUBLE_TRAIT.destroy(clone);
    return 0;
}

// cbox_new_with_trait, cbox_clone, cbox_print, cbox_free
int test_cbox_lifecycle() {
    int value = 42;
    CBox* box = cbox_new_with_trait(&value, sizeof(int), CBOX_INT, &CBOX_INT_TRAIT);
    if (!box || *(int*)box->data != 42) return 1;

    CBox* clone = cbox_clone(box);
    if (!clone || *(int*)clone->data != 42 || clone->parent != box) return 2;

    printf("Expected cbox_print: CBox<int>: 42\nActual: ");
    cbox_print(box);

    cbox_free(box);
    cbox_free(clone);
    return 0;
}

// int_serialize_json, float_serialize_json, double_serialize_json
int test_json_serializers() {
    char json[256];

    int i = 7;
    CBox* bi = cbox_new_with_trait(&i, sizeof(int), CBOX_INT, &CBOX_INT_TRAIT);
    cbox_to_json(bi, json, sizeof(json));
    if (!json_contains(json, "value", "7")) return 1;
    cbox_free(bi);

    float f = 1.23f;
    CBox* bf = cbox_new_with_trait(&f, sizeof(float), CBOX_FLOAT, &CBOX_FLOAT_TRAIT);
    cbox_to_json(bf, json, sizeof(json));
    if (!strstr(json, "\"value\":1.23")) return 2;
    cbox_free(bf);

    double d = 9.81;
    CBox* bd = cbox_new_with_trait(&d, sizeof(double), CBOX_DOUBLE, &CBOX_DOUBLE_TRAIT);
    cbox_to_json(bd, json, sizeof(json));
    if (!strstr(json, "\"value\":9.81")) return 3;
    cbox_free(bd);

    return 0;
}

int main() {
    int failed = 0;

    printf("Running test_int_clone_destroy...\n");
    failed += test_int_clone_destroy();

    printf("Running test_float_trait...\n");
    failed += test_float_trait();

    printf("Running test_double_trait...\n");
    failed += test_double_trait();

    printf("Running test_cbox_lifecycle...\n");
    failed += test_cbox_lifecycle();

    printf("Running test_json_serializers...\n");
    failed += test_json_serializers();

    PRINT_TEST_RESULT("cbox_core tests", "cbox_core", failed);
    return failed;
}
