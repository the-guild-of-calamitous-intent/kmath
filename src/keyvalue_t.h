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
#include <stdbool.h>
#include "array_t.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct keyvalue_t keyvalue_t;
struct keyvalue_t {
  size_t key;
  array_t *value;
  keyvalue_t *next;
};

static
keyvalue_t* create_kvpair(size_t key) {
  // printf("start kvp\n");
  keyvalue_t* kvp = (keyvalue_t*) malloc(sizeof(keyvalue_t));
  // memset(kvp,0,sizeof(keyvalue_t));
  kvp->key = key;
  kvp->next = NULL;
  kvp->value = create_array(key);
  // printf("create kvp %zu\n", key);
  return kvp;
}

static
void free_kvpair(keyvalue_t* kvp) {
  if (kvp == NULL) return;

  keyvalue_t *next = NULL, *p = kvp;
  
  while (p != NULL) {
    next = p->next;
    free_array(p->value);
    free(p);
    p = next;
  }
}

#ifdef __cplusplus
};
#endif