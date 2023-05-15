#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

typedef struct s_slice {
    const char *data;
    size_t      size;
} t_slice;

t_slice  slice0(const char *string);
char    *slice_to_str(t_slice slice);

#endif // !UTIL_H