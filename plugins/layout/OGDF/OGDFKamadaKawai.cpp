#include <ogdf/energybased/SpringEmbedderKK.h>

#include "tulip2ogdf/OGDFLayoutPluginBase.h"

// comments below have been extracted from OGDF/src/energybased/SpringEmbedderKK.cpp
/*@{*/
/** \file
 * \brief Implementation of Kamada-Kaway layout algorithm.
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
		HTML_HELP_DEF( "type", "double" )
		HTML_HELP_BODY()
		"If set != 0, value zerolength is used to determine the desirable edge length by L = zerolength / max distance_ij. Otherwise, zerolength is determined using the node number and sizes."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "double" )
		HTML_HELP_BODY()
		"Sets desirable edge length directly."
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
		HTML_HELP_CLOSE()
};
}

class OGDFKamadaKawai : public OGDFLayoutPluginBase {

public:

	OGDFKamadaKawai(const tlp::PropertyContext &context) :OGDFLayoutPluginBase(context, new ogdf::SpringEmbedderKK()) {
		addParameter<double>("stop tolerance", paramHelp[0], "0.001");
		addParameter<bool>("used layout", paramHelp[1], "true");
		addParameter<double>("zero length", paramHelp[2], "0");
		addParameter<double>("edge length", paramHelp[3], "0");
		addParameter<bool>("compute max iterations", paramHelp[4], "true");
		addParameter<int>("global iterations", paramHelp[5], "50");
		addParameter<int>("local iterations", paramHelp[6], "50");
	}
	~OGDFKamadaKawai() {}

	void beforeCall(TulipToOGDF*, ogdf::LayoutModule *ogdfLayoutAlgo) {
		ogdf::SpringEmbedderKK *kamada = static_cast<ogdf::SpringEmbedderKK*>(ogdfLayoutAlgo);
		if (dataSet != 0) {
			double dval;
			int ival;
			bool bval;
			if (dataSet->get("stop tolerance", dval))
				kamada->setStopTolerance(dval);
			if (dataSet->get("use layout", bval))
				kamada->setUseLayout(bval);
			if (dataSet->get("zero length", dval))
				kamada->setZeroLength(dval);
			if (dataSet->get("edge length", dval))
				kamada->setDesLength(dval);
			if (dataSet->get("compute max iterations", bval))
				kamada->computeMaxIterations(bval);
			if (dataSet->get("global iterations", ival))
				kamada->setMaxGlobalIterations(ival);
			if (dataSet->get("local iterations", ival))
				kamada->setMaxLocalIterations(ival);

		}
	}

};


LAYOUTPLUGINOFGROUP(OGDFKamadaKawai,"Kamada Kawai (OGDF)","Karsten Klein","12/11/2007","Ok","1.0","Force Directed");
