/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include "tulip/MemoryChecker.h"

#include <QtCore/QMap>

static int miss_count = 0;
static int hit_count = 0;

class MemoryChecker {
  struct StackInfo {
    char** _strings;
    size_t _size;
  };

  QMap<void*,StackInfo> _stacks;

public:
  MemoryChecker() {}

  inline void insert(void* ptr,char** strings,size_t size) {
    StackInfo infos;
    infos._strings = strings;
    infos._size = size;
    _stacks[ptr] = infos;
  }

  inline void remove(void* ptr) {
    if (!_stacks.contains(ptr))
      return;

    StackInfo infos = _stacks[ptr];
    delete infos._strings;
    _stacks.remove(ptr);
  }

  inline void print() {
    foreach(void* ptr,_stacks.keys()) {
      StackInfo infos = _stacks[ptr];

      qWarning() << " ======================== ";
      qWarning() << "Possible memory leak at " << ptr << ": ";

      for (size_t i=0; i<infos._size; ++i)
        qWarning() << infos._strings[i];

      qWarning() << " ======================== ";
    }
  }

  inline void clear() {
    int entries = _stacks.size();
    foreach(void* ptr,_stacks.keys()) {
      StackInfo infos = _stacks[ptr];
      delete infos._strings;
    }
    _stacks.clear();
    qWarning() << "Removed " << entries << " into the memory checker";
  }
};

static MemoryChecker* memory_checker = NULL;
static bool block_inserts = false;

void memchecker_insert_stack(void* ptr,char** strings, size_t size) {
  if (block_inserts)
    return;

  block_inserts=true;

  if (!memory_checker)
    memory_checker = new MemoryChecker;

  memory_checker->insert(ptr,strings,size);
  block_inserts=false;
}

void memchecker_remove_stack(void* ptr) {
  block_inserts = true;
  memory_checker->remove(ptr);
  block_inserts = false;
}

void memory_checker_print_report() {
  block_inserts = true;
  memory_checker->print();
  block_inserts = false;
}

void memory_checker_clear_report() {
  block_inserts = true;
  memory_checker->clear();
  block_inserts = false;
}
