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

#include "matrix.h"
#include <math.h>

inline 
bool cmp(const float a, const float b, const float epsilon=1E-6) {
  return (fabs(a - b) < epsilon) ? true : false;
}

inline 
bool cmp(const double a, const double b, const double epsilon=1E-6) {
  return (abs(a - b) < epsilon) ? true : false;
}

static
bool is_zero(const matrix_t& m) {
  for (size_t i=0; i < m.e->size; ++i) {
    if (m.p[i] != 0.0) return false;
  }
  return true;
}