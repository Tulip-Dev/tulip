#include <cmath>

#include <string>

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Glyph.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTextureManager.h>

#include <tulip/Graph.h>
#include <tulip/GlTools.h>

using namespace std;
using namespace tlp;

/** \addtogroup glyph */
/*@{*/
/// A 3D glyph.
/** This glyph draws a textured cube using the "viewTexture" node
 * property value. If this property has no value, the cube is then colored
 * using the "viewColor" node property value.
 */ 
class Cube : public Glyph {
public:
  Cube(GlyphContext *gc=NULL);
  virtual ~Cube();
  virtual void draw(node n,float lod);
  virtual Coord getAnchor(const Coord & vector) const;

protected:
  void drawCube();
  /*void buildVertexList();

  GLuint cubeVertexVBO;
  GLuint cubeNormalVBO;
  GLubyte indexes[24];*/
};

GLYPHPLUGIN(Cube, "3D - Cube", "Bertrand Mathieu", "09/07/2002", "Textured cube", "1.0" , 0);

//===================================================================================
Cube::Cube(GlyphContext *gc): Glyph(gc) {
  //buildVertexList();
}
//=======================================================
Cube::~Cube() {
}
//=======================================================
void
Cube::draw(node n,float lod) {
  glEnable(GL_LIGHTING);
  glDisable(GL_COLOR_MATERIAL);

  if(GlDisplayListManager::getInst().beginNewDisplayList("Cube_cube")) {
    drawCube();
    GlDisplayListManager::getInst().endNewDisplayList();
  }

  setMaterial(glGraphInputData->elementColor->getNodeValue(n));
  string texFile = glGraphInputData->elementTexture->getNodeValue(n);
  if (texFile != "") {
    string texturePath=glGraphInputData->parameters->getTexturePath();
    GlTextureManager::getInst().activateTexture(texturePath+texFile);
  }
 
  /*glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);

  glBindBuffer( GL_ARRAY_BUFFER, cubeVertexVBO );
  glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL ); // Set The Vertex Pointer To The Vertex Buffer
  glBindBuffer( GL_ARRAY_BUFFER, cubeNormalVBO );
  glNormalPointer(GL_FLOAT, 0,  (char *) 0 );
  glDrawElements(GL_QUADS,6*4,GL_UNSIGNED_BYTE,indexes);

  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glDisableClientState( GL_VERTEX_ARRAY );
  glDisableClientState( GL_NORMAL_ARRAY ); // Enable Normal Arrays   */    
  GlDisplayListManager::getInst().callDisplayList("Cube_cube");
  GlTextureManager::getInst().desactivateTexture();
}
//=======================================================
Coord Cube::getAnchor(const Coord & vector ) const {
  float x,y,z, fmax;
  vector.get(x,y,z);
  fmax = std::max(std::max(fabsf(x), fabsf(y)), fabsf(z));
  if( fmax > 0.0f )
    return vector * (0.5f/fmax);
  else
    return vector;
}
//=======================================================
void Cube::drawCube() {
  glBegin(GL_QUADS);
  /* front face */
  glNormal3f(0.0f, 0.0f, 1.0f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.5f, -0.5f, 0.5f); 
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(0.5f, -0.5f, 0.5f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0.5f, 0.5f, 0.5f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  /* back face */
  glNormal3f(0.0f, 0.0f, -1.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-0.5f, -0.5f, -0.5f); 
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(0.5f, 0.5f, -0.5f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(0.5f, -0.5f, -0.5f);
  /* right face */
  glNormal3f(1.0f, 0.0f, 0.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(0.5f, -0.5f, -0.5f); 
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0.5f, 0.5f, -0.5f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(0.5f, 0.5f, 0.5f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(0.5f, -0.5f, 0.5f);
  /* left face */
  glNormal3f(-1.0f, 0.0f, 0.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-0.5f, -0.5f, 0.5f); 
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  /* top face */
  glNormal3f(0.0f, 1.0f, 0.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(0.5f, 0.5f, 0.5f); 
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0.5f, 0.5f, -0.5f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  /* bottom face */
  glNormal3f(0.0f, -1.0f, 0.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(0.5f, -0.5f, -0.5f); 
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0.5f, -0.5f, 0.5f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.5f, -0.5f, 0.5f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glEnd();
}

/*void Cube::buildVertexList() {
  GLfloat points[] = {
      0.5, 0.5, -0.5,   -0.5, 0.5, -0.5,  -0.5, -0.5, -0.5,   0.5, -0.5, -0.5,
      0.5, 0.5, -0.5,    0.5, 0.5, 0.5,   -0.5, 0.5, 0.5,    -0.5, 0.5, -0.5,
      -0.5, 0.5, -0.5,  -0.5, 0.5, 0.5,   -0.5, -0.5, 0.5,   -0.5, -0.5, -0.5,
      -0.5, -0.5, -0.5, -0.5, -0.5, 0.5,   0.5, -0.5, 0.5,    0.5, -0.5, -0.5,
      0.5, -0.5, -0.5,   0.5, -0.5, 0.5,   0.5, 0.5, 0.5,     0.5, 0.5, -0.5,
      -0.5, 0.5, 0.5,    0.5, 0.5, 0.5,    0.5, -0.5, 0.5,   -0.5, -0.5, 0.5
    };
    GLfloat normals[] = {
      0.0,0.0,1.0,   0.0,0.0,1.0,   0.0,0.0,1.0,   0.0,0.0,1.0 ,
      0.0,-1.0,0.0,    0.0,-1.0,0.0,    0.0,-1.0,0.0,    0.0,-1.0,0.0,
      1.0,0.0,0.0,  1.0,0.0,0.0,   1.0,0.0,0.0,   1.0,0.0,0.0,
      0.0,1.0,0.0,   0.0,1.0,0.0,   0.0,1.0,0.0,   0.0,1.0,0.0,
      -1.0,0.0,0.0,    -1.0,0.0,0.0,    -1.0,0.0,0.0,    -1.0,0.0,0.0,
      0.0,0.0,-1.0,   0.0,0.0,-1.0,   0.0,0.0,-1.0,   0.0,0.0,-1.0 ,
    };
    Gluint texture[] = {
      1,0,   0,0,     0,1,    1,1,
      0,0,   1,0,     1,1,    0,1,
      1,0,   0,0,     0,1,    1,1,
      0,1,   1,1,     1,0,    0,0,
      0,1,   
      }
    indexes[0] = 0;  indexes[1] = 1;  indexes[2] = 2;  indexes[3] = 3;
    indexes[4] = 4;  indexes[5] = 5;  indexes[6] = 6;  indexes[7] = 7;
    indexes[8] = 8;  indexes[9] = 9;  indexes[10] = 10;  indexes[11] = 11; //
    indexes[12] = 12;  indexes[13] = 13;  indexes[14] = 14;  indexes[15] = 15;
    indexes[16] = 16;  indexes[17] = 17;  indexes[18] = 18;  indexes[19] = 19;
    indexes[20] = 20;  indexes[21] = 21;  indexes[22] = 22;  indexes[23] = 23;
    
    // generate Vertex Buffer Object--------------
    glGenBuffers( 1, &cubeVertexVBO );	
    glBindBuffer( GL_ARRAY_BUFFER,cubeVertexVBO );   // Bind The Buffer
    glBufferData( GL_ARRAY_BUFFER, 24*3*sizeof(GLfloat), points, GL_STATIC_DRAW );
    
    glGenBuffers( 1, &cubeNormalVBO );	
    glBindBuffer( GL_ARRAY_BUFFER,cubeNormalVBO );   // Bind The Buffer
    glBufferData( GL_ARRAY_BUFFER, 24*3*sizeof(GLfloat), normals, GL_STATIC_DRAW );
    
    glBindBuffer( GL_ARRAY_BUFFER,0 );

    GLenum error=glGetError();
    if ( error != GL_NO_ERROR)
      cerr << "Open GL Error : " << error << " in " << __PRETTY_FUNCTION__ << endl;
      }*/
/*@}*/
