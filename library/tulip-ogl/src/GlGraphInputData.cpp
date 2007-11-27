#include "tulip/GlGraphInputData.h"

#include <tulip/Graph.h>

#include <tulip/LayoutProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/GraphProperty.h>

#include "tulip/GlyphManager.h"

namespace tlp {
  
  GlGraphInputData::GlGraphInputData(Graph* graph,GlGraphRenderingParameters* parameters):graph(graph),parameters(parameters) {
    elementRotation = graph->getProperty<DoubleProperty>("viewRotation");
    elementSelected = graph->getProperty<BooleanProperty>("viewSelection");
    elementLabel = graph->getProperty<StringProperty>("viewLabel");
    elementLabelColor = graph->getProperty<ColorProperty>("viewLabelColor");
    elementLabelPosition = graph->getProperty<IntegerProperty>("viewLabelPosition");
    elementColor = graph->getProperty<ColorProperty>("viewColor");
    elementShape = graph->getProperty<IntegerProperty>("viewShape");
    elementSize = graph->getProperty<SizeProperty>("viewSize");
    elementLayout = graph->getProperty<LayoutProperty>(parameters->getInputLayout() );
    elementGraph = graph->getProperty<GraphProperty>("viewMetaGraph");
    elementTexture = graph->getProperty<StringProperty>("viewTexture");
    elementBorderColor = graph->getProperty<ColorProperty>("viewBorderColor");
    elementBorderWidth = graph->getProperty<DoubleProperty>("viewBorderWidth");

    GlyphManager::getInst().initGlyphList(&this->graph,this,glyphs);
  }
  
}
