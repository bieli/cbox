#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cbox.h"
#include "cbox_date.h"

CBoxDate* cbox_date_parse(const char* token) {
    CBoxDate* date = malloc(sizeof(CBoxDate));
    if (!date) return NULL;

    if (sscanf(token, "%4d-%2d-%2d", &date->year, &date->month, &date->day) == 3) {
        return date;
    }

    free(date);
    return NULL;
}

void* date_clone(const void* data) {
    CBoxDate* copy = malloc(sizeof(CBoxDate));
    memcpy(copy, data, sizeof(CBoxDate));
    return copy;
}

void date_destroy(void* data) {
    free(data);
}

void date_print(const void* data) {
    const CBoxDate* date = (const CBoxDate*)data;
    printf("date: %04d-%02d-%02d", date->year, date->month, date->day);
}

void date_serialize_json(const void* data, char* out, size_t size) {
    const CBoxDate* date = (const CBoxDate*)data;
    snprintf(out, size, "{\"type\":\"date\",\"value\":\"%04d-%02d-%02d\"}", date->year, date->month, date->day);
}

CBoxTrait CBOX_DATE_TRAIT = {
    .clone = date_clone,
    .destroy = date_destroy,
    .print = date_print,
    .serialize_json = date_serialize_json
};
