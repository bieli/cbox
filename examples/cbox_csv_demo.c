#include "cbox_csv.h"

int main() {
    CBoxCSV* csv = read_csv("examples/data.csv", ',', false, 100);
    if (!csv) {
        fprintf(stderr, "[ ERROR ] Failed to read CSV file.\n");
        return 1;
    }

    CBox* box = CBOX_WRAP_CSV(csv);
    cbox_print(box);
    // csv_print_with_headers(box);

    char json[4096];
    cbox_to_json(box, json, sizeof(json));
    printf("JSON:\n%s\n", json);

    cbox_free(box);
    return 0;
}
