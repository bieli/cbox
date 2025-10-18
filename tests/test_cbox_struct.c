#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cbox.h"
#include "cbox_struct.h"
#include "test_utils.h"

// Helper to check if JSON contains a key-value pair
int json_has(const char* json, const char* key, const char* value) {
    char pattern[128];
    snprintf(pattern, sizeof(pattern), "\"%s\":\"%s\"", key, value);
    return strstr(json, pattern) != NULL;
}

int test_box_person() {
    Person p = { "Alice", 30 };
    CBox* box = CBOX_WRAP_PERSON(p);
    if (!box || box->size != sizeof(Person)) return 1;
    Person* data = (Person*)box->data;
    if (strcmp(data->name, "Alice") != 0 || data->age != 30) return 2;
    cbox_free(box);
    return 0;
}

int test_clone_person() {
    Person p = { "Bob", 25 };
    CBox* box = CBOX_WRAP_PERSON(p);
    CBox* clone = cbox_clone(box);
    if (!clone || clone->parent != box) return 1;
    Person* cp = (Person*)clone->data;
    if (strcmp(cp->name, "Bob") != 0 || cp->age != 25) return 2;
    cbox_free(box);
    cbox_free(clone);
    return 0;
}

int test_print_person() {
    Person p = { "Charlie", 40 };
    CBox* box = CBOX_WRAP_PERSON(p);
    printf("Expected: Person{name: Charlie, age: 40}\nActual: ");
    cbox_print(box);
    cbox_free(box);
    return 0;
}

int test_json_person() {
    Person p = { "Dana", 22 };
    CBox* box = CBOX_WRAP_PERSON(p);
    char json[256];
    cbox_to_json(box, json, sizeof(json));
    if (!json_has(json, "name", "Dana") || !strstr(json, "\"age\":22")) return 1;
    cbox_free(box);
    return 0;
}

int test_destroy_person() {
    Person* p = malloc(sizeof(Person));
    strcpy(p->name, "Eve");
    p->age = 35;
    CBox* box = cbox_new_with_trait(p, sizeof(Person), CBOX_STRUCT, &CBOX_PERSON_TRAIT);
    cbox_free(box); // should free memory
    return 0;
}

int main() {
    int failed = 0;

    printf("Running test_box_person...\n");
    failed += test_box_person();

    printf("Running test_clone_person...\n");
    failed += test_clone_person();

    printf("Running test_print_person...\n");
    failed += test_print_person();

    printf("Running test_json_person...\n");
    failed += test_json_person();

    printf("Running test_destroy_person...\n");
    failed += test_destroy_person();

    PRINT_TEST_RESULT("cbox_struct tests", "cbox_struct", failed);
    return failed;
}
