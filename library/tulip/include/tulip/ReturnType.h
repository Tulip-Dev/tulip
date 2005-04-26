#ifndef _TLPRETURNTYPE_
#define _TLPRETURNTYPE_

template <typename TYPE> 
struct ReturnType {
  typedef const TYPE &Value;
};

template <> 
struct ReturnType<bool> {
  typedef bool Value;
};

#endif
