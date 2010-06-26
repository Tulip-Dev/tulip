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
#ifndef _TLPRETURNTYPE_
#define _TLPRETURNTYPE_

#include "tulip/Node.h"
#include "tulip/Edge.h"
#include "tulip/Color.h"

#ifndef DOXYGEN_NOTFOR_DEVEL
namespace tlp {

  template <typename TYPE> 
    struct ReturnType {
      typedef const TYPE& ConstValue;
      typedef TYPE &Value;
    };

 
  // macro for basic types declaration
#define DECL_BASIC_RETURN(T)			\
  template <>					\
    struct ReturnType<T> {			\
    typedef const T ConstValue;			\
    typedef T Value;				\
  }

  DECL_BASIC_RETURN(char);
  DECL_BASIC_RETURN(bool);
  DECL_BASIC_RETURN(node);
  DECL_BASIC_RETURN(edge);
  DECL_BASIC_RETURN(Color);
  DECL_BASIC_RETURN(int);
  DECL_BASIC_RETURN(long);
  DECL_BASIC_RETURN(unsigned int);
  DECL_BASIC_RETURN(unsigned long);
  DECL_BASIC_RETURN(float);
  DECL_BASIC_RETURN(void*);

  template <typename TYPE> 
    struct StoredValueType {
      typedef TYPE Value;

      enum {isPointer=0};
      inline static TYPE& get(const TYPE& val) {
	return (TYPE&) val;
      }

      inline static bool equal(const TYPE& val1, const TYPE& val2) {
	return val2 == val1;
      }

      inline static TYPE copy(const TYPE& val) {
	return val;
      }

      inline static void destroy(TYPE) {}
    };

  // macro for structure types declaration
#define DECL_STORED_STRUCT(T)					\
  template <>							\
    struct StoredValueType<T> {					\
    typedef T *Value;						\
								\
    enum {isPointer=1};						\
								\
    inline static T& get(Value val) {				\
      return *val;						\
    }								\
								\
    inline static bool equal(Value val1, const T& val2) {	\
      return val2 == *val1;					\
    }								\
								\
    inline static Value copy(const T& val) {			\
      return new T(val);					\
    }								\
								\
    inline static void destroy(Value val) {			\
      delete val;						\
    }								\
  }
}
#endif
#endif
