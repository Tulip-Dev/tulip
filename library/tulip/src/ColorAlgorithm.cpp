/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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

#include "tulip/Types.h"
#include "tulip/ColorAlgorithm.h"
#include "tulip/Graph.h"
#include "tulip/ColorProperty.h"

tlp::ColorAlgorithm::ColorAlgorithm (const tlp::PropertyContext &context):tlp::PropertyAlgorithm(context) {
  colorResult = (tlp::ColorProperty *)context.propertyProxy;
}

tlp::ColorAlgorithm::~ColorAlgorithm() {
}
