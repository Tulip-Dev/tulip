//*********************************************************************************
//** GlQuad.cpp : Source for a general quad display class
//** 
//** author : Delorme Maxime
//** date   : 07/07/05
//*********************************************************************************
#include "tulip/GlQuad.h"

using namespace std;
using namespace tlp;

GlQuad::GlQuad()
{
  // No default constructor :)
}

void GlQuad::setPosition(const Coord& position)
{
  // No setPosition
}

void GlQuad::setColor(const Color& color)
{
  for(int i=0; i < N_QUAD_POINTS; i++) {
    *(this->colors[i]) = color;
  }
}

GlQuad::GlQuad(Coord positions[N_QUAD_POINTS], const Color &color)
{
  for(int i=0; i < N_QUAD_POINTS; i++)
    {
      this->positions[i] = new Coord(positions[i]);
      this->colors[i]    = new Color(color);
    }
}

GlQuad::GlQuad(Coord positions[N_QUAD_POINTS], Color colors[N_QUAD_POINTS])
{

  boundingBox.first=positions[0];
  boundingBox.second=positions[0];

  for(int i=0; i < N_QUAD_POINTS; i++) {
    this->positions[i] = new Coord(positions[i]);
    this->colors[i]    = new Color(colors[i]);

    if(positions[i][0]<boundingBox.first[0])
      boundingBox.first[0]=positions[i][0];
    if(positions[i][1]<boundingBox.first[1])
      boundingBox.first[1]=positions[i][1];
    if(positions[i][2]<boundingBox.first[2])
      boundingBox.first[2]=positions[i][2];
    if(positions[i][0]>boundingBox.second[0])
      boundingBox.second[0]=positions[i][0];
    if(positions[i][1]>boundingBox.second[1])
      boundingBox.second[1]=positions[i][1];
    if(positions[i][2]>boundingBox.second[2])
      boundingBox.second[2]=positions[i][2];
  }   
}

GlQuad::~GlQuad()
{
  for(int i=0; i < N_QUAD_POINTS; i++)
    {
      delete positions[i];
      delete colors[i];
    }
}

void GlQuad::setPosition(int idPosition, const Coord &position)
{
  if (idPosition < 0 || idPosition >= N_QUAD_POINTS)
    return;

  delete this->positions[idPosition];

  this->positions[idPosition] = new Coord(position);
}

void GlQuad::setColor(int idColor, const Color &color)
{
  if (idColor < 0 || idColor >= N_QUAD_POINTS)
    return;

  delete this->colors[idColor];

  this->colors[idColor] = new Color(color);
}

Coord* GlQuad::getPosition(int idPosition) const
{
  if (idPosition < 0 || idPosition >= N_QUAD_POINTS)
    return NULL;

  return positions[idPosition];
}

Color* GlQuad::getColor(int idColor) const
{
  if (idColor < 0 || idColor >= N_QUAD_POINTS)
    return NULL;

  return colors[idColor];
}

void GlQuad::draw(float lod)
{

  GLfloat* cols[N_QUAD_POINTS];

  for(int i=0; i < N_QUAD_POINTS; i++)
    cols[i] = colors[i]->getGL();

  glEnable(GL_COLOR_MATERIAL);
  glBegin(GL_QUADS);
  
  for(int i=0; i < N_QUAD_POINTS; i++){
    glColor4fv(cols[i]);
    glVertex3f(positions[i]->getX(), positions[i]->getY(), positions[i]->getZ());
  }
  
  glEnd();
 
  /*  
  if (renderOptions.getRenderState(GlAD_Wireframe))
    {
	
      if (Solid)
	{
	  GLfloat colBk[4] = {0.0f, 0.0f, 0.0f, 255.0f};

	  for (int i=0; i < N_QUAD_POINTS; i++)
	    cols[i] = colBk;
	}

      glBegin(GL_LINE_STRIP);
	   
      for(int i=0; i <= N_QUAD_POINTS; i++)
	{
	  int id = i % N_QUAD_POINTS;
	    
	  glColor4fv(cols[id]);
	  glVertex3f(positions[id]->getX(), positions[id]->getY(), positions[id]->getZ());
	}
	
      glEnd();
      */
}
