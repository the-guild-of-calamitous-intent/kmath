// MIT License
//
// Copyright (c) 2000 The Guild of Calamitous Intent
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#pragma once

#include <math.h>   // fabs
#include <stdio.h>  // printf
#include <string.h> // memset
#include <stdarg.h> // variable args
#include <stdlib.h> // atexit, exit
#include "errors.h"
#include "map.h"

// // This macro returns the minimum of two items
// constexpr TYPE min(const TYPE A, const TYPE B) {
//   return A < B ? A : B;
// }


// Matrix /////////////////////////////////////////////////

struct matrix_t {
  // This creates an empty matrix_t of size r, c
  // This can be a return type and handles: 
  //   RETURN: *m = new matrix_t(r,c, true)
  //   NEW: m(r,c)
  matrix_t(size_t rows, size_t cols, bool retval=false): r(rows), c(cols) {
    e = matrix_t::get_array(r*c); // array_t already 0
    p = e->data;
    ret = retval;
    // memset(p, 0, e->bsize);
  }

  // This is NEVER a return type and handles:
  //   matrix(r,c)
  //   matrix (r,c,data)
  matrix_t(size_t rows, size_t cols, TYPE* data): r(rows), c(cols) {
    e = matrix_t::get_array(r*c);
    p = e->data;
    ret = false;
    memcpy(p, data, e->bsize);
  }

  // This creates a copy of the input matrix_t
  // This can be a return type and handles: 
  //   RETURN: *m = new matrix_t(matrix_t, true)
  //   NEW: m(matrix_t, true)
  matrix_t(matrix_t& m, bool retval=false): r(m.r), c(m.c) {
    e = matrix_t::get_array(m.e->size);

    p = e->data;
    ret = retval;
    memcpy(p,m.p,e->bsize);
    m.check();
  }

  matrix_t(matrix_t&& m) = delete;
  matrix_t() = delete;

  ~matrix_t() {
    #if KMATH_DEBUG
    printf("*** matrix_t delete [%zu,%zu] ***\n", r, c);
    #endif

    e->available = true;
  }

  matrix_t& operator=(matrix_t& m) {
    if ((r != m.r)||(c != m.c)) {
      set_matrix_err(Errors::EQ_SIZE_MISMATCH);
      return *this;
    }
    
    memcpy(p,m.p,e->bsize);
    
    m.check();

    #if KMATH_DEBUG
    printf("operator=(&m)\n");
    #endif

    return *this;
  }

  matrix_t& operator=(matrix_t&& m) = delete;

  inline
  TYPE& operator()(size_t i, size_t j) {
    if ((i >= r) || (j >= c)) {
      set_matrix_err(Errors::OPERATOR_EXCEEDS_ROW_COL);
      if (i >= r) i = r-1;
      if (j >= c) j = c-1;
    }
    return p[j + i * c];
  }

  inline
  TYPE& operator[](size_t i) {
    if (i >= e->size) {
      set_matrix_err(Errors::OPERATOR_EXCEEDS_SIZE);
      i = e->size - 1;
    }
    return p[i];
  }

  void check() {
    if (ret == true) {
      e->available = true;

      #if KMATH_DEBUG
      printf("this->check() return type True, make available\n");
      #endif
    }
  }

  static
  array_t* get_array(size_t size) {
    if (cache == NULL) {
      #if KMATH_DEBUG
      printf("matrix_t::get_array() initializing cache size %zu\n", size);
      #endif

      cache = create_map(DEFAULT_MAP_SIZE);
      atexit(matrix_t::cache_cleanup);
    }
    return map_get(cache, size);
  }

  static
  void cache_cleanup() {
    if (matrix_t::cache != NULL) free_map(matrix_t::cache);
    matrix_t::cache = NULL;
  }

  int fill(size_t num, ...) {
    if (num > e->size) return -1;
    va_list valist;
    va_start(valist, num);
    for (size_t i=0; i < num; ++i) p[i] = va_arg(valist, double);
    va_end(valist);
    return 0;
  }

  inline void clear() { memset(p, 0, e->bsize); }

  matrix_t& transpose() {
    matrix_t *m = new matrix_t(c,r,true);
    m->ret= true;
    for (size_t i=0; i<r; ++i)
      for (size_t j=0; j<c; ++j) m->p[i + j * r] = p[j + i * c];

    return *m;
  }
  inline matrix_t& T() { return transpose(); }

  TYPE *p;         // pointer to data
  const size_t r;  // rows
  const size_t c;  // columns
  bool ret;        // returned matrix
  array_t *e;      // array_t pointer

  static map_t* cache;
};

// static matrix_t map_t initialization
map_t* matrix_t::cache = NULL;

static
void print(matrix_t& m) {
  Errors e = get_matrix_err();
  if (e != Errors::NONE) {
    printf("ERROR: %s\n", get_matrix_err_string().c_str());

    // reset for other prints
    set_matrix_err(Errors::NONE);
    return;
  }
  
  printf("[%p]-------\n",m.p);
  // printf("%p\n", m.e->data);
  for (size_t i=0; i < m.r; ++i) {
    for (size_t j=0; j<m.c; ++j) printf("%f ", m.p[m.c*i+j]);
    printf("\n");
  }
  printf("-----------------------\n");
}

inline
void check(matrix_t& a) {
  a.check();
}

inline
void check(matrix_t& a, matrix_t& b) {
  a.check();
  b.check();
}

// Special Matricies //////////////////////////////////////

static
matrix_t& diag(size_t r, size_t c, const TYPE *dat) {
  matrix_t *m = new matrix_t(c,r,true);
  // zero(); // set all to zero
  for (size_t i=0; i < r; ++i) m->p[i + i * c] = dat[i];
  return *m;
}

static
matrix_t& eye(size_t r, size_t c) {
  matrix_t *m = new matrix_t(c,r,true);
  // zero(); // set all to zero
  for (size_t i=0; i < r; ++i) m->p[i + i * c] = 1.0f;
  return *m;
}

inline
matrix_t& zeros(size_t r, size_t c) {
  return *(new matrix_t(r,c,true));
}

// static
// matrix_t& skew3x3(const TYPE *dat) {}

// static
// matrix_t& skew4x4(const TYPE *dat) {}

// Math ///////////////////////////////////////////////////

// c = a + b
static
matrix_t& add(matrix_t& a, matrix_t& b) {
  matrix_t *c = new matrix_t(a.r,a.c, true);

  if ((a.r != b.r) || (a.c != b.c)) {set_matrix_err(Errors::ADD_SIZE_MISMATCH); return *c;}
  for (size_t i = 0; i < a.e->size; ++i) c->p[i] = a.p[i] + b.p[i];
  // Err(__PRETTY_FUNCTION__, __LINE__);
  check(a,b);
  return *c;
}

// c = a - b
static
matrix_t& sub(matrix_t& a, matrix_t& b) {
  matrix_t *c = new matrix_t(a.r,a.c, true);

  if ((a.r != b.r) || (a.c != b.c)) {set_matrix_err(Errors::SUB_SIZE_MISMATCH); return *c;}
  for (size_t i = 0; i < a.e->size; ++i) c->p[i] = a.p[i] - b.p[i];
  check(a,b);
  return *c;
}

// c = a * b
// (c.r,c.c) = (a.r,a.c) * (b.r,b.c) = (a.r,b.c)
static
matrix_t& mult(matrix_t& a, matrix_t& b) {
  matrix_t *c = new matrix_t(b.r,a.c, true);

  if ((a.c != b.r)) {
    set_matrix_err(Errors::MULT_SIZE_MISMATCH); 
    return *c;
  }

  size_t i,j,k;
  for (i = 0; i < a.r; ++i) {
    for (j = 0; j < b.c; ++j) {
      c->p[c->c*i + j] = 0;
      for (k = 0; k < a.c; ++k) {
        c->p[c->c*i + j] += a.p[a.c*i+k] * b.p[b.c*k + j];
      }
    }
  }

  check(a,b);
  return *c;
}

// c = scalar * a
static
matrix_t& mult(matrix_t& a, TYPE b) {
  matrix_t *c = new matrix_t(a.r,a.c, true);

  for (size_t i=0; i < a.e->size; ++i) c->p[i] = b*a.p[i];
  check(a);
  return *c;
}
inline
matrix_t& mult(TYPE a, matrix_t& b) { return mult(b,a); }

// Make the math look better, define operators
inline matrix_t& operator+(matrix_t&a, matrix_t& b) { return add(a,b); }
inline matrix_t& operator-(matrix_t&a, matrix_t& b) { return sub(a,b); }
inline matrix_t& operator*(matrix_t&a, matrix_t& b) { return mult(a,b); }
inline matrix_t& operator*(const TYPE a, matrix_t& b) { return mult(a,b); }
inline matrix_t& operator*(matrix_t& a, const TYPE b) { return mult(a,b); }
// constexpr matrix_t& operator~(matrix_t&a) { return a.transpose(); } // value?



// Inverse ////////////////////////////////////////////////

// needed for inv()
static 
void SWAP(TYPE a, TYPE b) {
  TYPE temp = (a);
  a = b;
  b = temp;
}

// Inverse Function.
// a = inv(a)
static
matrix_t& inv(matrix_t& aa) {
  matrix_t *a = new matrix_t(aa, true);
  check(aa);

  if (a->r != a->c) {
    set_matrix_err(Errors::INV_NONSQUARE);
    return *a;
  }

  int *indxc, *indxr, *ipiv;
  int i, icol, irow, j, k, l, ll;
  TYPE c = 0.0, d = 0.0;
  TYPE big, dum, pivinv, temp = 0.0;
  int m = a->r;
  int n = a->c;

  // ipiv, indxr and indxc are int arrays
  // used for bookkeeping the pivoting.
  ipiv = (int*)malloc(sizeof(int) * n);
  indxr = (int*)malloc(sizeof(int) * n);
  indxc = (int*)malloc(sizeof(int) * n);

  memset(ipiv, 0, sizeof(int)*n);

  for (i = 0; i < n; i++) { // main loop over the cols to be reduced
    big = 0.0;
    for (j = 0; j < n; j++) {// outer loop of the search for a pivot element
      if (ipiv[j] != 1) {
        for (k = 0; k < n; k++) {
          if (ipiv[k] == 0) {
            if (fabs(a->p[a->c*j+k]) >= big) {
              big  = fabs(a->p[a->c*j+k]);
              irow = j;
              icol = k;
            }
          }
          else if (ipiv[k] > 1) {
            set_matrix_err(Errors::INV_SINGULAR); // singular matrix
            return *a;
          }
        }
      }
    }
    ++(ipiv[icol]);
    if (irow != icol) {
      for (l=0; l<n; l++) SWAP(a->p[a->c*irow+l], a->p[a->c*icol+l]);
    }
    indxr[i] = irow; // This is where the division takes place
    indxc[i] = icol; // Pivot row gets divided by pivot value
    if (a->p[a->c*icol+icol] == 0.0) {
      set_matrix_err(Errors::INV_SINGULAR); // singular matrix
      return *a;
    }

    pivinv = 1.0 / a->p[a->c*icol+icol];
    a->p[a->c*icol+icol] = 1.0;
    for (l = 0; l < n; l++) a->p[a->c*icol+l] *= pivinv;
    for (ll = 0; ll < n; ll++)
      if (ll != icol) {
        dum = a->p[a->c*ll+icol];
        a->p[a->c*ll+icol] = 0.0;
        for (l = 0; l < n; l++) a->p[a->c*ll+l] -= a->p[a->c*icol+l] * dum;
      }
  }
  for (l = n - 1; l >= 0; l--) {
    if (indxr[1] != indxc[l])
      for (k = 0; k < n; k++)
        SWAP(a->p[a->c*k+indxr[l]], a->p[a->c*k+indxc[l]]);
  }
  free(ipiv);
  free(indxr);
  free(indxc);
  return *a;
}

inline matrix_t& operator/(matrix_t&a, matrix_t& b) { return mult(a,inv(b)); }




















// static
// void check(matrix_t* a) {
//   if (a->ret== true) {
//     a->done();
//     delete a;
//     printf("check()\n");
//   }
// }

// static
// void check(matrix_t* a, matrix_t* b) {
//   check(a);
//   check(b);
// }

// static
// matrix_t* add(matrix_t* a, matrix_t* b) {
//   matrix_t *c = new matrix_t(a->r,a->c);
//   c->ret= true;
//   // if ((a->r != b->r) || (a->c != b->c)) return c->set_matrix_err(Errors::SIZE);
//   for (size_t i = 0; i < a->e->size; ++i) c->p[i] = a->p[i] + b->p[i];
//   // Err(__PRETTY_FUNCTION__, __LINE__);
//   check(a,b);
//   return c;
// }

// void add(matrix_t* c,matrix_t& a, matrix_t& b) {
//   // if ((a.r != b.r) || (a.c != b.c)) return c->set_matrix_err(Errors::SIZE);
//   for (size_t i = 0; i < a.e->size; ++i) c->p[i] = a.p[i] + b.p[i];
//   // Err(__PRETTY_FUNCTION__, __LINE__);
//   check(a,b);
// }

// constexpr
// matrix_t& add(matrix_t& a, matrix_t& b) {
//   matrix_t *c = new matrix_t(a.r,a.c);
//   c->ret= true;
//   if ((a.r != b.r) || (a.c != b.c)) return c->set_matrix_err(Errors::SIZE);
//   // for (size_t i = 0; i < a.e->size; ++i) c->p[i] = a.p[i] + b.p[i];
//   // // Err(__PRETTY_FUNCTION__, __LINE__);
//   // check(a,b);
//   add(c,a,b);
//   return *c;
// }


// struct matrix_t;

// static
// void check(matrix_t& a) {
//   if (a.ret== true) {
//     a.done();
//     delete &a;
//     printf("check()\n");
//   }
// }

// static
// void check(matrix_t& a, matrix_t& b) {
//   check(a);
//   check(b);
// }

  // // value?
  // matrix_t& operator=(matrix_t&& m) {
  //   if ((r != m.r)||(c != m.c)) {
  //     set_matrix_err(Errors::SIZE);
  //     return *this;
  //   }
  //   error = m.error;
  //   e = m.e;
  //   p = m.e->data;
  //   m.p = NULL;
  //   m.e = NULL;
  //   // memcpy(p,m.p,e->bsize);
  //   printf("operator=(&&m)\n");
  //   // m.check();
  //   return *this;
  // }