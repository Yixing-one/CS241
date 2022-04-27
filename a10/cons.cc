#include <iostream>
#include <cstdint>
#include "a10p1.h"

/*
 * Example usage:
 * cons(1, cons(2, cons(3, nullptr)));
 * This creates a linked list of the form 1 -> 2 -> 3.
 */

int64_t *freep = arena();
int64_t *starting_address = arena();
int64_t available_size = *freep / 8;


//returns the address of a pair of words from the arena, initialized to a and b respectively, or 0 if no memory is available.
int64_t *cons(int64_t a, int64_t *b) {
    if(available_size < 2) {
        return 0;
    }
    int64_t *freep2;
    int64_t *freep3 = freep;
    if(available_size >= 4) {
        if(*(freep+1) == 0) {
            freep2 = freep + 2;
        } else {
            freep2 = *(freep + 1) + starting_address;
        }
    } else {
        freep2 = NULL;
    }
    *freep = a;
    *(freep + 1) = (b - starting_address);
    freep = freep2;
    available_size -= 2;
    return freep3;
}

//returns the first element of the pair whose address is p.
int64_t car(int64_t *p){
    return *p;
}

//returns the second element of the pair whose address is p.
int64_t *cdr(int64_t *p) {
    int64_t num = *(p+1);
    return num+starting_address;
}

//sets the first element of p to the value v and returns p.
int64_t *setcar(int64_t *p, int64_t v) {
    *p = v;
    return p;
}

//sets the second element of p to the value v and returns p.
int64_t *setcdr(int64_t *p, int64_t *v) {
     *(p+1) = (v - starting_address);
     return p;
}

//deletes the pair whose address is p, or does nothing if p is null.
void snoc(int64_t *p) {
    *(p+1) = (freep - starting_address);
    freep = p;
    available_size += 2;
}