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
#ifndef TLP_HASH_H
#define TLP_HASH_H

/**
 * @brief This file defines what class is used to provide a hashmap.
 * The TLP_HASH_MAP macro defines which implementation is used for hash maps.
 * The TLP_HASH_SET macro defines which implementation is used for hash sets.
 *
 * TLP_BEGIN_HASH_NAMESPACE is defined to open the namespace in which the hash classes are defined, to define new hashes (e.g. for Edge).
 * TLP_END_HASH_NAMESPACE is definde to close the namespace (only used when using std::tr1)
 */

//VS2010 and later can use C++0x's unordered_map; vs2008 uses boost's tr1 implementation
#if(_MSC_VER > 1500) 
#  include <unordered_map>
#  include <unordered_set>
#  define TLP_HASH_MAP std::unordered_map
#  define TLP_HASH_SET std::unordered_set
#  define TLP_BEGIN_HASH_NAMESPACE namespace std 
#  define TLP_END_HASH_NAMESPACE
//clang, and GCC versions prior to the 4.x series do not have tr1; using ext
#elif  (!_MSC_VER && (__GNUC__ < 4 || __GNUC_MINOR__ < 1 || __clang__))
#  include <tulip/tulipconf.h>
#  if (__GNUC__ < 3 || __clang__)
#    include <hash_map>
#    include <hash_set>
#  else
#    include <ext/hash_map>
#    include <ext/hash_set>
#  endif
#  define TLP_HASH_MAP stdext::hash_map
#  define TLP_HASH_SET stdext::hash_set
#  define TLP_BEGIN_HASH_NAMESPACE namespace stdext
#  define TLP_END_HASH_NAMESPACE

#  include <string>

namespace stdext {
  template<> struct hash<const std::string>{
    size_t operator()(const std::string &s) const {return hash<const char *>()(s.c_str()); }
  };
  template<> struct hash<std::string>{
    size_t operator()(const std::string &s) const {return hash<const char *>()(s.c_str()); }
  };
  template<>
    struct hash<double> {
      size_t operator()(const double s) const { return (size_t) s; }
    };
}
//MSVC < 2010 use tr1 from boost, and GCC 4.X provides tr1 too.
#else
#  include <tr1/unordered_map>
#  include <tr1/unordered_set>
#  define TLP_HASH_MAP std::tr1::unordered_map
#  define TLP_HASH_SET std::tr1::unordered_set
#  define TLP_BEGIN_HASH_NAMESPACE namespace std { namespace tr1
#  define TLP_END_HASH_NAMESPACE }
#endif

#endif
