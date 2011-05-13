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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (http://booggie.org/)                                                   |
// +-------------------------------------------------------------------------+

namespace tlp {

std::string TulipLibDir;
std::string TulipPluginsPath;
std::string TulipDocProfile;
std::string TulipUserHandBookIndex;
std::string TulipBitmapDir;

void initTulipLib(char* appDirPath=0);

void loadPlugins(tlp::PluginLoader *plug=0);
bool loadPlugin(const std::string &filename, tlp::PluginLoader *plug=0);
void loadPluginsFromDir(std::string dir, std::string type, tlp::PluginLoader *loader=0);

std::string getMajor(const std::string& v);
std::string getMinor(const std::string& v);
};