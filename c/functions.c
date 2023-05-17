#include "functions.h"

void errMaloc(void *pointer, char *file, int line)
{
    if (pointer == NULL)
        errx(1, "Error: malloc error at %s:%d", file, line);
}