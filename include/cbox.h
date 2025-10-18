#ifndef CBOX_H
#define CBOX_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum {
    CBOX_INT,
    CBOX_FLOAT,
    CBOX_DOUBLE,
    CBOX_STRUCT,
    CBOX_ARRAY
} CBoxType;

typedef struct CBox CBox;

typedef struct {
    void* (*clone)(const void* data);
    void  (*destroy)(void* data);
    void  (*print)(const void* data);
    void  (*serialize_json)(const void* data, char* out, size_t maxlen);
} CBoxTrait;


typedef struct CBox {
    void* data;
    size_t size;
    CBoxType type;
    CBoxTrait* trait;
    const CBox* parent; // reference to original CBox if cloned
} CBox;

CBox* cbox_new_with_trait(void* value, size_t size, CBoxType type, CBoxTrait* trait);
CBox* cbox_clone(const CBox* original);
void  cbox_to_json(const CBox* box, char* out, size_t maxlen);
void  cbox_print(const CBox* box);
void  cbox_free(CBox* box);

// Traits for simple types
extern CBoxTrait CBOX_INT_TRAIT;
extern CBoxTrait CBOX_FLOAT_TRAIT;
extern CBoxTrait CBOX_DOUBLE_TRAIT;

#define CBOX_WRAP_INT(x)     cbox_new_with_trait(&(x), sizeof(int), CBOX_INT, &CBOX_INT_TRAIT)
#define CBOX_WRAP_FLOAT(x)   cbox_new_with_trait(&(x), sizeof(float), CBOX_FLOAT, &CBOX_FLOAT_TRAIT)
#define CBOX_WRAP_DOUBLE(x)  cbox_new_with_trait(&(x), sizeof(double), CBOX_DOUBLE, &CBOX_DOUBLE_TRAIT)

#endif // CBOX_H

