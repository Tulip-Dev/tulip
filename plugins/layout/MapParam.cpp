#include <cmath>
#include <LayoutProperty.h>
#include <tulip/TlpTools.h>
#include <tulip/DoubleProperty.h>

struct MapParam: public LayoutAlgorithm {

  MapParam(const PropertyContext &context):Layout(context){}
  ~MapParam(){}
  bool run () {
    bool cached,errresult;
    string errmsg;
    Metric *p1=getLocalProperty<DoubleProperty>(graph,"Spreading Interpolation",cached,errresult,errmsg,pluginProgress);
    Metric *p2=getLocalProperty<DoubleProperty>(graph,"StrahlerAllGeneral",cached,errresult,errmsg,pluginProgress);
    Metric *p3=getLocalProperty<DoubleProperty>(graph,"Arity",cached,errresult,errmsg,pluginProgress);

    Iterator<node> *itN=graph->getNodes();
    for (;itN->hasNext();) {
      node itn=itN->next();
      double max;
      if (p1->getNodeMax()>p2->getNodeMax()) max=p1->getNodeMax(); else max=p2->getNodeMax();
      if (p3->getNodeMax()>max) max=p3->getNodeMax();
      layoutResult->setNodeValue(itn,Coord(p1->getNodeValue(itn)*1024,p2->getNodeValue(itn)*1024,p3->getNodeValue(itn)*1024));
    }delete itN;
    return true;
  }
};

DOUBLEPLUGIN(MapParam,"MapParam","David Auber","18/06/2002","Ok","0","1")
