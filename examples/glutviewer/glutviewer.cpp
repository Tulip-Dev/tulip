#include <iostream>

#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include <tulip/Graph.h>
#include <tulip/TlpTools.h>
#include <tulip/Glyph.h>
#include <tulip/GlGraph.h>
#include <tulip/ForEach.h>

using namespace std;
using namespace tlp;
 
static int win;

unsigned int frameCount = 5;
GLuint LList;
bool listOk = false;
//==============================================================================
class GLGlut: public GlGraph {
public:
  GLGlut(string name, const int width=640, const int height=480) {
    drawing=false;
  }
  virtual ~GLGlut(){}
  void setupOpenGlContext() {
    glutSetWindow(win);
  }
  void draw_handler(bool b) {
    drawing = b;
  }
  void timerCallBack() {
    if (!drawing) return;
    drawing = !drawPart();
    glutSwapBuffers();
  }
  bool drawing;
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
static bool incremental=false;
static char strFrameRate[50] = {0};      

using namespace tlp;
//=============================================
void idle(void) {  
  glGlutScreen->timerCallBack();
  if (!glGlutScreen->drawing) {
    glGlutScreen->rotateScene(rx*2., ry*2., rz*2.);
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
}

static void setRasterPosition(unsigned int x, unsigned int y) {
  float *val;
  unsigned char *tmp;
  glGetFloatv(GL_CURRENT_RASTER_POSITION, val);
  glBitmap(0,0,0,0,-val[0] + x, -val[1] + y, tmp);
}
//=============================================
static void printMessage(string str,bool b) {
  cout << str << " => " << (b ? "On" : "Off") << endl;
}
//=============================================
static void Key(unsigned char key, int x, int y) {
  GlGraphRenderingParameters param = glGlutScreen->getRenderingParameters();
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
    glGlutScreen->zoom(2);
    break;
  case '-':
    glGlutScreen->zoom(-2);
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
  case 'a':
    param.setViewArrow(!param.isViewArrow());
    printMessage("Arrow ",param.isViewArrow());
    break;
  case 'o':
    param.setViewOrtho(!param.isViewOrtho());
    printMessage("Projection orthogonal",param.isViewOrtho());
    break;
  case 'l':
    param.setViewNodeLabel(!param.isViewNodeLabel());
    printMessage("Labels",param.isViewNodeLabel());
    param.setFontsType(1);
    break;
  case 's':
    param.setElementOrdered(!param.isElementOrdered());
    printMessage("StrahlerMode",param.isElementOrdered());
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
  case 'I':
    incremental = !incremental;
    param.setIncrementalRendering(!param.isIncrementalRendering());
    printMessage("Incremental rendering", param.isIncrementalRendering());
    break;
  default:
    return;
  }
  glGlutScreen->setRenderingParameters(param);
}
//==============================================================================
void importGraph(const string &filename, GlGraph &glGraph) {
  DataSet dataSet;
  dataSet.set("file::filename", filename);
  Graph *newGraph = tlp::importGraph("tlp", dataSet, NULL);
  if (newGraph != 0) {
    GlGraphRenderingParameters param = glGraph.getRenderingParameters();
    param.setGraph(newGraph);
    glGraph.setRenderingParameters(param);

    DataSet glGraphData;
    if (dataSet.get<DataSet>("displaying", glGraphData)){
      param.setParameters(glGraphData);
      glGraph.setRenderingParameters(param);
    }
    else
      glGraph.centerScene();
  }
}
//=============================================
void Reshape(int widt, int heigh) {
  cerr << __PRETTY_FUNCTION__ << endl;
  width=widt;
  height=heigh;
  GlGraphRenderingParameters param = glGlutScreen->getRenderingParameters();
  Vector<int, 4> viewport;
  viewport[0] = 0;
  viewport[1] = 0;
  viewport[2] = widt;
  viewport[3] = heigh;
  param.setViewport(viewport);
  glGlutScreen->setRenderingParameters(param);
}
//=============================================
void Draw(void) {
  glGlutScreen->draw();
  if (!glGlutScreen->getRenderingParameters().isIncrementalRendering()) 
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
  cout << "\t s\t: (de)Activate strahler ordering" << endl;
  cout << "\t l\t: (de)Activate labels" << endl;
  cout << "\t a\t: (de)Activate arrows" << endl;
  cout << "\t E\t: (de)Activate edge rendering" << endl;
  cout << "\t e\t: (de)Activate 3D edges" << endl;
  cout << "\t o\t: (de)Activate Orthogonal projection" << endl;
  cout << "\t b\t: (de)Activate Frame rate displaying" << endl;
  cout << "\t 1\t: (de)Activate full screen" << endl;
  cout << "\t I\t: (de)Activate incremental rendering" << endl;
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
    cerr << "Impossible to create the new window" << endl;
    exit(1);
  }

  helpMessage();

  tlp::initTulipLib();
  tlp::loadPlugins();   // library side plugins
  GlGraph::loadPlugins();

  glGlutScreen = new GLGlut("", width, height);
  glutIdleFunc(idle);
  importGraph(argv[1], *glGlutScreen);
  glGlutScreen->centerScene();
  timer = glutGet(GLUT_ELAPSED_TIME);
  GlGraphRenderingParameters param = glGlutScreen->getRenderingParameters();
  param.setIncrementalRendering(false);
  glGlutScreen->setRenderingParameters(param);
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Key);
  glutDisplayFunc(Draw);
  int res;
  glGetIntegerv(GL_AUX_BUFFERS, &res);
  cerr << "=========> " << res << endl;
  glutMainLoop();  
  return EXIT_SUCCESS;
}
