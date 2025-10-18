#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cbox_struct.h"

void* person_clone(const void* data) {
    Person* copy = malloc(sizeof(Person));
    if (copy) memcpy(copy, data, sizeof(Person));
    return copy;
}

void person_destroy(void* data) {
    free(data);
}

void person_print(const void* data) {
    const Person* p = (const Person*)data;
    printf("Person{name: %s, age: %d}\n", p->name, p->age);
}

void person_serialize_json(const void* data, char* out, size_t maxlen) {
    const Person* p = (const Person*)data;
    snprintf(out, maxlen,
        "{\"type\":\"Person\",\"name\":\"%s\",\"age\":%d,\"address\":\"%p\"}",
        p->name, p->age, data);
}

CBoxTrait CBOX_PERSON_TRAIT = {
    .clone = person_clone,
    .destroy = person_destroy,
    .print = person_print,
    .serialize_json = person_serialize_json
};

