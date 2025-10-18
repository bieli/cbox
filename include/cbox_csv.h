#ifndef CBOX_CSV_H
#define CBOX_CSV_H

#include <stdbool.h>
#include "cbox.h"
#include "cbox_container.h"

typedef struct {
    size_t row_count;
    size_t column_count;
    char** column_names;
    CBoxContainer** rows;  // each row is a container of CBoxes
} CBoxCSV;

extern CBoxTrait CBOX_CSV_TRAIT;

#define CBOX_WRAP_CSV(ptr) cbox_new_with_trait((ptr), sizeof(CBoxCSV), CBOX_CSV, &CBOX_CSV_TRAIT)

CBoxCSV* read_csv(const char* path, char separator, bool skip_header, size_t max_rows);

#endif
