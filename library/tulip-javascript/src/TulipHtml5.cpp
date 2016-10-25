/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

/**
 *
 * tulip.js : port of the Tulip framework to JavaScript through emscripten
 * Copyright (c) 2016 Antoine Lambert, Thales Services SAS
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <ctime>

#include <tulip/Graph.h>
#include <tulip/GraphAbstract.h>
#include <tulip/TlpTools.h>
#include <tulip/PluginLibraryLoader.h>
#include <tulip/PropertyTypes.h>
#include <tulip/LayoutProperty.h>
#include <tulip/GraphProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/PluginLister.h>
#include <tulip/ExportModule.h>
#include <tulip/SimplePluginProgress.h>
#include <tulip/Color.h>
#include <tulip/PropertyTypes.h>
#include <tulip/ColorScale.h>
#include <tulip/DrawingTools.h>

#include <tulip/NanoVGManager.h>
#include <tulip/GlGraph.h>
#include <tulip/Camera.h>
#include <tulip/Light.h>
#include <tulip/GlRect2D.h>
#include <tulip/GlUtils.h>
#include <tulip/GlTextureManager.h>
#include <tulip/LabelsRenderer.h>
#include <tulip/GlScene.h>
#include <tulip/GlLayer.h>
#include <tulip/GlLODCalculator.h>
#include <tulip/ConcaveHullBuilder.h>
#include <tulip/GlConcavePolygon.h>
#include <tulip/ShaderManager.h>
#include <tulip/GlShaderProgram.h>
#include <tulip/GlyphsRenderer.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlGraphInputData.h>

#include "Interactors.h"

#include <sys/stat.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <dlfcn.h>

using namespace tlp;

static std::vector<std::string> canvasIds;
static std::map<std::string, EMSCRIPTEN_WEBGL_CONTEXT_HANDLE> webGlContextHandle;
static std::map<std::string, GlScene *> glScene;
static std::map<std::string, GlGraph *> glGraph;
static std::map<std::string, tlp::Graph *> graph;
static std::map<std::string, bool> canDeleteGraph;
static std::map<tlp::Graph *, std::string> graphToCanvas;
static std::map<std::string, GlSceneInteractor *> currentCanvasInteractor;

static std::map<std::string, GlLayer *> hullsLayer;
static std::map<std::string, std::map<unsigned int, GlConcavePolygon *>> graphsHulls;

static std::string currentCanvasId;

extern "C" {
void refreshWebGLCanvas(void (*drawFunc)());
void resizeWebGLCanvas(const char *canvasId, int width, int height, bool sizeRelativeToContainer);
void requestFullScreenCanvas(const char *canvasId);
void safeSetTimeout(unsigned int msecs, void (*func)(void *value), void *value);
void setCurrentCanvas(const char *canvasId);
bool domElementExists(const char *elementId);
bool canXhrOnUrl(const char *url);
void loadImageFromUrl(const char *url, void (*imageLoadedFunc)(const char *, const unsigned char *, unsigned int, unsigned int),
                      void (*errorFunc)(unsigned int, void *, int));
bool isChrome();
void blurCanvas(const char *canvasId);
void focusCanvas(const char *canvasId);

void EMSCRIPTEN_KEEPALIVE updateGlScene(const char *canvasId) {
  std::string curCanvasBak = currentCanvasId;
  setCurrentCanvas(canvasId);

  glScene[canvasId]->draw();

  if (curCanvasBak == canvasId && currentCanvasInteractor[canvasId]) {
    currentCanvasInteractor[canvasId]->draw();
  }

  if (GlShaderProgram::getCurrentActiveShader()) {
    GlShaderProgram::getCurrentActiveShader()->desactivate();
  }

  checkOpenGLError();
  setCurrentCanvas(curCanvasBak.c_str());
}

static void drawCallback(void) {
  for (size_t i = 0; i < canvasIds.size(); ++i) {
    if (domElementExists(canvasIds[i].c_str())) {
      updateGlScene(canvasIds[i].c_str());
    } else {
      std::cout << canvasIds[i] << " does not exist anymore" << std::endl;
    }
  }
}

void EMSCRIPTEN_KEEPALIVE draw(bool refreshDisplay = true) {
  if (refreshDisplay) {
    refreshWebGLCanvas(drawCallback);
  } else {
    drawCallback();
  }
}
}

static int getModifiers(const EmscriptenMouseEvent &mouseEvent) {
  int modifiers = 0;
  if (mouseEvent.ctrlKey) {
    modifiers |= ACTIVE_CTRL;
  }
  if (mouseEvent.altKey) {
    modifiers |= ACTIVE_ALT;
  }
  if (mouseEvent.shiftKey) {
    modifiers |= ACTIVE_SHIFT;
  }
  return modifiers;
}

static int getModifiers(const EmscriptenKeyboardEvent &keyEvent) {
  int modifiers = 0;
  if (keyEvent.ctrlKey) {
    modifiers |= ACTIVE_CTRL;
  }
  if (keyEvent.altKey) {
    modifiers |= ACTIVE_ALT;
  }
  if (keyEvent.shiftKey) {
    modifiers |= ACTIVE_SHIFT;
  }
  return modifiers;
}

static EM_BOOL mouseCallback(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
  std::string canvasId = reinterpret_cast<const char *>(userData);
  if (currentCanvasInteractor[canvasId]) {
    MouseButton button;
    if (mouseEvent->button == 0) {
      button = LEFT_BUTTON;
    } else if (mouseEvent->button == 1) {
      button = MIDDLE_BUTTON;
    } else {
      button = RIGHT_BUTTON;
    }
    if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN) {
      return currentCanvasInteractor[canvasId]->mouseCallback(button, DOWN, mouseEvent->targetX, mouseEvent->targetY, getModifiers(*mouseEvent));
    } else if (eventType == EMSCRIPTEN_EVENT_MOUSEUP) {
      return currentCanvasInteractor[canvasId]->mouseCallback(button, UP, mouseEvent->targetX, mouseEvent->targetY, getModifiers(*mouseEvent));
    } else if (eventType == EMSCRIPTEN_EVENT_MOUSEMOVE) {
      return currentCanvasInteractor[canvasId]->mouseMoveCallback(mouseEvent->targetX, mouseEvent->targetY, getModifiers(*mouseEvent));
    } else if (eventType == EMSCRIPTEN_EVENT_MOUSEENTER) {
      setCurrentCanvas(canvasId.c_str());
    } else if (eventType == EMSCRIPTEN_EVENT_MOUSELEAVE) {
      currentCanvasInteractor[canvasId]->mouseCallback(button, UP, 0, 0, getModifiers(*mouseEvent));
      setCurrentCanvas("");
    }
  }
  return false;
}

static EM_BOOL wheelCallback(int /* eventType */, const EmscriptenWheelEvent *wheelEvent, void *userData) {
  std::string canvasId = reinterpret_cast<const char *>(userData);
  if (currentCanvasInteractor[canvasId]) {
    double delta = wheelEvent->deltaY;
    if (wheelEvent->mouse.shiftKey && isChrome()) {
      delta = wheelEvent->deltaX;
    }
    if (delta > 0) {
      return currentCanvasInteractor[canvasId]->mouseCallback(WHEEL, DOWN, wheelEvent->mouse.targetX, wheelEvent->mouse.targetY,
                                                              getModifiers(wheelEvent->mouse));
    } else {
      return currentCanvasInteractor[canvasId]->mouseCallback(WHEEL, UP, wheelEvent->mouse.targetX, wheelEvent->mouse.targetY,
                                                              getModifiers(wheelEvent->mouse));
    }
  }
  return false;
}

static EM_BOOL keyCallback(int /* eventType */, const EmscriptenKeyboardEvent *keyEvent, void *userData) {
  std::string canvasId = reinterpret_cast<const char *>(userData);
  if (currentCanvasInteractor[canvasId]) {
    return currentCanvasInteractor[canvasId]->keyboardCallback(keyEvent->key, getModifiers(*keyEvent));
  }
  return false;
}

static std::ostringstream oss;
static std::string staticString;

void timerFunc(unsigned int msecs, void (*func)(void *value), void *value) {
  safeSetTimeout(msecs, func, value);
}

static int nbTextureToLoad = 0;

static std::vector<std::string> cachedTextureName;

static void onTextureLoaded(unsigned int, void *, const char *texture) {
  GlTextureManager::instance(currentCanvasId)->addTextureFromFile(texture, true);
  --nbTextureToLoad;
  if (nbTextureToLoad == 0) {
    for (auto it = glScene.begin(); it != glScene.end(); ++it) {
      it->second->forceRedraw();
    }
    draw();
  }
}

static void onTextureLoadError(unsigned int, void *texture, int) {
  std::cout << "Error when loading texture " << reinterpret_cast<const char *>(texture) << std::endl;
  if (std::string(reinterpret_cast<const char *>(texture)).substr(0, 4) == "http") {
    std::cout << "CORS requests is probably not supported by the server hosting the image" << std::endl;
  }
  --nbTextureToLoad;
  if (nbTextureToLoad == 0) {
    for (auto it = glScene.begin(); it != glScene.end(); ++it) {
      it->second->forceRedraw();
    }
    draw();
  }
}

static void registerTextureFromData(const char *textureName, const unsigned char *textureData, unsigned int width, unsigned int height) {
  GlTextureManager::instance(currentCanvasId)->addTextureFromData(textureName, textureData, width, height, true);
  --nbTextureToLoad;
  if (nbTextureToLoad == 0) {
    for (auto it = glScene.begin(); it != glScene.end(); ++it) {
      it->second->forceRedraw();
    }
    draw();
  }
}

static void loadTexture(const std::string &texture) {
  if (!texture.empty()) {
    if (std::find(cachedTextureName.begin(), cachedTextureName.end(), texture) == cachedTextureName.end()) {
      cachedTextureName.push_back(texture);
      ++nbTextureToLoad;
      if (texture.substr(0, 4) == "http") {
        loadImageFromUrl(cachedTextureName.back().c_str(), registerTextureFromData, onTextureLoadError);
      } else {
        if (!canXhrOnUrl(cachedTextureName.back().c_str())) {
          return;
        }
        std::vector<std::string> paths;
        tokenize(texture, paths, "/");
        if (paths.size() > 1) {
          std::string curPath;
          for (size_t i = 0; i < paths.size() - 1; ++i) {
            curPath += "/" + paths[i];
            mkdir(curPath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
          }
        }
        emscripten_async_wget2(cachedTextureName.back().c_str(), cachedTextureName.back().c_str(), "GET", "",
                               reinterpret_cast<void *>(const_cast<char *>(cachedTextureName.back().c_str())), onTextureLoaded, onTextureLoadError,
                               NULL);
      }
    }
  }
}

class GlDrawObserver : public tlp::Observable {

public:
  void treatEvent(const tlp::Event &event) {
    const GlSceneEvent *glSceneEvent = dynamic_cast<const GlSceneEvent *>(&event);
    const GlLayerEvent *glLayerEvent = dynamic_cast<const GlLayerEvent *>(&event);
    const tlp::PropertyEvent *propertyEvent = dynamic_cast<const tlp::PropertyEvent *>(&event);

    if (glSceneEvent) {
      if (glSceneEvent->getType() == GlSceneEvent::LAYER_ADDED_IN_SCENE) {
        glSceneEvent->getGlLayer()->addListener(this);
      } else if (glSceneEvent->getType() == GlSceneEvent::LAYER_REMOVED_FROM_SCENE) {
        glSceneEvent->getGlLayer()->removeListener(this);
      } else if (glSceneEvent->getType() == GlSceneEvent::DRAW_REQUEST) {
        draw();
      }
    }

    if (glLayerEvent) {
      if (glLayerEvent->getType() == GlLayerEvent::ENTITY_ADDED_IN_LAYER) {
        glLayerEvent->getGlEntity()->addObserver(this);
      } else if (glLayerEvent->getType() == GlLayerEvent::ENTITY_REMOVED_FROM_LAYER) {
        glLayerEvent->getGlEntity()->removeObserver(this);
      }
    }

    if (propertyEvent) {
      tlp::PropertyInterface *prop = propertyEvent->getProperty();
      if (propertyEvent->getType() == tlp::PropertyEvent::TLP_AFTER_SET_NODE_VALUE && prop->getName() == "viewTexture") {
        loadTexture(prop->getNodeStringValue(propertyEvent->getNode()));
      }
      if (propertyEvent->getType() == tlp::PropertyEvent::TLP_AFTER_SET_EDGE_VALUE && prop->getName() == "viewTexture") {
        loadTexture(prop->getEdgeStringValue(propertyEvent->getEdge()));
      }
      if (propertyEvent->getType() == tlp::PropertyEvent::TLP_AFTER_SET_ALL_NODE_VALUE && prop->getName() == "viewTexture") {
        loadTexture(prop->getNodeDefaultStringValue());
      }
      if (propertyEvent->getType() == tlp::PropertyEvent::TLP_AFTER_SET_ALL_EDGE_VALUE && prop->getName() == "viewTexture") {
        loadTexture(prop->getEdgeDefaultStringValue());
      }
    }
  }

  void treatEvents(const std::vector<tlp::Event> &events) {
    bool drawNeeded = false;
    for (size_t i = 0; i < events.size(); ++i) {
      GlEntity *glEntity = dynamic_cast<GlEntity *>(events[i].sender());
      drawNeeded = drawNeeded || (glEntity != NULL);
    }
    if (drawNeeded) {
      draw();
    }
  }
};

static GlDrawObserver glDrawObserver;

extern "C" {

void EMSCRIPTEN_KEEPALIVE activateInteractor(const char *canvasId, const char *interactorName) {
  if (interactorsMap.find(interactorName) == interactorsMap.end()) {
    std::cerr << "Error : no interactor named \"" << interactorName << "\"" << std::endl;
  } else {
    if (currentCanvasInteractor[canvasId]) {
      currentCanvasInteractor[canvasId]->desactivate();
    }
    currentCanvasInteractor[canvasId] = interactorsMap[interactorName];
    currentCanvasInteractor[canvasId]->setScene(glScene[canvasId]);
    currentCanvasInteractor[canvasId]->activate();
  }
}

void EMSCRIPTEN_KEEPALIVE desactivateInteractor(const char *canvasId) {
  if (currentCanvasInteractor[canvasId]) {
    currentCanvasInteractor[canvasId]->desactivate();
  }
  currentCanvasInteractor[canvasId] = NULL;
}

void cleanManagedCanvasIfNeeded() {
  std::vector<std::string> removedCanvas;
  for (size_t i = 0; i < canvasIds.size(); ++i) {
    if (!domElementExists(canvasIds[i].c_str())) {
      removedCanvas.push_back(canvasIds[i]);
    }
  }
  for (size_t i = 0; i < removedCanvas.size(); ++i) {
    setCurrentCanvas(removedCanvas[i].c_str());
    canvasIds.erase(std::remove(canvasIds.begin(), canvasIds.end(), removedCanvas[i]), canvasIds.end());
    delete glScene[removedCanvas[i]];
    if (canDeleteGraph[removedCanvas[i]]) {
      delete graph[removedCanvas[i]]->getRoot();
    }
    emscripten_webgl_destroy_context(webGlContextHandle[removedCanvas[i]]);
    glScene.erase(removedCanvas[i]);
    graph.erase(removedCanvas[i]);
    glGraph.erase(removedCanvas[i]);
    graphToCanvas.erase(graph[removedCanvas[i]]);
    graph.erase(removedCanvas[i]);
    currentCanvasInteractor.erase(removedCanvas[i]);
  }
}

void EMSCRIPTEN_KEEPALIVE initCanvas(const char *canvasId, int width, int height, bool sizeRelativeToContainer) {

  cleanManagedCanvasIfNeeded();

  currentCanvasId = canvasId;

  if (glScene.find(currentCanvasId) == glScene.end() || !glScene[currentCanvasId]) {

    canvasIds.push_back(currentCanvasId);

    resizeWebGLCanvas(canvasIds.back().c_str(), width, height, sizeRelativeToContainer);

    EmscriptenWebGLContextAttributes webGlContextAttributes;
    emscripten_webgl_init_context_attributes(&webGlContextAttributes);
    webGlContextAttributes.stencil = true;
    webGlContextAttributes.alpha = true;
    webGlContextAttributes.preserveDrawingBuffer = true;

    webGlContextHandle[canvasId] = emscripten_webgl_create_context(canvasId, &webGlContextAttributes);

    setCurrentCanvas(canvasId);

    emscripten_set_mousedown_callback(canvasIds.back().c_str(), reinterpret_cast<void *>(const_cast<char *>(canvasIds.back().c_str())), false,
                                      mouseCallback);
    emscripten_set_mouseup_callback(canvasIds.back().c_str(), reinterpret_cast<void *>(const_cast<char *>(canvasIds.back().c_str())), false,
                                    mouseCallback);
    emscripten_set_mousemove_callback(canvasIds.back().c_str(), reinterpret_cast<void *>(const_cast<char *>(canvasIds.back().c_str())), false,
                                      mouseCallback);
    emscripten_set_mouseenter_callback(canvasIds.back().c_str(), reinterpret_cast<void *>(const_cast<char *>(canvasIds.back().c_str())), false,
                                       mouseCallback);
    emscripten_set_mouseleave_callback(canvasIds.back().c_str(), reinterpret_cast<void *>(const_cast<char *>(canvasIds.back().c_str())), false,
                                       mouseCallback);
    emscripten_set_wheel_callback(canvasIds.back().c_str(), reinterpret_cast<void *>(const_cast<char *>(canvasIds.back().c_str())), false,
                                  wheelCallback);
    emscripten_set_keypress_callback(canvasIds.back().c_str(), reinterpret_cast<void *>(const_cast<char *>(canvasIds.back().c_str())), false,
                                     keyCallback);

    tlp::Vec4i viewport(0, 0, width, height);

    glScene[currentCanvasId] = new GlScene();
    glScene[currentCanvasId]->addListener(&glDrawObserver);
    glScene[currentCanvasId]->getMainLayer()->addListener(&glDrawObserver);

    glScene[currentCanvasId]->setViewport(viewport);

    glGraph[currentCanvasId] = new GlGraph();
    glScene[currentCanvasId]->getMainLayer()->addGlEntity(glGraph[currentCanvasId], "graph");

    hullsLayer[currentCanvasId] =
        new GlLayer("hulls", glScene[currentCanvasId]->getMainLayer()->getCamera(), glScene[currentCanvasId]->getMainLayer()->getLight());

    activateInteractor(currentCanvasId.c_str(), "ZoomAndPan");

    draw();
  }
}

void EMSCRIPTEN_KEEPALIVE setCurrentCanvas(const char *canvasId) {
  currentCanvasId = canvasId;
  if (std::string(canvasId) != "") {
    emscripten_webgl_make_context_current(webGlContextHandle[canvasId]);
    ShaderManager::setCurrentCanvasId(canvasId);
    GlTextureManager::setCurrentCanvasId(canvasId);
    LabelsRenderer::setCurrentCanvasId(canvasId);
    GlyphsRenderer::setCurrentCanvasId(canvasId);
    NanoVGManager::setCurrentCanvasId(canvasId);
    if (currentCanvasInteractor[canvasId]) {
      currentCanvasInteractor[canvasId]->setScene(glScene[canvasId]);
    }
  }

  for (size_t i = 0; i < canvasIds.size(); ++i) {
    if (canvasIds[i] != currentCanvasId) {
      blurCanvas(canvasIds[i].c_str());
    }
  }

  if (std::string(canvasId) != "") {
    focusCanvas(canvasId);
  }
}

const char *EMSCRIPTEN_KEEPALIVE getCurrentCanvas() {
  return currentCanvasId.c_str();
}

void EMSCRIPTEN_KEEPALIVE resizeCanvas(const char *canvasId, int width, int height, bool sizeRelativeToContainer) {
  resizeWebGLCanvas(canvasId, width, height, sizeRelativeToContainer);
  tlp::Vec4i viewport(0, 0, width, height);
  glScene[canvasId]->setViewport(viewport);
}

bool EMSCRIPTEN_KEEPALIVE graphHasHull(const char *canvasId, tlp::Graph *g) {
  return graphsHulls[canvasId].find(g->getId()) != graphsHulls[canvasId].end();
}

void EMSCRIPTEN_KEEPALIVE addGraphHull(const char *canvasId, tlp::Graph *g, bool forceHullComputation = false) {
  tlp::Color hullColor = genRandomColor(100);
  if (graphsHulls[canvasId].find(g->getId()) != graphsHulls[canvasId].end()) {
    hullsLayer[canvasId]->deleteGlEntity(graphsHulls[canvasId][g->getId()]);
    delete graphsHulls[canvasId][g->getId()];
  }

  std::ostringstream oss;

  std::vector<std::vector<tlp::Coord>> hullVertices;

  unsigned int i = 0;
  while (1) {
    oss.str("");
    oss << "hullVertices" << i;
    std::vector<tlp::Coord> vc;
    if (g->getAttribute(oss.str(), vc)) {
      hullVertices.push_back(vc);
      g->removeAttribute(oss.str());
    } else {
      break;
    }
  }

  if (hullVertices.empty() && forceHullComputation) {
    hullVertices = ConcaveHullBuilder::computeGraphHullVertices(g, false);
  }

  if (!hullVertices.empty()) {
    oss.str("");
    oss << "hull_" << g->getId();
    graphsHulls[canvasId][g->getId()] = new GlConcavePolygon(hullVertices, hullColor, tlp::Color::Black);
    hullsLayer[canvasId]->addGlEntity(graphsHulls[canvasId][g->getId()], oss.str());
  }
}

void addSubGraphsHull(const char *canvasId, tlp::Graph *g) {
  tlp::Graph *sg = NULL;
  forEach(sg, g->getSubGraphs()) {
    addGraphHull(canvasId, sg);
    addSubGraphsHull(canvasId, sg);
  }
}

void setGraphHullsToDisplay(const char *canvasId) {
  tlp::Graph *g = graph[canvasId];
  std::set<unsigned int> sgIds;

  tlp::Graph *sg = NULL;
  forEach(sg, g->getSubGraphs()) {
    sgIds.insert(sg->getId());
  }

  std::map<unsigned int, GlConcavePolygon *>::iterator it = graphsHulls[canvasId].begin();
  for (; it != graphsHulls[canvasId].end(); ++it) {
    it->second->setVisible(sgIds.find(it->first) != sgIds.end());
  }
}

void EMSCRIPTEN_KEEPALIVE setGraphsHullsVisible(const char *canvasId, bool visible, bool onTop = true) {
  glScene[canvasId]->removeLayer(hullsLayer[canvasId]);
  if (visible) {
    if (onTop) {
      glScene[canvasId]->addExistingLayerAfter(hullsLayer[canvasId], "Main");
    } else {
      glScene[canvasId]->addExistingLayerBefore(hullsLayer[canvasId], "Main");
    }
  }
}

void EMSCRIPTEN_KEEPALIVE clearGraphsHulls(const char *canvasId) {
  hullsLayer[canvasId]->clear(true);
  graphsHulls[canvasId].clear();
}

void EMSCRIPTEN_KEEPALIVE setCanvasGraph(const char *canvasId, tlp::Graph *g, bool viewTakesGraphOwnership) {

  setCurrentCanvas(canvasId);

  if (graph.find(canvasId) != graph.end() && graph[canvasId]->getRoot() != g->getRoot()) {

    clearGraphsHulls(canvasId);

    if (canDeleteGraph[canvasId]) {
      delete graph[canvasId]->getRoot();
    }
  }

  tlp::StringProperty *viewTexture = g->getProperty<tlp::StringProperty>("viewTexture");
  tlp::LayoutProperty *viewLayout = g->getProperty<tlp::LayoutProperty>("viewLayout");
  tlp::SizeProperty *viewSize = g->getProperty<tlp::SizeProperty>("viewSize");
  tlp::DoubleProperty *viewRotation = g->getProperty<tlp::DoubleProperty>("viewRotation");

  tlp::node n;
  nbTextureToLoad = 0;
  forEach(n, g->getNodes()) {
    loadTexture(viewTexture->getNodeValue(n));
  }
  viewTexture->addListener(&glDrawObserver);
  graph[canvasId] = g;
  canDeleteGraph[canvasId] = viewTakesGraphOwnership;
  graphToCanvas[g] = canvasId;
  glGraph[canvasId]->setGraph(g);

  glScene[canvasId]->centerScene(tlp::computeBoundingBox(g, viewLayout, viewSize, viewRotation));
  setGraphHullsToDisplay(canvasId);
  if (nbTextureToLoad == 0) {
    draw();
  }
}

void EMSCRIPTEN_KEEPALIVE centerScene(const char *canvasId) {
  glScene[canvasId]->centerScene();
  draw();
}

static std::vector<SelectedEntity> selectedNodes;
static std::vector<SelectedEntity> selectedEdges;

unsigned int EMSCRIPTEN_KEEPALIVE selectNodes(const char *canvasId, int x, int y, int w, int h) {
  int width = w;
  int height = h;
  if (w == 0) {
    x -= 1;
    y -= 1;
    width = 3;
    height = 3;
  }
  selectedNodes.clear();
  glScene[canvasId]->selectEntities(RenderingNodes, x, y, width, height, selectedNodes);
  return selectedNodes.size();
}

void EMSCRIPTEN_KEEPALIVE getSelectedNodes(unsigned int *nodesIds) {
  for (size_t i = 0; i < selectedNodes.size(); ++i) {
    *nodesIds++ = selectedNodes[i].getNode().id;
  }
}

unsigned int EMSCRIPTEN_KEEPALIVE selectEdges(const char *canvasId, int x, int y, int w, int h) {
  int width = w;
  int height = h;
  if (w == 0) {
    x -= 1;
    y -= 1;
    width = 3;
    height = 3;
  }
  selectedEdges.clear();
  glScene[canvasId]->selectEntities(RenderingEdges, x, y, width, height, selectedEdges);
  return selectedEdges.size();
}

void EMSCRIPTEN_KEEPALIVE getSelectedEdges(unsigned int *edgesIds) {
  for (size_t i = 0; i < selectedEdges.size(); ++i) {
    *edgesIds++ = selectedEdges[i].getEdge().id;
  }
}

void EMSCRIPTEN_KEEPALIVE startGraphViewData(const char *canvasId) {
  glGraph[canvasId]->setGraph(graph[canvasId]);
  glGraph[canvasId]->clearObservers();
}

void EMSCRIPTEN_KEEPALIVE endGraphViewData(const char *canvasId) {
  glGraph[canvasId]->initObservers();
  glGraph[canvasId]->prepareEdgesData();
  glGraph[canvasId]->computeGraphBoundingBox();
}

void EMSCRIPTEN_KEEPALIVE startGraphViewUpdate(const char *canvasId, bool clearGraph = false) {
  if (clearGraph) {
    graph[canvasId]->clear();
  }
}

void EMSCRIPTEN_KEEPALIVE endGraphViewUpdate(const char *canvasId) {
  addSubGraphsHull(canvasId, graph[canvasId]);
  setGraphHullsToDisplay(canvasId);
}

//==============================================================

GlGraphRenderingParameters *EMSCRIPTEN_KEEPALIVE getViewRenderingParameters(const char *canvasId) {
  return &(glGraph[canvasId]->getRenderingParameters());
}

void EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_setDisplayNodes(GlGraphRenderingParameters *glgrp, bool state) {
  glgrp->setDisplayNodes(state);
}

bool EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_displayNodes(GlGraphRenderingParameters *glgrp) {
  return glgrp->displayNodes();
}

void EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_setBillboardedNodes(GlGraphRenderingParameters *glgrp, bool state) {
  glgrp->setBillboardedNodes(state);
}

bool EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_billboardedNodes(GlGraphRenderingParameters *glgrp) {
  return glgrp->billboardedNodes();
}

void EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_setDisplayNodesLabels(GlGraphRenderingParameters *glgrp, bool state) {
  glgrp->setDisplayNodesLabels(state);
}

bool EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_displayNodesLabels(GlGraphRenderingParameters *glgrp) {
  return glgrp->displayNodesLabels();
}

void EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_setLabelsScaled(GlGraphRenderingParameters *glgrp, bool state) {
  glgrp->setLabelsScaled(state);
}

bool EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_labelsScaled(GlGraphRenderingParameters *glgrp) {
  return glgrp->labelsScaled();
}

void EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_setDisplayEdges(GlGraphRenderingParameters *glgrp, bool state) {
  glgrp->setDisplayEdges(state);
}

bool EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_displayEdges(GlGraphRenderingParameters *glgrp) {
  return glgrp->displayEdges();
}

void EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_setInterpolateEdgesColors(GlGraphRenderingParameters *glgrp, bool state) {
  glgrp->setInterpolateEdgesColors(state);
}

bool EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_interpolateEdgesColors(GlGraphRenderingParameters *glgrp) {
  return glgrp->interpolateEdgesColors();
}

void EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_setInterpolateEdgesSizes(GlGraphRenderingParameters *glgrp, bool state) {
  glgrp->setInterpolateEdgesSizes(state);
}

bool EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_interpolateEdgesSizes(GlGraphRenderingParameters *glgrp) {
  return glgrp->interpolateEdgesSizes();
}

void EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_setDisplayEdgesExtremities(GlGraphRenderingParameters *glgrp, bool state) {
  glgrp->setDisplayEdgesExtremities(state);
}

bool EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_displayEdgesExtremities(GlGraphRenderingParameters *glgrp) {
  return glgrp->displayEdgesExtremities();
}

void EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_setEdges3D(GlGraphRenderingParameters *glgrp, bool state) {
  glgrp->setEdges3D(state);
}

bool EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_edges3D(GlGraphRenderingParameters *glgrp) {
  return glgrp->edges3D();
}

void EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_setMinSizeOfLabels(GlGraphRenderingParameters *glgrp, float minLabelsSize) {
  glgrp->setMinSizeOfLabels(minLabelsSize);
}

float EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_minSizeOfLabels(GlGraphRenderingParameters *glgrp) {
  return glgrp->minSizeOfLabels();
}

void EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_setMaxSizeOfLabels(GlGraphRenderingParameters *glgrp, float maxLabelsSize) {
  glgrp->setMaxSizeOfLabels(maxLabelsSize);
}

float EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_maxSizeOfLabels(GlGraphRenderingParameters *glgrp) {
  return glgrp->maxSizeOfLabels();
}

void EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_setElementsOrdered(GlGraphRenderingParameters *glgrp, const bool state) {
  glgrp->setElementsOrdered(state);
}

bool EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_elementsOrdered(GlGraphRenderingParameters *glgrp) {
  return glgrp->elementsOrdered();
}

void EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_setElementOrderedDescending(GlGraphRenderingParameters *glgrp, const bool state) {
  glgrp->setElementOrderedDescending(state);
}

bool EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_elementsOrderedDescending(GlGraphRenderingParameters *glgrp) {
  return glgrp->elementsOrderedDescending();
}

void EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_setElementsOrderingProperty(GlGraphRenderingParameters *glgrp, tlp::NumericProperty *property) {
  glgrp->setElementsOrderingProperty(property);
}

tlp::NumericProperty *EMSCRIPTEN_KEEPALIVE GlGraphRenderingParameters_elementsOrderingProperty(GlGraphRenderingParameters *glgrp) {
  return glgrp->elementsOrderingProperty();
}

//==============================================================

void EMSCRIPTEN_KEEPALIVE setGraphRenderingDataReady(const char *canvasId, bool ready) {
  if (!ready) {
    clearGraphsHulls(canvasId);
  }
}

void EMSCRIPTEN_KEEPALIVE fullScreen(const char *canvasId) {
  requestFullScreenCanvas(canvasId);
}

//==============================================================

void EMSCRIPTEN_KEEPALIVE computeGraphHullVertices(tlp::Graph *graph, bool withHoles = false) {
  std::vector<std::vector<tlp::Coord>> hullVertices = ConcaveHullBuilder::computeGraphHullVertices(graph, withHoles);
  std::ostringstream oss;
  for (size_t i = 0; i < hullVertices.size(); ++i) {
    oss.str("");
    oss << "hullVertices" << i;
    graph->setAttribute(oss.str(), hullVertices[i]);
  }
}

//==============================================================

GlGraphInputData *EMSCRIPTEN_KEEPALIVE getViewInputData(const char *canvasId) {
  return &(glGraph[canvasId]->getInputData());
}

tlp::ColorProperty *EMSCRIPTEN_KEEPALIVE GlGraphInputData_getElementColor(GlGraphInputData *inputData) {
  return inputData->getElementColor();
}

void EMSCRIPTEN_KEEPALIVE GlGraphInputData_setElementColor(GlGraphInputData *inputData, tlp::ColorProperty *property) {
  inputData->setElementColor(property);
}

tlp::ColorProperty *EMSCRIPTEN_KEEPALIVE GlGraphInputData_getElementLabelColor(GlGraphInputData *inputData) {
  return inputData->getElementLabelColor();
}

void EMSCRIPTEN_KEEPALIVE GlGraphInputData_setElementLabelColor(GlGraphInputData *inputData, tlp::ColorProperty *property) {
  inputData->setElementLabelColor(property);
}

tlp::SizeProperty *EMSCRIPTEN_KEEPALIVE GlGraphInputData_getElementSize(GlGraphInputData *inputData) {
  return inputData->getElementSize();
}

void EMSCRIPTEN_KEEPALIVE GlGraphInputData_setElementSize(GlGraphInputData *inputData, tlp::SizeProperty *property) {
  inputData->setElementSize(property);
}

tlp::IntegerProperty *EMSCRIPTEN_KEEPALIVE GlGraphInputData_getElementLabelPosition(GlGraphInputData *inputData) {
  return inputData->getElementLabelPosition();
}

void EMSCRIPTEN_KEEPALIVE GlGraphInputData_setElementLabelPosition(GlGraphInputData *inputData, tlp::IntegerProperty *property) {
  inputData->setElementLabelPosition(property);
}

tlp::IntegerProperty *EMSCRIPTEN_KEEPALIVE GlGraphInputData_getElementShape(GlGraphInputData *inputData) {
  return inputData->getElementShape();
}

void EMSCRIPTEN_KEEPALIVE GlGraphInputData_setElementShape(GlGraphInputData *inputData, tlp::IntegerProperty *property) {
  inputData->setElementShape(property);
}

tlp::DoubleProperty *EMSCRIPTEN_KEEPALIVE GlGraphInputData_getElementRotation(GlGraphInputData *inputData) {
  return inputData->getElementRotation();
}

void EMSCRIPTEN_KEEPALIVE GlGraphInputData_setElementRotation(GlGraphInputData *inputData, tlp::DoubleProperty *property) {
  inputData->setElementRotation(property);
}

tlp::BooleanProperty *EMSCRIPTEN_KEEPALIVE GlGraphInputData_getElementSelection(GlGraphInputData *inputData) {
  return inputData->getElementSelection();
}

void EMSCRIPTEN_KEEPALIVE GlGraphInputData_setElementSelection(GlGraphInputData *inputData, tlp::BooleanProperty *property) {
  inputData->setElementSelection(property);
}

tlp::StringProperty *EMSCRIPTEN_KEEPALIVE GlGraphInputData_getElementFont(GlGraphInputData *inputData) {
  return inputData->getElementFont();
}

void EMSCRIPTEN_KEEPALIVE GlGraphInputData_setElementFont(GlGraphInputData *inputData, tlp::StringProperty *property) {
  inputData->setElementFont(property);
}

tlp::IntegerProperty *EMSCRIPTEN_KEEPALIVE GlGraphInputData_getElementFontSize(GlGraphInputData *inputData) {
  return inputData->getElementFontSize();
}

void EMSCRIPTEN_KEEPALIVE GlGraphInputData_setElementFontSize(GlGraphInputData *inputData, tlp::IntegerProperty *property) {
  inputData->setElementFontSize(property);
}

tlp::StringProperty *EMSCRIPTEN_KEEPALIVE GlGraphInputData_getElementLabel(GlGraphInputData *inputData) {
  return inputData->getElementLabel();
}

void EMSCRIPTEN_KEEPALIVE GlGraphInputData_setElementLabel(GlGraphInputData *inputData, tlp::StringProperty *property) {
  inputData->setElementLabel(property);
}

tlp::LayoutProperty *EMSCRIPTEN_KEEPALIVE GlGraphInputData_getElementLayout(GlGraphInputData *inputData) {
  return inputData->getElementLayout();
}

void EMSCRIPTEN_KEEPALIVE GlGraphInputData_setElementLayout(GlGraphInputData *inputData, tlp::LayoutProperty *property) {
  inputData->setElementLayout(property);
}

tlp::StringProperty *EMSCRIPTEN_KEEPALIVE GlGraphInputData_getElementTexture(GlGraphInputData *inputData) {
  return inputData->getElementTexture();
}

void EMSCRIPTEN_KEEPALIVE GlGraphInputData_setElementTexture(GlGraphInputData *inputData, tlp::StringProperty *property) {
  inputData->setElementTexture(property);
}

tlp::ColorProperty *EMSCRIPTEN_KEEPALIVE GlGraphInputData_getElementBorderColor(GlGraphInputData *inputData) {
  return inputData->getElementBorderColor();
}

void EMSCRIPTEN_KEEPALIVE GlGraphInputData_setElementBorderColor(GlGraphInputData *inputData, tlp::ColorProperty *property) {
  inputData->setElementBorderColor(property);
}

tlp::DoubleProperty *EMSCRIPTEN_KEEPALIVE GlGraphInputData_getElementBorderWidth(GlGraphInputData *inputData) {
  return inputData->getElementBorderWidth();
}

void EMSCRIPTEN_KEEPALIVE GlGraphInputData_setElementBorderWidth(GlGraphInputData *inputData, tlp::DoubleProperty *property) {
  inputData->setElementBorderWidth(property);
}

tlp::IntegerProperty *EMSCRIPTEN_KEEPALIVE GlGraphInputData_getElementSrcAnchorShape(GlGraphInputData *inputData) {
  return inputData->getElementSrcAnchorShape();
}

void EMSCRIPTEN_KEEPALIVE GlGraphInputData_setElementSrcAnchorShape(GlGraphInputData *inputData, tlp::IntegerProperty *property) {
  inputData->setElementSrcAnchorShape(property);
}

tlp::SizeProperty *EMSCRIPTEN_KEEPALIVE GlGraphInputData_getElementSrcAnchorSize(GlGraphInputData *inputData) {
  return inputData->getElementSrcAnchorSize();
}

void EMSCRIPTEN_KEEPALIVE GlGraphInputData_setElementSrcAnchorSize(GlGraphInputData *inputData, tlp::SizeProperty *property) {
  inputData->setElementSrcAnchorSize(property);
}

tlp::IntegerProperty *EMSCRIPTEN_KEEPALIVE GlGraphInputData_getElementTgtAnchorShape(GlGraphInputData *inputData) {
  return inputData->getElementTgtAnchorShape();
}

void EMSCRIPTEN_KEEPALIVE GlGraphInputData_setElementTgtAnchorShape(GlGraphInputData *inputData, tlp::IntegerProperty *property) {
  inputData->setElementTgtAnchorShape(property);
}

tlp::SizeProperty *EMSCRIPTEN_KEEPALIVE GlGraphInputData_getElementTgtAnchorSize(GlGraphInputData *inputData) {
  return inputData->getElementTgtAnchorSize();
}

void EMSCRIPTEN_KEEPALIVE GlGraphInputData_setElementTgtAnchorSize(GlGraphInputData *inputData, tlp::SizeProperty *property) {
  inputData->setElementTgtAnchorSize(property);
}

tlp::StringProperty *EMSCRIPTEN_KEEPALIVE GlGraphInputData_getElementFontAwesomeIcon(GlGraphInputData *inputData) {
  return inputData->getElementFontAwesomeIcon();
}

void EMSCRIPTEN_KEEPALIVE GlGraphInputData_setElementFontAwesomeIcon(GlGraphInputData *inputData, tlp::StringProperty *property) {
  inputData->setElementFontAwesomeIcon(property);
}

tlp::BooleanProperty *EMSCRIPTEN_KEEPALIVE GlGraphInputData_getElementGlow(GlGraphInputData *inputData) {
  return inputData->getElementGlow();
}

void EMSCRIPTEN_KEEPALIVE GlGraphInputData_setElementGlow(GlGraphInputData *inputData, tlp::BooleanProperty *property) {
  inputData->setElementGlow(property);
}

tlp::ColorProperty *EMSCRIPTEN_KEEPALIVE GlGraphInputData_getElementGlowColor(GlGraphInputData *inputData) {
  return inputData->getElementGlowColor();
}

void EMSCRIPTEN_KEEPALIVE GlGraphInputData_setElementGlowColor(GlGraphInputData *inputData, tlp::ColorProperty *property) {
  inputData->setElementGlowColor(property);
}

void EMSCRIPTEN_KEEPALIVE GlGraphInputData_reloadGraphProperties(GlGraphInputData *inputData, bool resetDefaulProperties = false) {
  inputData->reloadGraphProperties(resetDefaulProperties);
}

//==============================================================

Camera *EMSCRIPTEN_KEEPALIVE getViewCamera(const char *canvasId) {
  return glScene[canvasId]->getLayer("Main")->getCamera();
}

void EMSCRIPTEN_KEEPALIVE Camera_getViewport(Camera *camera, int *buffer) {
  tlp::Vec4i vp = camera->getViewport();
  for (int i = 0; i < 4; ++i) {
    buffer[i] = vp[i];
  }
}

void EMSCRIPTEN_KEEPALIVE Camera_modelViewMatrix(Camera *camera, float *buffer) {
  const tlp::MatrixGL &mdvMat = camera->modelviewMatrix();
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      buffer[i * 4 + j] = mdvMat[i][j];
    }
  }
}

void EMSCRIPTEN_KEEPALIVE Camera_setModelViewMatrix(Camera *camera, float *buffer) {
  tlp::MatrixGL mdvMat;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      mdvMat[i][j] = buffer[i * 4 + j];
    }
  }
  camera->setModelViewMatrix(mdvMat);
}

void EMSCRIPTEN_KEEPALIVE Camera_projectionMatrix(Camera *camera, float *buffer) {
  const tlp::MatrixGL &projMat = camera->projectionMatrix();
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      buffer[i * 4 + j] = projMat[i][j];
    }
  }
}

void EMSCRIPTEN_KEEPALIVE Camera_setProjectionMatrix(Camera *camera, float *buffer) {
  tlp::MatrixGL projMat;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      projMat[i][j] = buffer[i * 4 + j];
    }
  }
  camera->setProjectionMatrix(projMat);
}

//==============================================================

void EMSCRIPTEN_KEEPALIVE setViewBackgroundColor(const char *canvasId, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
  glScene[canvasId]->setBackgroundColor(tlp::Color(r, g, b, a));
  glScene[canvasId]->requestDraw();
}

void EMSCRIPTEN_KEEPALIVE setViewBackupBackBuffer(const char *canvasId, bool backup) {
  glScene[canvasId]->setBackupBackBuffer(backup);
}
}
