#include "ph_alloc.h"

#include <stdlib.h>

void* 
ph_alloc_allocate(ph_alloc *self, size_t size)
{
    return self->alloc(self, size);
}

void 
ph_alloc_free(ph_alloc *self, void *to_free)
{
    return self->free(self, to_free);
}


//@@ CRT

void*
crt_alloc(ph_alloc *self, size_t size)
{
    return malloc(size);
}

void 
crt_free(ph_alloc *self, void *toFree)
{
    free(toFree);
}

void 
ph_alloc_crt_init(ph_alloc *i_alloc)
{
    i_alloc->alloc = crt_alloc;
    i_alloc->free = crt_free;
}
