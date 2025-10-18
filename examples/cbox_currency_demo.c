#include <stdio.h>
#include "locale.h"
#include "cbox.h"
#include "cbox_currency.h"

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");

    // Create a currency object: 8425.95 EUR
    CBox* currency = CBOX_WRAP_CURRENCY(8425.95, "EUR");
    // CBox* currency = cbox_currency_new(8425.95, "EUR");
    if (!currency) {
        printf("[ ERROR ] Failed to create currency object.\n");
        return 1;
    }

    // Print the currency object
    printf("Printing currency object:\n");
    cbox_print(currency);
    printf("\n");

    // Serialize to JSON
    char json[4096];
    cbox_to_json(currency, json, sizeof(json));
    printf("JSON output:\n%s\n\n", json);

    CBox* parsed = cbox_currency_parse("$10.12");
    cbox_print(parsed);

    cbox_to_json(parsed, json, sizeof(json));
    printf("\nJSON output:\n%s\n", json);

    // Clean up
    cbox_free(currency);
    return 0;
}
