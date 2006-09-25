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
static int win;

unsigned int frameCount = 5;
GLuint LList;
bool listOk = false;
//==============================================================================
class GLGlut: public GlGraph {
public:
  GLGlut(string name, const int width=640, const int height=480) {
    drawing=false;
    makeCurrent();
  }
  virtual ~GLGlut(){}
  void makeCurrent() {
    glutSetWindow(win);
  }
  void draw_handler(bool b) {
    drawing = b;
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
  if (glGlutScreen->drawing) {
    glGlutScreen->drawing = !glGlutScreen->drawPart();
    glutSwapBuffers();
  }
  else {
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
//=============================================
static void printMessage(string str,bool b) {
  cout << str << " => " << (b ? "On" : "Off") << endl;
}
//=============================================
static void Key(unsigned char key, int x, int y) {
  switch (key) {
  case '1':
    glutFullScreen();
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
    glGlutScreen->setEdge3D(!glGlutScreen->isEdged3D());
    printMessage("3D edges",glGlutScreen->isEdged3D());
    break;
  case 'a':
    glGlutScreen->setViewArrow(!glGlutScreen->isViewArrow());
    printMessage("Arrow ",glGlutScreen->isViewArrow());
    break;
  case 'o':
    glGlutScreen->setViewOrtho(!glGlutScreen->isViewOrtho());
    printMessage("Projection orthogonal",glGlutScreen->isViewOrtho());
    break;
  case 'l':
    glGlutScreen->setViewLabel(!glGlutScreen->isViewLabel());
    printMessage("Labels",glGlutScreen->isViewLabel());
    glGlutScreen->setFontsType(1);
    break;
  case 's':
    glGlutScreen->setViewStrahler(!glGlutScreen->isViewStrahler());
    printMessage("StrahlerMode",glGlutScreen->isViewStrahler());
    break;
  case 'E':
    glGlutScreen->setDisplayEdges(!glGlutScreen->isDisplayEdges());
    printMessage("Edge displaying", glGlutScreen->isDisplayEdges());
    break;
  case 'b':
    frameRateDisplaying=!frameRateDisplaying;
    printMessage("Display frame rate",frameRateDisplaying);
    frame=0;
    timer=glutGet(GLUT_ELAPSED_TIME);
    glutSetWindowTitle("Tulip Glut Viewer");
    break;
  case 'I':
    incremental=!incremental;
    glGlutScreen->setIncrementalRendering(incremental);
    printMessage("Incremental rendering",incremental);
    break;
  default:
    return;
  }
}
//==============================================================================
void importGraph(const string &filename, GlGraph &glGraph) {
  DataSet dataSet;
  dataSet.set("file::filename", filename);
  Graph *newGraph=tlp::importGraph("tlp", dataSet, NULL);
  if (newGraph!=0) {
    glGraph.setGraph(newGraph);
    glGraph.centerScene();
    DataSet glGraphData;
    if (dataSet.get<DataSet>("displaying", glGraphData))
      glGraph.setParameters(glGraphData);
  }
}
//=============================================
void Reshape(int widt, int heigh) {
  width=widt;
  height=heigh;
  glGlutScreen->changeViewport(0,0,width,height);
}
//=============================================
void Draw(void) {
  glGlutScreen->draw();
  if (!glGlutScreen->isIncrementalRendering()) 
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
  glGlutScreen->setIncrementalRendering(false);
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Key);
  glutDisplayFunc(Draw);
  
  glutMainLoop();  
  return EXIT_SUCCESS;
}
