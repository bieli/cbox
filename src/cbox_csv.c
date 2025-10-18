#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cbox_csv.h"
#include "cbox_decimal.h"
#include "cbox_container.h"

static CBox* guess_type(const char* token) {
    if (strchr(token, '.') != NULL) {
        double* val = malloc(sizeof(double));
        *val = atof(token);
        return cbox_new_with_trait(val, sizeof(double), CBOX_DOUBLE, &CBOX_DOUBLE_TRAIT);
    } else if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
        int* val = malloc(sizeof(int));
        *val = atoi(token);
        return cbox_new_with_trait(val, sizeof(int), CBOX_INT, &CBOX_INT_TRAIT);
    } else {
        char* copied = malloc(strlen(token) + 1);
        strcpy(copied, token);
        return cbox_new_with_trait(copied, strlen(token) + 1, CBOX_STRUCT, &CBOX_STRUCT_TRAIT);
    }
}

CBoxCSV* read_csv(const char* path, char separator, bool skip_header, size_t max_rows) {
    FILE* file = fopen(path, "r");
    if (!file) return NULL;

    CBoxCSV* csv = malloc(sizeof(CBoxCSV));
    csv->row_count = 0;
    csv->column_count = 0;
    csv->column_names = NULL;
    csv->rows = malloc(sizeof(CBoxContainer*) * max_rows);

    char line[1024];
    size_t line_index = 0;

    while (fgets(line, sizeof(line), file) && csv->row_count < max_rows) {
        line[strcspn(line, "\r\n")] = 0;  // strip newline
        char* tokens[100];
        size_t token_count = 0;

        char* token = strtok(line, &separator);
        while (token && token_count < 100) {
            tokens[token_count++] = token;
            token = strtok(NULL, &separator);
        }

        if (line_index == 0 && !skip_header) {
            csv->column_count = token_count;
            csv->column_names = malloc(sizeof(char*) * token_count);
            for (size_t i = 0; i < token_count; ++i) {
                //csv->column_names[i] = strdup(tokens[i]);
                csv->column_names[i] = malloc(strlen(tokens[i]) + 1);
                if (csv->column_names[i]) {
                    strcpy(csv->column_names[i], tokens[i]);
                }
            }
        } else {
            if (csv->column_count == 0) csv->column_count = token_count;
            CBoxContainer* row = cbox_container_create(token_count);
            for (size_t i = 0; i < token_count; ++i) {
                cbox_container_add(row, guess_type(tokens[i]));
            }
            csv->rows[csv->row_count++] = row;
        }

        line_index++;
    }

    fclose(file);
    return csv;
}

void* csv_clone(const void* data) {
    const CBoxCSV* original = (const CBoxCSV*)data;
    CBoxCSV* copy = malloc(sizeof(CBoxCSV));
    copy->row_count = original->row_count;
    copy->column_count = original->column_count;

    copy->column_names = malloc(sizeof(char*) * copy->column_count);
    for (size_t i = 0; i < copy->column_count; ++i) {
        // copy->column_names[i] = strdup(original->column_names[i]);
        copy->column_names[i] = malloc(strlen(original->column_names[i]) + 1);
        if (copy->column_names[i]) {
            strcpy(copy->column_names[i], original->column_names[i]);
        }
    }

    copy->rows = malloc(sizeof(CBoxContainer*) * copy->row_count);
    for (size_t i = 0; i < copy->row_count; ++i) {
        copy->rows[i] = (CBoxContainer*)container_clone(original->rows[i]);
    }

    return copy;
}

void csv_destroy(void* data) {
    CBoxCSV* csv = (CBoxCSV*)data;
    for (size_t i = 0; i < csv->row_count; ++i) {
        container_destroy(csv->rows[i]);
    }
    free(csv->rows);
    for (size_t i = 0; i < csv->column_count; ++i) {
        free(csv->column_names[i]);
    }
    free(csv->column_names);
    free(csv);
}

void csv_print(const void* data) {
    const CBoxCSV* csv = (const CBoxCSV*)data;
    if (!csv) return;

    printf("CBoxCSV: %zu rows × %zu columns\n", csv->row_count, csv->column_count);

    if (csv->column_names) {
        printf("Columns: ");
        for (size_t i = 0; i < csv->column_count; ++i) {
            if (csv->column_names[i]) {
                printf("%s%s", i > 0 ? ", " : "", csv->column_names[i]);
            }
        }
        printf("\n");
    }

    for (size_t r = 0; r < csv->row_count; ++r) {
        CBoxContainer* row = csv->rows[r];
        if (!row) continue;
        printf("Row %zu: ", r);
        for (size_t c = 0; c < row->count; ++c) {
            if (row->items[c]) {
                cbox_print(row->items[c]);
                printf(" ");
            }
        }
        printf("\n");
    }
}

void csv_serialize_json(const void* data, char* out, size_t size) {
    const CBoxCSV* csv = (const CBoxCSV*)data;
    char* cursor = out;
    size_t remaining = size;

    int written = snprintf(cursor, remaining, "{\"type\":\"CBoxCSV\",\"rows\":[");
    cursor += written;
    remaining -= written;

    for (size_t r = 0; r < csv->row_count; ++r) {
        written = snprintf(cursor, remaining, "%s[", r > 0 ? "," : "");
        cursor += written;
        remaining -= written;

        CBoxContainer* row = csv->rows[r];
        for (size_t c = 0; c < row->count; ++c) {
            char cell_json[256];
            if (row->items[c] && row->items[c]->trait && row->items[c]->trait->serialize_json) {
                row->items[c]->trait->serialize_json(row->items[c]->data, cell_json, sizeof(cell_json));
            } else {
                snprintf(cell_json, sizeof(cell_json), "\"<unknown>\"");
            }
            written = snprintf(cursor, remaining, "%s%s", c > 0 ? "," : "", cell_json);
            cursor += written;
            remaining -= written;
        }

        written = snprintf(cursor, remaining, "]");
        cursor += written;
        remaining -= written;
    }

    snprintf(cursor, remaining, "]}");
}

CBoxTrait CBOX_CSV_TRAIT = {
    .clone = csv_clone,
    .destroy = csv_destroy,
    .print = csv_print,
    .serialize_json = csv_serialize_json
};
