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
class AbstractPluginInfo {

%TypeHeaderCode
#include <tulip/AbstractPluginInfo.h>
%End
public:
	virtual ~AbstractPluginInfo();

	virtual  std::string getName() const = 0;

	virtual std::string getGroup() const = 0;

	virtual  std::string getAuthor() const = 0;

	virtual  std::string getDate() const = 0;

	virtual  std::string getInfo() const = 0;

	virtual  std::string getRelease() const = 0;

	virtual  std::string getMajor() const = 0;

	virtual  std::string getMinor() const = 0;

	virtual  std::string getTulipRelease() const = 0;

	virtual  std::string getTulipMajor() const = 0;

	virtual  std::string getTulipMinor() const = 0;
};
};
