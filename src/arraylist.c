#include "arraylist.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
// #include <time.h>

void drop(void ** data, int cnt) {
    for (int i = 0; i < 0; i++) {
        free(data[i]);
    }
}

arrlist_t * arrlist_new_empty() {
    arrlist_t * result = (arrlist_t*) malloc(sizeof(arrlist_t));
    memset(result, 0, sizeof(arrlist_t));
    return result;
}

arrlist_t * arraylist_new() {
    arrlist_t * result = arrlist_new_empty();
    result->data = malloc(sizeof(void*)*MIN_SIZE);
    memset(result->data, 0, sizeof(void*)*MIN_SIZE);
    result->size = MIN_SIZE;
    return result;
}

void arraylist_resize(arrlist_t * list, int direction) {
    switch (direction) {
        case 1:
            list->data = realloc(list->data, sizeof(void*)*(list->size*2));
            list->size = list->size*2;
            break;
        case 0:
            // free old list missing
            if (list->size >= MIN_SIZE * 2) {
                list->data = realloc(list->data, sizeof(void*)*(list->size/2));
                list->size = list->size/2;
            }
            break;
    }
}

void arraylist_push(arrlist_t * list, void * ptr) {
    if (list->cnt < list->size) {
        list->data[list->cnt] = ptr;
        list->cnt++;
    } else {
        arraylist_resize(list, 1);
        arraylist_push(list, ptr);
    }
}

void* arraylist_pop(arrlist_t * list) {
    if (list->cnt > 0) {
        void * result = list->data[list->cnt-1];
        list->data[list->cnt-1] = 0;
        if (list->cnt <= list->size/3 && list->size > MIN_SIZE) {
            arraylist_resize(list, 0);
        }
        return result;
    }
    return NULL;
}

void* arraylist_get(arrlist_t * list, int idx) {
    if (idx < list->size) {
        void * result = list->data[idx];
        return result;
    }
    return NULL;
}

const void* arraylist_const_get(const arrlist_t * list, int idx) {
    if (idx < list->size) {
        const void * result = list->data[idx];
        return result;
    }
    return NULL;
}

void arraylist_set(arrlist_t * list, int idx, void * ptr) {
    if (idx < list->size) {
        list->data[idx] = ptr;
    }
}

void arraylist_filter(arrlist_t * list, filter filter_fnc) {
    void ** new_data = malloc(sizeof(void*)*list->size);
    int count = list->cnt;
    int jdx = 0;
    for (int idx = 0; idx < count; idx++) {
        if (!(filter_fnc)(list->data[idx])) {
            new_data[jdx] = list->data[idx];
            jdx++;
        }
    }
    list->cnt = jdx;
    void ** old = list->data;
    list->data = new_data;
    drop(old,list->cnt);
    free(old);
    if (list->cnt < list->size/3) {
        arraylist_resize(list, 0);
    }
}

void arraylist_filter_unstable_no_resize(arrlist_t *list, filter filter_fnc) {
    for (int idx = 0; idx < list->cnt; idx++) {
        if ((filter_fnc)(list->data[idx])) {
            list->data[idx] = list->data[--list->cnt];
            continue;
        }
    }
}

void arraylist_map(arrlist_t *list, map map_fnc) {
    for (int idx = 0; idx < list->cnt; idx++) {
        map_fnc(list->data[idx]);
    }
}

void arraylist_foreach(const arrlist_t *list, foreach foreach_fnc) {
    for (int idx = 0; idx < list->cnt; idx++) {
        (foreach_fnc)(list->data[idx]);
    }
}

void arraylist_destroy(arrlist_t *list) {
    drop(list->data,list->cnt);
    free(list->data);
    free(list);
}