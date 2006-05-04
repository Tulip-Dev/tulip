#include "tulip/PropertyManager.h"

//================================================================================
template<typename ATTRIBUTETYPE>
ATTRIBUTETYPE Graph::getAttribute(const std::string &name) {
  DataSet &data=getAttributes();
  ATTRIBUTETYPE tmp;
  data.get(name,tmp);
  return tmp;
}
//================================================================================
template<typename ATTRIBUTETYPE>
void Graph::setAttribute(const std::string &name,const ATTRIBUTETYPE&value) {
  DataSet &data=getAttributes();
  data.set(name,value);
}
//================================================================================
template<typename Proxytype>
Proxytype* Graph::getLocalProperty(const std::string &name) { 
  Proxytype *tmp;
  if (existLocalProperty(name)) {
    PropertyInterface* tmpProxy=getProperty(name);
    assert ( typeid((*tmpProxy)) == typeid(Proxytype) );
    return ( (Proxytype *)(tmpProxy) );
  }
  else {
    PropertyManager *proxyManager=this->getPropertyManager();
    proxyManager->setLocalProxy(name,tmp=new Proxytype(this));
  }
  return (tmp);
}
//====================================================================================
template<typename Proxytype>
Proxytype* Graph::getProperty(const std::string &name) {
  if (existProperty(name)) {
    PropertyInterface* tmpProxy=getProperty(name);
    assert ( typeid((*tmpProxy)) == typeid(Proxytype) );
    return ( (Proxytype *)(tmpProxy) );
  }
  else {
    return getLocalProperty<Proxytype>(name);
  }
}
//====================================================================================
template<typename Proxytype>
bool Graph::computeProperty(const std::string &algorithm, Proxytype result, std::string &msg, 
				 PluginProgress *progress, DataSet *data) {
  bool resultBool;
  PropertyContext context;

  PluginProgress *tmpProgress;
  if (progress==0) 
    tmpProgress=new PluginProgress();
  else 
    tmpProgress=progress;

  context.pluginProgress = tmpProgress;
  context.graph = this;
  context.dataSet = data;
  
  resultBool = result->compute(algorithm, msg, context);
  if (progress==0) delete tmpProgress;
  
  return resultBool;
}
//====================================================================================
