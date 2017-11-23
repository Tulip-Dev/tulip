/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#ifndef TLP_ARRAY_H
#define TLP_ARRAY_H

#include <array>
#include <iostream>

namespace tlp {

template <typename T, size_t N>
using Array = std::array<T, N>;

/**
 * @brief operator << stream operator to easily print an array, or save it to a file.
 * @param os The stream to output to.
 * @param array The array to output.
 * @return The stream to output to, to chain calls.
 */
template <typename T, size_t N>
std::ostream &operator<<(std::ostream &os, const Array<T, N> &array);

/**
 * @brief operator >> stream operator to easily read an array
 * @param is The stream to read from.
 * @param array A reference to an array, that will be written to.
 * @return The stream to read from, to chain calls.
 */
template <typename T, size_t N>
std::istream &operator>>(std::istream &is, Array<T, N> &array);
}

#include "cxx/Array.cxx"

#endif // TLP_ARRAY_H
