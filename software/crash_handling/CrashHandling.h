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

#ifndef CRASHHANDLING_H_
#define CRASHHANDLING_H_

#include <string>

#define TLP_PLATEFORM_HEADER "TLP_PLATEFORM"
#define TLP_ARCH_HEADER "TLP_ARCH"
#define TLP_COMPILER_HEADER "TLP_COMPILER"
#define TLP_VERSION_HEADER "TLP_VERSION"
#define TLP_DUMP_HEADER "TLP_DUMP"
#define TLP_STACK_BEGIN_HEADER "TLP_STACK_BEGIN"
#define TLP_STACK_END_HEADER "TLP_STACK_END"

extern void setDumpPath(std::string);

extern void start_crash_handler();

#endif /* CRASHHANDLING_H_ */
