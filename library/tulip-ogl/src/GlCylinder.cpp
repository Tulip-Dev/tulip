



#include <cmath>
#include <tulip/Color.h>

#include <tulip/OpenGlConfigManager.h>
#include "tulip/GlDisplayListManager.h"
#include "tulip/GlTextureManager.h"
#include "tulip/GlTools.h"

#include "tulip/GlCylinder.h"

using namespace std;

namespace tlp {
/**
 * Init Arrow
 */
GlCylinder::GlCylinder(const Coord &source, const Coord& head, const Coord& target,
                       float rad_source, float rad_head1, float rad_head2,
                       const Color& col_source, const Color& col_head, const Color& col_target,
                       unsigned int nb_points=16)
  :_source(source),_head(head),_target(target),
   _rad_source(rad_source),_rad_head1(rad_head1),_rad_head2(rad_head2),
   _col_source(col_source),_col_head(col_head),_col_target(col_target),
   _nb_points(nb_points) {

  _arrow = true;
  Coord min = source;
  Coord max = target;

  for (int i=0; i<3; i++)
    if (min[i] > max [i]) {
      min[i] = max[i];
      max[i] = source[i];
    }

  float rad_max = (_rad_source>_rad_head1)?rad_source:_rad_head1;
  rad_max = (rad_max>_rad_head2)?rad_max:_rad_head2;

  this->buildCylinder();

  boundingBox[0]=Coord(min[0]-rad_max,min[1]-rad_max,min[2]-rad_max);
  boundingBox[1]=Coord(max[0]+rad_max,max[1]+rad_max,max[2]+rad_max);
}

/**
 * Init Cylinder
 */
GlCylinder::GlCylinder(const Coord &source, const Coord& head,
                       float rad_source, float rad_head,
                       const Color& col_source, const Color& col_head,
                       unsigned int nb_points=16)
  :_source(source),_head(head),
   _rad_source(rad_source),_rad_head1(rad_head),
   _col_source(col_source),_col_head(col_head),
   _nb_points(nb_points) {

  _arrow= false;
  Coord min = source;
  Coord max = head;

  for (int i=0; i<3; i++)
    if (min[i] > max [i]) {
      min[i] = max[i];
      max[i] = source[i];
    }

  float rad_max = (rad_source>_rad_head1)?rad_source:_rad_head1;

  this->buildCylinder();

  boundingBox[0]=Coord(min[0]-rad_max,min[1]-rad_max,min[2]-rad_max);
  boundingBox[1]=Coord(max[0]+rad_max,max[1]+rad_max,max[2]+rad_max);
}

void GlCylinder::buildCylinder() {
  Coord Hcyl = _head-_source;
  // génére un premier vecteur
  Coord Vect1;

  if (Hcyl[0] == 0)
    Vect1 = Coord(0, Hcyl[2], -Hcyl[1]);
  else {
    if (Hcyl[1] == 0)
      Vect1 = Coord(Hcyl[2], 0, -Hcyl[0]);
    else {
      if (Hcyl[2] == 0)
        Vect1 = Coord(Hcyl[1], -Hcyl[0], 0);
      else
        Vect1 = Coord(Hcyl[1], -Hcyl[0], 0);
    }
  }

  double normVect1 = Vect1.norm();

  if (normVect1 != 0)
    Vect1 /= normVect1;

  // génère le deuxième par produit vectoriel
  Coord Vect2(Vect1[1]*Hcyl[2]-Vect1[2]*Hcyl[1],
              Vect1[2]*Hcyl[0]-Vect1[0]*Hcyl[2],
              Vect1[0]*Hcyl[1]-Vect1[1]*Hcyl[0]);
  double normVect2 = Vect2.norm();

  if (normVect2 != 0)
    Vect2 /= normVect2;

  float delta = (2.0f * M_PI) / (float)_nb_points;

  for (int i=0; i<_nb_points; i++) {
    float deltaX = cos(i * delta);
    float deltaY = sin(i * delta);

    _source_points.push_back(_source+deltaX*_rad_source*Vect1+deltaY*_rad_source*Vect2);
    _head_points1.push_back(_head+deltaX*_rad_head1*Vect1+deltaY*_rad_head1*Vect2);

    if (_arrow)
      _head_points2.push_back(_head+deltaX*_rad_head2*Vect1+deltaY*_rad_head2*Vect2);
  }
}



void GlCylinder::draw(float /* lod */, Camera* /*camera*/) {
  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  glPushMatrix();
  OpenGlConfigManager::getInst().activatePolygonAntiAliasing();
  glBegin(GL_POLYGON);
  setColor(_col_source);

  for (int i=0; i<_nb_points; i++)
    glVertex3f(_source_points[i][0],_source_points[i][1],_source_points[i][2]);

  glEnd();

  glBegin(GL_TRIANGLE_STRIP);

  for (int i=0; i<=_nb_points; i++) {
    setColor(_col_source);
    glVertex3f(_source_points[i%_nb_points][0],_source_points[i%_nb_points][1],_source_points[i%_nb_points][2]);
    setColor(_col_head);
    glVertex3f(_head_points1[i%_nb_points][0],_head_points1[i%_nb_points][1],_head_points1[i%_nb_points][2]);
  }

  glEnd();

  glBegin(GL_POLYGON);
  setColor(_col_head);

  for (int i=_nb_points; i>=0; i--) {
    glVertex3f(_head_points1[i%_nb_points][0],_head_points1[i%_nb_points][1],_head_points1[i%_nb_points][2]);
  }

  glEnd();

  if (_arrow) {
    glBegin(GL_POLYGON);
    setColor(_col_head);

    for (int i=0; i<=_nb_points; i++) {
      glVertex3f(_head_points2[i%_nb_points][0],_head_points2[i%_nb_points][1],_head_points2[i%_nb_points][2]);
    }

    glEnd();

    glBegin(GL_POLYGON);
    setColor(_col_head);

    for (int i=_nb_points; i>=0; i--) {
      glVertex3f(_head_points2[i%_nb_points][0],_head_points2[i%_nb_points][1],_head_points2[i%_nb_points][2]);
    }

    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    setColor(_col_target);
    glVertex3f(_target[0], _target[1], _target[2]);
    setColor(_col_head);

    for (int i=_nb_points; i>=0; i--)
      glVertex3f(_head_points2[i%_nb_points][0],_head_points2[i%_nb_points][1],_head_points2[i%_nb_points][2]);

    glEnd();
  }

  OpenGlConfigManager::getInst().desactivatePolygonAntiAliasing();
  glPopMatrix();
  glDisable(GL_COLOR_MATERIAL);
  glDisable(GL_LIGHTING);
}
//===========================================================
void GlCylinder::translate(const Coord& mouvement) {
  boundingBox[0]+=mouvement;
  boundingBox[1]+=mouvement;

  _source+=mouvement;
  _head+=mouvement;
  _target+=mouvement;

  for (int i=0; i<_nb_points; i++) {
    _source_points[i]+=mouvement;
    _head_points1[i]+=mouvement;
    _head_points2[i]+=mouvement;
  }
}
//===========================================================
void GlCylinder::getXML(xmlNodePtr) {
}
//============================================================
void GlCylinder::setWithXML(xmlNodePtr) {
}

}
