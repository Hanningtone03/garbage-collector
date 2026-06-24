#ifndef GC_H
#define GC_H

#include "object.h"

#define ROOTS_MAX 32

void gc_init(void);
void gc_push_root(Object *obj);
void gc_pop_root(void);
void gc_collect(void);
int gc_root_count(void);

#endif