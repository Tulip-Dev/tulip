#include "GlyphPreviewGenerator.h"
#include <tulip/Graph.h>
#include <tulip/GlOffscreenRenderer.h>
#include <tulip/EdgeExtremityGlyphManager.h>
using namespace tlp;
using namespace std;

GlyphPreviewGenerator* GlyphPreviewGenerator::_instance=NULL;

GlyphPreviewGenerator::GlyphPreviewGenerator()
{
}

GlyphPreviewGenerator& GlyphPreviewGenerator::getInst(){
    if(_instance==NULL){
        _instance = new GlyphPreviewGenerator();
    }
    return *_instance;
}

QPixmap GlyphPreviewGenerator::getPreview(unsigned int pluginId){
    if(_previews.find(pluginId) == _previews.end()){
        Graph* graph = newGraph();
        GlGraphRenderingParameters parameters;
        GlGraphInputData inputData(graph,&parameters);
        inputData.elementSize->setAllNodeValue(Size(1,1,1));
        inputData.elementShape->setAllNodeValue(pluginId);
        node n = graph->addNode();
        GlOffscreenRenderer *renderer = GlOffscreenRenderer::getInstance();
        renderer->setViewPortSize(16,16);
        renderer->clearScene();
        renderer->addGraphToScene(graph);
        renderer->renderScene(true);
        QImage preview = renderer->getImage();
        _previews[pluginId] = QPixmap::fromImage(preview);
        delete graph;
    }
    return _previews[pluginId];
}

EdgeExtremityGlyphPreviewGenerator* EdgeExtremityGlyphPreviewGenerator::_instance = NULL;
EdgeExtremityGlyphPreviewGenerator::EdgeExtremityGlyphPreviewGenerator(){
    //No edge extremity pixmap
    _previews[EdgeExtremityGlyphManager::NoEdgeExtremetiesId] = QPixmap();
}

EdgeExtremityGlyphPreviewGenerator & EdgeExtremityGlyphPreviewGenerator::getInst(){
    if(_instance==NULL){
        _instance = new EdgeExtremityGlyphPreviewGenerator();
    }
    return *_instance;
}

QPixmap EdgeExtremityGlyphPreviewGenerator::getPreview(unsigned int pluginId){
    if(_previews.find(pluginId) == _previews.end()){
        Graph* graph = newGraph();
        GlGraphRenderingParameters parameters;
        GlGraphInputData inputData(graph,&parameters);
        inputData.elementSize->setAllNodeValue(Size(0.01,0.2,0.1));
        inputData.elementSize->setAllEdgeValue(Size(0.125,0.125,0.125));
        inputData.elementColor->setAllNodeValue(Color(255,255,255,0));
        inputData.elementBorderColor->setAllNodeValue(Color(255,255,255,0));
        node n1 = graph->addNode();
        node n2= graph->addNode();
        edge e = graph->addEdge(n1,n2);
        inputData.elementLayout->setNodeValue(n1,Coord(0,0,0));
        inputData.elementLayout->setNodeValue(n2,Coord(0.3,0,0));
        vector<Coord> bends;
        bends.push_back(Coord(0.01,0,0));
        inputData.elementLayout->setAllEdgeValue(bends);

        inputData.elementSrcAnchorShape->setAllEdgeValue(EdgeExtremityGlyphManager::NoEdgeExtremetiesId);
        inputData.elementTgtAnchorSize->setAllEdgeValue(Size(2,2,1));
        inputData.elementTgtAnchorShape->setAllEdgeValue(pluginId);

        GlOffscreenRenderer *renderer = GlOffscreenRenderer::getInstance();
        renderer->setViewPortSize(16,16);
        renderer->clearScene();
        renderer->addGraphToScene(graph);
        GlGraphRenderingParameters renderingParamerters = renderer->getScene()->getGlGraphComposite()->getRenderingParameters();
        renderingParamerters.setEdgeColorInterpolate(false);
        renderingParamerters.setEdgeSizeInterpolate(false);
        renderingParamerters.setViewArrow(true);
        renderer->getScene()->getGlGraphComposite()->setRenderingParameters(renderingParamerters);
        renderer->renderScene(true);
        QImage preview = renderer->getImage();
        _previews[pluginId] = QPixmap::fromImage(preview);
        delete graph;
    }
    return _previews[pluginId];
}

