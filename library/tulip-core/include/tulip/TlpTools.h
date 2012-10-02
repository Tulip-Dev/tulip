/*
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

#ifndef _TLPTOOLS_H
#define _TLPTOOLS_H

#include <iostream>
#include <tulip/tulipconf.h>

namespace tlp {
extern TLP_SCOPE const char PATH_DELIMITER;
extern TLP_SCOPE std::string TulipLibDir;
extern TLP_SCOPE std::string TulipPluginsPath;
extern TLP_SCOPE std::string TulipDocProfile;
extern TLP_SCOPE std::string TulipUserHandBookIndex;
extern TLP_SCOPE std::string TulipBitmapDir;
extern TLP_SCOPE std::string TulipShareDir;

/**
 * @ingroup Plugins
 *
 * @brief Initializes the Tulip library.
 * Looks for the Tulip plug-ins directory, and loads the plug-ins from it.
 * The plug-ins directory can be defined in different ways, given by order of prevalence :
 * 1. the TLP_DIR environment variable, if it has a value
 * 2. the appDirPath parameter, if it is not NULL
 * 3. a fallback value of 'C:/Tulip/lib/' on windows, or '/usr/local/lib/' on unices.
 */
extern TLP_SCOPE void initTulipLib(const char* appDirPath = NULL);

/**
 * @ingroup Plugins
 *
 * @brief Demangles the name of a C++ class defined in the tlp namespace.
 * Simply removes the 'tlp::' prefix from the class name.
 * @return string The demangled name of a Tulip C++ class.
 */
TLP_SCOPE std::string demangleTlpClassName(const char *className);

}

#endif
