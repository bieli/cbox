#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cbox_product.h"

void* product_clone(const void* data) {
    Product* copy = malloc(sizeof(Product));
    if (copy) memcpy(copy, data, sizeof(Product));
    return copy;
}

void product_destroy(void* data) {
    free(data);
}

void product_print(const void* data) {
    const Product* p = (const Product*)data;
    printf("Product{name: %s, price: %.2f, quantity: %d}\n", p->name, p->price, p->quantity);
}

void product_serialize_json(const void* data, char* out, size_t maxlen) {
    const Product* p = (const Product*)data;
    snprintf(out, maxlen,
        "{\"type\":\"Product\",\"name\":\"%s\",\"price\":%.2f,\"quantity\":%d,\"address\":\"%p\"}",
        p->name, p->price, p->quantity, data);
}

CBoxTrait CBOX_PRODUCT_TRAIT = {
    .clone = product_clone,
    .destroy = product_destroy,
    .print = product_print,
    .serialize_json = product_serialize_json
};

