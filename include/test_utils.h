#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdbool.h>

#define PRINT_TEST_RESULT(label, test_name, failed_count)                          \
    do {                                                                           \
        if ((failed_count) == 0) {                                                 \
            char msg[128];                                                         \
            snprintf(msg, sizeof(msg), "[   OK   ] %s passed.\n", (test_name));    \
            print_colored_status(msg, true);                                       \
        } else {                                                                   \
            char msg[128];                                                         \
            snprintf(msg, sizeof(msg), "[ FAILED ] %d %s failed.\n",               \
                     (failed_count), (test_name));                                 \
            print_colored_status(msg, false);                                      \
        }                                                                          \
    } while (0)

void print_colored_status(const char* message, bool success);

#endif
