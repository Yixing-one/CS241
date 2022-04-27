#include<iostream>
#include "a10p2.h"

int main() {
    // If cons is successful, it returns a non-zero memory address.
    // This program produces a nonzero exit code (indicating failure)
    // if cons returns a zero memory address.
    return (cons(1,0) == 0);
}