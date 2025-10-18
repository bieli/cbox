#include <stdio.h>
#include "cbox.h"
#include "cbox_product.h"
#include "cbox_debug.h"

int main() {
    Product p = { "Laptop", 1499.99, 3 };

    CBox* box = CBOX_WRAP_PRODUCT(p);

    CBOX_DEBUG_ALL(box);

    cbox_free(box);

    return 0;
}

