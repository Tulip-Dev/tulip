/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#include <tulip/StringCollection.h>
#include <tulip/TlpTools.h>

#include <tulip2ogdf/OGDFLayoutPluginBase.h>
#include <ogdf/energybased/FMMMLayout.h>

using namespace std;

#define PAGEFORMATLIST "square;portrait;landscape"
#define SQUARE 0
#define PORTRAIT 1
#define LANDSCAPE 2

#define QUALITYVSSPEEDLIST "beautiful and fast;nice and incredible speed;gorgeous and efficient"
#define BEAUTIFULANDFAST 0
#define NICEANDINCREDIBLESPEED 1
#define GORGEOUSANDEFFICIENT 2

#define EDGELENGTHMEASUREMENTLIST "bounding circle;midpoint"
#define BOUNDINGCIRCLE 0
#define MIDPOINT 1

#define ALLOWEDPOSITIONSLIST "integer;exponent;all"
#define INTEGER 0
#define EXPONENT 1
#define ALL 2

#define TIPOVERLIST "no growing row;always;none"
#define NOGROWINGROW 0
#define ALWAYS 1
#define NONE 2

#define PRESORTLIST "decreasing height;decreasing width;none;"
#define DECREASINGHEIGHT 0
#define DECREASINGWDTH 1

#define GALAXYCHOICELIST "non uniform lower mass;non uniform higher mass;uniform probability"
#define NONUNIFORMPROBLOWERMASS 0
#define NONUNIFORMPROBHIGHERMASS 1
#define UNIFORMPROB 2

#define MAXITERCHANGELIST "linearly decreasing;rapidly decreasing;constant"
#define LINEARLYDECREASING 0
#define RAPIDLYDECREASING 1
#define CONSTANT 2

#define INITIALPLACEMENTMULTLIST "advanced;simple"
#define ADVANCED 0
#define SIMPLE 1

#define FORCEMODEL "Force Model"
#define FORCEMODELLIST "new;fruchterman reingold;eades"
#define NEW 0
#define FRUCHTERMANNREINGOLD 1
#define EADES 2

#define REPULSIVEFORCEMETHODLIST "nmm;exact;grid approximation"
//#define NMM 0
#define EXACT 1
#define GRIDAPPROXIMATION 2

#define INITIALPLACEMENTFORCESLIST "default;random seed;random time;uniform grid;keep positions"
#define RANDOMRANDITERNR 1
#define RANDOMTIME 2
#define UNIFORMGRID 3
#define KEEPPOSITIONS 4

#define REDUCEDTREECONSTRUCTIONLIST "subtree by subtree;path by path"
#define SUBTREEBYSUBTREE 0
#define PATHBYPATH 1

#define SMALLESTCELLFINDINGLIST "iteratively;aluru"
#define ITERATIVELY 0
#define ALURU 1

using namespace tlp;
using namespace ogdf;

static const char *paramHelp[] = {

    // Edge Length Property
    "A numeric property giving the unit edge length to use.",

    // Node Size
    "The node sizes.",

    // Unit edge length
    "The unit edge length. Not used if \"edge length property\" is set.",

    // New initial layout
    "Indicates the initial layout before running algorithm. "
    "This is a high level option inducing an implicit value to the 'initial layout forces' "
    "parameter.",

    // Fixed iterations
    "The fixed number of iterations for the stop criterion. "
    "If not set to 0, it supersedes the value induced by the 'Quality vs Speed' parameter.",

    // Threshold
    "The threshold for the stop criterion.",

    // Page Format
    "Possible page formats.",

    // Quality vs Speed
    "Trade-off between run-time and quality. "
    "This is a high level option inducing an implicit value to the 'Fixed iterations' parameter.",

    // Edge Length Measurement
    "Specifies how the length of an edge is measured.",

    // Allowed Positions
    "Specifies which positions for a node are allowed.",

    // Tip Over
    "Specifies in which case it is allowed to tip over drawings of connected components.",

    // Presort
    "Specifies how connected components are sorted before the packing algorithm is applied.",

    // Galaxy Choice
    "Specifies how sun nodes of galaxies are selected.",

    // max iterations change
    "Specifies how MaxIterations is changed in subsequent multilevels.",

    // Initial layout
    "Specifies how the initial layout is generated.",

    // Force Model
    "Specifies the force-model.",

    // Repulsive Force Model
    "Specifies how to calculate repulsive forces.",

    // Initial layout Forces
    "Specifies how the initial layout is done. "
    "If not set to default, it supersedes the value induced by the \"new initial layout\" parameter.",

    // Reduced Tree Construction
    "Specifies how the reduced bucket quadtree is constructed.",

    // Smallest Cell Finding
    "Specifies how to calculate the smallest quadratic cell surrounding particles of a node in the "
    "reduced bucket quadtree."};

static const char *pageFormatValuesDescription = "square <i>(square format)</i><br/>"
                                                 "portrait <i>(A4 portrait page)</i><br/>"
                                                 "landscape <i>(A4 landscape page)</i>";

static const char *qualityVsSpeedValuesDescription =
    "beautiful and fast <i>(medium quality and speed)</i><br/>"
    "nice and incredible speed <i>(best speed)</i><br/>"
    "gorgeous and efficient <i>(best quality)</i>";

static const char *edgeLengthMeasurementValuesDescription =
    "bounding circle <i>(measure from border of circle surrounding edge end points)</i><br/>"
    "midpoint <i>(measure from center point of edge end points)</i>";

static const char *presortValuesDescription =
    "decreasing height <i>(presort by decreasing height of components)</i><br/>"
    "decreasing width <i>(presort by decreasing width of components)</i><br/>"
    "none <i>(Do not presort)</i>";

static const char *galaxyChoiceValuesDescription =
    "non uniform lower mass <i>(use non-uniform probability depending on the lower star masses)</i><br/>"
    "non uniform probability higher mass<i>(use non-uniform probability depending on the higher star masses)</i><br/>"
    "uniform probability <i>(use uniform random probability)</i>";

static const char *maxIterChangeValuesDescription = "linearly decreasing<br/>"
                                                    "rapidly decreasing<br/>"
                                                    "constant";

static const char *forceModelValuesDescription =
    "new <i>(new force-model)</i><br>"
    "fruchterman reingold <i>(force-model by Fruchterman and Reingold)</i><br/>"
    "eades <i>(force-model by Eades)</i>";

static const char *repulsiveForceValuesDescription =
    "nmm <i>(calculation as for new multipole method)</i><br/>"
    "exact <i>(exact calculation)</i><br/>"
    "grid approximation <i>(grid approximation)</i>";

static const char *initialPlacementValuesDescription =
    "default <i>(use default of \"new initial layout\" parameter)"
    "random seed <i>(random layout, based on random seed)</i><br>"
    "random time <i>(random layout, based on current time)</i><br>"
    "uniform grid <i>(uniform layout on a grid)</i><br>"
    "keep positions <i>(No change in layout)</i>";

static const char *smallestCellFindingValuesDescription =
    "iteratively <i>(iteratively, in constant time)</i><br>"
    "aluru <i>(according to formula by Aluru et al., in constant time)</i>";

class OGDFFm3 : public OGDFLayoutPluginBase {

  tlp::StringCollection stringCollection;

public:
  PLUGININFORMATION("FM^3 (OGDF)", "Stephan Hachul", "09/11/2007",
                    "Implements the FM³ layout algorithm by Hachul and Jünger. It is a multilevel, "
                    "force-directed layout algorithm that can be applied to very large graphs.",
                    "1.5", "Force Directed")
  OGDFFm3(const tlp::PluginContext *context);
  ~OGDFFm3() override;
  void beforeCall() override;
  void callOGDFLayoutAlgorithm(ogdf::GraphAttributes &gAttributes) override;
};

/*Nom de la classe, Nom du plugins, nom de l'auteur,date de
 creation,information, release, groupe*/
PLUGIN(OGDFFm3)

OGDFFm3::OGDFFm3(const tlp::PluginContext *context)
    : OGDFLayoutPluginBase(context, context ? new ogdf::FMMMLayout() : nullptr) {
  addInParameter<NumericProperty *>("edge length property", paramHelp[0], "", false);
  addInParameter<SizeProperty>("node size", paramHelp[1], "viewSize", false);
  addInParameter<double>("unit edge length", paramHelp[2], "10.0", false);
  addInParameter<bool>("new initial layout", paramHelp[3], "true");
  addInParameter<int>("fixed iterations", paramHelp[4], "0");
  addInParameter<double>("threshold", paramHelp[5], "0.01");
  addInParameter<StringCollection>("page format", paramHelp[6], PAGEFORMATLIST, true,
                                   pageFormatValuesDescription);
  addInParameter<StringCollection>("quality vs speed", paramHelp[7], QUALITYVSSPEEDLIST, true,
                                   qualityVsSpeedValuesDescription);
  addInParameter<StringCollection>("edge length measurement", paramHelp[8],
                                   EDGELENGTHMEASUREMENTLIST, true,
                                   edgeLengthMeasurementValuesDescription);
  addInParameter<StringCollection>("allowed positions", paramHelp[9], ALLOWEDPOSITIONSLIST, true,
                                   "integer<br/>exponent<br/>all");
  addInParameter<StringCollection>("tip over", paramHelp[10], TIPOVERLIST, true,
                                   "no growing row<br/>always<br/>none");
  addInParameter<StringCollection>("presort", paramHelp[11], PRESORTLIST, true,
                                   presortValuesDescription);
  addInParameter<StringCollection>("galaxy choice", paramHelp[12], GALAXYCHOICELIST, true,
                                   galaxyChoiceValuesDescription);
  addInParameter<StringCollection>("max iterations change", paramHelp[13], MAXITERCHANGELIST, true,
                                   maxIterChangeValuesDescription);
  addInParameter<StringCollection>("initial layout", paramHelp[14], INITIALPLACEMENTMULTLIST, true,
                                   "advanced<br/>simple");
  addInParameter<StringCollection>("force model", paramHelp[15], FORCEMODELLIST, true,
                                   forceModelValuesDescription);
  addInParameter<StringCollection>("repulsive force method", paramHelp[16],
                                   REPULSIVEFORCEMETHODLIST, true, repulsiveForceValuesDescription);
  addInParameter<StringCollection>("initial layout forces", paramHelp[17],
                                   INITIALPLACEMENTFORCESLIST, true,
                                   initialPlacementValuesDescription);
  addInParameter<StringCollection>("reduced tree construction", paramHelp[18],
                                   REDUCEDTREECONSTRUCTIONLIST, true,
                                   "subtree by subtree<br/>path by path<br/>");
  addInParameter<StringCollection>("smallest cell finding", paramHelp[19], SMALLESTCELLFINDINGLIST,
                                   true, smallestCellFindingValuesDescription);
}

OGDFFm3::~OGDFFm3() {}

void OGDFFm3::beforeCall() {
  ogdf::FMMMLayout *fmmm = static_cast<ogdf::FMMMLayout *>(ogdfLayoutAlgo);

  // init seed of possible random sequence
  auto seed = tlp::getSeedOfRandomSequence();
  if (seed != UINT_MAX)
    fmmm->randSeed(seed);

  if (dataSet != nullptr) {
    double edgeLenth = 10;

    if (dataSet->get("unit edge length", edgeLenth))
      fmmm->unitEdgeLength(edgeLenth);

    bool bval = false;

    if (dataSet->get("new initial layout", bval)) {
      fmmm->newInitialPlacement(bval);
    }

    if (dataSet->get("page format", stringCollection)) {
      switch (stringCollection.getCurrent()) {
      case PORTRAIT:
        fmmm->pageFormat(FMMMOptions::PageFormatType::Portrait);
        break;
      case LANDSCAPE:
        fmmm->pageFormat(FMMMOptions::PageFormatType::Landscape);
        break;
      default:
        fmmm->pageFormat(FMMMOptions::PageFormatType::Square);
      }
    }

    if (dataSet->get("quality vs speed", stringCollection)) {
      switch (stringCollection.getCurrent()) {
      case NICEANDINCREDIBLESPEED:
        fmmm->qualityVersusSpeed(FMMMOptions::QualityVsSpeed::NiceAndIncredibleSpeed);
        break;
      case GORGEOUSANDEFFICIENT:
        fmmm->qualityVersusSpeed(FMMMOptions::QualityVsSpeed::GorgeousAndEfficient);
        break;
      default:
        fmmm->qualityVersusSpeed(FMMMOptions::QualityVsSpeed::BeautifulAndFast);
      }
    }

    // Since we choosed to expose some low level options
    // we must ensure they will be used
    fmmm->useHighLevelOptions(false);
    // and to do our own processing of high level options
    // using the same processing that the one in
    // FMMMLayout::update_low_level_options_due_to_high_level_options_settings()
    // but avoiding to reinitialize all low level options
    switch (fmmm->pageFormat()) {
    case FMMMOptions::PageFormatType::Square:
      fmmm->pageRatio(1.0);
      break;
    case FMMMOptions::PageFormatType::Landscape:
      fmmm->pageRatio(1.4142);
      break;
    case FMMMOptions::PageFormatType::Portrait:
      fmmm->pageRatio(0.7071);
    }

    if (fmmm->newInitialPlacement())
      fmmm->initialPlacementForces(FMMMOptions::InitialPlacementForces::RandomTime);
    else
      fmmm->initialPlacementForces(FMMMOptions::InitialPlacementForces::RandomRandIterNr);

    switch (fmmm->qualityVersusSpeed()) {
    case FMMMOptions::QualityVsSpeed::GorgeousAndEfficient:
      fmmm->fixedIterations(60);
      fmmm->fineTuningIterations(40);
      fmmm->nmPrecision(6);
      break;
    case FMMMOptions::QualityVsSpeed::BeautifulAndFast:
      fmmm->fixedIterations(30);
      fmmm->fineTuningIterations(20);
      fmmm->nmPrecision(4);
      break;
    case FMMMOptions::QualityVsSpeed::NiceAndIncredibleSpeed:
      fmmm->fixedIterations(15);
      fmmm->fineTuningIterations(10);
      fmmm->nmPrecision(2);
    }

    SizeProperty *size = nullptr;

    if (dataSet->get("node size", size))
      tlpToOGDF->copyTlpNodeSizeToOGDF(size);

    int ival = 0;

    if (dataSet->get("fixed iterations", ival)) {
      if (ival)
        fmmm->fixedIterations(ival);
    }

    double dval = 0;

    if (dataSet->get("threshold", dval)) {
      fmmm->threshold(dval);
    }

    if (dataSet->get("edge length measurement", stringCollection)) {
      if (stringCollection.getCurrent() == BOUNDINGCIRCLE) {
        fmmm->edgeLengthMeasurement(FMMMOptions::EdgeLengthMeasurement::BoundingCircle);
      } else {
        fmmm->edgeLengthMeasurement(FMMMOptions::EdgeLengthMeasurement::Midpoint);
      }
    }

    if (dataSet->get("allowed positions", stringCollection)) {
      switch (stringCollection.getCurrent()) {
      case INTEGER:
        fmmm->allowedPositions(FMMMOptions::AllowedPositions::Integer);
        break;
      case EXPONENT:
        fmmm->allowedPositions(FMMMOptions::AllowedPositions::Exponent);
        break;
      default:
        fmmm->allowedPositions(FMMMOptions::AllowedPositions::All);
      }
    }

    if (dataSet->get("tip over", stringCollection)) {
      switch (stringCollection.getCurrent()) {
      case NONE:
        fmmm->tipOverCCs(FMMMOptions::TipOver::None);
        break;
      case NOGROWINGROW:
        fmmm->tipOverCCs(FMMMOptions::TipOver::NoGrowingRow);
        break;
      default:
        fmmm->tipOverCCs(FMMMOptions::TipOver::Always);
      }
    }

    if (dataSet->get("presort", stringCollection)) {
      if (stringCollection.getCurrent() == NONE) {
        fmmm->presortCCs(FMMMOptions::PreSort::None);
      } else if (stringCollection.getCurrent() == DECREASINGHEIGHT) {
        fmmm->presortCCs(FMMMOptions::PreSort::DecreasingHeight);
      } else {
        fmmm->presortCCs(FMMMOptions::PreSort::DecreasingWidth);
      }
    }

    if (dataSet->get("galaxy choice", stringCollection)) {
      switch (stringCollection.getCurrent()) {
      case UNIFORMPROB:
        fmmm->galaxyChoice(FMMMOptions::GalaxyChoice::UniformProb);
        break;
      case NONUNIFORMPROBLOWERMASS:
        fmmm->galaxyChoice(FMMMOptions::GalaxyChoice::NonUniformProbLowerMass);
        break;
      default:
        fmmm->galaxyChoice(FMMMOptions::GalaxyChoice::NonUniformProbHigherMass);
      }
    }

    if (dataSet->get("max iterations change", stringCollection)) {
      switch (stringCollection.getCurrent()) {
      case CONSTANT:
        fmmm->maxIterChange(FMMMOptions::MaxIterChange::Constant);
        break;
      case LINEARLYDECREASING:
        fmmm->maxIterChange(FMMMOptions::MaxIterChange::LinearlyDecreasing);
        break;
      default:
        fmmm->maxIterChange(FMMMOptions::MaxIterChange::RapidlyDecreasing);
      }
    }

    if (dataSet->get("initial layout", stringCollection)) {
      if (stringCollection.getCurrent() == ADVANCED) {
        fmmm->initialPlacementMult(FMMMOptions::InitialPlacementMult::Advanced);
      } else {
        fmmm->initialPlacementMult(FMMMOptions::InitialPlacementMult::Simple);
      }
    }

    if (dataSet->get("force model", stringCollection)) {
      switch (stringCollection.getCurrent()) {
      case FRUCHTERMANNREINGOLD:
        fmmm->forceModel(FMMMOptions::ForceModel::FruchtermanReingold);
        break;
      case EADES:
        fmmm->forceModel(FMMMOptions::ForceModel::Eades);
        break;
      default:
        fmmm->forceModel(FMMMOptions::ForceModel::New);
      }
    }

    if (dataSet->get("repulsive force method", stringCollection)) {
      switch (stringCollection.getCurrent()) {
      case EXACT:
        fmmm->repulsiveForcesCalculation(FMMMOptions::RepulsiveForcesMethod::Exact);
        break;
      case GRIDAPPROXIMATION:
        fmmm->repulsiveForcesCalculation(FMMMOptions::RepulsiveForcesMethod::GridApproximation);
        break;
      default:
        fmmm->repulsiveForcesCalculation(FMMMOptions::RepulsiveForcesMethod::NMM);
      }
    }

    if (dataSet->get("initial layout forces", stringCollection)) {
      auto current = stringCollection.getCurrent();
      if (current != 0) {
        switch (current) {
        case UNIFORMGRID:
          fmmm->initialPlacementForces(FMMMOptions::InitialPlacementForces::UniformGrid);
          break;
        case RANDOMTIME:
          fmmm->initialPlacementForces(FMMMOptions::InitialPlacementForces::RandomTime);
          break;
        case RANDOMRANDITERNR:
          fmmm->initialPlacementForces(FMMMOptions::InitialPlacementForces::RandomRandIterNr);
          break;
        case KEEPPOSITIONS:
          fmmm->initialPlacementForces(FMMMOptions::InitialPlacementForces::KeepPositions);
        default:
          break;
        }
      }
    }

    if (dataSet->get("reduced tree construction", stringCollection)) {
      if (stringCollection.getCurrent() == SUBTREEBYSUBTREE) {
        fmmm->nmTreeConstruction(FMMMOptions::ReducedTreeConstruction::SubtreeBySubtree);
      } else {
        fmmm->nmTreeConstruction(FMMMOptions::ReducedTreeConstruction::PathByPath);
      }
    }

    if (dataSet->get("smallest cell finding", stringCollection)) {
      if (stringCollection.getCurrent() == ITERATIVELY) {
        fmmm->nmSmallCell(FMMMOptions::SmallestCellFinding::Iteratively);
      } else {
        fmmm->nmSmallCell(FMMMOptions::SmallestCellFinding::Aluru);
      }
    }
  }
}

void OGDFFm3::callOGDFLayoutAlgorithm(ogdf::GraphAttributes &gAttributes) {

  FMMMLayout *fmmm = static_cast<ogdf::FMMMLayout *>(ogdfLayoutAlgo);

  NumericProperty *length = nullptr;

  if (dataSet->get("edge length property", length) && length) {
    EdgeArray<double> edgeLength(tlpToOGDF->getOGDFGraph());

    const std::vector<tlp::edge> &edges = graph->edges();
    unsigned int nbEdges = edges.size();

    for (unsigned int i = 0; i < nbEdges; ++i) {
      edgeLength[tlpToOGDF->getOGDFGraphEdge(i)] = length->getEdgeDoubleValue(edges[i]);
    }

    fmmm->call(gAttributes, edgeLength);
  } else {
    OGDFLayoutPluginBase::callOGDFLayoutAlgorithm(gAttributes);
  }
}
