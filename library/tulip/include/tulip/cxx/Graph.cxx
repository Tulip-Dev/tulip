
//================================================================================
template<typename ATTRIBUTETYPE>
ATTRIBUTETYPE tlp::Graph::getAttribute(const std::string &name) {
  tlp::DataSet &data=getAttributes();
  ATTRIBUTETYPE tmp;
  data.get(name,tmp);
  return tmp;
}
//================================================================================
template<typename ATTRIBUTETYPE>
bool tlp::Graph::getAttribute(const std::string &name, ATTRIBUTETYPE& value) {
  return getAttributes().get(name, value);
}
//================================================================================
template<typename ATTRIBUTETYPE>
void tlp::Graph::setAttribute(const std::string &name,const ATTRIBUTETYPE&value) {
  tlp::DataSet &data=getAttributes();
  data.set(name,value);
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
    tmp=new Proxytype(this);
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
template<typename Proxytype>
bool tlp::Graph::computeProperty(const std::string &algorithm, Proxytype result, std::string &msg, 
				 tlp::PluginProgress *progress, tlp::DataSet *data) {
  bool resultBool;
  tlp::PropertyContext context;

  tlp::PluginProgress *tmpProgress;
  if (progress==0) 
    tmpProgress=new tlp::PluginProgress();
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
