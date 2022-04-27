#include <iostream>
#include <cstdint>
#include "a10p2.h"

void print(int64_t* cp) {
    if (cp == NULL) {
        return;
    }
    int64_t* cp2 = cdr(cp);
    print(cp2);
    char c = car(cp);
    std::cout << c;
}

// === Insert any helper functions here
int64_t wain(int64_t*, int64_t) {
    char c;
    int64_t c_num;
    int64_t numbyte = 0;
    int64_t* cp = NULL;
    int64_t* cp2 = NULL;
    while(std::cin >> std::noskipws >> c) {
        cp2 = cp;
        c_num = c;
        cp = cons(c_num, cp);
        if(cp == 0) {
            break;
            
        } else {
            numbyte += 1;
            cp = setcar(cp, c_num);
            cp = setcdr(cp, cp2);
        }
    }
    cp2 = cp;
    print(cp);
    cp = cp2;
    print(cp);
    while(cp2 != NULL) {
        cp = cp2;
        cp2 = cdr(cp2);
        snoc(cp);
    }
  return numbyte;
}


// Do not modify the code below.
int main(int argc, char *argv[]) {
  int ret = wain(0,0);
  std::cerr << ret << std::endl;
  return ret;
}