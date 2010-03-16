#include "tulip/GlGraphInputData.h"

#include <tulip/Graph.h>
#include <tulip/DrawingTools.h>

#include "tulip/GlyphManager.h"
#include <tulip/EdgeExtremityGlyphManager.h>
#include "tulip/GlGraphRenderingParameters.h"

namespace tlp {

  // define some specific MetaValueCalculator classes
  // viewColor
  class ViewColorCalculator :public AbstractColorProperty::MetaValueCalculator {
  public:
    virtual void computeMetaValue(AbstractColorProperty* color, node mN, Graph* sg) {
      // meta node color is half opaque white
      color->setNodeValue(mN, Color(255, 255, 255, 127));
    }

    virtual void computeMetaValue(AbstractColorProperty* color, edge mE,
				  Iterator<edge>*itE, Graph* g) {
      // meta edge color is the color of the first underlying edge
      color->setEdgeValue(mE, color->getEdgeValue(itE->next()));
    }
  };

  // viewLabel
  class ViewLabelCalculator :public AbstractStringProperty::MetaValueCalculator {
  public:
    // set the meta node label to label of viewMetric max corresponding node
    void computeMetaValue(AbstractStringProperty* label,
			  node mN, Graph* sg) {
      // nothing to do if viewMetric does not exist
      if (!sg->existProperty("viewMetric"))
	return;
      node viewMetricMaxNode;
      double vMax = -DBL_MAX;
      DoubleProperty *metric = sg->getProperty<DoubleProperty>("viewMetric");
      Iterator<node> *itN= sg->getNodes();
      while (itN->hasNext()){
	node itn = itN->next();
	const double& value = metric->getNodeValue(itn);
	if (value > vMax) {
	  vMax = value;
	  viewMetricMaxNode = itn;
	}
      } delete itN;
      label->setNodeValue(mN, label->getNodeValue(viewMetricMaxNode));
    }
  };

  // viewLayout
  class ViewLayoutCalculator :public AbstractLayoutProperty::MetaValueCalculator {
  public:
    void computeMetaValue(AbstractLayoutProperty* layout,
			  node mN, Graph* sg) {
      Graph* super = sg->getSuperGraph();
      SizeProperty* size = super->getProperty<SizeProperty>("viewSize");
      DoubleProperty* rot = super->getProperty<DoubleProperty>("viewRotation");

      std::pair<Coord, Coord> box =
	tlp::computeBoundingBox(sg, (LayoutProperty *) layout, size, rot);
      Coord maxL = box.first;
      Coord minL = box.second;
      layout->setNodeValue(mN, (maxL + minL) / 2.0 );
      Coord v = (maxL - minL);
      if (v[2] < 0.0001) v[2] = 0.1;
      size->setNodeValue(mN, Size(v[0],v[1],v[2]));
    }
  };

  // corresponding static instances
  static ViewColorCalculator vColorCalc;
  static ViewLabelCalculator vLabelCalc;
  static ViewLayoutCalculator vLayoutCalc;

  GlGraphInputData::GlGraphInputData(Graph* graph,GlGraphRenderingParameters* parameters,GlMetaNodeRenderer *renderer):
    elementColorPropName("viewColor"), elementLabelColorPropName("viewLabelColor"), elementSizePropName("viewSize"),
    elementLabelPositionPropName("viewLabelPosition"),elementShapePropName("viewShape"), elementRotationPropName("viewRotation"),
    elementSelectedPropName("viewSelection"),elementFontPropName("viewFont"),elementFontSizePropName("viewFontSize"),
    elementLabelPropName("viewLabel"), elementTexturePropName("viewTexture"),
    elementBorderColorPropName("viewBorderColor"), elementBorderWidthPropName("viewBorderWidth"), elementLayoutPropName(""),
    elementSrcAnchorShapePropName("viewSrcAnchorShape"),elementSrcAnchorSizePropName("viewSrcAnchorSize"),
    elementTgtAnchorShapePropName("viewTgtAnchorShape"),elementTgtAnchorSizePropName("viewTgtAnchorSize"), graph(graph),
    parameters(parameters) {

    reloadAllProperties();
	GlyphManager::getInst().initGlyphList(&this->graph, this, glyphs);

	EdgeExtremityGlyphManager::getInst().initGlyphList(&this->graph, this,
			extremityGlyphs);
    if(renderer)
      metaNodeRenderer=renderer;
    else
      metaNodeRenderer=new GlMetaNodeRenderer();
}

GlGraphInputData::~GlGraphInputData() {
	GlyphManager::getInst().clearGlyphList(&this->graph, this, glyphs);
	EdgeExtremityGlyphManager::getInst().clearGlyphList(&this->graph, this,
			extremityGlyphs);
}

void GlGraphInputData::reloadLayoutProperty() {
    if(!graph->attributeExist("viewLayout")){
      if(elementLayoutPropName==""){
	elementLayout = graph->getProperty<LayoutProperty>("viewLayout");
	elementLayout->setMetaValueCalculator(&vLayoutCalc);
      }else{
	elementLayout = graph->getProperty<LayoutProperty>(elementLayoutPropName);
      }
    }else{
      graph->getAttribute("viewLayout",elementLayout);
	}
}

void GlGraphInputData::reloadAllProperties() {
	reloadLayoutProperty();

	elementRotation = graph->getProperty<DoubleProperty> (
			elementRotationPropName);
    elementFont = graph->getProperty<StringProperty>(elementFontPropName);
    elementFontSize = graph->getProperty<IntegerProperty>(elementFontSizePropName);
	elementSelected = graph->getProperty<BooleanProperty> (
			elementSelectedPropName);
	elementLabel = graph->getProperty<StringProperty> (elementLabelPropName);
	elementLabel->setMetaValueCalculator(&vLabelCalc);
    	elementLabelColor = graph->getProperty<ColorProperty> (
			elementLabelColorPropName);
	elementLabelPosition = graph->getProperty<IntegerProperty> (
			elementLabelPositionPropName);
	elementColor = graph->getProperty<ColorProperty> (elementColorPropName);
	elementColor->setMetaValueCalculator(&vColorCalc);
	elementShape = graph->getProperty<IntegerProperty> (elementShapePropName);
	elementSize = graph->getProperty<SizeProperty> (elementSizePropName);
	elementTexture
			= graph->getProperty<StringProperty> (elementTexturePropName);
	elementBorderColor = graph->getProperty<ColorProperty> (
			elementBorderColorPropName);
	elementBorderWidth = graph->getProperty<DoubleProperty> (
			elementBorderWidthPropName);

	elementSrcAnchorShape = graph->getProperty<IntegerProperty> (
			elementSrcAnchorShapePropName);
	elementSrcAnchorSize = graph->getProperty<SizeProperty> (
			elementSrcAnchorSizePropName);
	elementTgtAnchorShape = graph->getProperty<IntegerProperty> (
			elementTgtAnchorShapePropName);
	elementTgtAnchorSize = graph->getProperty<SizeProperty> (
			elementTgtAnchorSizePropName);
}

}
