#include "cbox_container.h"
#include "cbox_decimal.h"
#include "cbox_array.h"

int main() {
    CBoxContainer* container = cbox_container_create(2);

    Decimal d = {1, 99};
    CBox* dec = CBOX_WRAP_DECIMAL(d);
    cbox_container_add(container, dec);

    int values[] = {10, 20};
    IntArray* arr = intarray_create(2, values);
    CBox* boxed_arr = CBOX_WRAP_INTARRAY(arr);
    cbox_container_add(container, boxed_arr);

    int values1[] = {101, 102, 103, 104};
    IntArray* arr1 = intarray_create(4, values1);
    CBox* boxed_arr1 = CBOX_WRAP_INTARRAY(arr1);
    cbox_container_add(container, boxed_arr1);

    CBox* box = CBOX_WRAP_CONTAINER(container);
    cbox_print(box);
    cbox_free(box);
    return 0;
}
