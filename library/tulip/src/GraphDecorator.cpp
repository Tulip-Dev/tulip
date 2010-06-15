#include <tulip/GraphDecorator.h>

using namespace tlp;

//============================================================
void GraphDecorator::clear(){
  graph_component->clear();
}

//============================================================
Graph *GraphDecorator::addSubGraph(BooleanProperty *selection,
				   unsigned int id){
  Graph* sg = graph_component->addSubGraph(selection, id);
  notifyAddSubGraph(sg);
  return sg;
}

//============================================================
void GraphDecorator::delSubGraph(Graph *s){
  notifyDelSubGraph(this, s);
  graph_component->delSubGraph(s);
  notifyObservers();
}

//============================================================
void GraphDecorator::restoreSubGraph(Graph *, bool){
  std::cerr << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation" << std::endl;
}

//============================================================
void GraphDecorator::setSubGraphToKeep(Graph * s){
  std::cerr << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation" << std::endl;
}

//============================================================
void GraphDecorator::removeSubGraph(Graph*, bool){
  std::cerr << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation" << std::endl;
}

//============================================================
void GraphDecorator::clearSubGraphs(){
  std::cerr << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation" << std::endl;
}

//============================================================
void GraphDecorator::delAllSubGraphs(Graph *s) {
  notifyDelSubGraph(this, s);
  graph_component->delAllSubGraphs(s);
  notifyObservers();
}

//============================================================
Graph* GraphDecorator::getSuperGraph()const{
  return graph_component->getSuperGraph();
}

//============================================================
void GraphDecorator::setSuperGraph(Graph* s){
  graph_component->setSuperGraph(s);
}

//============================================================
Graph* GraphDecorator::getRoot() const{
  return graph_component->getRoot();
}

//============================================================
Iterator<Graph *> * GraphDecorator::getSubGraphs() const{
  return graph_component->getSubGraphs();
}

//=========================================================================
bool GraphDecorator::isSubGraph(Graph* sg) const {
  return graph_component->isSubGraph(sg);
}
//=========================================================================
bool GraphDecorator::isDescendantGraph(Graph* sg) const {
  return graph_component->isDescendantGraph(sg);
}
//=========================================================================
Graph* GraphDecorator::getSubGraph(unsigned int sgId) const {
  return graph_component->getSubGraph(sgId);
}
//=========================================================================
Graph* GraphDecorator::getDescendantGraph(unsigned int sgId) const {
  return graph_component->getDescendantGraph(sgId);
}

//============================================================
node GraphDecorator::source(const edge e)const{
  return graph_component->source(e);
}

//============================================================
node GraphDecorator::target(const edge e)const {
  return graph_component->target(e);
}

//============================================================
node GraphDecorator::opposite(const edge e, const node n)const {
  return graph_component->opposite(e,n);
}

//============================================================
edge GraphDecorator::existEdge(const node n, const node m,
			       bool directed) const {
  return graph_component->existEdge(n, m, directed);
}

//============================================================
const std::pair<node, node>& GraphDecorator::ends(const edge e) const {
  return graph_component->ends(e);
}
//============================================================
void GraphDecorator::reverse(const edge e){
  notifyReverseEdge(this, e);
  graph_component->reverse(e);
}

//============================================================
unsigned int GraphDecorator::deg(const node n)const {
  return graph_component->deg(n);
}

//============================================================
unsigned int GraphDecorator::indeg(const node n)const {
  return graph_component->indeg(n);
}

//============================================================
unsigned int GraphDecorator::outdeg(const node n)const {
  return graph_component->outdeg(n);
}

//============================================================
Graph* GraphDecorator::getNodeMetaInfo(const node n)const {
  return graph_component->getNodeMetaInfo(n);
}

//============================================================
node GraphDecorator::getOneNode() const{
  return graph_component->getOneNode();
}

//============================================================
node GraphDecorator::getInNode(const node n,unsigned int i)const{
  return graph_component->getInNode(n,i);
}

//============================================================
node GraphDecorator::getOutNode(const node n,unsigned int i) const{
  return graph_component->getOutNode(n,i);
}

edge GraphDecorator::getOneEdge() const{
  return graph_component->getOneEdge();
}

//============================================================
unsigned int GraphDecorator::numberOfNodes()const{
  return graph_component->numberOfNodes();
}

//============================================================
unsigned int GraphDecorator::numberOfEdges()const{
  return graph_component->numberOfEdges();
}

//============================================================
bool GraphDecorator::isElement(const node n) const{
  return graph_component->isElement(n);
}

//============================================================
bool GraphDecorator::isMetaNode(const node n) const{
  return graph_component->isMetaNode(n);
}

//============================================================
bool GraphDecorator::isElement(const edge e) const{
  return graph_component->isElement(e);
}

//============================================================
bool GraphDecorator::isMetaEdge(const edge e) const{
  return graph_component->isMetaEdge(e);
}

//============================================================
node GraphDecorator::addNode(){
  node newNode = graph_component->addNode();
  notifyAddNode(this, newNode);
  notifyObservers();
  return newNode;
}

//============================================================
void GraphDecorator::addNode(const node n){
  std::cerr << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation" << std::endl;
}

//============================================================
node GraphDecorator::restoreNode(node n){
  std::cerr << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation" << std::endl;
  return n;
}

//============================================================
void GraphDecorator::removeNode(const node){
  std::cerr << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation" << std::endl;
}

//============================================================
edge GraphDecorator::addEdge(const node n, const node n2){
  edge newEdge = graph_component->addEdge(n, n2);
  notifyAddEdge(this, newEdge);
  notifyObservers();
  return newEdge;
}

//============================================================
void GraphDecorator::addEdge(const edge e){
  std::cerr << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation" << std::endl;
}

//============================================================
edge GraphDecorator::restoreEdge(edge e, node source, node target){
  std::cerr << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation" << std::endl;
  return e;
}

//============================================================
void GraphDecorator::removeEdge(const edge, const node){
  std::cerr << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation" << std::endl;
}

//============================================================
void GraphDecorator::delNode(const node n){
  notifyDelNode(this, n);
  graph_component->delNode(n);
  notifyObservers();
}

//============================================================ 
void GraphDecorator::delAllNode(const node n){
  notifyDelNode(this, n);
  graph_component->delAllNode(n);
  notifyObservers();
}

//============================================================
void GraphDecorator::delEdge(const edge e){
  notifyDelEdge(this,e);
  graph_component->delEdge(e);
  notifyObservers();
}

//============================================================
void GraphDecorator::delAllEdge(const edge e){
  notifyDelEdge(this,e);
  graph_component->delAllEdge(e);
  notifyObservers();
}

//============================================================
void GraphDecorator::setEdgeOrder(const node n,const std::vector<edge> & s){
  graph_component->setEdgeOrder(n,s);
}

void GraphDecorator::swapEdgeOrder(const node n ,const edge e1, const edge e2){
  graph_component->swapEdgeOrder(n, e1, e2);
} 

//============================================================
Iterator<node>* GraphDecorator::getNodes() const {
  return graph_component->getNodes();
}

//============================================================
Iterator<node>* GraphDecorator::getInNodes(const node n) const {
  return graph_component->getInNodes(n);
}

//============================================================
Iterator<node>* GraphDecorator::getOutNodes(const node n) const {
  return graph_component->getOutNodes(n);
}

//============================================================
Iterator<node>* GraphDecorator::getInOutNodes(const node n) const {
  return graph_component->getInOutNodes(n);
}

//============================================================
Iterator<edge>* GraphDecorator::getEdges() const {
  return graph_component->getEdges();
}

//============================================================
Iterator<edge>* GraphDecorator::getOutEdges(const node n) const {
  return graph_component->getOutEdges(n);
}

//============================================================
Iterator<edge>* GraphDecorator::getInOutEdges(const node n) const {
  return graph_component->getInOutEdges(n);
}

//============================================================
Iterator<edge>* GraphDecorator::getInEdges(const node n) const {
  return graph_component->getInEdges(n);
}

//============================================================
Iterator<edge>* GraphDecorator::getEdgeMetaInfo(const edge e) const {
  return graph_component->getEdgeMetaInfo(e);
}

//============================================================
DataSet& GraphDecorator::getNonConstAttributes(){
  return graph_component->getNonConstAttributes();
}
//============================================================
PropertyInterface* GraphDecorator::getProperty(const std::string &name){
  return graph_component->getProperty(name);
}

//============================================================ 
bool GraphDecorator::existProperty(const std::string&name){
  return graph_component->existProperty(name);
}

//============================================================
bool GraphDecorator::existLocalProperty(const std::string&name){
  return graph_component->existLocalProperty(name) ;
}

//============================================================
void GraphDecorator::delLocalProperty(const std::string&name){
  notifyDelLocalProperty(this, name);
  graph_component->delLocalProperty(name);
  notifyObservers();
}

//============================================================
void GraphDecorator::addLocalProperty(const std::string &name, PropertyInterface *prop) {
  graph_component->addLocalProperty(name, prop);
  notifyAddLocalProperty(this, name);
  notifyObservers();
}

//============================================================
Iterator<std::string>* GraphDecorator::getLocalProperties(){
  return graph_component->getLocalProperties();
}

//============================================================
Iterator<std::string>* GraphDecorator::getInheritedProperties(){
  return graph_component->getInheritedProperties();
}

//============================================================
Iterator<std::string>* GraphDecorator::getProperties(){
  return graph_component->getProperties();
}
//============================================================
Iterator<PropertyInterface*>* GraphDecorator::getLocalObjectProperties(){
  return graph_component->getLocalObjectProperties();
}

//============================================================
Iterator<PropertyInterface*>* GraphDecorator::getInheritedObjectProperties(){
  return graph_component->getInheritedObjectProperties();
}

//============================================================
Iterator<PropertyInterface*>* GraphDecorator::getObjectProperties(){
  return graph_component->getObjectProperties();
}
//----------------------------------------------------------------
bool GraphDecorator::canPop() {
  return graph_component->canPop();
}
bool GraphDecorator::canUnpop() {
  return graph_component->canUnpop();
}
//----------------------------------------------------------------
void GraphDecorator::push(bool unpopAllowed) {
  return graph_component->push(unpopAllowed);
}
//----------------------------------------------------------------
void GraphDecorator::pop(bool unpopAllowed) {
  return graph_component->pop(unpopAllowed);
}
//----------------------------------------------------------------
bool GraphDecorator::nextPopKeepPropertyUpdates(PropertyInterface* prop) {
  return graph_component->nextPopKeepPropertyUpdates(prop);
}
//----------------------------------------------------------------
void GraphDecorator::unpop() {
  return graph_component->unpop();
}
