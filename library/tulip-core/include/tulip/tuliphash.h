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

// needed because the definition of tlp_hash_combine template
// has been moved into TlpTools.h
#include <tulip/TlpTools.h>

#endif

///@endcond
