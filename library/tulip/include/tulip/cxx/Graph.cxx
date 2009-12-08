
//================================================================================
template<typename ATTRIBUTETYPE>
ATTRIBUTETYPE tlp::Graph::getAttribute(const std::string &name) const {
  const tlp::DataSet &data=getAttributes();
  ATTRIBUTETYPE tmp;
  data.get(name,tmp);
  return tmp;
}
//================================================================================
template<typename ATTRIBUTETYPE>
bool tlp::Graph::getAttribute(const std::string &name, ATTRIBUTETYPE& value) const{
  return getAttributes().get(name, value);
}
//================================================================================
template<typename ATTRIBUTETYPE>
void tlp::Graph::setAttribute(const std::string &name,const ATTRIBUTETYPE&value) {
  tlp::DataSet &data=getNonConstAttributes();
  notifyBeforeSetAttribute(this, name);
  data.set(name,value);
  notifyAfterSetAttribute(this, name);
}
//================================================================================
template<typename Proxytype>
Proxytype* tlp::Graph::getLocalProperty(const std::string &name) { 
  Proxytype *tmp;
  if (existLocalProperty(name)) {
    tlp::PropertyInterface* tmpProxy=getProperty(name);
    assert ( typeid((*tmpProxy)) == typeid(Proxytype) );
    return ( (Proxytype *)(tmpProxy) );
  }
  else {
    tmp=new Proxytype(this, name);
    this->addLocalProperty(name, tmp);
  }
  return (tmp);
}
//====================================================================================
template<typename Proxytype>
Proxytype* tlp::Graph::getProperty(const std::string &name) {
  if (existProperty(name)) {
    tlp::PropertyInterface* tmpProxy=getProperty(name);
    assert ( typeid((*tmpProxy)) == typeid(Proxytype) );
    return ( (Proxytype *)(tmpProxy) );
  }
  else {
    return getLocalProperty<Proxytype>(name);
  }
}
//====================================================================================
template<typename PropertyType>
bool tlp::Graph::computeProperty(const std::string &algorithm, PropertyType* prop,
				 std::string &msg,  tlp::PluginProgress *progress,
				 tlp::DataSet *data) {
  bool result;
  tlp::PropertyContext context;

  // check if this is a subgraph of prop->graph
  if (getRoot() != prop->graph) {
    tlp::Graph *currentGraph = this;
    while(currentGraph->getSuperGraph() != currentGraph) {
      if (currentGraph == prop->graph)
	break;
      currentGraph = currentGraph->getSuperGraph();
    }
    if (currentGraph != prop->graph)
      return false;
  }    

#ifndef NDEBUG
  std::cerr << __PRETTY_FUNCTION__ << std::endl;
#endif
  if(circularCalls.find(prop) != circularCalls.end()) {
#ifndef NDEBUG
    std::cerr << "Circular call of " << __PRETTY_FUNCTION__ << " " << msg << std::endl;
#endif
    return false;
  }

  // nothing to do if the graph is empty
  if (numberOfNodes() == 0) {
    msg= "The graph is empty";
    return false;
  }

  tlp::PluginProgress *tmpProgress;
  if (progress==0) 
    tmpProgress=new tlp::PluginProgress();
  else 
    tmpProgress=progress;

  context.pluginProgress = tmpProgress;
  context.graph = this;
  context.dataSet = data;
  
  tlp::Observable::holdObservers();
  circularCalls.insert(prop);
  tlp::PropertyContext tmpContext(context);
  tmpContext.propertyProxy = prop;
  typename PropertyType::PAlgorithm *tmpAlgo =
    PropertyType::factory->getPluginObject(algorithm, tmpContext);
  if (tmpAlgo != 0) {
    result = tmpAlgo->check(msg);
    if (result) {
      tmpAlgo->run();
    }
    delete tmpAlgo;
  }
  else {
    msg = "No algorithm available with this name";
    result=false;
  }
  circularCalls.erase(prop);
  notifyObservers();
  tlp::Observable::unholdObservers();

  if (progress==0) delete tmpProgress;
  
  return result;
}
//====================================================================================
