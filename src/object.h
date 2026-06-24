#ifndef OBJECT_H
#define OBJECT_H

#define MAX_REFS 8

typedef enum {
    OBJ_INT,
    OBJ_PAIR
} ObjectType;

typedef struct Object Object;

struct Object {
    ObjectType type;
    int marked;

    Object *next;

    union {
        int value;
        struct {
            Object *refs[MAX_REFS];
            int ref_count;
        } pair;
    } data;
};

Object *object_new_int(int value);
Object *object_new_pair(void);
void object_add_ref(Object *pair, Object *child);
void object_print(Object *obj);

#endif