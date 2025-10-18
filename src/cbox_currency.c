#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cbox.h"
#include "cbox_decimal.h"
#include "cbox_currency.h"
#include "cbox_container.h"

CBox* cbox_currency_new(double amount, const char* iso_code) {
    // Allocate and box the decimal value
    double* val = malloc(sizeof(double));
    if (!val) return NULL;
    *val = amount;
    // printf("[ DEBUG ] amount: %f\n", amount);

    // CBox* decimal_box = cbox_new_with_trait(val, sizeof(double), CBOX_DECIMAL, &CBOX_DECIMAL_TRAIT);
    CBox* decimal_box = parse_double_to_cbox_decimal(amount);
    // CBox* decimal_box = CBOX_WRAP_DECIMAL(val);
    if (!decimal_box) {
        free(val);
        return NULL;
    }
    // printf("[ DEBUG ] decimal_box:\n");
    // cbox_print(decimal_box);
    // printf("\n");

    // Allocate and box the ISO code string
    size_t code_len = strlen(iso_code);
    char* code = malloc(code_len + 1);
    if (!code) {
        cbox_free(decimal_box);
        return NULL;
    }
    strcpy(code, iso_code);

    CBox* code_box = cbox_new_with_trait(code, code_len + 1, CBOX_STRUCT, &CBOX_STRUCT_TRAIT);
    if (!code_box) {
        free(code);
        cbox_free(decimal_box);
        return NULL;
    }

    // Create container and add both boxed values
    CBoxContainer* container = cbox_container_create(2);
    if (!container) {
        cbox_free(decimal_box);
        cbox_free(code_box);
        return NULL;
    }

    cbox_container_add(container, decimal_box);
    cbox_container_add(container, code_box);

    // Wrap the container in a CBox with CBOX_CURRENCY type
    return cbox_new_with_trait(container, sizeof(CBoxContainer), CBOX_CURRENCY, &CBOX_CURRENCY_TRAIT);
}

static void* currency_clone(const void* data) {
    return container_clone((const CBoxContainer*)data);
}

static void currency_destroy(void* data) {
    container_destroy((CBoxContainer*)data);
}

static void currency_print(const void* data) {
    const CBoxContainer* container = (const CBoxContainer*)data;
    if (container && container->count == 2) {
        printf("CBox<currency>: ");
        cbox_print(container->items[0]);  // decimal
        printf(" ");
        cbox_print(container->items[1]);  // ISO code
    } else {
        printf("CBox<currency>: [invalid]");
    }
}

static void currency_serialize_json(const void* data, char* out, size_t size) {
    const CBoxContainer* container = (const CBoxContainer*)data;
    if (container && container->count == 2) {
        char amount_json[512];
        char code_json[64];
        cbox_to_json(container->items[0], amount_json, sizeof(amount_json));
        cbox_to_json(container->items[1], code_json, sizeof(code_json));
        snprintf(out, size,
            "{\"type\":\"currency\",\"amount\":%s,\"code\":%s}",
            amount_json, code_json);
    } else {
        snprintf(out, size, "{\"type\":\"currency\",\"error\":\"invalid container\"}");
    }
}

CBox* cbox_currency_parse(const char* token) {
    if (!token || strlen(token) < 2) return NULL;

    size_t len = strlen(token);
    const char* numeric_start = token;
    size_t numeric_len = len;
    const char* iso_code = NULL;

    // Detect $ at beginning
    if (token[0] == '$') {
        numeric_start = token + 1;
        numeric_len = len - 1;
        iso_code = "USD";
    }
    // Detect $ at end
    else if (token[len - 1] == '$') {
        numeric_start = token;
        numeric_len = len - 1;
        iso_code = "USD";
    }
    // Detect € at beginning (UTF-8: 3 bytes)
    else if (strncmp(token, "€", 3) == 0) {
        numeric_start = token + 3;
        numeric_len = len - 3;
        iso_code = "EUR";
    }
    // Detect € at end
    else if (len >= 3 && strncmp(&token[len - 3], "€", 3) == 0) {
        numeric_start = token;
        numeric_len = len - 3;
        iso_code = "EUR";
    } else {
        return NULL;
    }

    // Copy numeric portion
    char* numeric = malloc(numeric_len + 1);
    if (!numeric) return NULL;

    strncpy(numeric, numeric_start, numeric_len);
    numeric[numeric_len] = '\0';

    // Convert to double
    char* endptr = NULL;
    double value = strtod(numeric, &endptr);
    free(numeric);

    // Validate conversion
    if (endptr == numeric || *endptr != '\0') {
        return NULL;
    }

    // Use the currency constructor to wrap both value and code
    return cbox_currency_new(value, iso_code);
}

CBoxTrait CBOX_CURRENCY_TRAIT = {
    .clone = currency_clone,
    .destroy = currency_destroy,
    .print = currency_print,
    .serialize_json = currency_serialize_json
};
