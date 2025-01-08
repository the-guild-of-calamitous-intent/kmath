#define KMATH_DEBUG 1

#include <stdio.h>
#include "kmath.h"

void eq() {
  float p[] = {1,2,3,4};
  matrix_t a(2,2);
  matrix_t b(2,2,p);

  a = b;

  printf("eq(): ");
  for (size_t i=0; i<4; ++i) printf("%f ", a[i]);
  printf("\n");
}

void eq2() {
  matrix_t a = zeros(2,2);

  printf("eq2(): ");
  for (size_t i=0; i<4; ++i) printf("%f ", a[i]);
  printf("\n");
}

int main() {
  eq();
  eq2();

  return 0;
}