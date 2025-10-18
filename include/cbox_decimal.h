#ifndef CBOX_DECIMAL_H
#define CBOX_DECIMAL_H

#include "cbox.h"

typedef struct {
    int whole;
    int fraction;  // e.g., 0.75 -> whole = 0 and fraction = 75
} Decimal;

extern CBoxTrait CBOX_DECIMAL_TRAIT;

#define CBOX_WRAP_DECIMAL(d) cbox_new_with_trait(&(d), sizeof(Decimal), CBOX_DECIMAL, &CBOX_DECIMAL_TRAIT)

#endif
