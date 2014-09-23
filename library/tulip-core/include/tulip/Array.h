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
#ifndef _TLP_GEO_ARRAY_H
#define _TLP_GEO_ARRAY_H

#include <cassert>
#include <iostream>
#include <tulip/tulipconf.h>

namespace tlp {

/**
 * @brief Fixed-size array encapsulation.
 * @ingroup Structures
 * In debug mode, a bound check is performed at each access.
 * Stream operators implementations are provided.
 *
 * @author : David Auber auber@tulip-software.org
 */
template <typename Obj,unsigned int SIZE>
struct Array {
  /**
   * @brief array The underlying array of data.
   */
  Obj array[SIZE];

  /**
   * @brief operator [] Read-only accessor.
   * @param i The index of the element to read.
   * @return The element at index i.
   */
  inline Obj  operator[](const unsigned int i) const;

  /**
   * @brief operator [] Write accessor.
   * @param i The index at which to write a value.
   * @return A reference to the value at index i.
   */
  inline Obj& operator[](const unsigned int i);
};

template <typename Obj,unsigned int SIZE>
/**
 * @brief operator << stream operator to easily print an array, or save it to a file.
 * @param os The stream to output to.
 * @param array The array to output.
 * @return The stream to output to, to chain calls.
 */
std::ostream& operator<<(std::ostream &os,const Array<Obj,SIZE> &array);

template <typename Obj,unsigned int SIZE>
/**
 * @brief operator >> stream operator to easily read an array
 * @param is The stream to read from.
 * @param array A reference to an array, that will be written to.
 * @return The stream to read from, to chain calls.
 */
std::istream& operator>>(std::istream &is, Array<Obj,SIZE> &array);

//template <typename Obj,unsigned int SIZE>
//QDebug operator<<(QDebug dbg,const Array<Obj,SIZE>& s);
}

#include "cxx/Array.cxx"

#endif
