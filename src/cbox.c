#include "cbox.h"

// Traits impl. for simple types

void* int_clone(const void* data) {
    int* copy = malloc(sizeof(int));
    if (copy) *copy = *(int*)data;
    return copy;
}

void int_destroy(void* data) {
    free(data);
}

void int_print(const void* data) {
    printf("CBox<int>: %d\n", *(int*)data);
}

void* float_clone(const void* data) {
    float* copy = malloc(sizeof(float));
    if (copy) *copy = *(float*)data;
    return copy;
}

void float_destroy(void* data) {
    free(data);
}

void float_print(const void* data) {
    printf("CBox<float>: %f\n", *(float*)data);
}


void* double_clone(const void* data) {
    double* copy = malloc(sizeof(double));
    if (copy) *copy = *(double*)data;
    return copy;
}

void double_destroy(void* data) {
    free(data);
}

void double_print(const void* data) {
    printf("CBox<double>: %lf\n", *(double*)data);
}

CBox* cbox_new_with_trait(void* value, size_t size, CBoxType type, CBoxTrait* trait) {
    CBox* box = malloc(sizeof(CBox));
    if (!box) return NULL;

    box->data = malloc(size);
    if (!box->data) {
        free(box);
        return NULL;
    }

    memcpy(box->data, value, size);
    box->size = size;
    box->type = type;
    box->trait = trait;

    return box;
}

CBox* cbox_clone(const CBox* original) {
    if (!original || !original->trait || !original->trait->clone) return NULL;

    CBox* clone = malloc(sizeof(CBox));
    if (!clone) return NULL;

    clone->data = original->trait->clone(original->data);
    clone->size = original->size;
    clone->type = original->type;
    clone->trait = original->trait;
    clone->parent = original;  // Track origin

    return clone;
}


void cbox_print(const CBox* box) {
    if (!box || !box->trait || !box->trait->print) {
        printf("CBox: [null or no trait]\n");
        return;
    }
    box->trait->print(box->data);
}

void cbox_to_json(const CBox* box, char* out, size_t maxlen) {
    if (!box || !box->trait || !box->trait->serialize_json) {
        snprintf(out, maxlen, "\"CBox\": null");
        return;
    }
    box->trait->serialize_json(box->data, out, maxlen);
}

void int_serialize_json(const void* data, char* out, size_t maxlen) {
    snprintf(out, maxlen, "{\"type\":\"int\",\"value\":%d,\"address\":\"%p\"}", *(int*)data, data);
}

void float_serialize_json(const void* data, char* out, size_t maxlen) {
    snprintf(out, maxlen, "{\"type\":\"float\",\"value\":%f,\"address\":\"%p\"}", *(float*)data, data);
}

void double_serialize_json(const void* data, char* out, size_t maxlen) {
    snprintf(out, maxlen, "{\"type\":\"double\",\"value\":%lf,\"address\":\"%p\"}", *(double*)data, data);
}

void cbox_free(CBox* box) {
    if (box) {
        if (box->trait && box->trait->destroy)
            box->trait->destroy(box->data);
        else
            free(box->data);
        free(box);
    }
}


CBoxTrait CBOX_INT_TRAIT = {
    .clone = int_clone,
    .destroy = int_destroy,
    .print = int_print,
    .serialize_json = int_serialize_json
};

CBoxTrait CBOX_FLOAT_TRAIT = {
    .clone = float_clone,
    .destroy = float_destroy,
    .print = float_print,
    .serialize_json = float_serialize_json
};

CBoxTrait CBOX_DOUBLE_TRAIT = {
    .clone = double_clone,
    .destroy = double_destroy,
    .print = double_print,
    .serialize_json = double_serialize_json
};

