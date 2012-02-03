#ifndef Tulip_GLCYLINDER_H
#define Tulip_GLCYLINDER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <tulip/Coord.h>
#include <tulip/Color.h>

#include "tulip/GlSimpleEntity.h"

namespace tlp {

class TLP_GL_SCOPE GlCylinder : public GlSimpleEntity {

public:

  GlCylinder() {}

  /**
   * Create a cylinder
   */
  GlCylinder(const Coord& source, const Coord& head,
             float rad_source, float rad_head1,
             const Color& col_source, const Color& col_head, unsigned int nb_p);
  /**
   * Create an arrow
   */
  GlCylinder(const Coord& source, const Coord& head, const Coord& target,
             float rad_source, float rad_head1, float rad_head2,
             const Color& col_source, const Color& col_head, const Color& col_target, unsigned int nb_p);

  /**
   * Build the points lists
   */
  void buildCylinder();
  /**
   * Draw the cylinder
   */
  virtual void draw(float lod,Camera *camera);

  /**
   * Translate entity
   */
  virtual void translate(const Coord& mouvement);

  /**
   * Function to export data in XML
   */
  virtual void getXML(xmlNodePtr rootNode);

  /**
   * Function to set data with XML
   */
  virtual void setWithXML(xmlNodePtr rootNode);

private:
  bool  _arrow;
  Coord _source;
  Coord _head;
  Coord _target;
  float _rad_source;
  float _rad_head1;
  float _rad_head2;
  Color _col_source;
  Color _col_head;
  Color _col_target;
  int   _nb_points;
  std::vector<Coord> _source_points;
  std::vector<Coord> _head_points1;
  std::vector<Coord> _head_points2;

  std::string textureFile;
  Coord rot;

};

}

#endif // Tulip_GLCYLINDER_H
