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

#include <ogdf/energybased/FMMMLayout.h>

#include "tulip2ogdf/OGDFLayoutPluginBase.h"

using namespace std;

// comments below have been extracted from OGDF/src/energybased/FMMMLayout.cpp
/*@{*/
/** \file
 * \brief Implementation of Fast Multipole Multilevel Method (FM^3).
 *
 * This is an implementation of Fast Multipole Multilevel Method (FM^3).
 * \author Stefan Hachul
 * 
 * \par License:
 * This is part of the Open Graph Drawing Framework (OGDF).
 * Copyright (C) 2005-2007
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

const char * paramHelp[] = {HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "DoubleProperty" )
		HTML_HELP_BODY()
		"A double property containing unit edge length to use."
		HTML_HELP_CLOSE(), HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "SizeProperty" )
		HTML_HELP_BODY()
		"The nodes size."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "double" )
		HTML_HELP_BODY()
		"The unit edge length."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "bool" )
		HTML_HELP_BODY()
		"set new initial placement before running algorithm."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "String Collection" )
		HTML_HELP_DEF("values", "<FONT COLOR=\"red\"> Portrait : <FONT COLOR=\"black\"> A4 portrait page. <BR> <FONT COLOR=\"red\"> Landscape : <FONT COLOR=\"black\"> 	A4 landscape page. <BR> <FONT COLOR=\"red\"> Square : <FONT COLOR=\"black\"> Square format.")
		HTML_HELP_DEF( "default", "Square" )
		HTML_HELP_BODY()
		"Possible page formats."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "String Collection" )
		HTML_HELP_DEF("values", "<FONT COLOR=\"red\"> GorgeousAndEfficient : <FONT COLOR=\"black\"> Best quality. <BR> <FONT COLOR=\"red\"> BeautifulAndFast : <FONT COLOR=\"black\"> Medium quality and speed.<BR> <FONT COLOR=\"red\"> NiceAndIncredibleSpeed : <FONT COLOR=\"black\"> Best speed.")
		HTML_HELP_DEF( "default", "BeautifulAndFast" )
		HTML_HELP_BODY()
		"Trade-off between run-time and quality."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "String Collection" )
		HTML_HELP_DEF("values", "<FONT COLOR=\"red\"> Midpoint : <FONT COLOR=\"black\"> Measure from center point of edge end points. <BR> <FONT COLOR=\"red\"> BoundingCircle : <FONT COLOR=\"black\"> Measure from border of circle s surrounding edge end points. ")
		HTML_HELP_DEF( "default", "BoundingCircle" )
		HTML_HELP_BODY()
		"Specifies how the length of an edge is measured."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "String Collection" )
		HTML_HELP_DEF("values", "All <BR> Integer <BR> Exponent")
		HTML_HELP_DEF( "default", "Integer" )
		HTML_HELP_BODY()
		"Specifies which positions for a node are allowed."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "String Collection" )
		HTML_HELP_DEF("values", "None <BR> NoGrowingRow <BR> Always")
		HTML_HELP_DEF( "default", "NoGrowingRow" )
		HTML_HELP_BODY()
		"Specifies in which case it is allowed to tip over drawings of connected components."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "String Collection" )
		HTML_HELP_DEF("values", "<FONT COLOR=\"red\"> None : <FONT COLOR=\"black\"> Do not presort. <BR> <FONT COLOR=\"red\"> DecreasingHeight :  <FONT COLOR=\"black\"> Presort by decreasing height of components. <BR> <FONT COLOR=\"red\"> DecreasingWidth : <FONT COLOR=\"black\"> Presort by decreasing width of components. ")
		HTML_HELP_DEF( "default", "DecreasingHeight" )
		HTML_HELP_BODY()
		"Specifies how connected components are sorted before the packing algorithm is applied."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "String Collection" )
		HTML_HELP_DEF("values", "UniformProb <BR> NonUniformProbLowerMass <BR> NonUniformProbHigherMass")
		HTML_HELP_DEF( "default", "NonUniformProbLowerMass" )
		HTML_HELP_BODY()
		"Specifies how sun nodes of galaxies are selected. "
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "String Collection" )
		HTML_HELP_DEF("values", "Constant <BR> LinearlyDecreasing <BR> RapidlyDecreasing")
		HTML_HELP_DEF( "default", "LinearlyDecreasing" )
		HTML_HELP_BODY()
		"Specifies how MaxIterations is changed in subsequent multilevels. "
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "String Collection" )
		HTML_HELP_DEF("values", "Simple <BR> Advanced")
		HTML_HELP_DEF( "default", "Advanced" )
		HTML_HELP_BODY()
		"Specifies how the initial placement is generated."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "String Collection" )
		HTML_HELP_DEF("values", "<FONT COLOR=\"red\"> FruchtermanReingold : <FONT COLOR=\"black\"> The force-model by Fruchterman, Reingold.  <BR> <FONT COLOR=\"red\"> Eades : <FONT COLOR=\"black\"> The force-model by Eades. <BR> <FONT COLOR=\"red\"> New : <FONT COLOR=\"black\"> The new force-model. ")
		HTML_HELP_DEF( "default", "New" )
		HTML_HELP_BODY()
		"Specifies the force model. "
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "String Collection" )
		HTML_HELP_DEF("values", "<FONT COLOR=\"red\"> Exact : <FONT COLOR=\"black\"> Exact calculation. <BR> <FONT COLOR=\"red\"> GridApproximation : <FONT COLOR=\"black\"> Grid approximation. <BR> <FONT COLOR=\"red\"> NMM : <FONT COLOR=\"black\"> Calculation as for new multipole method. ")
		HTML_HELP_DEF( "default", "NMM" )
		HTML_HELP_BODY()
		"Specifies how to calculate repulsive forces."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "String Collection" )
		HTML_HELP_DEF("values", "<FONT COLOR=\"red\"> UniformGrid : <FONT COLOR=\"black\"> Uniform placement on a grid.  <BR> <FONT COLOR=\"red\"> RandomTime : <FONT COLOR=\"black\"> Random placement (based on current time).  <BR> <FONT COLOR=\"red\"> RandomRandIterNr : <FONT COLOR=\"black\"> Random placement (based on randIterNr()). <BR> <FONT COLOR=\"red\"> KeepPositions : <FONT COLOR=\"black\"> No change in placement. ")
		HTML_HELP_DEF( "default", "RandomRandIterNr" )
		HTML_HELP_BODY()
		"Specifies how the initial placement is done. "
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "String Collection" )
		HTML_HELP_DEF("values", "PathByPath <BR> SubtreeBySubtree")
		HTML_HELP_DEF( "default", "Advanced" )
		HTML_HELP_BODY()
		"Specifies how the reduced bucket quadtree is constructed."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "String Collection" )
		HTML_HELP_DEF("values", "<FONT COLOR=\"red\"> Iteratively : <FONT COLOR=\"black\"> 	Iteratively (in constant time). <BR> <FONT COLOR=\"red\"> Aluru : <FONT COLOR=\"black\"> According to formula by Aluru et al. (in constant time).")
		HTML_HELP_DEF( "default", "Advanced" )
		HTML_HELP_BODY()
		"Specifies how to calculate the smallest quadratic cell surrounding particles of a node in the reduced bucket quadtree."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "int" )
		HTML_HELP_BODY()
		"the fixed number of iterations for the stop criterion."
		HTML_HELP_CLOSE(),
		HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "double" )
		HTML_HELP_BODY()
		"the threshold for the stop criterion."
		HTML_HELP_CLOSE()
};

}

#define ELT_PAGEFORMAT "Page Format"
#define ELT_PAGEFORMATLIST "Square;Portrait;Landscape"
#define ELT_SQUARE 0
#define ELT_PORTRAIT 1
#define ELT_LANDSCAPE 2

#define ELT_QUALITYVSSPEED "Quality vs Speed"
#define ELT_QUALITYVSSPEEDLIST "BeautifulAndFast;NiceAndIncredibleSpeed;GorgeousAndEfficient"
#define ELT_BEAUTIFULANDFAST 0
#define ELT_NICEANDINCREDIBLESPEED 1
#define ELT_GORGEOUSANDEFFICIENT 2

#define ELT_EDGELENGTHMEASUREMENT "Edge Length Measurement"
#define ELT_EDGELENGTHMEASUREMENTLIST "BoundingCircle;Midpoint"
#define ELT_BOUNDINGCIRCLE 0
#define ELT_MIDPOINT 1

#define ELT_ALLOWEDPOSITIONS "Allowed Positions"
#define ELT_ALLOWEDPOSITIONSLIST "Integer;Exponent;All"
#define ELT_INTEGER 0
#define ELT_EXPONENT 1
#define ELT_ALL 2

#define ELT_TIPOVER "Tip Over"
#define ELT_TIPOVERLIST "NoGrowingRow;Always;None"
#define ELT_NOGROWINGROW 0
#define ELT_ALWAYS 1
#define ELT_NONE 2

#define ELT_PRESORT "Pre Sort"
#define ELT_PRESORTLIST "DecreasingHeight;DecreasingWidth;None;"
#define ELT_DECREASINGHEIGHT 0
#define ELT_DECREASINGWDTH 1

#define ELT_GALAXYCHOICE "Galaxy Choice"
#define ELT_GALAXYCHOICELIST "NonUniformProbLowerMass;NonUniformProbHigherMass;UniformProb"
#define ELT_NONUNIFORMPROBLOWERMASS 0
#define ELT_NONUNIFORMPROBHIGHERMASS 1
#define ELT_UNIFORMPROB 2

#define ELT_MAXITERCHANGE "Max Iter Change"
#define ELT_MAXITERCHANGELIST "LinearlyDecreasing;RapidlyDecreasing;Constant"
#define ELT_LINEARLYDECREASING 0
#define ELT_RAPIDLYDECREASING 1
#define ELT_CONSTANT 2

#define ELT_INITIALPLACEMENTMULT "Initial Placement Mult"
#define ELT_INITIALPLACEMENTMULTLIST "Advanced;Simple"
#define ELT_ADVANCED 0
#define ELT_SIMPLE 1

#define ELT_FORCEMODEL "Force Model"
#define ELT_FORCEMODELLIST "New;FruchtermanReingold;Eades"
#define ELT_NEW 0
#define ELT_FRUCHTERMANNREINGOLD 1
#define ELT_EADES 2

#define ELT_REPULSIVEFORCEMETHOD "Repulsive Force Method"
#define ELT_REPULSIVEFORCEMETHODLIST "NMM;Exact;GridApproximation"
#define ELT_NMM 0
#define ELT_EXACT 1
#define ELT_GRIDAPPROXIMATION 2

#define ELT_INITIALPLACEMENTFORCES "Initial Placement Forces"
#define ELT_INITIALPLACEMENTFORCESLIST "RandomRandIterNr;RandomTime;UniformGrid;KeepPositions"
#define ELT_RANDOMRANDITERNR 0
#define ELT_RANDOMTIME 1
#define ELT_UNIFORMGRID 2
#define ELT_KEEPPOSITIONS 3

#define ELT_REDUCEDTREECONSTRCUCTION "Reduced Tree Construction"
#define ELT_REDUCEDTREECONSTRCUCTIONLIST "SubtreeBySubtree;PathByPath"
#define ELT_SUBTREEBYSUBTREE 0
#define ELT_PATHBYPATH 1

#define ELT_SMALLESTCELLFINDING "Smallest Cell Finding"
#define ELT_SMALLESTCELLFINDINGLIST "Iteratively;Aluru"
#define ELT_ITERATIVELY 0
#define ELT_ALURU 1

class OGDFFm3: public OGDFLayoutPluginBase {

	tlp::StringCollection stringCollection;

public:

	OGDFFm3(const tlp::PropertyContext &context);
	~OGDFFm3();
	void beforeCall(TulipToOGDF *tlpToOGDF, ogdf::LayoutModule *ogdfLayoutAlgo);
};

/*Nom de la classe, Nom du plugins, nom de l'auteur,date de
 creation,informations, release, groupe*/
LAYOUTPLUGINOFGROUP(OGDFFm3,"FM^3 (OGDF)", "Stephan Hachul", "09/11/2007", "Alpha", "1.2" ,"Force Directed");

OGDFFm3::OGDFFm3(const tlp::PropertyContext &context) :
						OGDFLayoutPluginBase(context, new ogdf::FMMMLayout()) {
	addParameter<DoubleProperty> ("Edge Length Property", paramHelp[0],
			"viewMetric", false);
	addParameter<SizeProperty> ("Node Size", paramHelp[1], "viewSize", false);
	addParameter<double> ("Unit edge length", paramHelp[2], "10.0", false);
	addParameter<bool> ("New initial placement", paramHelp[3], "true");
	addParameter<int>("Fixed iterations", paramHelp[18], "30");
	addParameter<double>("Threshold", paramHelp[19], "0.01");
	addParameter<StringCollection> (ELT_PAGEFORMAT, paramHelp[4],
			ELT_PAGEFORMATLIST);
	addParameter<StringCollection> (ELT_QUALITYVSSPEED, paramHelp[5],
			ELT_QUALITYVSSPEEDLIST);
	addParameter<StringCollection> (ELT_EDGELENGTHMEASUREMENT, paramHelp[6],
			ELT_EDGELENGTHMEASUREMENTLIST);
	addParameter<StringCollection> (ELT_ALLOWEDPOSITIONS, paramHelp[7],
			ELT_ALLOWEDPOSITIONSLIST);
	addParameter<StringCollection> (ELT_TIPOVER, paramHelp[8], ELT_TIPOVERLIST);
	addParameter<StringCollection> (ELT_PRESORT, paramHelp[9], ELT_PRESORTLIST);
	addParameter<StringCollection> (ELT_GALAXYCHOICE, paramHelp[10],
			ELT_GALAXYCHOICELIST);
	addParameter<StringCollection> (ELT_MAXITERCHANGE, paramHelp[11],
			ELT_MAXITERCHANGELIST);
	addParameter<StringCollection> (ELT_INITIALPLACEMENTMULT, paramHelp[12],
			ELT_INITIALPLACEMENTMULTLIST);
	addParameter<StringCollection> (ELT_FORCEMODEL, paramHelp[13],
			ELT_FORCEMODELLIST);
	addParameter<StringCollection> (ELT_REPULSIVEFORCEMETHOD, paramHelp[14],
			ELT_REPULSIVEFORCEMETHODLIST);
	addParameter<StringCollection> (ELT_INITIALPLACEMENTFORCES, paramHelp[15],
			ELT_INITIALPLACEMENTFORCESLIST);
	addParameter<StringCollection> (ELT_REDUCEDTREECONSTRCUCTION, paramHelp[16],
			ELT_REDUCEDTREECONSTRCUCTIONLIST);
	addParameter<StringCollection> (ELT_SMALLESTCELLFINDING, paramHelp[17],
			ELT_SMALLESTCELLFINDINGLIST);
}

OGDFFm3::~OGDFFm3() {
}

void OGDFFm3::beforeCall(TulipToOGDF *tlpToOGDF,
		ogdf::LayoutModule *ogdfLayoutAlgo) {
	ogdf::FMMMLayout *fmmm = static_cast<ogdf::FMMMLayout*> (ogdfLayoutAlgo);
	if (dataSet != 0) {
		fmmm->useHighLevelOptions(true);
		SizeProperty * size = NULL;
		if (dataSet->get("Node Size", size))
			tlpToOGDF->copyTlpNodeSizeToOGDF(size);
		DoubleProperty * length = NULL;
		if (dataSet->get("Edge Length", length))
			tlpToOGDF->copyTlpDoublePropertyToOGDFEdgeLength(length);

		double edgeLenth = 10;
		if (dataSet->get("Unit edge length", edgeLenth))
			fmmm->unitEdgeLength(edgeLenth);

		bool bval = false;
		if (dataSet->get("New initial placement", bval)) {
			fmmm->newInitialPlacement(bval);
		}


		int ival = 0;
		if (dataSet->get("Fixed iterations", ival)) {
			fmmm->fixedIterations(ival);
		}

		double dval = 0;
		if (dataSet->get("Threshold", dval)) {
			fmmm->threshold(dval);
		}

		if (dataSet->get(ELT_PAGEFORMAT, stringCollection)) {
			if (stringCollection.getCurrent() == ELT_PORTRAIT) {
				fmmm->pageFormat(ogdf::FMMMLayout::pfPortrait);
			} else if (stringCollection.getCurrent() == ELT_LANDSCAPE) {
				fmmm->pageFormat(ogdf::FMMMLayout::pfLandscape);
			} else {
				fmmm->pageFormat(ogdf::FMMMLayout::pfSquare);
			}
		}

		if (dataSet->get(ELT_QUALITYVSSPEED, stringCollection)) {
			if (stringCollection.getCurrent() == ELT_NICEANDINCREDIBLESPEED) {
				fmmm->qualityVersusSpeed(
						ogdf::FMMMLayout::qvsNiceAndIncredibleSpeed);
			} else if (stringCollection.getCurrent() == ELT_GORGEOUSANDEFFICIENT) {
				fmmm->qualityVersusSpeed(ogdf::FMMMLayout::qvsGorgeousAndEfficient);
			} else {
				fmmm->qualityVersusSpeed(ogdf::FMMMLayout::qvsBeautifulAndFast);
			}
		}

		if (dataSet->get(ELT_EDGELENGTHMEASUREMENT, stringCollection)) {
			if (stringCollection.getCurrent() == ELT_BOUNDINGCIRCLE) {
				fmmm->edgeLengthMeasurement(ogdf::FMMMLayout::elmBoundingCircle);
			} else {
				fmmm->edgeLengthMeasurement(ogdf::FMMMLayout::elmMidpoint);
			}
		}

		if (dataSet->get(ELT_ALLOWEDPOSITIONS, stringCollection)) {
			if (stringCollection.getCurrent() == ELT_INTEGER) {
				fmmm->allowedPositions(ogdf::FMMMLayout::apInteger);
			} else if (stringCollection.getCurrent() == ELT_EXPONENT) {
				fmmm->allowedPositions(ogdf::FMMMLayout::apExponent);
			} else {
				fmmm->allowedPositions(ogdf::FMMMLayout::apAll);
			}
		}

		if (dataSet->get(ELT_TIPOVER, stringCollection)) {
			if (stringCollection.getCurrent() == ELT_NONE) {
				fmmm->tipOverCCs(ogdf::FMMMLayout::toNone);
			} else if (stringCollection.getCurrent() == ELT_NOGROWINGROW) {
				fmmm->tipOverCCs(ogdf::FMMMLayout::toNoGrowingRow);
			} else {
				fmmm->tipOverCCs(ogdf::FMMMLayout::toAlways);
			}
		}

		if (dataSet->get(ELT_PRESORT, stringCollection)) {
			if (stringCollection.getCurrent() == ELT_NONE) {
				fmmm->presortCCs(ogdf::FMMMLayout::psNone);
			} else if (stringCollection.getCurrent() == ELT_DECREASINGHEIGHT) {
				fmmm->presortCCs(ogdf::FMMMLayout::psDecreasingHeight);
			} else {
				fmmm->presortCCs(ogdf::FMMMLayout::psDecreasingWidth);
			}
		}

		if (dataSet->get(ELT_GALAXYCHOICE, stringCollection)) {
			if (stringCollection.getCurrent() == ELT_UNIFORMPROB) {
				fmmm->galaxyChoice(ogdf::FMMMLayout::gcUniformProb);
			} else if (stringCollection.getCurrent() == ELT_NONUNIFORMPROBLOWERMASS) {
				fmmm->galaxyChoice(ogdf::FMMMLayout::gcNonUniformProbLowerMass);
			} else {
				fmmm->galaxyChoice(ogdf::FMMMLayout::gcNonUniformProbHigherMass);
			}
		}

		if (dataSet->get(ELT_MAXITERCHANGE, stringCollection)) {
			if (stringCollection.getCurrent() == ELT_CONSTANT) {
				fmmm->maxIterChange(ogdf::FMMMLayout::micConstant);
			} else if (stringCollection.getCurrent() == ELT_LINEARLYDECREASING) {
				fmmm->maxIterChange(ogdf::FMMMLayout::micLinearlyDecreasing);
			} else {
				fmmm->maxIterChange(ogdf::FMMMLayout::micRapidlyDecreasing);
			}
		}

		if (dataSet->get(ELT_INITIALPLACEMENTMULT, stringCollection)) {
			if (stringCollection.getCurrent() == ELT_ADVANCED) {
				fmmm->initialPlacementMult(ogdf::FMMMLayout::ipmAdvanced);
			} else {
				fmmm->initialPlacementMult(ogdf::FMMMLayout::ipmSimple);
			}
		}

		if (dataSet->get(ELT_FORCEMODEL, stringCollection)) {
			if (stringCollection.getCurrent() == ELT_FRUCHTERMANNREINGOLD) {
				fmmm->forceModel(ogdf::FMMMLayout::fmFruchtermanReingold);
			} else if (stringCollection.getCurrent() == ELT_EADES) {
				fmmm->forceModel(ogdf::FMMMLayout::fmEades);
			} else {
				fmmm->forceModel(ogdf::FMMMLayout::fmNew);
			}
		}

		if (dataSet->get(ELT_REPULSIVEFORCEMETHOD, stringCollection)) {
			if (stringCollection.getCurrent() == ELT_EXACT) {
				fmmm->repulsiveForcesCalculation(ogdf::FMMMLayout::rfcExact);
			} else if (stringCollection.getCurrent() == ELT_GRIDAPPROXIMATION) {
				fmmm->repulsiveForcesCalculation(
						ogdf::FMMMLayout::rfcGridApproximation);
			} else {
				fmmm->repulsiveForcesCalculation(ogdf::FMMMLayout::rfcNMM);
			}
		}

		if (dataSet->get(ELT_INITIALPLACEMENTFORCES, stringCollection)) {
			if (stringCollection.getCurrent() == ELT_UNIFORMGRID) {
				fmmm->initialPlacementForces(ogdf::FMMMLayout::ipfUniformGrid);
			} else if (stringCollection.getCurrent() == ELT_RANDOMTIME) {
				fmmm->initialPlacementForces(ogdf::FMMMLayout::ipfRandomTime);
			} else if (stringCollection.getCurrent() == ELT_RANDOMRANDITERNR) {
				fmmm->initialPlacementForces(ogdf::FMMMLayout::ipfRandomRandIterNr);
			} else {
				fmmm->initialPlacementForces(ogdf::FMMMLayout::ipfKeepPositions);
			}
		}

		if (dataSet->get(ELT_REDUCEDTREECONSTRCUCTION, stringCollection)) {
			if (stringCollection.getCurrent() == ELT_SUBTREEBYSUBTREE) {
				fmmm->nmTreeConstruction(ogdf::FMMMLayout::rtcSubtreeBySubtree);
			} else {
				fmmm->nmTreeConstruction(ogdf::FMMMLayout::rtcPathByPath);
			}
		}

		if (dataSet->get(ELT_SMALLESTCELLFINDING, stringCollection)) {
			if (stringCollection.getCurrent() == ELT_ITERATIVELY) {
				fmmm->nmSmallCell(ogdf::FMMMLayout::scfIteratively);
			} else {
				fmmm->nmSmallCell(ogdf::FMMMLayout::scfAluru);
			}
		}
	}

}

