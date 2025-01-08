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

#include <stdio.h>  // printf
#include <stdlib.h> // malloc/free
#include <string.h> // memset
#include <stdbool.h> // bool for C
// #include "errors.h"

#ifdef __cplusplus
extern "C" {
#endif

// static
// void mem_alloc_error() {
//   fprintf(stderr, "Memory allocation failed\n");
//   exit(EXIT_FAILURE);
// }

typedef struct {
  TYPE *data;     // data array
  size_t size;    // number of elements
  size_t bsize;   // number of bytes
  bool available; // available for use [true, false]
} array_t;

static
void clear_array(array_t* a) {
  memset(a->data, 0, a->bsize);
}


static
array_t* create_array(size_t size) {
  // printf("start array\n");
  array_t* a = NULL;
  a = (array_t*) malloc(sizeof(array_t));
  a->size = size;
  a->bsize = size*sizeof(TYPE);
  a->available = false;

  // calloc initialized to 0 also in one step
  a->data = (TYPE*) calloc(size, sizeof(TYPE));

  // a->data = (TYPE*) malloc(a->bsize);
  // clear_array(a);

  // printf("create array %zu\n", size);
  return a;
}

static
void pprint(array_t* a) {
  printf("size: %zu avail: %u data: %p\n [", a->size,a->available,a->data);
  for (size_t i=0; i < a->size; ++i) printf("%f,", a->data[i]);
  printf("]\n");
}

static
void free_array(array_t* a) {
  // printf(" - free %zu array_t\n", a->size);
  free(a->data);
}

#ifdef __cplusplus
};
#endif