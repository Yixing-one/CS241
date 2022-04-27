#include <iostream>
#include <cstdint>
#include "a10p1.h"
using namespace std;

int64_t *freep = arena();
int64_t *starting_address = arena();
int64_t available_size = *freep;
bool initial_step = true;

// allocates a block of memory of at least size words (not bytes), 
// and returns the address of that memory or 0 (NULL) if memory could not be allocated.
int64_t *mymalloc(int64_t size) {
    //cerr << "size" << size << endl; 
    if(size == 0) {
        return 0;
    }
    if(size < 2) {
        size = 2;
    }
    if(initial_step) {
        *freep  = *freep / 8;
        initial_step = false;
    }
    int64_t *freepointer = freep;
    if(freep == NULL) {
        //cerr << "free null" << endl;
    }
    while(freepointer != NULL) {
        //cerr << "location "  << freepointer  - starting_address << endl;
        if(*freepointer < (size + 1)) {
            if(*(freepointer+2) != 0) {
                freepointer = *(freepointer+2) + starting_address;
            } else {
                return NULL;
            }
        } else {
            if(*freepointer < (size + 4)) {
                int64_t *freelast = *(freepointer+1) + starting_address;
                int64_t *freenext = *(freepointer+2) + starting_address;
                if((*(freepointer+2) != 0) && (*(freepointer+1) != 0)) {
                    *(freenext + 1) = *(freepointer+1);
                    *(freelast + 2) = *(freepointer+2);
                } else if (*(freepointer+1) != 0) {
                    *(freelast+2) = 0;
                } else if (*(freepointer+2)!= 0) {
                    *(freenext + 1) = 0;
                    freep = freenext;
                } else {
                    freep = NULL;
                }
                return (freepointer + 1);
            } else {
                int64_t original_size = *freepointer;
                int64_t lastaddress = *(freepointer+1);
                int64_t nextaddress = *(freepointer+2);
                *freepointer = size + 1;
                int64_t *freenext = freepointer + (size + 1);
                int64_t nextsize = original_size - (size + 1); 
                *freenext = nextsize;
                *(freenext + 1) = lastaddress;
                *(freenext + 2) = nextaddress;
                if(lastaddress == 0) {
                    freep = freenext;
                } else {
                    int64_t* last = *(freepointer + 1) + starting_address;
                    *(last + 2) = freenext - starting_address;
                }
                return freepointer + 1;
                }
            }
        }
        return 0;
    }


// deallocates the memory stored at address.
// assumes that address contains either an address allocated by mymalloc, in which case it deallocates that memory, 
// or the value 0 (NULL), in which case myfree does nothing.
void myfree(int64_t *address) {
    if(address == NULL) {
        return;
    }
    address = address - 1;
    int64_t *freepointer = freep;
    *(address+1) = 0;
    *(address+2) = freepointer - starting_address;
    *(freepointer+1) = address - starting_address;
    freep = address;
}

int main() {
    int64_t result = 0;
    int64_t* one = mymalloc(241);
    //int64_t* two = mymalloc(241);
    myfree(one);

    int64_t* evil = mymalloc(240);
    int64_t* good = mymalloc(1);
    evil[3] = 240; 
    good[0] = evil[3]+1;
    myfree(evil);

    int64_t* cs[241];
    for(int i=0; i<241; i++) {
        cs[i] = mymalloc(1);
        cs[i][0] = 1;
    }
    int64_t* big = mymalloc(1000);
    for(int i=0; i<1000; i++) {
        big[i] = 1000;
    }
    for(int i=0; i<241; i++) {
        result += cs[i][0];
        myfree(cs[i]);
    }

    result += good[0];
    myfree(good);

    // Returns nonzero exit code if the result is wrong
    return result != 241*2;
}