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

#ifndef _MSC_VER
#  include <ext/slist>
#  define TLP_SLIST stdext::slist
#endif

#if(_MSC_VER)
#  include <unordered_map>
#  include <unordered_set>
#  define TLP_HASH_MAP std::unordered_map
#  define TLP_HASH_SET std::unordered_set
#  define TLP_BEGIN_HASH_NAMESPACE namespace std 
#  define TLP_END_HASH_NAMESPACE
#  include <forward_list>
#elif  (__GNUC__ < 4 || __GNUC_MINOR__ < 1 )
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
  template<> struct hash_value<const std::string>{
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
#else
#  include <tr1/unordered_map>
#  include <tr1/unordered_set>
#  define TLP_HASH_MAP std::tr1::unordered_map
#  define TLP_HASH_SET std::tr1::unordered_set
#  define TLP_BEGIN_HASH_NAMESPACE namespace std { namespace tr1
#  define TLP_END_HASH_NAMESPACE }
#endif

#endif
