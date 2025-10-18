#ifndef CBOX_PRODUCT_H
#define CBOX_PRODUCT_H

#include "cbox.h"

#define MAX_CBOX_PRODUCT_LEN 64

typedef struct {
    char name[MAX_CBOX_PRODUCT_LEN];
    double price;
    int quantity;
} Product;

extern CBoxTrait CBOX_PRODUCT_TRAIT;

#define CBOX_WRAP_PRODUCT(p) cbox_new_with_trait(&(p), sizeof(Product), CBOX_STRUCT, &CBOX_PRODUCT_TRAIT)

#endif // CBOX_PRODUCT_H

