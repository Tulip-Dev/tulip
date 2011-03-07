#include <ogdf/upward/UpwardPlanarizationLayout.h>
#include "tulip2ogdf/OGDFLayoutPluginBase.h"

// comments below have been extracted from OGDF/src/upward/UpwardPlanarizationLayout.h
/*@{*/
/** \file
 * \brief Declaration of upward planarization layout algorithm.
 *
 * \author Hoi-Ming Wong
 *
 * \par License:
 * This file is part of the Open Graph Drawing Framework (OGDF).
 * Copyright (C) 2005-2008
 *
 * \par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 or 3 as published by the Free Software Foundation
 * and appearing in the files LICENSE_GPL_v2.txt and
 * LICENSE_GPL_v3.txt included in the packaging of this file.
 *
 * \par
 * In addition, as a special exception, you have permission to link
 * this software with the libraries of the COIN-OR Osi project
 * (http://www.coin-or.org/projects/Osi.xml), all libraries required
 * by Osi, and all LP-solver libraries directly supported by the
 * COIN-OR Osi project, and distribute executables, as long as
 * you follow the requirements of the GNU General Public License
 * in regard to all of the software in the executable aside from these
 * third-party libraries.
 *
 * \par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * \par
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * \see  http://www.gnu.org/copyleft/gpl.html
 ***************************************************************/


namespace {
const char * paramHelp[] = {
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "bool" )
		HTML_HELP_BODY()
		"Sets the option for transposing layout vertically ."
		HTML_HELP_CLOSE()
};
}

class OGDFUpwardPlanarization : public OGDFLayoutPluginBase {

public:

	OGDFUpwardPlanarization(const tlp::PropertyContext &context) :OGDFLayoutPluginBase(context, new ogdf::UpwardPlanarizationLayout()) {
		addParameter<bool>("transpose", paramHelp[0], "false");
	}

	~OGDFUpwardPlanarization() {}

	void afterCall(TulipToOGDF*, ogdf::LayoutModule*) {
		if (dataSet != 0) {
			bool bval = false;
			if (dataSet->get("transpose", bval)) {
				if (bval) {
					transposeLayoutVertically();
				}
			}
		}
	}

};


LAYOUTPLUGINOFGROUP(OGDFUpwardPlanarization,"Upward Planarization (OGDF)","Hoi-Ming Wong","12/11/2007","Ok","1.0","Hierarchical");
