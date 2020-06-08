#include <stdio.h>

#define POW(x) (x*x)

/*
 * Clang defaults to C99 and not GNU sematics, which means a raw inline is different from both static inline and extern inline.
 * In particular, a raw inline means that the function still has external linkage, but the inline definition does not provide the external one (you'd need extern inline for that).
 */ 
static inline int power_inline(int x) {
  return x*x;
}

int power_func(int x) {
  return x*x;
}

int main() {
  printf("Fun MARCO\n");
  int a = 2;
  printf("a = %d\n", POW(a++));
  a = 2;
  printf("a = %d\n", power_func(a++));
  a = 2;
  printf("a = %d\n", power_inline(a));
  return 0;
}

