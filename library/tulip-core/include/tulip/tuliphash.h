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

#ifndef TLP_HASH_H
#define TLP_HASH_H

/**
 * @brief This file defines what class is used to provide a hashmap.
 * The TLP_HASH_MAP macro defines which implementation is used for hash maps.
 * The TLP_HASH_SET macro defines which implementation is used for hash sets.
 *
 * TLP_BEGIN_HASH_NAMESPACE is defined to open the namespace in which the hash classes are defined,
 * to define new hashes (e.g. for Edge).
 * TLP_END_HASH_NAMESPACE is defined to close the namespace
 * TLP_HASH_NAMESPACE allows to use a specific hasher class when declaring a hash set or a hash map.
 */

#include <unordered_map>
#include <unordered_set>
#define TLP_HASH_MAP std::unordered_map
#define TLP_HASH_SET std::unordered_set
#define TLP_HASH_NAMESPACE std
#define TLP_BEGIN_HASH_NAMESPACE namespace std
#define TLP_END_HASH_NAMESPACE

// The hash_combine function from the boost library
// Call it repeatedly to incrementally create a hash value from several variables.

// Explanation of the formula from StackOverflow
// (http://stackoverflow.com/questions/4948780/magic-numbers-in-boosthash-combine) :
// The magic number 0x9e3779b9 = 2^32 / ((1 + sqrt(5)) / 2) is the reciprocal of the golden ratio.
// It is supposed to be 32 random bits, where each is equally likely to be 0 or 1, and with no
// simple correlation between the bits.
// So including this number "randomly" changes each bit of the seed; as you say, this means that
// consecutive values will be far apart.
// Including the shifted versions of the old seed makes sure that, even if hash_value() has a fairly
// small range of values,
// differences will soon be spread across all the bits.
TLP_BEGIN_HASH_NAMESPACE {
  template <class T>
  inline void hash_combine(std::size_t & seed, const T &v) {
    hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }
}
TLP_END_HASH_NAMESPACE

#endif
///@endcond
