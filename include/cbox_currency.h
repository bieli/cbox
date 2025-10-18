#ifndef CBOX_CURRENCY_H
#define CBOX_CURRENCY_H

#include "cbox.h"

extern CBoxTrait CBOX_CURRENCY_TRAIT;

CBox* cbox_currency_new(double amount, const char* iso_code);
CBox* cbox_currency_parse(const char* token);

#define CBOX_WRAP_CURRENCY(amount, code) cbox_currency_new((amount), (code))

#endif
