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

CBoxTrait CBOX_INT_TRAIT = {
    .clone = int_clone,
    .destroy = int_destroy,
    .print = int_print
};

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

CBoxTrait CBOX_FLOAT_TRAIT = {
    .clone = float_clone,
    .destroy = float_destroy,
    .print = float_print
};

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

CBoxTrait CBOX_DOUBLE_TRAIT = {
    .clone = double_clone,
    .destroy = double_destroy,
    .print = double_print
};

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

    return clone;
}

void cbox_print(const CBox* box) {
    if (!box || !box->trait || !box->trait->print) {
        printf("CBox: [null or no trait]\n");
        return;
    }
    box->trait->print(box->data);
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

