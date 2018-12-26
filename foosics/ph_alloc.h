#pragma once

struct ph_alloc
{
    void* (*alloc)(ph_alloc *self, size_t size);
    void (*free)(ph_alloc *self, void* toFree);
};

#define PH_ALLOC(type, allocator) (type*)ph_alloc_allocate(i_allocator, sizeof(type));

void *ph_alloc_allocate(ph_alloc *alloc, size_t size);
void  ph_alloc_free(ph_alloc *alloc, void *to_free);

void ph_alloc_crt_init(ph_alloc *i_alloc);