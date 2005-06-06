#ifndef _TLPRETURNTYPE_
#define _TLPRETURNTYPE_

#include "Node.h"
#include "Edge.h"


template <typename TYPE> 
struct ReturnType {
  typedef const TYPE &Value;
};

template <> 
struct ReturnType<bool> {
  typedef bool Value;
};

template <> 
struct ReturnType<node> {
  typedef node Value;
};

template <> 
struct ReturnType<edge> {
  typedef edge Value;
};
#endif
