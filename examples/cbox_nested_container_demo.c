#include <stdio.h>
#include <stdlib.h>
#include "cbox.h"
#include "cbox_container.h"
#include "cbox_decimal.h"
#include "cbox_array.h"
#include "cbox_debug.h"

int main() {
    // Create first inner container
    CBoxContainer* inner1 = cbox_container_create(2);

    Decimal d1 = {1, 11};
    Decimal d2 = {2, 22};
    cbox_container_add(inner1, CBOX_WRAP_DECIMAL(d1));
    cbox_container_add(inner1, CBOX_WRAP_DECIMAL(d2));

    CBox* boxed_inner1 = CBOX_WRAP_CONTAINER(inner1);

    // Create second inner container
    int values[] = {100, 200, 300};
    IntArray* arr = intarray_create(3, values);

    CBoxContainer* inner2 = cbox_container_create(2);
    cbox_container_add(inner2, CBOX_WRAP_INTARRAY(arr));

    Decimal d3 = {3, 33};
    cbox_container_add(inner2, CBOX_WRAP_DECIMAL(d3));

    CBox* boxed_inner2 = CBOX_WRAP_CONTAINER(inner2);

    // Create outer container and add both inner containers
    CBoxContainer* outer = cbox_container_create(2);
    cbox_container_add(outer, boxed_inner1);
    cbox_container_add(outer, boxed_inner2);

    CBox* boxed_outer = CBOX_WRAP_CONTAINER(outer);

    // Print full nested structure
    printf("== Nested container structure:\n");
    cbox_print(boxed_outer);

    // Serialize to JSON (basic summary)
    // size_t buffer_size = 4096;
    // char* json = malloc(buffer_size);
    // if (json) {
    //     cbox_to_json(boxed_outer, json, buffer_size);
    //     printf("== JSON summary: %s\n", json);
    //     free(json);
    // }
    char json[CBOX_CONTAINER_JSON_MAX_LEN];
    cbox_to_json(boxed_outer, json, sizeof(json));
    printf("== JSON summary: %s\n", json);

    // Clean up
    cbox_free(boxed_outer);
    return 0;
}
