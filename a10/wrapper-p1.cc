#include <iostream>
#include <cstdint>
#include "a10p1.h"

// === Insert any helper functions here

int64_t wain(int64_t* a, int64_t n) {
  // Insert mainline code here
  int64_t * ap = arena();
  int count = 0;
  for (int i = 0; i < n; i++) {
      int num = *(a+i);
      *(ap+num) += 1;
      if(*(ap+num) > count) {
          count = *(ap+num);
      }
  }
 return count; // replace as appropriate
}

// Do not modify the code below.
int main(int argc, char *argv[]) {
  int64_t l, c;
  int64_t* a;
  std::cout << "Enter length of array: ";
  std::cin >> l; 
  a = new int64_t[l];
  for(int64_t i = 0; i < l; ++i) {
    std::cout << "Enter value of array element " << i << " ";
    std::cin >> a[i];
  }
  c = wain(a,l);
  delete [] a;
  std::cerr << c << std::endl;
  return c;
}