# MathLib

Simple matrix library for embedded systems.

> This is yet another re-write of something I wrote long ago in college ... YAML (Yet Another Math Library)

> **WARNING:** this is **_NOT_** designed to be thread safe, so
> run this in separate processes!

## Architecture

- `TYPE`: set to `float` or `double` in the `CMakeLists.txt` file
- `array_t`: the matrix is a single array of size = rows * columns
- `keyvalue_t`: when a new matrix is needed or a temporary return value needed, this will return an `array_t` of the correct size. The goal is to minimize `malloc()`/`free()` calls
- `matrix_t`: this is the main `struct` that can handle all common matrix operations. 
- `get_matrix_err()`: like `errnum()` in C, global function that returns an error number
- `get_matrix_err_string()`: returns a `std::string` for printing the error

> There are no vectors in this library, everything is a `matrix_t` with correct rows/columns

## Usage

You can change the `TYPE` to `float` or `double` in the top level `CMakeLists.txt`

| Define             | Default | Options                                   |
|--------------------|---------|-------------------------------------------|
| `TYPE`             | `float` | `float` or `double`
| `KMATH_DEBUG`      | 0       | 0 or 1 for off or on, prints debug info
| `DEFAULT_MAP_SIZE` | 10      | how many array sizes do you need

```cmake
# kmath library -------------------------------------------
add_library(${PROJECT_NAME} INTERFACE src)
target_compile_definitions(${PROJECT_NAME} INTERFACE TYPE=float)
target_compile_definitions(${PROJECT_NAME} INTERFACE KMATH_DEBUG=0)
target_compile_definitions(${PROJECT_NAME} INTERFACE DEFAULT_MAP_SIZE=10)
```

or by putting this at the top of the C/C++ file:

```c
#define KMATH_DEBUG 1
```

## ToDo

- [ ] Maybe add a namespace like `kmath`
- [ ] Change `enum class Errors` to `enum Errors` because C code can't use it
  - Append `KMATH_` to all errors
  - Keep the `get_matrix_err_string()` in C++ so I can use `std::string`

# MIT License

**Copyright (c) 2000 The Guild of Calamitous Intent**

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.