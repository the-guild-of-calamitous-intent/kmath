#define KMATH_DEBUG 0

#include <gtest/gtest.h> // googletest header file
// #include <stdlib.h>
#include <kmath.h>

void check_array_t(array_t* a, size_t size) {
    EXPECT_EQ(a->size, size);
    EXPECT_EQ(a->bsize, size*sizeof(TYPE));
    EXPECT_EQ(a->available, false);
    for (int i=0; i<size; ++i) EXPECT_FLOAT_EQ(a->data[i], 0.0f);
}

TEST(kmath, map_t) {
    map_t *map = create_map(10);

    array_t* a = map_get(map, 8);
    check_array_t(a, 8);
    array_t* b = map_get(map, 10);
    check_array_t(b, 10);
    array_t* c = map_get(map, 3);
    check_array_t(c, 3);

    EXPECT_EQ(map->size, 3);
    EXPECT_EQ(map->capacity, 10); // max size

    free_map(map);

    // EXPECT_EQ(map, NULL); // freed, so it should be NULL
    // EXPECT_TRUE(map == NULL); // freed, so it should be NULL
}

// void check_matrix(matrix& a, size_t r, )

TEST(kmath, matrix_t) {
  matrix_t a(3,4);
  EXPECT_EQ(a.r, 3);
  EXPECT_EQ(a.c, 4);
  EXPECT_EQ(a.e->size, 3*4);
  EXPECT_EQ(a.e->bsize, 3*4*sizeof(TYPE));
  EXPECT_EQ(a.p, a.e->data);

  matrix_t e = eye(2,2);
  EXPECT_EQ(e.r, 2);
  EXPECT_EQ(e.c, 2);
  EXPECT_FLOAT_EQ(e.p[0], 1.0f);
  EXPECT_FLOAT_EQ(e.p[1], 0.0f);
  EXPECT_FLOAT_EQ(e.p[2], 0.0f);
  EXPECT_FLOAT_EQ(e.p[3], 1.0f);

  e(0,1) = 2.345f;
  e(1,0) = 2.345f;
  EXPECT_FLOAT_EQ(e.p[1], 2.345f);
  EXPECT_FLOAT_EQ(e.p[2], 2.345f);
  EXPECT_FLOAT_EQ(e[1], 2.345f);
  EXPECT_FLOAT_EQ(e[2], 2.345f);
  EXPECT_FLOAT_EQ(e(0,1), 2.345f);
  EXPECT_FLOAT_EQ(e(1,0), 2.345f);

  matrix_t n = eye(4,4);
  matrix_t m = n+2.0*n/n*2.0 - n; // 4*I
  EXPECT_EQ(get_matrix_err(), Errors::NONE);
  // print(m);
  EXPECT_FLOAT_EQ(m.p[0],  4.0f);
  EXPECT_FLOAT_EQ(m.p[5],  4.0f);
  EXPECT_FLOAT_EQ(m.p[10], 4.0f);
  EXPECT_FLOAT_EQ(m.p[15], 4.0f);

  TYPE p[] = {2.0,3.0};
  matrix_t f = diag(2,2,p);
  EXPECT_EQ(f.r, 2);
  EXPECT_EQ(f.c, 2);
  EXPECT_FLOAT_EQ(f.p[0], 2.0f);
  EXPECT_FLOAT_EQ(f.p[1], 0.0f);
  EXPECT_FLOAT_EQ(f.p[2], 0.0f);
  EXPECT_FLOAT_EQ(f.p[3], 3.0f);

  EXPECT_FLOAT_EQ(f[0], 2.0f);
  EXPECT_FLOAT_EQ(f[1], 0.0f);
  EXPECT_FLOAT_EQ(f[2], 0.0f);
  EXPECT_FLOAT_EQ(f[3], 3.0f);

  EXPECT_FLOAT_EQ(f(0,0), 2.0f);
  EXPECT_FLOAT_EQ(f(0,1), 0.0f);
  EXPECT_FLOAT_EQ(f(1,0), 0.0f);
  EXPECT_FLOAT_EQ(f(1,1), 3.0f);


  // exceed range with operators //////////////////////////
  TYPE val = f[32];
  EXPECT_EQ(get_matrix_err(), Errors::OPERATOR_EXCEEDS_SIZE);
  EXPECT_FLOAT_EQ(val, 3.0f);

  val = f(55,55);
  EXPECT_EQ(get_matrix_err(), Errors::OPERATOR_EXCEEDS_ROW_COL);
  EXPECT_FLOAT_EQ(val, 3.0f);

  set_matrix_err(Errors::NONE);
  val = f(0,0);
  EXPECT_EQ(get_matrix_err(), Errors::NONE);
  EXPECT_FLOAT_EQ(val, 2.0f);

  // matrix_t b(30,40);
  // printf("\nb size: %d\n", (int)sizeof(b));
  // printf("\nb.e size: %d\n", (int)sizeof(*b.e));
  // printf("\nb.e->data size: %d\n", (int)sizeof(*b.e->data));

  matrix_t aa(3,3,true);
  matrix_t bb = aa;
  // EXPECT_EQ(aa.p, bb.p);
}


TEST(kmath, linalg) {
  float p[] = {10,0,0,10};
  matrix_t f(2,2,p);
  matrix_t g = inv(f);
  EXPECT_FLOAT_EQ(g.p[0], 0.1f);
  EXPECT_FLOAT_EQ(g.p[1], 0.0f);
  EXPECT_FLOAT_EQ(g.p[2], 0.0f);
  EXPECT_FLOAT_EQ(g.p[3], 0.1f);

  matrix_t a(3,4);
  matrix_t b = inv(a);
  EXPECT_EQ(get_matrix_err(), Errors::INV_NONSQUARE);
}