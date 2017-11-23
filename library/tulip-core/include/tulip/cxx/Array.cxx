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
//=================================================================
template <typename T, size_t N>
std::ostream &tlp::operator<<(std::ostream &os, const tlp::Array<T, N> &a) {
  os << "(";

  for (unsigned int i = 0; i < N; ++i) {
    if (i > 0)
      os << ",";

    os << a[i];
  }

  os << ")";
  return os;
}

template <typename T, size_t N>
std::istream &tlp::operator>>(std::istream &is, tlp::Array<T, N> &outA) {
  char c;
  int pos = is.tellg();
  is.clear();

  // skip spaces
  while (bool(is >> c) && isspace(c)) {
  }

  if (c != '(') {
    is.seekg(pos);
    is.setstate(std::ios::failbit);
    return is;
  }

  for (unsigned int i = 0; i < N; ++i) {
    bool ok;

    if (i > 0) {
      // skip spaces
      while ((ok = bool(is >> c)) && isspace(c)) {
      }

      if (!ok || c != ',') {
        is.seekg(pos);
        is.setstate(std::ios::failbit);
        return is;
      }
    }

    // skip spaces
    while ((ok = bool(is >> c)) && isspace(c)) {
    }

    is.unget();
    bool done = bool(is >> outA[i]);

    if (!done) {
      is.seekg(pos);
      is.setstate(std::ios::failbit);
      return is;
    }
  }

  // skip spaces
  while (bool(is >> c) && isspace(c)) {
  }

  if (c != ')') {
    is.seekg(pos);
    is.setstate(std::ios::failbit);
    return is;
  }

  return is;
}
//=================================================================
