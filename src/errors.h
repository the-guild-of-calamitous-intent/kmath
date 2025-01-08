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

#include <string>

enum class Errors {
  NONE,     // 0: no error
  INV_SINGULAR, // 2: singular matrix
  INV_NONSQUARE,
  ADD_SIZE_MISMATCH,
  SUB_SIZE_MISMATCH,
  MULT_SIZE_MISMATCH,
  EQ_SIZE_MISMATCH,
  OPERATOR_EXCEEDS_SIZE,
  OPERATOR_EXCEEDS_ROW_COL,
  MAP_CAPCITY_EXCEEDED,
  MAP_ALLOCATION_ERROR,
  KEYVALUE_ALLOCATION_ERROR
};

static Errors matrix_err = Errors::NONE;

static
std::string get_matrix_err_string() {
  switch(matrix_err) {
    case Errors::NONE: return "None";
    // case Errors::SIZE: return "Size mismatch";
    case Errors::INV_SINGULAR: return "Matrix is singular";
    case Errors::INV_NONSQUARE: return "Matrix is nonsquare in Inv()";
    case Errors::EQ_SIZE_MISMATCH: return "Equate size mismatch";
    case Errors::ADD_SIZE_MISMATCH: return "Addition size mismatch";
    case Errors::SUB_SIZE_MISMATCH: return "Subtraction size mismatch";
    case Errors::MULT_SIZE_MISMATCH: return "Multiplication size mismatch";
    case Errors::OPERATOR_EXCEEDS_SIZE: return "Operator[] exceeds size";
    case Errors::OPERATOR_EXCEEDS_ROW_COL: return "Operator() exceeds row/col";
    case Errors::MAP_CAPCITY_EXCEEDED: return "Map capacity exceeded";
    case Errors::MAP_ALLOCATION_ERROR: return "Map create could not allocate memory";
    case Errors::KEYVALUE_ALLOCATION_ERROR: return "Keyvalue could not allocate memory";
    default: return "Invalid error";
  }
  return "";
}

static
void set_matrix_err(Errors err) {
  matrix_err = err;

  #if KMATH_DEBUG
  if (err == Errors::NONE) return;
  printf("-------------------\n");
  printf(" shit: %d %s\n", matrix_err, get_matrix_err_string().c_str());
  printf("-------------------\n");
  #endif
}

inline
Errors get_matrix_err() { return matrix_err; }