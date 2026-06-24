![CI](https://github.com/Hanningtone03/garbage-collector/actions/workflows/ci.yml/badge.svg)

# Garbage Collector

A mark-and-sweep garbage collector built in C; heap allocation, reachability tracing from a root set, and automatic reclamation of unreachable memory, including cyclic references.

## How it works

Objects are allocated on a tracked heap and linked together in a list. A small root stack represents what's directly reachable from the program at any moment. Collection happens in two phases: mark walks the object graph from every root, flagging everything reachable; sweep then walks the full heap, freeing anything left unmarked and resetting marks on survivors for the next cycle.

Unlike naive reference counting, this correctly reclaims cyclic references; two objects pointing to each other get collected once nothing external roots them, which reference counting alone cannot do.

## Project structure

```
src/
├── heap.c / heap.h
├── object.c / object.h
├── gc.c / gc.h
└── main.c
```

## Building

```bash
gcc -Wall -o gc src/main.c src/heap.c src/object.c src/gc.c
```

## Running

```bash
./gc
```

Runs four demos: basic reclamation, reachability through a pair, cyclic references, and heap pressure with periodic collection.

## Example output

```
========================================
  Demo 3: Cyclic references
========================================
  After creating a <-> b cycle, rooted through 'a'
  Heap size: 2 objects, 1 root(s) on stack
  Running collection -- cycle should survive since 'a' is rooted
  [gc] collection complete: 2 -> 2 objects
  Popping root -- cycle is now unreachable despite referencing each other
  [gc] reclaimed 2 object(s)
  [gc] collection complete: 2 -> 0 objects
```

## Tech

- C
- No external dependencies
