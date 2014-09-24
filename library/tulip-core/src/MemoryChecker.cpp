/**
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

#if !defined(NDEBUG) && !defined(__EMSCRIPTEN__)

#include "tulip/tulipconf.h"
#include "tulip/MemoryChecker.h"
#include <cstring>
#include <map>

static int miss_count = 0;
static int hit_count = 0;

class MemoryChecker {
  struct StackInfo {
    char** _strings;
    size_t _size;
  };

  std::map<void*,StackInfo> _stacks;

public:
  MemoryChecker() {}

  inline void insert(void* ptr,char** strings,size_t size) {
    StackInfo infos;
    infos._strings = strings;
    infos._size = size;
    _stacks[ptr] = infos;
  }

  inline void remove(void* ptr) {
    std::map<void*, StackInfo>::iterator it = _stacks.find(ptr);

    if (it != _stacks.end()) {
      delete it->second._strings;
      _stacks.erase(it);
    }
  }

  inline void print() {
    for(std::map<void*, StackInfo>::iterator it = _stacks.begin();
        it != _stacks.end(); ++it) {
      StackInfo& infos = it->second;

      tlp::warning() << " ======================== " << std::endl;
      tlp::warning() << "Possible memory leak at " << it->first << ": " << std::endl;

      for (size_t i=0; i<infos._size; ++i)
        tlp::warning() << infos._strings[i] << std::endl;

      tlp::warning() << " ======================== "  << std::endl  << std::endl;
    }
  }

  inline void clear() {
    int entries = _stacks.size();

    for(std::map<void*, StackInfo>::iterator it = _stacks.begin();
        it != _stacks.end(); ++it) {
      delete it->second._strings;
    }

    _stacks.clear();
    tlp::warning() << "Removed " << entries << " into the memory checker" << std::endl;
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

#endif
