#include <ogdf/energybased/StressMajorizationSimple.h>

#include "tulip2ogdf/OGDFLayoutPluginBase.h"

// comments below have been extracted from OGDF/src/energybased/StressMajorizationSimple.cpp
/*@{*/
/** \file
 * \brief Implementation of stress-majorization layout algorithm.
 *
 * \author Karsten Klein
 *
 * \par License:
 * This file is part of the Open Graph Drawing Framework (OGDF).
 * Copyright (C) 2005-2009
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
		"Sets a fixed number of iterations for stress majorization in main step."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "double" )
		HTML_HELP_BODY()
		"Sets the value for the stop tolerance, below which the system is regarded stable (balanced) and the optimization stopped. "
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "bool" )
		HTML_HELP_BODY()
		"If set to true, the given layout is used for the initial positions."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "bool" )
		HTML_HELP_BODY()
		"f set to true, number of iterations is computed depending on G."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "int" )
		HTML_HELP_BODY()
		"Sets the number of global iterations to i."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "int" )
		HTML_HELP_BODY()
		"Sets the number of local iterations to i."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "bool" )
		HTML_HELP_BODY()
		"If set to true, radial constraints are added."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "bool" )
		HTML_HELP_BODY()
		"If set to true, upward constraints are added. "
		HTML_HELP_CLOSE(),
};
}

class OGDFStressMajorization : public OGDFLayoutPluginBase {

public:

	OGDFStressMajorization(const tlp::PropertyContext &context) :OGDFLayoutPluginBase(context, new ogdf::StressMajorization()) {
		addParameter<int>("iterations", paramHelp[0], "300");
		addParameter<double>("stop tolerance", paramHelp[1], "0.001");
		addParameter<bool>("used layout", paramHelp[2], "false");
		addParameter<bool>("compute max iterations", paramHelp[3], "true");
		addParameter<int>("global iterations", paramHelp[4], "50");
		addParameter<int>("local iterations", paramHelp[5], "50");
		addParameter<bool>("radial", paramHelp[6], "false");
		addParameter<bool>("upward", paramHelp[7], "false");
	}
	~OGDFStressMajorization() {}

	void beforeCall(TulipToOGDF*, ogdf::LayoutModule *ogdfLayoutAlgo) {
		ogdf::StressMajorization *stressm = static_cast<ogdf::StressMajorization*>(ogdfLayoutAlgo);
		if (dataSet != 0) {
			double dval;
			int ival;
			bool bval;
			if (dataSet->get("iterations", ival))
				stressm->setIterations(ival);
			if (dataSet->get("stop tolerance", dval))
				stressm->setStopTolerance(dval);
			if (dataSet->get("use layout", bval))
				stressm->setUseLayout(bval);
			if (dataSet->get("compute max iterations", bval))
				stressm->computeMaxIterations(bval);
			if (dataSet->get("global iterations", ival))
				stressm->setMaxGlobalIterations(ival);
			if (dataSet->get("local iterations", ival))
				stressm->setMaxLocalIterations(ival);
			if (dataSet->get("radial", bval))
				stressm->radial(bval);
			if (dataSet->get("upward", bval))
				stressm->upward(bval);
		}
	}

};


LAYOUTPLUGINOFGROUP(OGDFStressMajorization,"Stress Majorization (OGDF)","Karsten Klein","12/11/2007","Ok","1.0","Force Directed");
