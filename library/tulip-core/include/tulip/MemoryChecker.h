/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
///@cond DOXYGEN_HIDDEN

#if !defined(NDEBUG) && !defined(__EMSCRIPTEN__)

#ifndef MEMORYCHECKER_ON
#define MEMORYCHECKER_ON

#include <execinfo.h>
#include <cstdlib>
#include <iostream>

void memchecker_insert_stack(void* ptr,char** strings, size_t size);
void memchecker_remove_stack(void* ptr);
void memory_checker_print_report();
void memory_checker_clear_report();

inline void* operator new(size_t size) throw(std::bad_alloc) {
  void *ptr = (void *)malloc(size);

  void* stack_array[10];
  size_t stack_size = backtrace(stack_array,10);
  char** stack_data = backtrace_symbols(stack_array,stack_size);
  memchecker_insert_stack(ptr,stack_data,stack_size);

  return ptr;
}

inline void* operator new[](size_t size) throw(std::bad_alloc) {
  void *ptr = (void *)malloc(size);

  void* stack_array[10];
  size_t stack_size = backtrace(stack_array,10);
  char** stack_data = backtrace_symbols(stack_array,stack_size);
  memchecker_insert_stack(ptr,stack_data,stack_size);

  return ptr;
}

inline void operator delete(void *ptr) throw() {
  memchecker_remove_stack(ptr);
  free(ptr);
}

inline void operator delete[](void *ptr) throw() {
  memchecker_remove_stack(ptr);
  free(ptr);
}


#endif // MEMORYCHECKER_ON
#endif // NDEBUG
///@endcond
