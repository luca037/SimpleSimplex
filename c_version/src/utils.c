#include "../include/utils.h"

#include <stdlib.h>

void free_and_null(char **ptr) {
    if (*ptr != NULL) {
        free(*ptr);
        *ptr = NULL;
    }
}
