/*  dynarray.h - typesafe dynamic array library 
    version 0.1.2 - Iain Dorsch - 2026

    To use this library, do the following in *one* of your .c files:
        #define DYNARRAY_IMPLEMENTATION
        #include "dynarray.h"
    In any other .c file that needs the library, just include the header:
        #include "dynarray.h"

    Too avoid collisions with other libraries, the standard API uses a "da_" prefix.
    Short names are also provided. 
    If prefix names are required short names can be disabled:
        #define DYNARRAY_NO_SHORT_NAMES

    Unit tests can be enabled by including the library as described above and 
    defining DYNARRAY_UNIT_TESTS. The function _da_unit_tests() must then be called to run the tests.
    Example:
        #define DYNARRAY_IMPLEMENTATION
        #define DYNARRAY_UNIT_TESTS
        #include "dynarray.h"

        int main() {
            _da_unit_tests();
            return 0;
        }
    
    Functionality provided by this library:
        - Dynamic array of any type
        - Amortized O(1) push and pop operations
        - O(1) get and set operations
        - O(1) swap remove
        - O(n) insert and remove
        - O(n + m) concatenation
        - O(n) partitioning
        - O(n) mapping and filtering
        - Unit tests
        - realloc and free can be custom defined

    Table of contents:
        - Library instructionss
        - short name API
        - Internal functions declarations
        - prefix name API
        - Internal functions implementations
        - Unit tests
*/

#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <stddef.h>
#include <string.h>

#define DYNARRAY_VERSION "0.1.2"
#define DA_UNUSED(...) (void)(__VA_ARGS__)
#define DA_MIN(a,b) ((a) < (b) ? (a) : (b))
#define DA_MAX(a,b) ((a) > (b) ? (a) : (b))

#if defined(DA_REALLOC) && !defined(DA_FREE) || !defined(DA_REALLOC) && defined(DA_FREE)
#error "Both realloc and free must be defined, one is not sufficient."
#endif
#if !defined(DA_REALLOC) && !defined(DA_FREE)
#include <stdlib.h>
#define DA_REALLOC(c,pointer,size) realloc(pointer,size)
#define DA_FREE(c,pointer) free(pointer)
#endif

typedef struct {
    size_t capacity;
    size_t cnt;
    char * data;
} dynarray_t;

#define DA_MIN_CAPACITY 100

// // // // // // // // // // // // // // //
extern void _da_unit_tests(void);
// // // // // // // // // // // // // // // 
// Short names API
#ifndef DYNARRAY_NO_SHORT_NAMES

#define arr                     da_arr
#define arr_reserve             da_arr_reserve
#define arr_set_value           da_arr_set_value
#define arr_set                 da_arr_set
#define arr_with                da_arr_with
#define arr_foreach             da_arr_foreach
#define arr_filter_each         da_arr_filter_each
#define arr_map_each            da_arr_map_each
#define arr_map_each_ctx        da_arr_map_each_ctx
#define arr_len                 da_arr_len
#define arr_capacity            da_arr_capacity
#define arr_is_empty            da_arr_is_empty
#define arr_clear               da_arr_clear
#define arr_push_value          da_arr_push_value
#define arr_push                da_arr_push
#define arr_free                da_arr_free
#define arr_swap_remove         da_arr_swap_remove
#define arr_filter_remove_unstable da_arr_filter_remove_unstable
#define arr_pop                 da_arr_pop
#define arr_get                 da_arr_get
#define arr_peek                da_arr_peek
#define arr_insert_value        da_arr_insert_value
#define arr_insert              da_arr_insert
#define arr_remove              da_arr_remove
#define arr_push_front          da_arr_push_front
#define arr_concat              da_arr_concat
#define arr_swap                da_arr_swap
#define arr_partition           da_arr_partition
#define arr_partition_ctx       da_arr_partition_ctx
#define arr_partition_ctx_range da_arr_partition_ctx_range
#define arr_ptr                 da_arr_ptr

#endif
// // // // // // // // // // // // // // // 
// Internal functions
extern dynarray_t *_array_empty();
extern dynarray_t *_array_init(size_t size, size_t init_capacity);
extern void _array_set(size_t size, dynarray_t *arr, size_t idx, void *elem);
extern dynarray_t * _array_init_with(size_t size, size_t count, void * elem);
extern void _array_push(size_t size, dynarray_t *arr, void *elem, size_t init_capacity);
extern void _array_swap_remove(size_t size, dynarray_t *arr, size_t idx, size_t init_capacity);
extern char *_array_pop(size_t size, dynarray_t *arr, size_t init_capacity);
extern char *_array_get(size_t size, dynarray_t *arr, size_t idx);
extern void _array_free(dynarray_t *arr);
extern void _array_insert(size_t size, dynarray_t *arr, size_t idx, void *elem, size_t init_capacity);
extern void _array_remove(size_t size, dynarray_t *arr, size_t idx, size_t init_capacity);
extern void _array_concat(size_t size, dynarray_t *dest, dynarray_t *other);
extern void _array_swap_item(size_t size, dynarray_t *array, size_t idx_one, size_t idx_two);
extern size_t _array_partition_range(size_t size, dynarray_t *array, size_t start, size_t end, int (*f)(const char *, void *ctx), void *ctx);
// // // // // // // // // // // // // // // 
// No short names API
#define da_arr_reserve(Type, size) (dynarray_t *)_array_init(sizeof(Type), size)
#define da_arr(Type) (dynarray_t *)_array_init(sizeof(Type), DA_MIN_CAPACITY)
#define da_arr_set_value(Type,array,index,elem) _array_set(sizeof(Type),array,index,(void *)&(Type){(elem)})
#define da_arr_set(Type,array,index,elem) _array_set(sizeof(Type),array,index,elem)
#define da_arr_with(Type, count, elem) (dynarray_t *)_array_init_with(sizeof(Type), count, elem)
#define da_arr_foreach(Type, item, array) for (Type *item = (Type *)array->data; item < (Type *)(array->data + array->cnt * sizeof(Type)); item++)
#define da_arr_filter_each(Type, item, array, condition) \
    for (Type *item = (Type *)array->data; \
         item < (Type *)(array->data + array->cnt * sizeof(Type)); \
         item++) \
            if (condition(item))\

#define da_arr_map_each(Type, item, array, map) \
    for (Type *item = (Type *)array->data; \
         item < (Type *)(array->data + array->cnt * sizeof(Type)); \
         map(item), item++) \

#define da_arr_map_each_ctx(Type, item, array, map, ctx) \
    for (Type *item = (Type *)array->data; \
         item < (Type *)(array->data + array->cnt * sizeof(Type)); \
         map(item, ctx), item++) \

#define da_arr_len(array) (array)->cnt
#define da_arr_capacity(array) (array)->capacity
#define da_arr_is_empty(array) ((array)->cnt == 0)
#define da_arr_clear(array) (array)->cnt = 0
#define da_arr_push_value(Type,array,elem) _array_push(sizeof(Type),array,(void *)&(Type){(elem)}, DA_MIN_CAPACITY)
#define da_arr_push(Type,array,elem) _array_push(sizeof(Type),array,elem, DA_MIN_CAPACITY)
#define da_arr_free(array) _array_free(array)
#define da_arr_swap_remove(Type,array,index) _array_swap_remove(sizeof(Type),array,index, DA_MIN_CAPACITY)
#define da_arr_filter_remove_unstable(Type, array, condition) do{\
    size_t idx = 0;\
    while(idx < array->cnt) {\
        Type *item = (Type *)(array->data + idx * sizeof(Type));\
        if(condition(item)) {\
            da_arr_swap_remove(Type, array, idx);\
        } else {\
            idx++;\
        }\
    }\
}while(0)

#define da_arr_pop(Type,array) (Type *)_array_pop(sizeof(Type), array, DA_MIN_CAPACITY)
#define da_arr_get(Type,array,index) (Type *)_array_get(sizeof(Type),array,index)
#define da_arr_peek(Type,array) da_arr_get(Type,array,array->cnt-1)
#define da_arr_insert_value(Type,array,index,elem) _array_insert(sizeof(Type),array,index,(void *)&(Type){(elem)}, DA_MIN_CAPACITY)
#define da_arr_insert(Type,array,index,elem) _array_insert(sizeof(Type),array,index,elem, DA_MIN_CAPACITY)
#define da_arr_remove(Type,array,index) _array_remove(sizeof(Type),array,index, DA_MIN_CAPACITY)
#define da_arr_push_front(Type,array,elem) da_arr_insert(Type,array,0,elem)
#define da_arr_concat(Type,array_one,array_two) _array_concat(sizeof(Type),array_one,array_two)
#define da_arr_swap(Type, array, idx_a, idx_b) _array_swap_item(sizeof(Type), array, idx_a, idx_b) 
#define da_arr_partition(Type,array,condition) _array_partition_range(sizeof(Type),array,0,array->cnt,(int(*)(const char *, void *))(condition), NULL)
#define da_arr_partition_ctx(Type,array,condition,ctx) _array_partition_range(sizeof(Type),array,0,array->cnt,(int(*)(const char *, void *))(condition), ctx)
#define da_arr_partition_ctx_range(Type,array,start,end,condition, ctx) _array_partition_range(sizeof(Type),array,start,end,(int(*)(const char *, void *))(condition), ctx)
#define da_arr_ptr(Type,array) ((Type*)((array)->data))
// // // // // // // // // // // // // // // 
// Internal functions implementations
#ifdef DYNARRAY_IMPLEMENTATION

#include <string.h>
#include <stdlib.h>
#include <stddef.h>

__attribute__((warn_unused_result))
dynarray_t *_array_empty() {
    dynarray_t *result = (dynarray_t *)DA_REALLOC(NULL,NULL,sizeof(dynarray_t));
    
    memset(result, 0, sizeof(dynarray_t));
    return result;
}

__attribute__((warn_unused_result))
dynarray_t *_array_init(size_t size, size_t init_capacity) {
    dynarray_t *ptr = _array_empty();
    ptr->data = (char *)DA_REALLOC(NULL,NULL,size*init_capacity);
    // memset(ptr->data, 0, size*init_capacity);
    ptr->capacity = init_capacity;
    return ptr;
}

__attribute((nonnull(2)))
void _array_set(size_t size, dynarray_t *arr, size_t idx, void *elem) {
    if (idx < arr->cnt) {
        char *ptr = arr->data;
        ptr = ptr + (idx * size);
        memcpy(ptr, elem, size);
    }
}

__attribute__((nonnull(3),warn_unused_result))
dynarray_t * _array_init_with(size_t size, size_t count, void * elem) {
    dynarray_t *ptr = _array_empty();
    ptr->data = (char *)DA_REALLOC(NULL,NULL,size*count);
    ptr->capacity = count;
    ptr->cnt = count;
    for (size_t i = 0; i < count; i++) {
        _array_set(size,ptr,i,elem);
    }
    return ptr;
}

__attribute__((nonnull(2)))
static void _array_resize(size_t size, dynarray_t *arr, int direction, size_t init_capacity) {
    switch (direction) {
        case 1:
            arr->data = (char *)DA_REALLOC(NULL, arr->data, size*arr->capacity*2);
            arr->capacity = arr->capacity*2;
            break;
        case 0:
            if (arr->capacity >= init_capacity * 2) {
                arr->data = (char *)DA_REALLOC(NULL, arr->data, size*(arr->capacity/2));
                arr->capacity = arr->capacity/2;
            }
            break;
    }   
}

__attribute((hot,nonnull(2)))
void _array_push(size_t size, dynarray_t *arr, void *elem, size_t init_capacity) {
    if (arr->cnt < arr->capacity) {
        char *ptr = arr->data;
        ptr = ptr + (size * arr->cnt);
        memcpy(ptr, elem, size);
        arr->cnt++;
    } else {
        _array_resize(size, arr, 1, init_capacity);
        _array_push(size, arr, elem, init_capacity);
    }
}

__attribute__((nonnull(2)))
void _array_swap_remove(size_t size, dynarray_t *arr, size_t idx, size_t init_capacity) {
    if (arr->cnt > 0) {
        if (idx == arr->cnt-1) {
            arr->cnt--;
        } else if (idx < arr->cnt) {
            char*ptr_dest = arr->data;
            ptr_dest = ptr_dest + (idx * size);
    
            char*ptr_src = arr->data;
            ptr_src = ptr_src + (arr->cnt-1)*size;
    
            memcpy(ptr_dest, ptr_src, size);
            arr->cnt--;
        }
        if (arr->cnt < arr->capacity/3) {
            _array_resize(size, arr, 0, init_capacity);
        }
    }

}

__attribute__((warn_unused_result,nonnull(2)))
char *_array_pop(size_t size, dynarray_t *arr, size_t init_capacity) {
    char *ptr = NULL;
    if (arr->cnt > 0) {
        ptr = arr->data + size * (arr->cnt - 1);
        arr->cnt--;
        if (arr->cnt < arr->capacity/3) {
            _array_resize(size, arr, 0, init_capacity);
        }
    }
    return ptr;
}

__attribute__((hot,warn_unused_result,nonnull(2)))
char *_array_get(size_t size, dynarray_t *arr, size_t idx) {
    char *ptr = NULL;
    if (idx < arr->cnt) {
        ptr = arr->data + (idx * size);
    }
    return ptr;
}

__attribute__((nonnull(1)))
void _array_free(dynarray_t *arr) {
    DA_FREE(NULL,arr->data);
    DA_FREE(NULL,arr);
}

__attribute__((nonnull(2,4)))
void _array_insert(size_t size, dynarray_t *arr, size_t idx, void *elem, size_t init_capacity) {
    if (idx == arr->cnt) {
        _array_push(size, arr, elem, init_capacity);
        return;
    }
    if (idx < arr->cnt) {
        if (arr->cnt < arr->capacity) {
            char*ptr = arr->data;
            char*ptr_src = ptr + (idx * size);
            char*ptr_dest = ptr_src + size;
            memcpy(ptr_dest, ptr_src, size * ((arr->cnt)-idx));
            memcpy(ptr_src, elem, size);
            arr->cnt++;
        } else {
            _array_resize(size, arr, 1, init_capacity);
            _array_insert(size, arr, idx, elem, init_capacity);
        }
    }
}

__attribute((nonnull(2)))
void _array_remove(size_t size, dynarray_t *arr, size_t idx, size_t init_capacity) {
    if (arr->cnt > 0) {
        if (idx == arr->cnt-1) {
            arr->cnt--;
            return;
        }
        if (idx < arr->cnt-1) {
            char*ptr = arr->data;
            char*ptr_dest = ptr + (idx * size);
            char*ptr_src = ptr_dest + size;
            memcpy(ptr_dest, ptr_src, size * ((arr->cnt-1)-idx));
            arr->cnt--;
        }
        if (arr->cnt < arr->capacity/3) {
            _array_resize(size, arr, 0, init_capacity);
        }
    }
}

__attribute__((nonnull(2,3)))
void _array_concat(size_t size, dynarray_t *dest, dynarray_t *other) {
    if (dest->capacity > dest->cnt + other->cnt) {

        char *ptr_dest = dest->data + size * dest->cnt;
        char *ptr_src = other->data;
        memcpy(ptr_dest, ptr_src, size * (other->cnt));

        dest->cnt = dest->cnt + other->cnt;
        _array_free(other);
    } else {
        dest->data = (char *)DA_REALLOC(NULL, dest->data, size*(dest->capacity + other->cnt));
        dest->capacity = dest->capacity + other->cnt;
        _array_concat(size, dest, other);
    }
}

__attribute__((hot,nonnull(2)))
void _array_swap_item(size_t size, dynarray_t *array, size_t idx_one, size_t idx_two) {
    if (idx_one < array->cnt && idx_two < array->cnt && idx_one != idx_two) {
        char * tmp_one = array->data + size * idx_one;
        char * tmp_two = array->data + size * idx_two;
        if (*tmp_one != *tmp_two) {
            *tmp_one = *tmp_one ^ *tmp_two;
            *tmp_two = *tmp_one ^ *tmp_two;
            *tmp_one = *tmp_one ^ *tmp_two;
            // char * tmp = (char *)malloc(size);
            // memcpy(tmp, tmp_one, size);
            // memcpy(tmp_one, tmp_two, size);
            // memcpy(tmp_two, tmp, size);
            // free(tmp);
        }
    }
}  

__attribute__((hot,warn_unused_result,nonnull(2,5)))
size_t _array_partition_range(size_t size, dynarray_t *array, size_t start, size_t end, int (*f)(const char *, void *ctx), void *ctx) {
    size_t tmp_true = start;
    size_t idx = start;

    for (char * elem = array->data + (start * size); elem < (char *)(array->data + DA_MIN(end * size,array->cnt*size)); elem += size) {
        if (f(elem, ctx)) {
            _array_swap_item(size, array, idx, tmp_true);
            tmp_true++;
        }
        idx ++;
    }
    return tmp_true;
}

#endif
// // // // // // // // // // // // // // // 
// Unit tests
#ifdef DYNARRAY_UNIT_TESTS

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <stdio.h>

static int _da_unit_test_part_condition(const int *item, void *ctx) {
    DA_UNUSED(ctx);
    return (*item % 2) == 0;
}

static void _da_unit_test_map(int *item) {
    *item = (*item) * 2;
}

void _da_unit_tests(void) {
    dynarray_t *array = arr(int);
    int i, j;
    // assert length of array in two states
    assert(arr_len(array) == 0);
    for (i = 0; i < 100; i++) {
        arr_push_value(int, array, i);
    }
    assert(arr_len(array) == 100);
    // test foreach macro by checking the values of the retrieved items
    arr_foreach(int, item, array) {
        assert(*item == j);
        j++;
    }
    // assert if the values in the array are correct by get operation
    for (i = 0; i < 100; i++) {
        assert(*arr_get(int, array, i) == i);
    }
    // pop all items from array and assert their values are correct
    for (i = 0; i < 100; i++) {
        assert(*arr_pop(int, array) == 99 - i);
    }
    // after popping all items the array should be emtpy
    assert(arr_is_empty(array));
    // numbers 0-99 pushed to array
    for (int i = 0; i < 100; i++) {
        arr_push_value(int, array, i);
    }
    // partition array with even numbers at the front and odd numbers at the back
    size_t partition_index = arr_partition(int, array, _da_unit_test_part_condition);
    // assert if partition is successfull the start of the odd numbers must be at index 50
    assert(partition_index == 50);

    // assert correctness of values after mapping operation
    arr_map_each(int, item, array, _da_unit_test_map);
    for (i = 0; i < 50; i++) {
        assert(*arr_get(int, array, i) == i*2*2);
    }

    for (i = 50; i < 100; i++) {
        arr_insert_value(int, array, i, 101);    
    }

    assert(arr_len(array) == 150);
    for (i = 50; i < 100; i++) {
        assert(*arr_get(int, array, i) == 101);
    }

    arr_free(array);

    int tmp = 5;
    array = arr_with(int, 10, &tmp);
    assert(arr_len(array) == 10);
    arr_foreach(int, item, array) {
        assert(*item == 5);
    }

    arr_set_value(int, array, 3, 3);
    assert(*arr_get(int, array, 3) == 3);

    arr_swap_remove(int, array, 3);
    assert(arr_len(array) == 9);
    
    for (i = 0; i < 9; i++) {
        assert(*arr_get(int, array, i) == 5);
    }

    arr_clear(array);
    arr_push_value(int,array,10);
    arr_push_value(int,array,5);

    assert(arr_ptr(int,array)[0] == 10);

    assert(arr_ptr(int,array)[1] == 5);
    
    arr_ptr(int,array)[1] = 11;

    assert(arr_ptr(int,array)[1] == 11);

    arr_free(array);
    array = NULL;
}

#endif

#endif
