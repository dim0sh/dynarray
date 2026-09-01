# A C standard lib project
providing general data-structures and functionality
## Features
### dynarray.h
General dynamic array 
General slices
Functionality provided by dynarray.h:
        - Dynamic array of any type
        - Slice of any type
        - Amortized O(1) push and pop operations
        - O(1) get and set operations
        - O(1) swap remove
        - O(n) insert and remove
        - O(n + m) concatenation
        - O(n) partitioning
        - O(n) mapping and filtering
        - O(n) rotate like c++ rotate
        - O(n) find first item maching condition (first_cond)
            can be made to be upper or lower bound by passing the according comparison function.
        - insertion sort using rotate and upper_bound.
            upper_bound is created by passing a a_greater_than_b function as the condition to first_cond.
            thus a_greater_than_b must be provided to insertion sort.
        - Unit tests
        - realloc and free can be custom defined
        - print all elements with specified format.
