#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <stdint.h>

#define MIN_SIZE 100

typedef int (*filter)(const void * data);

typedef void (*map)(void * data);

typedef void (*foreach)(const void * data);

typedef struct ArrayList {
    uint16_t cnt;
    uint16_t size;
    void ** data;
} arrlist_t;

// return pointer to new list
arrlist_t * arraylist_new();
// pushback onto list
void arraylist_push(arrlist_t * list, void * ptr);
// remove from back
void* arraylist_pop(arrlist_t * list);
// get data at index
void* arraylist_get(arrlist_t * list, int idx);
// set data at index
void arraylist_set(arrlist_t * list, int idx, void * ptr);
// remove elements from list matching condition of passed function
// arraylist_filter(list, (filter)&filter_fnc);
void arraylist_filter(arrlist_t * list, filter filter_fnc);
void arraylist_filter_unstable_no_resize(arrlist_t *list, filter filter_fnc);
// can change list data
// arraylist_map(list, (map)&map_fnc);
void arraylist_map(arrlist_t * list, map map_fnc);
// cannot change list data (const *)
// arraylist_foreach(list, (foreach)&foreach_fnc);
void arraylist_foreach(const arrlist_t *list, foreach foreach_fnc) ;
// free list
void arraylist_destroy(arrlist_t *list);

#endif 