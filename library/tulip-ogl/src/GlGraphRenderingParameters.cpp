#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/LayoutProperty.h>
#include <tulip/Vector.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/TlpTools.h>

using namespace tlp;

GlGraphRenderingParameters::GlGraphRenderingParameters() :
  _graph(0),
  _viewArrow(false), 
  _viewNodeLabel(false),
  _viewEdgeLabel(false),
  _viewMetaLabel(false), 
  _elementOrdered(false),
  _incrementalRendering(true),
  _edgeColorInterpolate(true), 
  _edge3D(false),
  _displayEdges(true),
  _viewOrtho(true), 
  _FontsType(0),
  _labelsBorder(2),
  _backgroundColor(255,255,255),
  _layoutName("viewLayout") {
  _fontsPath = tlp::TulipLibDir + "tlp/bitmaps/";
  _texturePath = "";
  _viewport[0] = 0;
  _viewport[1] = 0;
  _viewport[2] = 640;
  _viewport[3] = 480;
}
//This function should rewriten completly
DataSet GlGraphRenderingParameters::getParameters() const {
  TRACE_EXEC();
  DataSet data;
  //Color
  data.set("backgroundColor", _backgroundColor);
  //bool
  data.set("arrow", _viewArrow);
  data.set("nodeLabel", _viewNodeLabel);
  data.set("edgeLabel", _viewEdgeLabel);
  data.set("metaLabel", _viewMetaLabel);
  data.set("elementOrdered", _elementOrdered);
  data.set("autoScale", _viewAutoScale);
  data.set("incrementalRendering", _incrementalRendering);
  data.set("edgeColorInterpolation", _edgeColorInterpolate);
  data.set("edgeSizeInterpolation", _edgeSizeInterpolate);
  data.set("edge3D", _edge3D);
  data.set("orthogonalProjection", _viewOrtho);
  //unsigned int
  data.set("fontType", _FontsType);
  //int
  data.set("SupergraphId", _graph->getId());
  //Coord
  data.set("cameraEyes", _camera.eyes);
  data.set("cameraCenter", _camera.center);
  data.set("cameraUp", _camera.up);
  data.set("cameraZoomFactor", _camera.zoomFactor);
  data.set("distCam", _camera.sceneRadius);
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
  TRACE_EXEC();
  Color color;
  if (data.get<Color>("backgroundColor", color)) 
    _backgroundColor = color;
  bool b;
  if (data.get<bool>("arrow", b))
    setViewArrow(b);
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
  if (data.get<bool>("orthogonalProjection", b)) 
    _viewOrtho = b;
  unsigned int ui;
  if (data.get<unsigned int>("fontType", ui)) 
    _FontsType = ui;
  //restore all view parameters or none
  Coord ce,cc,cu;
  double czf;
  double dc;
  if (
      data.get<Coord>("cameraEyes", ce)
      && data.get<Coord>("cameraCenter", cc)
      && data.get<Coord>("cameraUp", cu)
      && data.get<double>("cameraZoomFactor", czf)
      && data.get<double>("distCam", dc))
    {
      _camera.eyes = ce;
      _camera.center = cc;
      _camera.up = cu;
      _camera.zoomFactor  = czf;
      _camera.sceneRadius = dc;
    }
}
//====================================================
tlp::Graph* GlGraphRenderingParameters::getGraph() const {
  return _graph;
}
//====================================================
void GlGraphRenderingParameters::setGraph(tlp::Graph *graph) {
  _graph = graph;
}
//====================================================
Camera GlGraphRenderingParameters::getCamera() const {
  return _camera;
}
//====================================================
void GlGraphRenderingParameters::setCamera(const Camera &cam) {
  _camera = cam;
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
void GlGraphRenderingParameters::setTexturePath(const std::string &path){
  _texturePath = path;
}
std::string GlGraphRenderingParameters::getTexturePath() {
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
bool GlGraphRenderingParameters::isViewArrow() const {
  return (_viewArrow);
}
void GlGraphRenderingParameters::setViewArrow(const bool b) {
  _viewArrow=b;
}
//====================================================
bool GlGraphRenderingParameters::isViewOrtho() const {
  return _viewOrtho;
}
void GlGraphRenderingParameters::setViewOrtho(const bool b) {
  _viewOrtho=b;
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
Color GlGraphRenderingParameters::getBackgroundColor() const {
  return _backgroundColor;
}
void GlGraphRenderingParameters::setBackgroundColor(const Color &color){
  _backgroundColor = color;
}
//====================================================
void GlGraphRenderingParameters::setViewport(const unsigned x, const unsigned y, const unsigned int width, const unsigned int height) {
  _viewport[0] = x;
  _viewport[1] = y;
  _viewport[2] = width;
  _viewport[3] = height;
}
void GlGraphRenderingParameters::setViewport(const Vector<int, 4>&viewport) {
  _viewport = viewport;
}

Vector<int, 4> GlGraphRenderingParameters::getViewport() const {
  return _viewport;
}  
//====================================================
void GlGraphRenderingParameters::setInputLayout(const std::string &layoutName) {
  _layoutName = layoutName;
}
std::string GlGraphRenderingParameters::getInputLayout() const {
  return _layoutName;
}
//====================================================
