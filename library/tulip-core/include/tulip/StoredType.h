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

#ifndef _TLPRETURNTYPE_
#define _TLPRETURNTYPE_

#ifndef DOXYGEN_NOTFOR_DEVEL
namespace tlp {

// the template below defines how are returned and stored
// the types of value a MutableContainer can managed
// basically a type is returned and stored by value
template <typename TYPE>
struct StoredType {
  // the type of the stored value
  typedef TYPE Value;
  // the type of the returned value
  typedef TYPE ReturnedValue;
  // the type of a returned const value
  typedef TYPE ReturnedConstValue;
  // indicates if a pointer to the value is stored
  enum {isPointer=0};
  // simply get
  inline static TYPE& get(const TYPE& val) {
    return (TYPE&) val;
  }
  // equallity test
  inline static bool equal(const TYPE& val1, const TYPE& val2) {
    return val2 == val1;
  }
  // cloning before storage
  inline static Value clone(const TYPE& val) {
    return val;
  }
  // destruction of stored value
  inline static void destroy(Value) {}
  // the default value of that type
  inline static Value defaultValue() {
    return (Value) 0;
  }
};

// non basic types are returned by reference
// and stored by pointer
// This last point enables a better management of the default value
// which can simply be flagged in storing a null pointer
// the macro below must be used to enable thies type of management

#ifdef TLP_NO_CONST_STORED_TYPE
#define DECL_STORED_STRUCT(T)         \
  template <>             \
    struct StoredType<T > {         \
    typedef T *Value;           \
    typedef T& ReturnedValue;         \
    typedef T ReturnedConstValue;      \
                \
    enum {isPointer=1};           \
                \
    inline static T& get(const Value& val) {       \
      return *val;            \
    }               \
                \
    inline static bool equal(Value val1, const T& val2) { \
      return val2 == *val1;         \
    }               \
                \
    inline static bool equal(const T& val2, Value val1) { \
      return val2 == *val1;         \
    }               \
                \
    inline static Value clone(const T& val) {     \
      return new T(val);          \
    }               \
                \
    inline static void destroy(Value val) {     \
      delete val;           \
    }               \
    inline static Value defaultValue() {      \
      return new T();           \
    }               \
  };
}
#else
#define DECL_STORED_STRUCT(T)         \
  template <>             \
    struct StoredType<T > {         \
    typedef T *Value;           \
    typedef T& ReturnedValue;         \
    typedef const T& ReturnedConstValue;      \
                \
    enum {isPointer=1};           \
                \
    inline static T& get(const Value& val) {       \
      return *val;            \
    }               \
                \
    inline static bool equal(Value val1, const T& val2) { \
      return val2 == *val1;         \
    }               \
                \
    inline static bool equal(const T& val2, Value val1) { \
      return val2 == *val1;         \
    }               \
                \
    inline static Value clone(const T& val) {     \
      return new T(val);          \
    }               \
                \
    inline static void destroy(Value val) {     \
      delete val;           \
    }               \
    inline static Value defaultValue() {      \
      return new T();           \
    }               \
  };
}
#endif
#endif
#endif
///@endcond
