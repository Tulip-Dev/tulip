/**
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

#include <tulip/tulipconf.h>

#include <iostream>

using namespace std;

static std::ostream* debugStream = NULL;
std::ostream& tlp::debug() {
  return debugStream ? *debugStream : std::cout;
}
void tlp::setDebugOutput(std::ostream& os) {
  debugStream = &os;
}

static std::ostream* warningStream = NULL;
std::ostream& tlp::warning() {
  return warningStream ? *warningStream : std::cerr;
}
void tlp::setWarningOutput(std::ostream& os) {
  warningStream = &os;
}

static std::ostream* errorStream = NULL;
std::ostream& tlp::error() {
  return errorStream ? *errorStream : std::cerr;
}
void tlp::setErrorOutput(std::ostream& os) {
  errorStream = &os;
}
