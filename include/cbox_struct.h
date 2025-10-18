#ifndef CBOX_STRUCT_H
#define CBOX_STRUCT_H

#include "cbox.h"

#define MAX_CBOX_STRUCT_LEN 32

typedef struct {
    char name[MAX_CBOX_STRUCT_LEN];
    int age;
} Person;

void* person_clone(const void* data);
void  person_destroy(void* data);
void  person_print(const void* data);
void  person_serialize_json(const void* data, char* out, size_t maxlen);

extern CBoxTrait CBOX_PERSON_TRAIT;

#define CBOX_WRAP_PERSON(p) cbox_new_with_trait(&(p), sizeof(Person), CBOX_STRUCT, &CBOX_PERSON_TRAIT)

#endif // CBOX_STRUCT_H

