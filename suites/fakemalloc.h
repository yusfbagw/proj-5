#ifndef FAKEMALLOC_H
#define FAKEMALLOC_H
#include <stdlib.h>

#ifndef NO_FAKE_MALLOC
#define MALLOC_COUNT_CHECK(n) ((bytes_until_fail == -1) || ((bytes_until_fail >= (int)(n)) && ((bytes_until_fail -= (int)(n)) >= 0)))

#define malloc(n)     (MALLOC_COUNT_CHECK(n) ? malloc(n) : NULL)
#define calloc(n, s)  (MALLOC_COUNT_CHECK(n*s) ? calloc(n, s) : NULL)
#define realloc(n, s) (MALLOC_COUNT_CHECK(s) ? realloc(n, s) : NULL)
#endif

extern int bytes_until_fail;
#endif
