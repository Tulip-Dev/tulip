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

  void openGraphOnGlMainWidget(Graph *graph,DataSet *dataSet,GlMainWidget *glMainWidget){
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
    glMainWidget->getScene()->addLayer(layer);
    glMainWidget->getScene()->addGlGraphCompositeInfo(layer, glGraphComposite);
  }

}
