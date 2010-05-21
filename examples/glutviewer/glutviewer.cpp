#include <iostream>

#include <stdio.h>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <tulip/Graph.h>
#include <tulip/GlyphManager.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/PluginLoaderTxt.h>
#include <tulip/GlTextureManager.h>
#include <tulip/EdgeExtremityGlyphManager.h>
#include <tulip/GlScene.h>
#include <tulip/GlLayer.h>
#include <tulip/TlpTools.h>

using namespace std;
using namespace tlp;
 
static int win;
const unsigned int WIN_SIZE = 500;
unsigned int frameCount = 5;
GLuint LList;
bool listOk = false;
//==============================================================================
class GLGlut {
public:
  GLGlut(string name, const int width = WIN_SIZE, const int height = WIN_SIZE) {}
  virtual ~GLGlut(){}
  void setupOpenGlContext() {
    glutSetWindow(win);
  }

  GlScene scene;
  private:
  int width,height;
};
//=============================================
static int width(WIN_SIZE), height(WIN_SIZE);
static GLGlut *glGlutScreen;
static int frame = 0;
static GLint timer;
static int rx = 0, ry = 0, rz = 0;
static bool frameRateDisplaying = false;
static char strFrameRate[50] = {0};      

using namespace tlp;
//=============================================
void idle(void) {  
  glGlutScreen->scene.rotateScene(rx*2, ry*2, rz*2);
  if (frameRateDisplaying) {
    if (frame%frameCount == 0) {
      GLint t = glutGet(GLUT_ELAPSED_TIME);
      GLfloat seconds = (t - timer) / 1000.0;
      GLfloat fps = frame / seconds;
      frameCount = int (fps + 5);
      sprintf(strFrameRate, "Tulip Glut Viewer: fps = %.2f", fps);
      glutSetWindowTitle(strFrameRate);
      frame = 0;
      timer = t;
    }
  }
  glutPostRedisplay();
}
//=============================================
static void printMessage(const string &str, const bool b) {
  cout << str << " => " << (b ? "On" : "Off") << endl;
}
//=============================================
static void changeOption(const int key) {
  GlGraphRenderingParameters param = glGlutScreen->scene.getGlGraphComposite()->getRenderingParameters();
  switch (key) {
  case '1':
    glutFullScreen();
    break;
  case 27:
    exit(EXIT_SUCCESS);
  case '+':
    glGlutScreen->scene.zoom(2);
    break;
  case '-':
    glGlutScreen->scene.zoom(-2);
    break;
  case 'x':
    rx = (rx+1)%2;
    break;
  case 'y':
    ry = (ry+1)%2;
    break;
  case 'z':
    rz = (rz+1)%2;
    break;
  case 'e':
    param.setEdge3D(!param.isEdge3D());
    printMessage("3D edges", param.isEdge3D());
    break;
  case 'c':
    param.setEdgeColorInterpolate(!param.isEdgeColorInterpolate());
    printMessage("Edge color interpolate", param.isEdgeColorInterpolate());
    break;
  case 'a':
    param.setViewArrow(!param.isViewArrow());
    printMessage("Arrow ",param.isViewArrow());
    break;
  case 'o':
    glGlutScreen->scene.setViewOrtho(!glGlutScreen->scene.isViewOrtho());
    printMessage("Projection orthogonal",glGlutScreen->scene.isViewOrtho());
    break;
  case 'l':
    param.setViewNodeLabel(!param.isViewNodeLabel());
    printMessage("Labels",param.isViewNodeLabel());
    param.setFontsType(1);
    break;
  case 'm':
    param.setElementOrdered(!param.isElementOrdered());
    printMessage("Metric ordering",param.isElementOrdered());
    break;
  case 'E':
    param.setDisplayEdges(!param.isDisplayEdges());
    printMessage("Edge displaying", param.isDisplayEdges());
    break;
  case 'b':
    frameRateDisplaying = !frameRateDisplaying;
    printMessage("Display frame rate",frameRateDisplaying);
    frame = 0;
    timer = glutGet(GLUT_ELAPSED_TIME);
    glutSetWindowTitle("Tulip Glut Viewer");
    break;
  default:
    return;
  }
  glGlutScreen->scene.getGlGraphComposite()->setRenderingParameters(param);
}
//=============================================
static void Key(const unsigned char key, const int x, const int y) {
  changeOption(key);
}
//=============================================
void Reshape(int widt, int heigh) {
  //cerr << __PRETTY_FUNCTION__ << endl;
  width = widt;
  height = heigh;
  Vector<int, 4> viewport;
  viewport[0] = 0;
  viewport[1] = 0;
  viewport[2] = width;
  viewport[3] = height;
  glGlutScreen->scene.setViewport(viewport);
}
//=============================================
void Draw(void) {
  glGlutScreen->scene.draw();
  glutSwapBuffers();
  if (frameRateDisplaying) ++frame;
}
//=============================================
void helpMessage() {
  cout << "********************************************" <<endl;
  cout << "Glut graph viewer demo" << endl;
  cout << "Author : Auber David 29/01/2003" << endl;
  cout << "********************************************" <<endl;
  cout << "The TLP_DIR environnment variable must be set to the tulip install dir" << endl;
  cout << "for instance /usr/local/lib/" << endl;
  cout << "key :" << endl;
  cout << "\t x\t: (de)Activate X axis rotation" << endl;
  cout << "\t y\t: (de)Activate Y axis rotation" << endl;
  cout << "\t z\t: (de)Activate Z axis rotation" << endl;
  cout << "\t +/-\t: zoom" << endl;
  cout << "\t m\t: (de)Activate metric ordering" << endl;
  cout << "\t l\t: (de)Activate labels" << endl;
  cout << "\t a\t: (de)Activate arrows" << endl;
  cout << "\t E\t: (de)Activate edge rendering" << endl;
  cout << "\t e\t: (de)Activate 3D edges" << endl;
  cout << "\t o\t: (de)Activate Orthogonal projection" << endl;
  cout << "\t b\t: (de)Activate Frame rate displaying" << endl;
  cout << "\t 1\t: (de)Activate full screen" << endl;
  cout << "\t esc\t: quit" << endl;
  cout << "menu :" << endl;
  cout << "\t Right click on the view to display options" << endl;
  cout << "**********************************************" <<endl;
}
//=============================================
void usage() {
  cerr << "usage :" << endl;
  cerr << "\t glutviewer <filename>" << endl;
  cerr << "file must be in tlp format (.tlp or tlp.gz)" << endl;
  exit(EXIT_FAILURE);
}
/***************************************************************************************************/
int main (int argc, char **argv) {
  glutInit(&argc, argv);
  if (argc!=2) 
    usage();
  glutInitWindowPosition(0, 0); 
  glutInitWindowSize( width, height);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL);
  if ((win=glutCreateWindow("Tulip Glut Viewer")) == GL_FALSE) {
    cerr << "Unable to create a OpenGl Glut window" << endl;
    exit(EXIT_FAILURE);
  }

  tlp::initTulipLib();
  PluginLoaderTxt plug;
  GlyphManager::getInst().loadPlugins(&plug);
  EdgeExtremityGlyphManager::getInst().loadPlugins(&plug);

  GlDisplayListManager::getInst().changeContext(0);
  GlTextureManager::getInst().changeContext(0);

  glGlutScreen = new GLGlut("", width, height);
  glutIdleFunc(idle);
  Graph *graph = tlp::loadGraph(argv[1]);

  GlLayer *layer=new GlLayer("Main");
  glGlutScreen->scene.addLayer(layer);

  GlGraphComposite* graphComposite = new GlGraphComposite(graph);
  glGlutScreen->scene.addGlGraphCompositeInfo(glGlutScreen->scene.getLayer("Main"), graphComposite);
  glGlutScreen->scene.getLayer("Main")->addGlEntity(graphComposite, "graph");
  glGlutScreen->scene.centerScene();

  Reshape(500, 500);
  
  //glGlutScreen->centerScene();
  timer = glutGet(GLUT_ELAPSED_TIME);
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Key);
  glutDisplayFunc(Draw);

  changeOption('b');
  
  glutCreateMenu(changeOption);
  glutAddMenuEntry("X Rotation (x)", 'x');
  glutAddMenuEntry("Y Rotation (y)", 'y');
  glutAddMenuEntry("Z Rotation (z)", 'z');
  glutAddMenuEntry("Zoom in (+)",    '+');
  glutAddMenuEntry("Zoom out (-)",   '-');
  glutAddMenuEntry("Metric Ordering (m)", 'm');
  glutAddMenuEntry("Labels (l)",    'l');
  glutAddMenuEntry("Arrows (a)",   '-');
  glutAddMenuEntry("Edge Rendering (E)",   'E');
  glutAddMenuEntry("3D Edges (e)",   'e');
  glutAddMenuEntry("Projection (o)",    'o');
  glutAddMenuEntry("Frame rate (b)",    'b');
  glutAddMenuEntry("Full screen (1)",   '1');
  glutAddMenuEntry("exit (esc)",   27);
  
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  helpMessage();
  glutMainLoop();  
  
  return EXIT_SUCCESS;
}
