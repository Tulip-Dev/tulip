#ifndef NDEBUG

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
