#include <stdio.h>
#include <stdlib.h>
#include "object.h"
#include "heap.h"

Object *object_new_int(int value) {
    Object *obj = heap_alloc();
    obj->type = OBJ_INT;
    obj->marked = 0;
    obj->data.value = value;
    return obj;
}

Object *object_new_pair(void) {
    Object *obj = heap_alloc();
    obj->type = OBJ_PAIR;
    obj->marked = 0;
    obj->data.pair.ref_count = 0;
    return obj;
}

void object_add_ref(Object *pair, Object *child) {
    if (pair->type != OBJ_PAIR) return;
    if (pair->data.pair.ref_count >= MAX_REFS) return;
    pair->data.pair.refs[pair->data.pair.ref_count++] = child;
}

void object_print(Object *obj) {
    if (!obj) {
        printf("(null)");
        return;
    }
    if (obj->type == OBJ_INT) {
        printf("Int(%d)", obj->data.value);
    } else {
        printf("Pair[");
        for (int i = 0; i < obj->data.pair.ref_count; i++) {
            if (i > 0) printf(", ");
            object_print(obj->data.pair.refs[i]);
        }
        printf("]");
    }
}