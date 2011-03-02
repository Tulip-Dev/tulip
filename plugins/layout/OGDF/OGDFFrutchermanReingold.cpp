//-*-c++-*-
/*
 Author: Didier Bathily, Nicolas Bellino, Jonathan Dubois, Christelle Jolly, Antoine Lambert, Nicolas Sorraing

 Email : didier.bathily@etu.u-bordeaux1.fr, nicolas.bellino@etu.u-bordeaux1.fr, jonathan.dubois@etu.u-bordeaux1.fr, christelle.jolly@etu.u-bordeaux1.fr, antoine.lambert@etu.u-bordeaux1.fr, nicolas.sorraing@etu.u-bordeaux1.fr

 Last modification : 03/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
 */

#include <ogdf/energybased/SpringEmbedderFRExact.h>
#include "tulip2ogdf/OGDFLayoutPluginBase.h"

// comments below have been extracted from OGDF/src/energybased/SpringEmbedderFRExact.cpp
/*@{*/
/** \file
 * \brief Implementation of Spring-Embedder (Fruchterman,Reingold)
 *        algorithm with exact force computations.
 *
 * \author Carsten Gutwenger
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

const char * paramHelp[] = { HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "int" )
		HTML_HELP_BODY()
		"Sets the number of iterations to i."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "bool" )
		HTML_HELP_BODY()
		"Sets the parameter noise. "
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "bool" )
		HTML_HELP_BODY()
		"set the use of node weights given in metric.  "
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "DoubleProperty" )
		HTML_HELP_BODY()
		"metric containing nodes weights.  "
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "StringCollection")
		HTML_HELP_DEF("values", "Factor <BR> Logarithmic")
		HTML_HELP_DEF( "default", "Factor" )
		HTML_HELP_BODY()
		"Sets the parameter cooling function"
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "double" )
		HTML_HELP_BODY()
		"The ideal edge length."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "double" )
		HTML_HELP_BODY()
		"The minimal distance between connected components."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "double" )
		HTML_HELP_BODY()
		"The page ratio used for packing connected components."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "bool" )
		HTML_HELP_BODY()
		"check convergence option. "
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "double" )
		HTML_HELP_BODY()
		"convergence tolerance parameter."
		HTML_HELP_CLOSE()
};
}

#define ELT_COOLING "Cooling function"
#define ELT_COOLINGLIST "Factor;Logarithmic"
#define ELT_FACTOR 0
#define ELT_LOGARITHMIC 1

class OGDFFrutchermanReingold: public OGDFLayoutPluginBase {

public:

	OGDFFrutchermanReingold(const tlp::PropertyContext &context);
	~OGDFFrutchermanReingold();

	void beforeCall(TulipToOGDF *tlpToOGDF, ogdf::LayoutModule *ogdfLayoutAlgo);
};

/*Nom de la classe, Nom du plugins, nom de l'auteur,date de
 creation,informations, realease, groupe*/
LAYOUTPLUGINOFGROUP(OGDFFrutchermanReingold, "Frutcherman Reingold (OGDF)","Stephan Hachul","15/11/2007", "Alpha", "1.1","Force Directed");

OGDFFrutchermanReingold::OGDFFrutchermanReingold(const tlp::PropertyContext &context) :
																											OGDFLayoutPluginBase(context, new ogdf::SpringEmbedderFRExact()) {
	addParameter<int>("iterations", paramHelp[0], "1000");
	addParameter<bool>("noise", paramHelp[1], "true");
	addParameter<bool>("use node weights", paramHelp[2], "false");
	addParameter<DoubleProperty>("node weights", paramHelp[3], "viewMetric");
	addParameter<StringCollection>(ELT_COOLING, paramHelp[4], ELT_COOLINGLIST);
	addParameter<double>("ideal edge length", paramHelp[5], "10.0");
	addParameter<double>("minDistCC", paramHelp[6], "20.0");
	addParameter<double>("pageRatio", paramHelp[7], "1.0");
	addParameter<bool>("check convergence", paramHelp[8], "true");
	addParameter<double>("convergence tolerance", paramHelp[9], "0.01");
}

OGDFFrutchermanReingold::~OGDFFrutchermanReingold() {}

void OGDFFrutchermanReingold::beforeCall(TulipToOGDF *tlpToOGDF, ogdf::LayoutModule *ogdfLayoutAlgo) {
	ogdf::SpringEmbedderFRExact *sefr = static_cast<ogdf::SpringEmbedderFRExact*>(ogdfLayoutAlgo);
	if (dataSet != 0) {
		int ival;
		double dval;
		bool bval;
		StringCollection sc;
		DoubleProperty *metric = NULL;
		if (dataSet->get("iterations", ival))
			sefr->iterations(ival);
		if (dataSet->get("noise", bval))
			sefr->noise(bval);
		if (dataSet->get("minDistCC", dval))
			sefr->minDistCC(dval);
		if (dataSet->get("pageRatio", dval))
			sefr->pageRatio(dval);
		if (dataSet->get(ELT_COOLING, sc)) {
			if (sc.getCurrent() == ELT_FACTOR) {
				sefr->coolingFunction(ogdf::SpringEmbedderFRExact::cfFactor);
			} else {
				sefr->coolingFunction(ogdf::SpringEmbedderFRExact::cfLogarithmic);
			}
		}
		if (dataSet->get("use node weights", bval)) {
			sefr->nodeWeights(bval);
			if (bval && dataSet->get("node weights", metric)) {
				tlpToOGDF->copyTlpDoublePropertyToOGDFNodeWeight(metric);
			}
		}
		if (dataSet->get("check convergence", bval))
			sefr->checkConvergence(bval);
		if (dataSet->get("convergence tolerance", dval))
			sefr->convTolerance(dval);

	}
}
