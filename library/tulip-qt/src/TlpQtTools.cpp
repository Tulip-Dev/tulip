#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/TlpQtTools.h"

#include <tulip/Reflect.h>


/**
 * For openDataSetDialog function : see OpenDataSet.cpp
 */

using namespace std;


namespace tlp {

  void openGraphOnGlGraphWidget(Graph *graph,DataSet *dataSet,GlGraphWidget *glGraphWidget){
    GlGraphComposite* glGraphComposite = new GlGraphComposite(graph);
    GlGraphRenderingParameters param =
      glGraphComposite->getRenderingParameters();
    DataSet glGraphData;
    if (dataSet->get<DataSet>("displaying", glGraphData)) {
      param.setParameters(glGraphData);
      glGraphComposite->setRenderingParameters(param);
    }
    GlLayer *layer = new GlLayer("Main");
    layer->addGlEntity(glGraphComposite, "graph");
    glGraphWidget->getScene()->addLayer(layer);
    glGraphWidget->getScene()->addGlGraphCompositeInfo(layer, glGraphComposite);
  }

}
