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

#ifndef TLP_HASH_H
#define TLP_HASH_H

// to search for _LIBCPP_VERSION
#include <ciso646>

/**
 * @brief This file defines what class is used to provide a hashmap.
 * The TLP_HASH_MAP macro defines which implementation is used for hash maps.
 * The TLP_HASH_SET macro defines which implementation is used for hash sets.
 *
 * TLP_BEGIN_HASH_NAMESPACE is defined to open the namespace in which the hash classes are defined, to define new hashes (e.g. for Edge).
 * TLP_END_HASH_NAMESPACE is definde to close the namespace (only used when using std::tr1)
 * TLP_HASH_NAMESPACE allows to use a specific hasher class when declaring a hash set or a hash map.
 */

//VS2010 and later can use C++0x's unordered_map; vs2008 uses boost's tr1 implementation
#if defined(_MSC_VER) && (_MSC_VER > 1500)
#  define TLP_USE_UNORDERED_MAP
#elif defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L || defined(_LIBCPP_VERSION)
#  define TLP_USE_UNORDERED_MAP
#endif

#ifdef TLP_USE_UNORDERED_MAP
#  include <unordered_map>
#  include <unordered_set>
#  define TLP_HASH_MAP std::unordered_map
#  define TLP_HASH_SET std::unordered_set
#  define TLP_HASH_NAMESPACE std
#  define TLP_BEGIN_HASH_NAMESPACE namespace std
#  define TLP_END_HASH_NAMESPACE
//clang, and GCC versions prior to the 4.x series do not have tr1; using ext
#elif  (! defined _MSC_VER && (__GNUC__ < 4 || __GNUC_MINOR__ < 1))
#  include <tulip/tulipconf.h>
#  if (__GNUC__ < 3)
#    include <hash_map>
#    include <hash_set>
#  else
#    include <ext/hash_map>
#    include <ext/hash_set>
#  endif
#  define TLP_HASH_MAP stdext::hash_map
#  define TLP_HASH_SET stdext::hash_set
#  define TLP_HASH_NAMESPACE stdext
#  define TLP_BEGIN_HASH_NAMESPACE namespace stdext
#  define TLP_END_HASH_NAMESPACE

#  include <string>

namespace stdext {
template<> struct hash<const std::string> {
  size_t operator()(const std::string &s) const {
    return hash<const char *>()(s.c_str());
  }
};
template<> struct hash<std::string> {
  size_t operator()(const std::string &s) const {
    return hash<const char *>()(s.c_str());
  }
};
template<>
struct hash<double> {
  size_t operator()(const double s) const {
    return (size_t) s;
  }
};
template<>
struct hash<float> {
  size_t operator()(const float s) const {
    return hash<unsigned int>()(*((unsigned int *) &s));
  }
};
}
//MSVC < 2010 use tr1 from boost, and GCC 4.X provides tr1 too.
#else
#  include <tr1/unordered_map>
#  include <tr1/unordered_set>
#  define TLP_HASH_MAP std::tr1::unordered_map
#  define TLP_HASH_SET std::tr1::unordered_set
#  define TLP_HASH_NAMESPACE std::tr1
#  define TLP_BEGIN_HASH_NAMESPACE namespace std { namespace tr1
#  define TLP_END_HASH_NAMESPACE }
#endif

// The hash_combine function from the boost library
// Call it repeatedly to incrementally create a hash value from several variables.

// Explanation of the formula from StackOverflow (http://stackoverflow.com/questions/4948780/magic-numbers-in-boosthash-combine) :
// The magic number 0x9e3779b9 = 2^32 / ((1 + sqrt(5)) / 2) is the reciprocal of the golden ratio.
// It is supposed to be 32 random bits, where each is equally likely to be 0 or 1, and with no simple correlation between the bits.
// So including this number "randomly" changes each bit of the seed; as you say, this means that consecutive values will be far apart.
// Including the shifted versions of the old seed makes sure that, even if hash_value() has a fairly small range of values,
// differences will soon be spread across all the bits.
TLP_BEGIN_HASH_NAMESPACE {
  template <class T>
  inline void hash_combine(std::size_t & seed, const T & v) {
    hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }
} TLP_END_HASH_NAMESPACE

#endif
///@endcond
