#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/LayoutProperty.h>
#include <tulip/Vector.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/TlpTools.h>

using namespace tlp;

GlGraphRenderingParameters::GlGraphRenderingParameters() :
  //_graph(0),
  _antialiased(true),
  _viewArrow(false),
  _viewNodeLabel(true),
  _viewEdgeLabel(false),
  _viewMetaLabel(false),
  _elementOrdered(false),
  _incrementalRendering(true),
  _edgeColorInterpolate(false),
  _edge3D(false),
  _edgeSizeInterpolate(false),
  _displayEdges(true),
  _displayNodes(true),
  _displayMetaNodes(true),
  _selectedNodesStencil(0x0002),
  _selectedMetaNodesStencil(0x0002),
  _selectedEdgesStencil(0x0002),
  _nodesStencil(0xFFFF),
  _metaNodesStencil(0xFFFF),
  _edgesStencil(0xFFFF),
  _nodesLabelStencil(0xFFFF),
  _metaNodesLabelStencil(0xFFFF),
  _edgesLabelStencil(0xFFFF),
  _FontsType(1),
  _labelsBorder(2),
  _layoutName("viewLayout") {
  _fontsPath = tlp::TulipLibDir + "tlp/bitmaps/";
  _texturePath = "";
  _feedbackRender=false;
}
//This function should rewriten completly
DataSet GlGraphRenderingParameters::getParameters() const {
  DataSet data;
  //bool
  data.set("antialiased", _antialiased);
  data.set("arrow", _viewArrow);
  data.set("displayNodes", _displayNodes);
  data.set("displayEdges", _displayEdges);
  data.set("displayMetaNodes", _displayMetaNodes);
  data.set("nodeLabel", _viewNodeLabel);
  data.set("edgeLabel", _viewEdgeLabel);
  data.set("metaLabel", _viewMetaLabel);
  data.set("elementOrdered", _elementOrdered);
  data.set("autoScale", _viewAutoScale);
  data.set("incrementalRendering", _incrementalRendering);
  data.set("edgeColorInterpolation", _edgeColorInterpolate);
  data.set("edgeSizeInterpolation", _edgeSizeInterpolate);
  data.set("edge3D", _edge3D);
  //unsigned int
  data.set("fontType", _FontsType);
  //int
  data.set("selectedNodesStencil", _selectedNodesStencil);
  data.set("selectedMetaNodesStencil", _selectedMetaNodesStencil);
  data.set("selectedEdgesStencil", _selectedEdgesStencil);
  data.set("nodesStencil", _nodesStencil);
  data.set("metaNodesStencil", _metaNodesStencil);
  data.set("edgesStencil", _edgesStencil);
  data.set("nodesLabelStencil", _nodesLabelStencil);
  data.set("metaNodesLabelStencil", _metaNodesLabelStencil);
  data.set("edgesLabelStencil", _edgesLabelStencil);
  //data.set("SupergraphId", _graph->getId());
  return data;
}

static Graph *findGraphById(Graph *sg, const int id) {
  if (sg->getId()==id) return sg;
  Iterator<Graph *> *itS=sg->getSubGraphs();
  while (itS->hasNext()) {
    Graph *tmp=findGraphById(itS->next(),id);
    if (tmp!=0) {delete itS; return tmp;}
  } delete itS;
  return 0;
}

//This function should rewriten completly
void GlGraphRenderingParameters::setParameters(const DataSet &data) {
  bool b;
  if (data.get<bool>("antialiased", b))
    setAntialiasing(b);
  if (data.get<bool>("arrow", b))
    setViewArrow(b);
  if (data.get<bool>("displayNodes", b))
    setDisplayNodes(b);
  if (data.get<bool>("displayMetaNodes", b))
    setDisplayMetaNodes(b);
  if (data.get<bool>("displayEdges", b))
    setDisplayEdges(b);
  if (data.get<bool>("nodeLabel", b))
    setViewNodeLabel(b);
  if (data.get<bool>("edgeLabel", b))
    setViewEdgeLabel(b);
  if (data.get<bool>("metaLabel", b))
    setViewMetaLabel(b);
  if (data.get<bool>("elementOrdered", b))
    setElementOrdered(b);
  if (data.get<bool>("autoScale", b))
    _viewAutoScale=b;
  if (data.get<bool>("incrementalRendering", b))
    setIncrementalRendering(b);
  if (data.get<bool>("edgeColorInterpolation", b))
    setEdgeColorInterpolate(b);
  if (data.get<bool>("edgeSizeInterpolation", b))
    setEdgeSizeInterpolate(b);
  if (data.get<bool>("edge3D", b))
    setEdge3D(b);
  unsigned int ui;
  if (data.get<unsigned int>("fontType", ui))
    _FontsType = ui;
  int i;
  if (data.get<int>("selectedNodesStencil", i))
    setSelectedNodesStencil(i);
  if (data.get<int>("selectedMetaNodesStencil", i))
    setSelectedMetaNodesStencil(i);
  if (data.get<int>("selectedEdgesStencil", i))
    setSelectedEdgesStencil(i);
  if (data.get<int>("nodesStencil", i))
    setNodesStencil(i);
  if (data.get<int>("metaNodesStencil", i))
    setMetaNodesStencil(i);
  if (data.get<int>("edgesStencil", i))
    setEdgesStencil(i);
  if (data.get<int>("nodesLabelStencil", i))
    setNodesLabelStencil(i);
  if (data.get<int>("metaNodesLabelStencil", i))
    setMetaNodesLabelStencil(i);
  if (data.get<int>("edgesLabelStencil", i))
    setEdgesLabelStencil(i);
}
//====================================================
unsigned int GlGraphRenderingParameters::getLabelsBorder() const {
  return _labelsBorder;
}
void GlGraphRenderingParameters::setLabelsBorder(const unsigned int border) {
  _labelsBorder = border;
}
//====================================================
bool GlGraphRenderingParameters::isViewMetaLabel()const {
  return (_viewMetaLabel);
}
void GlGraphRenderingParameters::setViewMetaLabel(bool b){
  _viewMetaLabel=b;
}
//====================================================
unsigned int GlGraphRenderingParameters::getFontsType()const {
  return (_FontsType);
}
void GlGraphRenderingParameters::setFontsType(unsigned int i) {
  _FontsType = i;
}
//====================================================
void GlGraphRenderingParameters::setFontsPath(const std::string &path) {
  _fontsPath = path;
}
std::string GlGraphRenderingParameters::getFontsPath() {
  return _fontsPath;
}
//====================================================
void GlGraphRenderingParameters::setTexturePath(const std::string &path) {
  _texturePath = path;
}
std::string GlGraphRenderingParameters::getTexturePath() const {
  return _texturePath;
}
//====================================================
bool GlGraphRenderingParameters::isIncrementalRendering() const {
  return (_incrementalRendering);
}
void GlGraphRenderingParameters::setIncrementalRendering(const bool b) {
  _incrementalRendering=b;
}
//====================================================
bool GlGraphRenderingParameters::isDisplayEdges() const {
  return _displayEdges;
}
void GlGraphRenderingParameters::setDisplayEdges(const bool b) {
  _displayEdges=b;
}
//====================================================
bool GlGraphRenderingParameters::isDisplayNodes() const {
  return _displayNodes;
}
void GlGraphRenderingParameters::setDisplayNodes(const bool b) {
  _displayNodes=b;
}
//====================================================
bool GlGraphRenderingParameters::isDisplayMetaNodes() const {
  return _displayMetaNodes;
}
void GlGraphRenderingParameters::setDisplayMetaNodes(const bool b) {
  _displayMetaNodes=b;
}
//====================================================
bool GlGraphRenderingParameters::isAntialiased() const {
  return (_antialiased);
}
void GlGraphRenderingParameters::setAntialiasing(const bool b) {
  _antialiased=b;
}
//====================================================
bool GlGraphRenderingParameters::isViewArrow() const {
  return (_viewArrow);
}
void GlGraphRenderingParameters::setViewArrow(const bool b) {
  _viewArrow=b;
}
//====================================================
bool GlGraphRenderingParameters::isElementOrdered() const {
  return (_elementOrdered);
}
void GlGraphRenderingParameters::setElementOrdered(const bool b) {
  _elementOrdered = b;
}
//====================================================
bool GlGraphRenderingParameters::isEdge3D() const {
  return _edge3D;
}
void GlGraphRenderingParameters::setEdge3D(const bool b) {
  _edge3D=b;
}
//====================================================
bool GlGraphRenderingParameters::isViewNodeLabel()const {
  return (_viewNodeLabel);
}
void GlGraphRenderingParameters::setViewNodeLabel(const bool b) {
  _viewNodeLabel=b;
}
//====================================================
bool GlGraphRenderingParameters::isViewEdgeLabel()const {
  return (_viewEdgeLabel);
}
void GlGraphRenderingParameters::setViewEdgeLabel(const bool b) {
  _viewEdgeLabel=b;
}
//====================================================
void GlGraphRenderingParameters::setSelectedNodesStencil(const int stencil) {
  _selectedNodesStencil=stencil;
}
int GlGraphRenderingParameters::getSelectedNodesStencil() {
  return _selectedNodesStencil;
}
//====================================================
void GlGraphRenderingParameters::setSelectedMetaNodesStencil(const int stencil) {
  _selectedMetaNodesStencil=stencil;
}
int GlGraphRenderingParameters::getSelectedMetaNodesStencil() {
  return _selectedMetaNodesStencil;
}
//====================================================
void GlGraphRenderingParameters::setSelectedEdgesStencil(const int stencil) {
  _selectedEdgesStencil=stencil;
}
int GlGraphRenderingParameters::getSelectedEdgesStencil() {
  return _selectedEdgesStencil;
}
//====================================================
void GlGraphRenderingParameters::setNodesStencil(const int stencil) {
  _nodesStencil=stencil;
}
int GlGraphRenderingParameters::getNodesStencil() {
  return _nodesStencil;
}
//====================================================
void GlGraphRenderingParameters::setMetaNodesStencil(const int stencil) {
  _metaNodesStencil=stencil;
}
int GlGraphRenderingParameters::getMetaNodesStencil() {
  return _metaNodesStencil;
}
//====================================================
void GlGraphRenderingParameters::setEdgesStencil(const int stencil) {
  _edgesStencil=stencil;
}
int GlGraphRenderingParameters::getEdgesStencil(){
  return _edgesStencil;
}
//====================================================
void GlGraphRenderingParameters::setNodesLabelStencil(const int stencil) {
  _nodesLabelStencil=stencil;
}
int GlGraphRenderingParameters::getNodesLabelStencil() {
  return _nodesLabelStencil;
}
//====================================================
void GlGraphRenderingParameters::setMetaNodesLabelStencil(const int stencil) {
  _metaNodesLabelStencil=stencil;
}
int GlGraphRenderingParameters::getMetaNodesLabelStencil() {
  return _metaNodesLabelStencil;
}
//====================================================
void GlGraphRenderingParameters::setEdgesLabelStencil(const int stencil) {
  _edgesLabelStencil=stencil;
}
int GlGraphRenderingParameters::getEdgesLabelStencil(){
  return _edgesLabelStencil;
}
//====================================================
bool GlGraphRenderingParameters::isEdgeColorInterpolate()const {
  return (_edgeColorInterpolate);
}
void GlGraphRenderingParameters::setEdgeColorInterpolate(const bool b) {
  _edgeColorInterpolate=b;
}
//====================================================
bool GlGraphRenderingParameters::isEdgeSizeInterpolate()const {
  return (_edgeSizeInterpolate);
}
void GlGraphRenderingParameters::setEdgeSizeInterpolate(const bool b){
  _edgeSizeInterpolate=b;
}
//====================================================
void GlGraphRenderingParameters::setInputLayout(const std::string &layoutName) {
  _layoutName = layoutName;
}
std::string GlGraphRenderingParameters::getInputLayout() const {
  return _layoutName;
}
//====================================================
void GlGraphRenderingParameters::setFeedbackRender(bool feedback){
  _feedbackRender=feedback;
}
//====================================================
bool GlGraphRenderingParameters::getFeedbackRender(){
  return _feedbackRender;
}
