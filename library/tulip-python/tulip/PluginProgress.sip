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
%TypeHeaderCode
#include <tulip/PluginProgress.h>
%End
class PluginProgress /Abstract/ {
public:

	PluginProgress();

	virtual int progress(int step, int max_step) = 0;

	virtual void cancel() = 0;

	virtual void stop() = 0;

	virtual bool isPreviewMode() const = 0;

	virtual void setPreviewMode(bool) = 0;

	virtual void showPreview(bool) = 0;

	virtual int state() const = 0;

	virtual std::string getError() = 0;

	virtual void setError(std::string error) = 0;

	virtual void setComment(std::string msg) = 0;

};
};
