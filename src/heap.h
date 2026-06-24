#ifndef HEAP_H
#define HEAP_H

#include "object.h"

#define HEAP_MAX 256

void heap_init(void);
Object *heap_alloc(void);
void heap_free(Object *obj);
int heap_object_count(void);
Object *heap_all_objects(void);
void heap_set_all_objects(Object *head);

#endif