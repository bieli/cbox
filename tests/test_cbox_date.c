#include <stdio.h>
#include <string.h>
#include "cbox.h"
#include "cbox_date.h"
#include "test_utils.h"

int test_cbox_date() {
    const char* input = "2020-09-05";
    CBoxDate* parsed = cbox_date_parse(input);
    if (!parsed) {
        printf("[ ERROR ] Failed to parse date: %s\n", input);
        return 1;
    }

    CBox* box = cbox_new_with_trait(parsed, sizeof(CBoxDate), CBOX_DATE, &CBOX_DATE_TRAIT);

    // Print test
    printf("Printing CBoxDate:\n");
    cbox_print(box);
    printf("\n");

    // JSON test
    char json[128];
    cbox_to_json(box, json, sizeof(json));
    printf("JSON: %s\n", json);

    if (!strstr(json, "\"value\":\"2020-09-05\"")) {
        printf("[ ERROR ] JSON serialization failed.\n");
        cbox_free(box);
        return 2;
    }

    cbox_free(box);
    return 0;
}

int main() {
    int failed = 0;
    printf("Running test_cbox_date...\n");
    failed += test_cbox_date();

    PRINT_TEST_RESULT("cbox_date tests", "cbox_date", failed);
    return failed;
}
