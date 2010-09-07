/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include <iostream>

#include <stdio.h>
#include <GL/glew.h>

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
#include <tulip/ForEach.h>
#include <tulip/OpenGlConfigManager.h>
#include <tulip/Curves.h>

using namespace std;
using namespace tlp;
 
static int win;
const unsigned int WIN_SIZE = 500;
unsigned int frameCount = 3;
static int width(WIN_SIZE), height(WIN_SIZE);
static int frame = 0;
static GLint timer;
static int rx = 0, ry = 0, rz = 0;
static char strFrameRate[300] = {0};      
GlScene scene;


using namespace tlp;
//=============================================
static void printMessage(const string &str, const bool b) {
  cout << str << " => " << (b ? "On" : "Off") << endl;
}
bool drawElm = true;
bool vbo = false;
bool buildVBO = true;


Graph          *graph;
LayoutProperty *layout;
ColorProperty  *color;
SizeProperty   *size;
string name;

//=============================================
static void changeOption(const int key) {
  frame = 0;
  frameCount = 3;
  GlGraphRenderingParameters param = scene.getGlGraphComposite()->getRenderingParameters();
  switch (key) {
  case '1':
    glutFullScreen();
    break;
  case 27:
    exit(EXIT_SUCCESS);
  case '+':
    scene.zoom(2);
    break;
  case '-':
    scene.zoom(-2);
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
    scene.setViewOrtho(!scene.isViewOrtho());
    printMessage("Projection orthogonal", scene.isViewOrtho());
    break;
  case 'l':
    param.setViewNodeLabel(!param.isViewNodeLabel());
    printMessage("Labels", param.isViewNodeLabel());
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
  case 'f':
    drawElm = !drawElm;
    printMessage("Fast renddering", drawElm);
    break;
  case 'v':
    vbo =! vbo;
    buildVBO = true;
    printMessage("rebuild VBO each time", vbo);
    break;
  case 'w':
    size->scale(Size(2,2,1))	;
    buildVBO = true;
    break;
  case 'd':
    size->scale(Size(0.5,0.5,1));
    buildVBO = true;
    break;

  default:
    return;
  }
  scene.getGlGraphComposite()->setRenderingParameters(param);
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
  scene.setViewport(viewport);
}
//====================================================
vector<Vec2f>   points;
vector<Color>   colors;
vector<GLuint>  indices;

vector<Vec2f>   quad_points;
vector<Color>   quad_colors;
vector<Vec2f>   quad_texc;
vector<GLuint>  quad_indices;

GLuint buffers[7];
//====================================================
void initEdgesArray() {
      size_t nbEdges = graph->numberOfEdges();
      size_t nbBends = 0;
      {
	edge e;
        forEach(e, graph->getEdges()) {
           nbBends += layout->getEdgeValue(e).size();
        }
      }
      points.resize(nbEdges*2 + nbBends); //todo: should be #|V| !!!
      indices.resize(nbEdges*2 + nbBends * 2);
      colors.resize(nbEdges*2 + nbBends);
      //cout << "nb lines = " << indices.size()/2 << endl;
      size_t i_point  = 0;
      size_t i_indices = 0;
      size_t i_col = 0;
      edge e;
      forEach(e, graph->getEdges()) {
	Color a = color->getNodeValue(graph->source(e));
	Color b = color->getNodeValue(graph->target(e));
	Vec4f ca, cb; 
	for (size_t i=0; i < 4; ++i) {
	  ca[i] = a[i];
	  cb[i] = b[i];
	}
	
	indices[i_indices++] = i_point;
	colors[i_col++] = a;
	points[i_point][0] = layout->getNodeValue(graph->source(e))[0];
	points[i_point++][1] = layout->getNodeValue(graph->source(e))[1];

	vector<Coord> bends = layout->getEdgeValue(e);
	for (size_t j=0; j < bends.size(); ++j) {
	  Vec4f tmp((ca - cb));
	  tmp *= 1./(bends.size() + 2);
	  tmp *= j+1;
	  tmp += ca;
	  colors[i_col++] = Color((int)tmp[0], (int)tmp[1], (int)tmp[2], (int)tmp[3]);
	  indices[i_indices++] = i_point;
	  indices[i_indices++] = i_point;
	  points[i_point][0] = bends[j][0];
	  points[i_point++][1] = bends[j][1];
	}
	
	indices[i_indices++] = i_point;
	colors[i_col++] = b;
	points[i_point][0] = layout->getNodeValue(graph->target(e))[0];
	points[i_point++][1] = layout->getNodeValue(graph->target(e))[1];

      }
}
//====================================================
void initNodesArray() {
  size_t nbNodes= graph->numberOfNodes();
      quad_points.resize(nbNodes * 4); 
      quad_indices.resize(nbNodes * 4);
      quad_colors.resize(nbNodes * 4);
      quad_texc.resize(nbNodes * 4);
      Vec2f tex[4];
      tex[0][0] = 0;
      tex[0][1] = 0;
      tex[1][0] = 1;
      tex[1][1] = 0;
      tex[2][0] = 1;
      tex[2][1] = 1;
      tex[3][0] = 0;
      tex[3][1] = 1;              
                     // i % x  i%3 
      float tab1[4] = { -1,     1,  1, -1};
      float tab2[4] = { -1,     -1,  1, 1};
      
      
  
	size_t i_point = 0;
	size_t i_col = 0;
	size_t i_indices = 0;
	size_t i_tex = 0;

	node n;
	forEach(n, graph->getNodes()) {
	  Coord p ( layout->getNodeValue(n));
	  Size  s ( size->getNodeValue(n) / 2.);
	  //Color c ( color->getNodeValue(n));
	  Color c(255,255,255,255);
	  for (int i =0; i<4; ++i) {
	    Vec3f a = p;
	    a[0] += s[0] * tab1[i];//s[0] * i%2 ;  // 0 1 1 0
	    a[1] += s[1] * tab2[i];//s[1] * i/2 %2;  // 0 0 1 1

	    quad_texc[i_tex] = tex[i_tex%4];
	    ++i_tex;
	    quad_colors[i_col++] = c;	  
	    quad_indices[i_indices++] = i_point;
	    quad_points[i_point][0] = a[0];
	    quad_points[i_point++][1] = a[1];
	  }	  	  
	}
 
}
//=============================================
void Draw(void) {
//      bool canUseGlew=OpenGlConfigManager::getInst().canUseGlew();

  if (!drawElm) {
   scene.draw();
  }
  else {
  scene.initGlParameters();
  scene.getCamera()->initGl();
 
    if (buildVBO || vbo) {
      initEdgesArray();
      initNodesArray();    
      buildVBO = false;
    }
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
      glEnableClientState(GL_VERTEX_ARRAY);    
      glEnableClientState(GL_COLOR_ARRAY);    
	glVertexPointer(2, GL_FLOAT, 2*sizeof(GLfloat), &points[0]);    
	glColorPointer(4, GL_UNSIGNED_BYTE, 4*sizeof(GLubyte), &colors[0]);
	size_t cur = 0;
	while(cur< indices.size()) {
	if (indices.size() - cur > 64000) {
	  glDrawElements(GL_LINES, 64000, GL_UNSIGNED_INT, &indices[cur]);
	}
	else
	  glDrawElements(GL_LINES, indices.size() - cur, GL_UNSIGNED_INT, &indices[cur]);
	cur += 64000;
	}
      
      GlTextureManager::getInst().activateTexture("circle.png");
//      glDisable(GL_BLEND);
 
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);    
      glVertexPointer(2, GL_FLOAT, 2*sizeof(GLfloat), &quad_points[0]);    
      glColorPointer(4, GL_UNSIGNED_BYTE, 4*sizeof(GLubyte), &quad_colors[0]);
      glTexCoordPointer(2, GL_FLOAT, 2*sizeof(GLfloat), &quad_texc[0]);
       cur = 0;
	while(cur< quad_indices.size()) {
	if (quad_indices.size() - cur > 64000) {
	  glDrawElements(GL_QUADS, 64000, GL_UNSIGNED_INT, &quad_indices[cur]);
	}
	else
	  glDrawElements(GL_QUADS, quad_indices.size() - cur, GL_UNSIGNED_INT, &quad_indices[cur]);
	cur += 64000;
	}
      
      //glDrawElements(GL_QUADS, quad_indices.size(), GL_UNSIGNED_INT, &quad_indices[0]);
      glDisableClientState(GL_VERTEX_ARRAY);
      glDisableClientState(GL_COLOR_ARRAY);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      GlTextureManager::getInst().desactivateTexture();	
  }

  glutSwapBuffers();
  
}
//=============================================
void helpMessage() {
  cout << "********************************************" <<endl;
  cout << "Glut Tulip Graph Viewer" << endl;
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
  cout << "\t 1\t: (de)Activate full screen" << endl;
  cout << "\t v\t: (de)Activate fast drawing" << endl;
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
//=============================================
void idle(void) {  
  stringstream tmpstr;
  scene.rotateScene(rx*2, ry*2, rz*2);
    if (frame%frameCount == 0) {
      GLint t = glutGet(GLUT_ELAPSED_TIME);
      if (frame !=0) {
	GLfloat seconds = (t - timer) / 1000.0;
	GLfloat fps = frame / seconds;
	frameCount = int (fps + 1);
	frameCount = std::min(frameCount, 200u);
	if (frameCount > 100000) frameCount = 3;
	stringstream tmpstr;
	tmpstr << "TlpGV : speed: " << fps << "fps (" << graph->numberOfNodes() << " nodes," <<  graph->numberOfEdges() << " edges)[" << name << "]";
	sprintf(strFrameRate, "%s", tmpstr.str().c_str());
	glutSetWindowTitle(strFrameRate);
      }
      frame = 0;
      timer = t;
    }
  ++frame;
  glutPostRedisplay();
}
/***************************************************************************************************/
int main (int argc, char **argv) {
  if (argc!=2) 
    usage();

  tlp::initTulipLib();
  PluginLoaderTxt plug;
  GlyphManager::getInst().loadPlugins(&plug);
  EdgeExtremityGlyphManager::getInst().loadPlugins(&plug);

  graph = tlp::loadGraph(argv[1]);
  if (graph == 0) {
    cout << "can't load the graph" << argv[1] << endl;
  }
  name = "untitled";
  graph->getAttribute("name", name);
  
  layout = graph->getProperty<LayoutProperty>("viewLayout");
  color = graph->getProperty<ColorProperty>("viewColor");
  size = graph->getProperty<SizeProperty>("viewSize");
 


  glutInit(&argc, argv);
  glutInitWindowPosition(0, 0); 
  glutInitWindowSize( width, height);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL);
  if ((win=glutCreateWindow("Tulip Glut Viewer")) == GL_FALSE) {
    cerr << "Unable to create a OpenGl Glut window" << endl;
    exit(EXIT_FAILURE);
  }

  GlDisplayListManager::getInst().changeContext(0);
  GlTextureManager::getInst().changeContext(0);

  GlLayer *layer=new GlLayer("Main");
  scene.addLayer(layer);

  GlGraphComposite* graphComposite = new GlGraphComposite(graph);
  scene.addGlGraphCompositeInfo(scene.getLayer("Main"), graphComposite);
  scene.getLayer("Main")->addGlEntity(graphComposite, "graph");
  scene.centerScene();



  Reshape(500, 500);
  OpenGlConfigManager::getInst().initGlew();
  frame = 0;
  timer = glutGet(GLUT_ELAPSED_TIME);
  glutIdleFunc(idle);
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Key);
  glutDisplayFunc(Draw);
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
  glutAddMenuEntry("Fast draw (f)",   'f');
  glutAddMenuEntry("vbo (v)",   'v');
  glutAddMenuEntry("exit (esc)",   27);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  helpMessage();

  glutMainLoop();  
  
  return EXIT_SUCCESS;
}
//====================================================

