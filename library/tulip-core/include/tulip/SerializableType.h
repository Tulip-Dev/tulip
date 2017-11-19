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
///@cond DOXYGEN_HIDDEN

#ifndef SERIALIZABLETYPE_H
#define SERIALIZABLETYPE_H

#include <set>
#include <vector>
#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include <cfloat>

#include <tulip/TypeInterface.h>

#define FORWARD_TOSTRING(T)                                                                        \
  static std::string toString(const T::RealType &v) {                                              \
    std::ostringstream oss;                                                                        \
    write(oss, v);                                                                                 \
    return oss.str();                                                                              \
  }
#define FORWARD_FROMSTRING(T)                                                                      \
  static bool fromString(T::RealType &v, const std::string &s) {                                   \
    std::istringstream iss(s);                                                                     \
    return read(iss, v);                                                                           \
  }
#define FORWARD_STRING_METHODS(T) FORWARD_FROMSTRING(T) FORWARD_TOSTRING(T)

namespace tlp {
template <typename T>
class TLP_SCOPE SerializableType : public TypeInterface<T> {
public:
  static void write(std::ostream &oss, const typename TypeInterface<T>::RealType &v) {
    oss << v;
  }
  static bool read(std::istream &iss, typename TypeInterface<T>::RealType &v) {
    return bool(iss >> v);
  }
  FORWARD_STRING_METHODS(typename TypeInterface<T>)
};

template <typename ELT_TYPE, typename ELT_READER, int openParen>
class TLP_SCOPE SerializableVectorType : public TypeInterface<std::vector<ELT_TYPE>> {
  static bool readVector(std::istream &is, std::vector<ELT_TYPE> &v, char openChar, char sepChar,
                         char closeChar) {
    v.clear();

    char c = ' ';
    ELT_TYPE val;
    bool firstVal = true;
    bool sepFound = false;

    // go to first non space char
    while ((is >> c) && isspace(c)) {
    }

    if (openChar) {
      if (c != openChar)
        return false;
    } else
      is.unget();

    for (;;) {
      if (!(is >> c))
        return (!sepFound && !closeChar);

      if (isspace(c))
        continue;

      if (c == closeChar) {
        if (!openChar || sepFound)
          return false;

        return true;
      }

      if (c == sepChar) {
        if (firstVal || sepFound)
          return false;

        sepFound = true;
      } else {
        if (firstVal || sepFound) {
          if (openParen && c != '(')
            return false;

          is.unget();

          if (!ELT_READER::read(is, val))
            return false;

          v.push_back(val);
          firstVal = false;
          sepFound = false;
        } else
          return false;
      }
    }
  }
  static void writeVector(std::ostream &os, const std::vector<ELT_TYPE> &v) {
    os << '(';

    for (unsigned int i = 0; i < v.size(); i++) {
      if (i)
        os << ", ";

      os << v[i];
    }

    os << ')';
  }

public:
  static void write(std::ostream &oss,
                    const typename TypeInterface<std::vector<ELT_TYPE>>::RealType &v) {
    writeVector(oss, v);
  }
  static void writeb(std::ostream &oss,
                     const typename TypeInterface<std::vector<ELT_TYPE>>::RealType &v) {
    unsigned int vSize = v.size();
    oss.write(reinterpret_cast<const char *>(&vSize), sizeof(vSize));
    oss.write(reinterpret_cast<const char *>(v.data()), vSize * sizeof(ELT_TYPE));
  }
  static bool read(std::istream &iss, typename TypeInterface<std::vector<ELT_TYPE>>::RealType &v,
                   char openChar = '(', char sepChar = ',', char closeChar = ')') {
    return readVector(iss, v, openChar, sepChar, closeChar);
  }
  static bool readb(std::istream &iss, typename TypeInterface<std::vector<ELT_TYPE>>::RealType &v) {
    unsigned int vSize;

    if (bool(iss.read(reinterpret_cast<char *>(&vSize), sizeof(vSize)))) {
      v.resize(vSize);
      return bool(iss.read(reinterpret_cast<char *>(v.data()), vSize * sizeof(ELT_TYPE)));
    }

    return false;
  }
  static unsigned int valueSize() {
    return 0; // means is not fixed
  }
  FORWARD_STRING_METHODS(typename TypeInterface<std::vector<ELT_TYPE>>)
};
}

#endif // SERIALIZABLETYPE_H
///@endcond
