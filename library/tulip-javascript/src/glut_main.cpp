#include <GL/glew.h>

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

#include <tulip/GlGraph.h>
#include <tulip/Camera.h>
#include <tulip/Light.h>
#include <tulip/GlRect2D.h>

#include "Interactors.h"

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

#include <GL/glut.h>

#include <QFileDialog>
#include <QApplication>

using namespace tlp;

static int CURRENT_WIDTH = 1024;
static int CURRENT_HEIGHT = 768;

static tlp::Color backgroundColor(255, 255, 255);
static GlScene *glScene;
static GlGraph *glGraph;
static tlp::Graph *graph;

static GlSceneInteractor *currentInteractor = NULL;

static void (*animFunc)(void *);
static void *animFuncParam;

static void animationFunc(int) {
  animFunc(animFuncParam);
}

void timerFunc(unsigned int msecs, void (*func)(void *value), void *value) {
  animFunc = func;
  animFuncParam = value;
  glutTimerFunc(msecs, animationFunc, 0);
}

void activateInteractor(const std::string &interactorName) {
  if (interactorsMap.find(interactorName) == interactorsMap.end()) {
    std::cerr << "Error : no interactor named \"" << interactorName << "\"" << std::endl;
  } else {
    if (currentInteractor) {
      currentInteractor->desactivate();
    }
    currentInteractor = interactorsMap[interactorName];
    currentInteractor->setScene(glScene);
    currentInteractor->activate();
  }
}

static void glutReshapeCallback(int width, int height) {
  CURRENT_WIDTH = width;
  CURRENT_HEIGHT = height;
  tlp::Vec4i viewport(0, 0, width, height);
  glScene->setViewport(viewport);
  glutPostRedisplay();
}

static void glutDrawCallback(void) {
  glScene->draw();
  currentInteractor->draw();
  glutSwapBuffers();
  checkOpenGLError();
}

static void mouseCallback(int button, int state, int x, int y) {
  MouseButton buttonVal = LEFT_BUTTON;
  MouseButtonState buttonStateVal = DOWN;
  if (state == GLUT_DOWN) {
    buttonStateVal = DOWN;
  } else {
    buttonStateVal = UP;
  }

  if (button == GLUT_LEFT_BUTTON) {
    buttonVal = LEFT_BUTTON;
  } else if (button == GLUT_RIGHT_BUTTON) {
    buttonVal = RIGHT_BUTTON;
  } else if (button == 3) {
    buttonVal = WHEEL;
    buttonStateVal = UP;
  } else if (button == 4) {
    buttonVal = WHEEL;
    buttonStateVal = DOWN;
  }
  currentInteractor->mouseCallback(buttonVal, buttonStateVal, x, y, glutGetModifiers());
}

static tlp::Graph *loadGraph(const char *filename) {
  tlp::Graph *g = NULL;
  if (filename) {
    g = tlp::loadGraph(filename);
  }
  if (g) {
    std::cout << "Graph " << filename << std::endl;
    std::cout << "- Number nodes : " << g->numberOfNodes() << std::endl;
    std::cout << "- Number edges : " << g->numberOfEdges() << std::endl;
    tlp::StringProperty *viewTexture = g->getProperty<tlp::StringProperty>("viewTexture");
    tlp::node n;
    forEach(n, g->getNodes()) {
      std::string texture = viewTexture->getNodeValue(n);
      if (!texture.empty()) {
        GlTextureManager::instance()->addTextureInAtlasFromFile(texture);
      }
    }
  }
  return g;
}

static void keyboardCallback(const unsigned char key, const int, const int) {
  if (key == '1') {
    activateInteractor("ZoomAndPan");
    glutPostRedisplay();
  } else if (key == '2') {
    activateInteractor("RectangleZoom");
    glutPostRedisplay();
  } else if (key == '3') {
    activateInteractor("Selection");
    glutPostRedisplay();
  } else if (key == '4') {
    activateInteractor("LassoSelection");
    glutPostRedisplay();
  } else if (key == '5') {
    activateInteractor("Neighborhood");
    glutPostRedisplay();
  } else if (key == '6') {
    activateInteractor("SelectionModifier");
    glutPostRedisplay();
  } else if (key == '7') {
    activateInteractor("Fisheye");
    glutPostRedisplay();
  } else if (key == 'o') {
    QString graphFile = QFileDialog::getOpenFileName(0, "open graph file", ".", "Tulip graph (*.tlp *.tlp.gz)");
    if (!graphFile.isEmpty()) {
      tlp::Graph *g = loadGraph(graphFile.toStdString().c_str());
      if (g) {
        delete graph;
        graph = g;
        glGraph->setGraph(graph);
        glScene->centerScene();
        glutPostRedisplay();
      }
    }
  } else {
    std::string keyStr;
    keyStr.push_back(key);
    currentInteractor->keyboardCallback(keyStr, glutGetModifiers());
  }
}

static void specialKeyboardCallback(int special, int, int) {

  std::string specialKey;

  switch (special) {

  case GLUT_KEY_F1: {
    specialKey = "F1";
    break;
  }
  case GLUT_KEY_F2: {
    specialKey = "F2";
    break;
  }
  case GLUT_KEY_F3: {
    specialKey = "F3";
    break;
  }
  case GLUT_KEY_F4: {
    specialKey = "F4";
    break;
  }
  case GLUT_KEY_F5: {
    specialKey = "F5";
    break;
  }
  case GLUT_KEY_F6: {
    specialKey = "F6";
    break;
  }
  case GLUT_KEY_F7: {
    specialKey = "F7";
    break;
  }
  case GLUT_KEY_F8: {
    specialKey = "F8";
    break;
  }
  case GLUT_KEY_F9: {
    specialKey = "F9";
    break;
  }
  case GLUT_KEY_F10: {
    specialKey = "F10";
    break;
  }
  case GLUT_KEY_F11: {
    specialKey = "F11";
    break;
  }
  case GLUT_KEY_F12: {
    specialKey = "F12";
    break;
  }
  case GLUT_KEY_LEFT: {
    specialKey = "Left";
    break;
  }
  case GLUT_KEY_UP: {
    specialKey = "Up";
    break;
  }
  case GLUT_KEY_RIGHT: {
    specialKey = "Right";
    break;
  }
  case GLUT_KEY_DOWN: {
    specialKey = "Down";
    break;
  }
  case GLUT_KEY_PAGE_UP: {
    specialKey = "PageUp";
    break;
  }
  case GLUT_KEY_PAGE_DOWN: {
    specialKey = "PageDown";
    break;
  }
  case GLUT_KEY_HOME: {
    specialKey = "Home";
    break;
  }
  case GLUT_KEY_END: {
    specialKey = "End";
    break;
  }
  default: {
    specialKey = "Insert";
    break;
  }
  }
  currentInteractor->keyboardCallback(specialKey, glutGetModifiers());
}

static void mouseMoveCallback(int x, int y) {
  currentInteractor->mouseMoveCallback(x, y, glutGetModifiers());
}

class GlDrawObserver : public tlp::Observable {

public:
  void treatEvent(const tlp::Event &event) {
    const GlSceneEvent *glSceneEvent = dynamic_cast<const GlSceneEvent *>(&event);

    if (glSceneEvent) {
      if (glSceneEvent->getType() == GlSceneEvent::DRAW_REQUEST) {
        glutPostRedisplay();
      }
    }
  }
};

static GlDrawObserver glDrawObserver;

//==============================================================
int main(int argc, char *argv[]) {

  QApplication app(argc, argv);

  tlp::initTulipLib();
  tlp::PluginLibraryLoader::loadPlugins();

  glutInit(&argc, argv);
  glutInitWindowSize(CURRENT_WIDTH, CURRENT_HEIGHT);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
  if ((glutCreateWindow("Tulip Glut Viewer")) == GL_FALSE) {
    std::cerr << "Unable to create an OpenGl Glut window" << std::endl;
    exit(EXIT_FAILURE);
  }

  glutDisplayFunc(glutDrawCallback);
  glutReshapeFunc(glutReshapeCallback);
  glutSpecialFunc(specialKeyboardCallback);
  glutMouseFunc(mouseCallback);
  glutKeyboardFunc(keyboardCallback);
  glutMotionFunc(mouseMoveCallback);
  glutPassiveMotionFunc(mouseMoveCallback);

  glewInit();

  tlp::Vec4i viewport(0, 0, CURRENT_WIDTH, CURRENT_HEIGHT);

  glScene = new GlScene();
  glScene->addListener(&glDrawObserver);

  glScene->setViewport(viewport);

  glGraph = new GlGraph();
  glScene->getMainLayer()->addGlEntity(glGraph, "graph");

  activateInteractor("ZoomAndPan");

  if (argc > 1) {
    graph = loadGraph(argv[1]);
  } else {
    std::cout << "Usage : " << argv[0] << " <graph file>" << std::endl;
  }
  if (graph) {
    glGraph->setGraph(graph);
    glScene->centerScene();
    glutMainLoop();
  }

  return 0;
}
