#include <ogdf/upward/DominanceLayout.h>
#include "tulip2ogdf/OGDFLayoutPluginBase.h"

// comments below have been extracted from OGDF/src/upward/DominanceLayout.cpp
/*@{*/
/** \file
 * \brief Implementation of dominance layout algorithm.
 *
 * \author Hoi-Ming Wong and Carsten Gutwenger
 *
 * \par License:
 * This file is part of the Open Graph Drawing Framework (OGDF).
 *
 * Copyright (C). All rights reserved.
 * See README.txt in the root directory of the OGDF installation for details.
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
		HTML_HELP_DEF( "type", "int" )
		HTML_HELP_BODY()
		"the minimum grid distance."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "bool" )
		HTML_HELP_BODY()
		"Sets the option for transposing layout vertically ."
		HTML_HELP_CLOSE()
};
}

class OGDFDominance : public OGDFLayoutPluginBase {

public:

	OGDFDominance(const tlp::PropertyContext &context) :OGDFLayoutPluginBase(context, new ogdf::DominanceLayout()) {
		addParameter<int>("minimum grid distance", paramHelp[0], "1");
		addParameter<bool>("transpose", paramHelp[1], "false");
	}
	~OGDFDominance() {}

	void beforeCall(TulipToOGDF*, ogdf::LayoutModule *ogdfLayoutAlgo) {
		ogdf::DominanceLayout *dominance = static_cast<ogdf::DominanceLayout*>(ogdfLayoutAlgo);
		if (dataSet != 0) {
			int ival;
			if (dataSet->get("minimum grid distance", ival))
				dominance->setMinGridDistance(ival);
		}
	}

	void afterCall(TulipToOGDF*, ogdf::LayoutModule*) {
		if (dataSet != 0) {
			bool bval;
			if (dataSet->get("transpose", bval)) {
				if (bval) {
					transposeLayoutVertically();
				}
			}
		}
	}

};


LAYOUTPLUGINOFGROUP(OGDFDominance,"Dominance (OGDF)","Hoi-Ming Wong","12/11/2007","Ok","1.0","Hierarchical");
