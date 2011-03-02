#include <ogdf/tree/TreeLayout.h>

#include "tulip2ogdf/OGDFLayoutPluginBase.h"

// comments below have been extracted from OGDF/src/tree/TreeLayout.cpp
/*@{*/
/** \file
 * \brief Linear time layout algorithm for trees (TreeLayout)
 * based on Walker's algorithm
 *
 * \author Christoph Buchheim
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
		HTML_HELP_DEF( "type", "double" )
		HTML_HELP_BODY()
		"The minimal required horizontal distance between siblings."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "double" )
		HTML_HELP_BODY()
		"The minimal required horizontal distance between subtrees."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "double" )
		HTML_HELP_BODY()
		"the minimal required vertical distance between levels."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "double" )
		HTML_HELP_BODY()
		"Returns the minimal required horizontal distance between trees in the forest."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "bool" )
		HTML_HELP_BODY()
		"Set whether orthogonal edge routing style is used or not."
		HTML_HELP_CLOSE(),
		HTML_HELP_DEF( "type", "StringCollection")
		HTML_HELP_DEF("values", "<FONT COLOR=\"red\"> topToBottom : <FONT COLOR=\"black\"> Edges are oriented from top to bottom. <BR> <FONT COLOR=\"red\"> bottomToTop : <FONT COLOR=\"black\">  Edges are oriented from bottom to top. <BR> <FONT COLOR=\"red\"> leftToRight : <FONT COLOR=\"black\"> Edges are oriented from left to right. <BR> <FONT COLOR=\"red\"> rightToLeft : <FONT COLOR=\"black\"> Edges are oriented from right to left.")
		HTML_HELP_DEF( "default", "topToBottom " )
		HTML_HELP_BODY()
		"The option that determines the orientation of the layout."
		HTML_HELP_CLOSE(),
		HTML_HELP_DEF( "type", "StringCollection")
		HTML_HELP_DEF("values", "<FONT COLOR=\"red\"> rootIsSource  : <FONT COLOR=\"black\"> Select a source in the graph.  <BR> <FONT COLOR=\"red\"> rootIsSink : <FONT COLOR=\"black\"> Select a sink in the graph. <BR> <FONT COLOR=\"red\"> rootByCoord : <FONT COLOR=\"black\"> Use the coordinates, e.g., select the topmost node if orientation is topToBottom.")
		HTML_HELP_DEF( "default", "rootIsSource " )
		HTML_HELP_BODY()
		"The option that determines how the root is selected."
		HTML_HELP_CLOSE()
};
}

#define ELT_ORIENTATION "Orientation"
#define ELT_ORIENTATIONLIST "topToBottom;bottomToTop;leftToRight;rightToLeft"
#define ELT_TOPTOBOTTOM 0
#define ELT_BOTTOMTOTOP 1
#define ELT_LEFTTORIGHT 2
#define ELT_RIGHTTOLEFT 3

#define ELT_ROOTSELECTION "Root selection"
#define ELT_ROOTSELECTIONLIST "rootIsSource;rootIsSink;rootByCoord"
#define ELT_ROOTSOURCE 0
#define ELT_ROOTSINK 1
#define ELT_ROOTCOORD 2

class OGDFTree : public OGDFLayoutPluginBase {

public:

	OGDFTree(const tlp::PropertyContext &context) :OGDFLayoutPluginBase(context, new ogdf::TreeLayout()) {
		addParameter<double>("siblings distance", paramHelp[0], "20");
		addParameter<double>("subtrees distance", paramHelp[1], "20");
		addParameter<double>("levels distance", paramHelp[2], "50");
		addParameter<double>("trees distance", paramHelp[3], "50");
		addParameter<bool>("orthogonal layout", paramHelp[4], "false");
		addParameter<StringCollection>(ELT_ORIENTATION, paramHelp[5], ELT_ORIENTATIONLIST);
		addParameter<StringCollection>(ELT_ROOTSELECTION, paramHelp[6], ELT_ROOTSELECTIONLIST);
	}

	~OGDFTree() {}

	void beforeCall(TulipToOGDF*, ogdf::LayoutModule *ogdfLayoutAlgo) {
		ogdf::TreeLayout *tree = static_cast<ogdf::TreeLayout*>(ogdfLayoutAlgo);
		if (dataSet != 0) {
			double dval;
			bool bval;
			StringCollection sc;
			if (dataSet->get("siblings distance", dval))
				tree->siblingDistance(dval);
			if (dataSet->get("subtrees distance", dval))
				tree->subtreeDistance(dval);
			if (dataSet->get("levels distance", dval))
				tree->levelDistance(dval);
			if (dataSet->get("trees distance", dval))
				tree->treeDistance(dval);
			if (dataSet->get("orthogonal layout", bval))
				tree->orthogonalLayout(bval);
			if (dataSet->get(ELT_ORIENTATION, sc)) {
				if (sc.getCurrent() == ELT_TOPTOBOTTOM) {
					tree->orientation(ogdf::topToBottom);
				} else if (sc.getCurrent() == ELT_BOTTOMTOTOP) {
					tree->orientation(ogdf::bottomToTop);
				} else if (sc.getCurrent() == ELT_LEFTTORIGHT) {
					tree->orientation(ogdf::leftToRight);
				} else {
					tree->orientation(ogdf::rightToLeft);
				}
			}
			if (dataSet->get(ELT_ROOTSELECTION, sc)) {
				if (sc.getCurrent() == ELT_ROOTSOURCE) {
					tree->rootSelection(ogdf::TreeLayout::rootIsSource);
				} else if (sc.getCurrent() == ELT_ROOTSINK) {
					tree->rootSelection(ogdf::TreeLayout::rootIsSink);
				} else {
					tree->rootSelection(ogdf::TreeLayout::rootByCoord);
				}
			}
		}
	}

};





LAYOUTPLUGINOFGROUP(OGDFTree,"Improved Walker (OGDF)","Christoph Buchheim","12/11/2007","Ok","1.5","Tree");
