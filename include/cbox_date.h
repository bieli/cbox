#ifndef CBOX_DATE_H
#define CBOX_DATE_H

#include "cbox.h"

typedef struct {
    int year;
    int month;
    int day;
} CBoxDate;

extern CBoxTrait CBOX_DATE_TRAIT;

CBoxDate* cbox_date_parse(const char* token);

#endif
