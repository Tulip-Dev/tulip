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
#include <tulip/GlScene.h>
#include <tulip/GlLayer.h>
#include <tulip/TlpTools.h>

using namespace std;
using namespace tlp;
 
static int win;

unsigned int frameCount = 5;
GLuint LList;
bool listOk = false;
//==============================================================================
class GLGlut {
public:
  GLGlut(string name, const int width=640, const int height=480) {}
  virtual ~GLGlut(){}
  void setupOpenGlContext() {
    glutSetWindow(win);
  }

  GlScene scene;
  private:
  int width,height;
};
//=============================================
static int width(640), height(480);
static GLGlut *glGlutScreen;
static int frame=0;
static GLint timer;
static int rx = 0,ry = 0,rz = 0;
static bool frameRateDisplaying=false;
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

static void setRasterPosition(unsigned int x, unsigned int y) {
  glRasterPos2f(x, y);
}
//=============================================
static void printMessage(string str,bool b) {
  cout << str << " => " << (b ? "On" : "Off") << endl;
}
//=============================================
static void Key(unsigned char key, int x, int y) {
  GlGraphRenderingParameters param = glGlutScreen->scene.getGlGraphComposite()->getRenderingParameters();
  switch (key) {
  case '1':
    cerr << "backup" << endl;
    //    glutFullScreen();
    glReadBuffer(GL_BACK);
    glDrawBuffer(GL_AUX0);
    setRasterPosition(0,0);
    glCopyPixels(0,0,width, height, GL_COLOR);
    glDrawBuffer(GL_BACK);
    break;
  case '2':
    cerr << "restore" << endl;
    int i;
    glDrawBuffer(GL_BACK);
    glReadBuffer(GL_AUX0);
    setRasterPosition(0,0);
    glCopyPixels(0,0,width, height, GL_COLOR);
    glutSwapBuffers();
    cin >> i;
    break;
  case 27:
    exit(1);
  case '+':
    glGlutScreen->scene.zoom(2);
    break;
  case '-':
    glGlutScreen->scene.zoom(-2);
    break;
  case 'x':
    rx=(rx+1)%2;
    break;
  case 'y':
    ry=(ry+1)%2;
    break;
  case 'z':
    rz=(rz+1)%2;
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
    frameRateDisplaying=!frameRateDisplaying;
    printMessage("Display frame rate",frameRateDisplaying);
    frame=0;
    timer=glutGet(GLUT_ELAPSED_TIME);
    glutSetWindowTitle("Tulip Glut Viewer");
    break;
  default:
    return;
  }
  glGlutScreen->scene.getGlGraphComposite()->setRenderingParameters(param);
}
//=============================================
void Reshape(int widt, int heigh) {
  //cerr << __PRETTY_FUNCTION__ << endl;
  width=widt;
  height=heigh;
  Vector<int, 4> viewport;
  viewport[0] = 0;
  viewport[1] = 0;
  viewport[2] = widt;
  viewport[3] = heigh;
  glGlutScreen->scene.setViewport(viewport);
}
//=============================================
void Draw(void) {
  glGlutScreen->scene.draw();
  glutSwapBuffers();
  if (frameRateDisplaying) frame++;
}
void helpMessage() {
  cout << "********************************************" <<endl;
  cout << "Glut graph viewer demo" << endl;
  cout << "Author : Auber David 29/01/2003" << endl;
  cout << "********************************************" <<endl;
  cout << "key :" << endl;
  cout << "\t x\t: (de)Activate X axis rotation" << endl;
  cout << "\t y\t: (de)Activate Y axis rotation" << endl;
  cout << "\t z\t: (de)Activate Z axis rotation" << endl;
  cout << "\t +/-: zoom" << endl;
  cout << "\t m\t: (de)Activate metric ordering" << endl;
  cout << "\t l\t: (de)Activate labels" << endl;
  cout << "\t a\t: (de)Activate arrows" << endl;
  cout << "\t E\t: (de)Activate edge rendering" << endl;
  cout << "\t e\t: (de)Activate 3D edges" << endl;
  cout << "\t o\t: (de)Activate Orthogonal projection" << endl;
  cout << "\t b\t: (de)Activate Frame rate displaying" << endl;
  cout << "\t 1\t: (de)Activate full screen" << endl;
  cout << "\t esc: quit" << endl;
  cout << "**********************************************" <<endl;
}
void usage() {
  cerr << "usage :" << endl;
  cerr << "\t glutviewer <filename>" << endl;
  cerr << "file must be in tlp format" << endl;
  exit(1);
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
    cerr << "Unable to create the new window" << endl;
    exit(1);
  }

  PluginLoaderTxt plug;

  tlp::initTulipLib();
  // library side plugins
  //tlp::loadPlugins(&plug);

  // glyph plugins
  GlyphManager::getInst().loadPlugins(&plug);

  GlDisplayListManager::getInst().changeContext(0);
  GlTextureManager::getInst().changeContext(0);

  glGlutScreen = new GLGlut("", width, height);
  glutIdleFunc(idle);
  Graph *graph = tlp::loadGraph(argv[1]);

  GlLayer *layer=new GlLayer("Main");

  glGlutScreen->scene.addLayer(layer);

  GlGraphComposite* graphComposite=new GlGraphComposite(graph);
  glGlutScreen->scene.addGlGraphCompositeInfo(glGlutScreen->scene.getLayer("Main"),graphComposite);
  glGlutScreen->scene.getLayer("Main")->addGlEntity(graphComposite,"graph");
  glGlutScreen->scene.centerScene();

  Reshape(640,480);
  
  //glGlutScreen->centerScene();
  timer = glutGet(GLUT_ELAPSED_TIME);
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Key);
  glutDisplayFunc(Draw);
  helpMessage();
  glutMainLoop();  
  return EXIT_SUCCESS;
}
