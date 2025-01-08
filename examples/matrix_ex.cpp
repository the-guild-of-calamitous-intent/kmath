
#define KMATH_DEBUG 1

#include <stdio.h>
#include "kmath.h"

void func() {
  printf("\n===========\n\n");
  float p[] = {1,2,3,4};
  matrix_t v(4,1,p);

  float pp[] = {11,2,3,4, 1,12,3,4, 1,2,13,4, 1,2,3,14};
  matrix_t a(4,4,pp);
  matrix_t b(4,4,pp);
  matrix_t d(4,4,pp);
  matrix_t e(4,4,pp);
  matrix_t f(4,4,pp);

  matrix_t u = (a*b*d/e)*inv(f)*v;
  if (get_matrix_err() != Errors::NONE) printf("\n>> crap\n\n");

  for (int i=0; i < 10; ++i) matrix_t c = (a * b) * (a + b) - inv(a)*inv(b);
  if (get_matrix_err() != Errors::NONE) printf("\n>> crap\n\n");

  print(matrix_t::cache);

  printf("\n===========\n\n");
}


int main() {
  float p[] = {10,0,0,10};
  matrix_t f(2,2,p);
  matrix_t g = inv(f);
  print(g); // [[0.1,0],[0,0.1]]

  float pp[] = {11,2,3,4, 1,12,3,4, 1,2,13,4, 1,2,3,14};
  // matrix_cache(5);
  matrix_t a(4,4,pp); print(a);
  matrix_t b(4,4,pp); print(b);
  b = mult(2.5, b); print(b);

  for (int i=0; i < 1; ++i) {
    matrix_t c = add(a,sub(b,b));
    c = add(a,mult(sub(a,mult(b,3)),sub(a,b)));
    c = add(a,mult(sub(a,mult(3,b)),a));
    c = add(a,mult(sub(a,mult(b,3.3)),a));
  }

  matrix_t m = a+2*a/a*2 - a; // 4*I
  print(m);

  func();
  func();
  func();

  printf("About to exit\n");
  // print(matrix_t::cache);
  return 0;
}