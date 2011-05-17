#include "GlyphPreviewGenerator.h"
#include <tulip/Graph.h>
#include <tulip/GlOffscreenRenderer.h>
#include <tulip/EdgeExtremityGlyphManager.h>
using namespace tlp;
using namespace std;

GlyphPreviewGenerator* GlyphPreviewGenerator::_instance=NULL;

GlyphPreviewGenerator::GlyphPreviewGenerator():_graph(newGraph()),_node(_graph->addNode())
{
    //Init graph parameters.
    GlGraphRenderingParameters parameters;
    GlGraphInputData inputData(_graph,&parameters);
    inputData.elementSize->setAllNodeValue(Size(1,1,1));

}

GlyphPreviewGenerator::~GlyphPreviewGenerator(){
delete _graph;
}

GlyphPreviewGenerator& GlyphPreviewGenerator::getInst(){
    if(_instance==NULL){
        _instance = new GlyphPreviewGenerator();
    }
    return *_instance;
}

QPixmap GlyphPreviewGenerator::getPreview(unsigned int pluginId){
    if(_previews.find(pluginId) == _previews.end()){
        _graph->getProperty<IntegerProperty>("viewShape")->setNodeValue(_node,pluginId);
        GlOffscreenRenderer *renderer = GlOffscreenRenderer::getInstance();
        renderer->setViewPortSize(16,16);
        renderer->clearScene();
        renderer->addGraphToScene(_graph);
        renderer->renderScene(true);
        QImage preview = renderer->getImage();
        _previews[pluginId] = QPixmap::fromImage(preview);
    }
    return _previews[pluginId];
}

EdgeExtremityGlyphPreviewGenerator* EdgeExtremityGlyphPreviewGenerator::_instance = NULL;
EdgeExtremityGlyphPreviewGenerator::EdgeExtremityGlyphPreviewGenerator():_graph(newGraph()){
    //No edge extremity pixmap
    _previews[EdgeExtremityGlyphManager::NoEdgeExtremetiesId] = QPixmap();

    //Init graph.
    GlGraphRenderingParameters parameters;
    GlGraphInputData inputData(_graph,&parameters);
    inputData.elementSize->setAllNodeValue(Size(0.01,0.2,0.1));
    inputData.elementSize->setAllEdgeValue(Size(0.125,0.125,0.125));
    inputData.elementColor->setAllNodeValue(Color(255,255,255,0));
    inputData.elementBorderColor->setAllNodeValue(Color(255,255,255,0));
    node n1 = _graph->addNode();
    node n2= _graph->addNode();
    _edge = _graph->addEdge(n1,n2);
    inputData.elementLayout->setNodeValue(n1,Coord(0,0,0));
    inputData.elementLayout->setNodeValue(n2,Coord(0.3,0,0));
    vector<Coord> bends;
    bends.push_back(Coord(0.01,0,0));
    inputData.elementLayout->setAllEdgeValue(bends);

    inputData.elementSrcAnchorShape->setAllEdgeValue(EdgeExtremityGlyphManager::NoEdgeExtremetiesId);
    inputData.elementTgtAnchorSize->setAllEdgeValue(Size(2,2,1));
}

EdgeExtremityGlyphPreviewGenerator::~EdgeExtremityGlyphPreviewGenerator(){
    delete _graph;
}

EdgeExtremityGlyphPreviewGenerator & EdgeExtremityGlyphPreviewGenerator::getInst(){
    if(_instance==NULL){
        _instance = new EdgeExtremityGlyphPreviewGenerator();
    }
    return *_instance;
}

QPixmap EdgeExtremityGlyphPreviewGenerator::getPreview(unsigned int pluginId){
    if(_previews.find(pluginId) == _previews.end()){
        _graph->getProperty<IntegerProperty>("viewTgtAnchorShape")->setEdgeValue(_edge,pluginId);
        GlOffscreenRenderer *renderer = GlOffscreenRenderer::getInstance();
        renderer->setViewPortSize(16,16);
        renderer->clearScene();
        renderer->addGraphToScene(_graph);
        GlGraphRenderingParameters renderingParamerters = renderer->getScene()->getGlGraphComposite()->getRenderingParameters();
        renderingParamerters.setEdgeColorInterpolate(false);
        renderingParamerters.setEdgeSizeInterpolate(false);
        renderingParamerters.setViewArrow(true);
        renderer->getScene()->getGlGraphComposite()->setRenderingParameters(renderingParamerters);
        renderer->renderScene(true);
        QImage preview = renderer->getImage();
        _previews[pluginId] = QPixmap::fromImage(preview);
    }
    return _previews[pluginId];
}

