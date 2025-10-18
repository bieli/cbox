#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cbox_decimal.h"

void* decimal_clone(const void* data) {
    const Decimal* original = (const Decimal*)data;
    Decimal* copy = malloc(sizeof(Decimal));
    if (!copy) return NULL;
    copy->whole = original->whole;
    copy->fraction = original->fraction;
    return copy;
}

void decimal_destroy(void* data) {
    free(data);
}

void decimal_print(const void* data) {
    const Decimal* d = (const Decimal*)data;
    printf("CBox<decimal>: %d.%02d\n", d->whole, d->fraction);
}

void decimal_serialize_json(const void* data, char* out, size_t size) {
    const Decimal* d = (const Decimal*)data;
    snprintf(out, size,
        "{\"type\":\"Decimal\",\"value\":\"%d.%02d\"}", d->whole, d->fraction);
}

CBoxTrait CBOX_DECIMAL_TRAIT = {
    .clone = decimal_clone,
    .destroy = decimal_destroy,
    .print = decimal_print,
    .serialize_json = decimal_serialize_json
};
