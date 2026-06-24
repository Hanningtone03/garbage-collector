#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

static Object *all_objects = NULL;
static int object_count = 0;

void heap_init(void) {
    all_objects = NULL;
    object_count = 0;
}

Object *heap_alloc(void) {
    if (object_count >= HEAP_MAX) {
        fprintf(stderr, "Heap exhausted: %d objects allocated, limit is %d\n", object_count, HEAP_MAX);
        exit(1);
    }

    Object *obj = malloc(sizeof(Object));
    obj->next = all_objects;
    all_objects = obj;
    object_count++;

    return obj;
}

void heap_free(Object *obj) {
    free(obj);
    object_count--;
}

int heap_object_count(void) {
    return object_count;
}

Object *heap_all_objects(void) {
    return all_objects;
}

void heap_set_all_objects(Object *head) {
    all_objects = head;
}