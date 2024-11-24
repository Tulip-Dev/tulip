/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
 * The tlp_hash_map macro defines which implementation is used for hash maps.
 *
 */
// we tried to find a high-performance alternative to std::unordered_map
// with the same api and an easy integration into Tulip.
// see
// https://martin.ankerl.com/2022/08/27/hashmap-bench-01/
// https://jacksonallan.github.io/c_cpp_hash_tables_benchmark/
#include <tulip/unordered_dense.h>
#define tlp_hash_map ankerl::unordered_dense::map

// needed because the definition of tlp_hash_combine template
// has been moved into TlpTools.h
#include <tulip/TlpTools.h>

#endif

///@endcond
