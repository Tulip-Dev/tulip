/**
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

#include <tulip/tulipconf.h>
#include <tulip/TulipRelease.h>

#include <iostream>
#include <fstream>

using namespace std;

static std::ostream *debugStream = nullptr;
static bool warningAllowed = true;

std::ostream &tlp::debug() {
  return debugStream ? *debugStream : std::cout;
}
void tlp::setDebugOutput(std::ostream &os) {
  debugStream = &os;
}

static std::ostream *warningStream = nullptr;
std::ostream &tlp::warning() {
  if (warningAllowed)
    return warningStream ? *warningStream : std::cerr;
  static std::ofstream nullstr;
  if (!nullstr.is_open())
#if defined(WIN32)
    nullstr.open("NULL", std::ofstream::out | std::ofstream::app);
#else
    nullstr.open("/dev/null", std::ofstream::out | std::ofstream::app);
#endif
  return nullstr;
}
void tlp::setWarningOutput(std::ostream &os) {
  warningStream = &os;
  warningAllowed = true;
}
void tlp::enableWarningOutput(bool flag) {
  warningAllowed = flag;
}
bool tlp::isWarningOutputEnabled() {
  return warningAllowed;
}

static std::ostream *errorStream = nullptr;
std::ostream &tlp::error() {
  return errorStream ? *errorStream : std::cerr;
}
void tlp::setErrorOutput(std::ostream &os) {
  errorStream = &os;
}

std::string tlp::getTulipVersion() {
  return TULIP_VERSION;
}
