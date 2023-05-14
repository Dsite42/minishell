#include "util.h"

#include <string.h>

t_slice slice0(const char *string) {
    t_slice slice;

    slice.data = string;
    slice.size = strlen(string);
    return slice;
}