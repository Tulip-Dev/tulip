#include <iostream>

#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include <tulip/TlpTools.h>
#include <tulip/Glyph.h>
#include <tulip/GlGraph.h>

using namespace std;
static int win;

//==============================================================================
class GLGlutStrategy: public GlGraphStrategy {
public:
  GlGraph *glgraph;
  GLGlutStrategy(string name, const int width=640, const int height=480) {
    drawing=false;
  }
  virtual ~GLGlutStrategy(){}
  void MakeCurrent() {glutSetWindow(win);}
  void UpdateGL(){
    glgraph->draw();
  }
  void setDoubleBuffering(bool b) {}
  bool timerIsActive() {return drawing;}
  int timerStart(int msec, bool sshot=false) {
    drawing=true;
    return true;
  }
  void timerStop() {
    drawing=false;
  }
  void mPaint(GlGraph *g) {}
  bool drawing;
private:
  int width,height;
};
//=============================================
static int width(640), height(480);
static GLGlutStrategy *glGlutScreen;
static GlGraph *glgraph;
static int frame=0;
static GLint timer;
static int rx=0,ry=0,rz=0;
static bool frameRateDisplaying=false;
static bool incremental=false;
static char strFrameRate[50] = {0};      

using namespace tlp;
//=============================================
void idle(void) {  
  if (glGlutScreen->drawing) {
    glGlutScreen->drawing = !glgraph->drawPart();
    glutSwapBuffers();
  }
  else {
    glgraph->rotateScene(rx*2,ry*2,rz*2);
    if (frameRateDisplaying) {
      if (frame%20==0) {
	GLint t = glutGet(GLUT_ELAPSED_TIME);
	GLfloat seconds = (t - timer) / 1000.0;
	GLfloat fps = frame / seconds;
	sprintf(strFrameRate, "Tulip Glut Viewer: fps = %.2f", fps);
	glutSetWindowTitle(strFrameRate);
	frame=0;
	timer=t;
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
    glgraph->zoom(2);
    break;
  case '-':
    glgraph->zoom(-2);
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
    glgraph->setEdge3D(!glgraph->isEdged3D());
    printMessage("3D edges",glgraph->isEdged3D());
    break;
  case 'a':
    glgraph->setViewArrow(!glgraph->isViewArrow());
    printMessage("Arrow ",glgraph->isViewArrow());
    break;
  case 'o':
    glgraph->setViewOrtho(!glgraph->isViewOrtho());
    printMessage("Projection orthogonal",glgraph->isViewOrtho());
    break;
  case 'l':
    glgraph->setViewLabel(!glgraph->isViewLabel());
    printMessage("Labels",glgraph->isViewLabel());
    glgraph->setFontsType(1);
    break;
  case 's':
    glgraph->setViewStrahler(!glgraph->isViewStrahler());
    printMessage("StrahlerMode",glgraph->isViewStrahler());
    break;
  case 'f':
    //    glgraph->setViewKey(!glgraph->isViewKey());
    //    printMessage("Fan renderring",glgraph->isViewKey());
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
    glgraph->setIncrementalRendering(incremental);
    printMessage("Incremental rendering",incremental);
    break;
  default:
    return;
  }
}
//==============================================================================
void importGraph(const string &filename, GlGraph &glGraph) {
  DataSet dataSet;
  dataSet.set("filename", filename);
  SuperGraph *newSuperGraph=tlp::importGraph("tlp", dataSet, NULL);
  if (newSuperGraph!=0) {
    glGraph.setSuperGraph(newSuperGraph);
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
  glgraph->changeViewport(0,0,width,height);
}
//=============================================
void Draw(void) {
  glgraph->draw();
  if (!glgraph->isIncrementalRendering()) 
    glutSwapBuffers();
  else
    idle();
  if (frameRateDisplaying) frame++;
}
/***************************************************************************************************/
int main (int argc, char **argv) {
  glutInit(&argc, argv);
  if (argc!=2) {
    cerr << "usage :" << endl;
    cerr << "\t " << argv[0] << " <filename>" << endl;
    cerr << "file must be in tlp format" << endl;
    exit(1);
  }

  tlp::initTulipLib();
  tlp::loadPlugins();   // library side plugins
  GlGraph::loadPlugins();

  /*  GlGraphWidget *mywidget = new GlGraphWidget(parent);
  mywidget->show();
  */


  cout << "********************************************" <<endl;
  cout << "Glut graph viewer demo" << endl;
  cout << "Author : Auber David 29/01/2003" << endl;
  cout << "********************************************" <<endl;
  cout << "key :" << endl;
  cout << "\t x: (de)Activate X axis rotation" << endl;
  cout << "\t y: (de)Activate Y axis rotation" << endl;
  cout << "\t z: (de)Activate Z axis rotation" << endl;
  cout << "\t s: (de)Activate strahler ordering" << endl;
  cout << "\t l: (de)Activate labels" << endl;
  cout << "\t a: (de)Activate arrows" << endl;
  cout << "\t f: (de)Activate fan rendering" << endl;
  cout << "\t e: (de)Activate 3D edges" << endl;
  cout << "\t o: (de)Activate Orthogonal projection" << endl;
  cout << "\t b: (de)Activate Frame rate displaying" << endl;
  cout << "\t 1: (de)Activate full screen" << endl;
  cout << "\t I: (de)Activate incremental rendering" << endl;
  cout << "\t esc: quit" << endl;
  cout << "**********************************************" <<endl;

  glutInitWindowPosition(0, 0); 
  glutInitWindowSize( width, height);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
  if ((win=glutCreateWindow("Tulip Glut Viewer")) == GL_FALSE) {
    exit(1);
  }

  glGlutScreen = new GLGlutStrategy("", width, height);
  glgraph = new GlGraph(*glGlutScreen);
  glGlutScreen->glgraph = glgraph;
  glgraph->initializeGL();
  glutIdleFunc(idle);
  importGraph(argv[1], *glgraph);
  glgraph->centerScene();
  timer = glutGet(GLUT_ELAPSED_TIME);
  glgraph->setIncrementalRendering(false);
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Key);
  glutDisplayFunc(Draw);
  glutMainLoop();  
  return EXIT_SUCCESS;
}
