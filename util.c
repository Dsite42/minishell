#include "util.h"

#include <string.h>
#include <stdlib.h>

t_slice slice0(const char *string) {
    t_slice slice;

    slice.data = string;
    slice.size = strlen(string);
    return slice;
}

char *slice_to_str(t_slice slice) {
    char *new_string;

    new_string = malloc(slice.size + 1);
    if (new_string == NULL) {
        return NULL;
    }
    memcpy(new_string, slice.data, slice.size);
    new_string[slice.size] = '\0';
    return new_string;
}