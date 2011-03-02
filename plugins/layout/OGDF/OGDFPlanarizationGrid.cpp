#include <ogdf/planarity/PlanarizationGridLayout.h>
#include <ogdf/planarity/FastPlanarSubgraph.h>
#include <ogdf/planarity/MaximalPlanarSubgraphSimple.h>
#include <ogdf/planarity/FixedEmbeddingInserter.h>
#include <ogdf/planarity/VariableEmbeddingInserter.h>

#include "tulip2ogdf/OGDFLayoutPluginBase.h"


// comments below have been extracted from OGDF/src/planarity/PlanarizationGridLayout.cpp
/*@{*/
/** \file
 * \brief Implements planarization with grid layout.
 *
 * \author Carsten Gutwenger
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
		"Sets the option pageRatio."
		HTML_HELP_CLOSE(),
		HTML_HELP_DEF( "type", "StringCollection")
		HTML_HELP_DEF("values", "<FONT COLOR=\"red\"> FastPlanarSubgraph : <FONT COLOR=\"black\"> Computation of a planar subgraph using PQ-trees. <BR> <FONT COLOR=\"red\"> MaximalPlanarSubgraphSimple")
		HTML_HELP_DEF( "default", "FastPlanarSubgraph " )
		HTML_HELP_BODY()
		"Sets the module option for the computation of the planar subgraph."
		HTML_HELP_CLOSE(),
		HTML_HELP_DEF( "type", "StringCollection")
		HTML_HELP_DEF("values", "<FONT COLOR=\"red\"> FixedEmbeddingInserter : <FONT COLOR=\"black\"> Edge insertion module that inserts each edge optimally into a fixed embedding. <BR> <FONT COLOR=\"red\"> VariableEmbeddingInserter : <FONT COLOR=\"black\"> optimal edge insertion algorithm, which inserts a single edge with a minum number of crossings into a planar graph. <BR> <FONT COLOR=\"red\"> VariableEmbeddingInserter2 ")
		HTML_HELP_DEF( "default", "FixedEmbeddingInserter " )
		HTML_HELP_BODY()
		"Sets the module option for edge insertion."
		HTML_HELP_CLOSE()
};
}


#define ELT_PLANARSUBGRAPH "Planar subgraph module"
#define ELT_PLANARSUBGRAPHLIST "FastPlanarSubgraph;MaximalPlanarSubgraphSimple"
#define ELT_FASTPLANAR 0
#define ELT_MAXIMALPLANAR 1

#define ELT_EDGEINSERTION "Edge insertion module"
#define ELT_EDGEINSERTIONLIST "FixedEmbeddingInserter;VariableEmbeddingInserter"
#define ELT_FIXEDEMBEDDING 0
#define ELT_VARIABLEMBEDDING 1

class OGDFPlanarizationGrid : public OGDFLayoutPluginBase {

public:

	OGDFPlanarizationGrid(const tlp::PropertyContext &context) :OGDFLayoutPluginBase(context, new ogdf::PlanarizationGridLayout()) {
		addParameter<double>("page ratio", paramHelp[0], "1.0");
		addParameter<StringCollection>(ELT_PLANARSUBGRAPH, paramHelp[1], ELT_PLANARSUBGRAPHLIST);
		addParameter<StringCollection>(ELT_EDGEINSERTION, paramHelp[2], ELT_EDGEINSERTIONLIST);
	}

	~OGDFPlanarizationGrid() {}

	void beforeCall(TulipToOGDF*, ogdf::LayoutModule *ogdfLayoutAlgo) {
		ogdf::PlanarizationGridLayout *pgl = static_cast<ogdf::PlanarizationGridLayout*>(ogdfLayoutAlgo);
		if (dataSet != 0) {
			double dval;
			StringCollection sc;
			if (dataSet->get("page ratio", dval))
				pgl->pageRatio(dval);
			if (dataSet->get(ELT_PLANARSUBGRAPH, sc)) {
				if (sc.getCurrent() == ELT_FASTPLANAR) {
					pgl->setSubgraph(new ogdf::FastPlanarSubgraph());
				} else {
					pgl->setSubgraph(new ogdf::MaximalPlanarSubgraphSimple());
				}
			}
			if (dataSet->get(ELT_EDGEINSERTION, sc)) {
				if (sc.getCurrent() == ELT_FIXEDEMBEDDING) {
					pgl->setInserter(new ogdf::FixedEmbeddingInserter());
				} else {
					pgl->setInserter(new ogdf::VariableEmbeddingInserter);
				}
			}
		}
	}

};


LAYOUTPLUGINOFGROUP(OGDFPlanarizationGrid,"Planarization Grid (OGDF)","Carsten Gutwenger","12/11/2007","Ok","1.0","Planar");
