#include <stdio.h>
#include "test_utils.h"

void print_colored_status(const char* message, bool success) {
    const char* color = success ? "\033[0;32m" : "\033[0;31m";  // green or red
    printf("%s%s\033[0m\n", color, message);
}
