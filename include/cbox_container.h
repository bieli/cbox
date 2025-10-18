#ifndef CBOX_CONTAINER_H
#define CBOX_CONTAINER_H

#include "cbox.h"

#define CBOX_CONTAINER_JSON_MAX_LEN 4096

typedef struct {
    size_t count;
    size_t capacity;
    CBox** items;
} CBoxContainer;

extern CBoxTrait CBOX_CONTAINER_TRAIT;

#define CBOX_WRAP_CONTAINER(ptr) cbox_new_with_trait((ptr), sizeof(CBoxContainer), CBOX_CONTAINER, &CBOX_CONTAINER_TRAIT)

CBoxContainer* cbox_container_create(size_t initial_capacity);
int cbox_container_add(CBoxContainer* container, CBox* item);

#endif // CBOX_CONTAINER_H
