#include <stdio.h>
#include "gc.h"
#include "heap.h"

static Object *roots[ROOTS_MAX];
static int root_count = 0;

void gc_init(void) {
    root_count = 0;
}

void gc_push_root(Object *obj) {
    if (root_count >= ROOTS_MAX) {
        fprintf(stderr, "Root stack overflow\n");
        return;
    }
    roots[root_count++] = obj;
}

void gc_pop_root(void) {
    if (root_count > 0) {
        root_count--;
    }
}

int gc_root_count(void) {
    return root_count;
}

static void mark(Object *obj) {
    if (!obj || obj->marked) return;

    obj->marked = 1;

    if (obj->type == OBJ_PAIR) {
        for (int i = 0; i < obj->data.pair.ref_count; i++) {
            mark(obj->data.pair.refs[i]);
        }
    }
}

static void mark_all(void) {
    for (int i = 0; i < root_count; i++) {
        mark(roots[i]);
    }
}

static void sweep(void) {
    Object *current = heap_all_objects();
    Object *previous = NULL;
    Object *head = NULL;
    int reclaimed = 0;

    while (current) {
        Object *next = current->next;

        if (!current->marked) {
            if (previous) {
                previous->next = next;
            }
            heap_free(current);
            reclaimed++;
        } else {
            current->marked = 0;
            if (!head) head = current;
            previous = current;
        }

        current = next;
    }

    heap_set_all_objects(head);

    if (reclaimed > 0) {
        printf("  [gc] reclaimed %d object(s)\n", reclaimed);
    }
}

void gc_collect(void) {
    int before = heap_object_count();
    mark_all();
    sweep();
    int after = heap_object_count();
    printf("  [gc] collection complete: %d -> %d objects\n", before, after);
}