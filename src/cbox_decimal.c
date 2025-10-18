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

CBox* parse_double_to_cbox_decimal(double value) {
    // Convert double to string with sufficient precision
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%.15g", value);  // Avoid trailing zeros and preserve precision

    // Delegate to string-based decimal parser
    return parse_string_to_cbox_decimal(buffer);
}

CBox* parse_string_to_cbox_decimal(const char* token) {
    if (!token || strlen(token) == 0) return NULL;

    const char* dot = strchr(token, '.');
    Decimal *dec = malloc(sizeof(Decimal));
    if (!dec) return NULL;

    if (dot) {
        size_t whole_len = dot - token;
        // char* whole_str = strndup(token, whole_len);
        // char* frac_str = strdup(dot + 1);

        // Allocate and copy whole part
        char* whole_str = malloc(whole_len + 1);
        if (!whole_str) return NULL;
        memcpy(whole_str, token, whole_len);
        whole_str[whole_len] = '\0';

        // Allocate and copy fractional part
        size_t frac_len = strlen(dot + 1);
        char* frac_str = malloc(frac_len + 1);
        if (!frac_str) {
            free(whole_str);
            return NULL;
        }
        memcpy(frac_str, dot + 1, frac_len);
        frac_str[frac_len] = '\0';

        dec->whole = atoll(whole_str);
        dec->fraction = atoll(frac_str);
        // dec->precision = strlen(frac_str);

        free(whole_str);
        free(frac_str);
    } else {
        dec->whole = atoll(token);
        dec->fraction = 0;
        // dec->precision = 0;
    }

    return cbox_new_with_trait(dec, sizeof(Decimal), CBOX_DECIMAL, &CBOX_DECIMAL_TRAIT);
}

// void decimal_print(const void* data) {
//     printf("CBox<decimal>: %.2f", *(const double*)data);
// }

// void decimal_serialize_json(const void* data, char* out, size_t size) {
//     snprintf(out, size, "{\"type\":\"Decimal\",\"value\":\"%.2f\"}", *(const double*)data);
// }

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
