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
#include "keyvalue_t.h"
// #include "errors.h"

// map_t
//  +- size_t size: number of current linked lists
//  +- size_t capcity: max number of linked lists, size <= capacity
//  +- keyvalue_t** pairs
//      +- keyvalue_t* next
//      +- size_t key
//      +- array_t* value
//          +- TYPE* data
//          +- size_t size
//          +- bool available


// TODO:
// - report number arrays created and number available per key for testing
// - bash print color?
// - change map_t to linkedlinkedlist_t or lllist_t

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    keyvalue_t **pairs;
    size_t size;
    size_t capacity;
} map_t;

static
void get_key_info(keyvalue_t* kv, size_t* key, size_t* used, size_t* total) {
  *total = 0;
  *used = 0;
  *key = 0;
  if (kv == NULL) return;

  *key = kv->key;
  while (kv != NULL) {
    if (kv->value->available == false) *used += 1;
    *total += 1;
    kv = kv->next;
  }
}

static
void print(map_t* m) {
  size_t key, used, total;
  printf("\nmap_t -----------------\n");
  printf("  size: %zu of %zu keyvalue_t\n",m->size, m->capacity);
  for (size_t i=0; i<m->capacity;++i) {
    if (m->pairs[i] == NULL) {
      printf("  - %zu: NULL\n", i);
      continue;
    }

    get_key_info(m->pairs[i], &key, &used, &total);
    printf("  - %zu [key %zu]: %zu active of %zu total\n", i, key, used, total);

    // kv = m->pairs[i];
    // while (kv != NULL) {
    //   printf("   > %p\n", kv);
    //   kv = kv->next;
    // }
  }
  printf("-----------------------\n\n");
}

static
map_t* create_map(size_t INITIAL_CAPACITY) {
  map_t *map = (map_t *)malloc(sizeof(map_t));
  memset(map,0,sizeof(map_t));
  if (map == NULL) {
    // set_matrix_err(Errors::MAP_ALLOCATION_ERROR);
    return NULL;
  }

  map->pairs = (keyvalue_t **)malloc(INITIAL_CAPACITY * sizeof(keyvalue_t*));
  if (map->pairs == NULL) {
    // set_matrix_err(Errors::KEYVALUE_ALLOCATION_ERROR);
    return NULL;
  }

  memset(map->pairs,0,INITIAL_CAPACITY * sizeof(keyvalue_t*));

  map->size = 0;
  map->capacity = INITIAL_CAPACITY;

  return map;
}

static
void free_map(map_t *map) {
  size_t key, used, total;

  #if KMATH_DEBUG
  printf("pre free map_t ----------------------------\n");
  print(map);
  printf("---------------------------------------\n");
  #endif

  // printf("  size: %zu of %zu keyvalue_t\n",map->size, map->capacity);
  for (size_t i=0; i < map->capacity; ++i) {
    // if (map->pairs[i] == NULL) {
    //   // printf("  - %zu: NULL\n", i);
    //   continue;
    // }

    // get_key_info(map->pairs[i], &key, &used, &total);
    // printf("  - %zu: key: %zu active/total: %zu/%zu\n", i, key, used, total);
    // printf("  - key[%zu]: %zu ",i,map->pairs[i]->key);
    if (map->pairs[i] != NULL) free_kvpair(map->pairs[i]);
    map->pairs[i] = NULL;
  }
  map->size = 0;

  #if KMATH_DEBUG
  printf("post free map_t ----------------------------\n");
  print(map);
  printf("---------------------------------------\n");
  #endif

  free(map->pairs);
  free(map);
  map = NULL;
}

// Given a keyvalue_t find an available array_t or create
// a new array_t
static
array_t* map_get_ll(keyvalue_t *p, size_t key) {
  keyvalue_t *last = NULL;
  while (p != NULL) {
    if (p->value->available == true) {
      p->value->available = false;

      #if KMATH_DEBUG
      printf("> Found size %zu \n", key);
      #endif

      return p->value;
    }
    last = p;
    p = p->next;
  }

  last->next = create_kvpair(key);
  last->next->value->available = false;
  return last->next->value;
}

// Given a map_t, find a keyvalue_t with size key and
// then return an available array_t. Returns NULL if 
// capcity is exceeded.
static
array_t* map_get(map_t *map, size_t key) {
  for (size_t i=0; i<map->capacity; ++i) {
    // Create new linked list of size key
    if (map->pairs[i] == NULL) {
      map->pairs[i] = create_kvpair(key);
      map->size++;
      return map->pairs[i]->value;
    }
    // Found existing linked list of size key,
    // Get existing element or create new element of size key
    else if (map->pairs[i]->key == key) {
      array_t *p = map_get_ll(map->pairs[i], key);
      memset(p->data, 0, p->bsize);
      return p;
    }
  }

  // set_matrix_err(Errors::MAP_CAPCITY_EXCEEDED);

  return NULL;
}

///////////////////////////////////////////////////////////

#ifdef __cplusplus
};
#endif