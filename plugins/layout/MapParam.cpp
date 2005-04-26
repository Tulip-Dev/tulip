#include <cmath>
#include <Layout.h>
#include <tulip/TlpTools.h>
#include <tulip/MetricProxy.h>

struct MapParam: public Layout {

  MapParam(const PropertyContext &context):Layout(context){}
  ~MapParam(){}
  bool run () {
    bool cached,errresult;
    string errmsg;
    MetricProxy *p1=getLocalProperty<MetricProxy>(superGraph,"Spreading Interpolation",cached,errresult,errmsg,pluginProgress);
    MetricProxy *p2=getLocalProperty<MetricProxy>(superGraph,"StrahlerAllGeneral",cached,errresult,errmsg,pluginProgress);
    MetricProxy *p3=getLocalProperty<MetricProxy>(superGraph,"Arity",cached,errresult,errmsg,pluginProgress);

    Iterator<node> *itN=superGraph->getNodes();
    for (;itN->hasNext();) {
      node itn=itN->next();
      double max;
      if (p1->getNodeMax()>p2->getNodeMax()) max=p1->getNodeMax(); else max=p2->getNodeMax();
      if (p3->getNodeMax()>max) max=p3->getNodeMax();
      layoutProxy->setNodeValue(itn,Coord(p1->getNodeValue(itn)*1024,p2->getNodeValue(itn)*1024,p3->getNodeValue(itn)*1024));
    }delete itN;
    return true;
  }
};

METRICPLUGIN(MapParam,"MapParam","David Auber","18/06/2002","Ok","0","1")
