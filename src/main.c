#include <stdio.h>
#include "object.h"
#include "heap.h"
#include "gc.h"

static void print_status(const char *label) {
    printf("\n  %s\n", label);
    printf("  Heap size: %d objects, %d root(s) on stack\n",
           heap_object_count(), gc_root_count());
}

static void demo_basic_reclaim(void) {
    printf("\n========================================\n");
    printf("  Demo 1: Basic reclamation\n");
    printf("========================================\n");

    heap_init();
    gc_init();

    Object *a = object_new_int(1);
    gc_push_root(a);
    print_status("After allocating 'a' (rooted)");

    Object *b = object_new_int(2);
    print_status("After allocating 'b' (NOT rooted)");

    printf("\n  Running collection -- 'b' should be reclaimed since it has no root\n");
    gc_collect();
    print_status("After collection");

    gc_pop_root();
}

static void demo_pair_graph(void) {
    printf("\n========================================\n");
    printf("  Demo 2: Reachability through a pair\n");
    printf("========================================\n");

    heap_init();
    gc_init();

    Object *pair = object_new_pair();
    gc_push_root(pair);

    Object *child1 = object_new_int(10);
    Object *child2 = object_new_int(20);
    object_add_ref(pair, child1);
    object_add_ref(pair, child2);

    print_status("After building pair -> [10, 20]");

    printf("\n  pair = ");
    object_print(pair);
    printf("\n");

    printf("\n  Running collection -- both children are reachable through pair, nothing should be reclaimed\n");
    gc_collect();
    print_status("After collection");

    gc_pop_root();

    printf("\n  Popping root and collecting again -- everything should now be reclaimed\n");
    gc_collect();
    print_status("After final collection");
}

static void demo_cycle(void) {
    printf("\n========================================\n");
    printf("  Demo 3: Cyclic references\n");
    printf("========================================\n");

    heap_init();
    gc_init();

    Object *a = object_new_pair();
    Object *b = object_new_pair();
    object_add_ref(a, b);
    object_add_ref(b, a);

    gc_push_root(a);
    print_status("After creating a <-> b cycle, rooted through 'a'");

    printf("\n  Running collection -- cycle should survive since 'a' is rooted\n");
    gc_collect();
    print_status("After collection");

    gc_pop_root();
    printf("\n  Popping root -- cycle is now unreachable despite referencing each other\n");
    gc_collect();
    print_status("After final collection");
}

static void demo_heap_pressure(void) {
    printf("\n========================================\n");
    printf("  Demo 4: Heap pressure with periodic collection\n");
    printf("========================================\n");

    heap_init();
    gc_init();

    Object *keep = object_new_int(999);
    gc_push_root(keep);

    for (int i = 0; i < 50; i++) {
        object_new_int(i);
        if (i % 10 == 0) {
            printf("\n  Allocated %d throwaway objects, collecting...\n", i + 1);
            gc_collect();
        }
    }

    print_status("After allocating 50 unrooted objects with periodic collection");
    gc_pop_root();
}

int main(void) {
    demo_basic_reclaim();
    demo_pair_graph();
    demo_cycle();
    demo_heap_pressure();

    printf("\n========================================\n");
    printf("  All demos complete\n");
    printf("========================================\n\n");

    return 0;
}