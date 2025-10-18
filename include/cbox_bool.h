#ifndef CBOX_BOOL_H
#define CBOX_BOOL_H

#include <stdbool.h>
#include "cbox.h"

// Trait for CBOX_BOOL
extern CBoxTrait CBOX_BOOL_TRAIT;

// Convenience macro to wrap a bool value
#define CBOX_WRAP_BOOL(value) cbox_new_with_trait(cbox_bool_alloc(value), sizeof(bool), CBOX_BOOL, &CBOX_BOOL_TRAIT)

// Allocates a bool on the heap for boxing
bool* cbox_bool_alloc(bool value);

#endif
