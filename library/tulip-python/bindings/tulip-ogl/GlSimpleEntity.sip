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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (http://booggie.org/)                                                   |
// +-------------------------------------------------------------------------+

namespace tlp {

class GlSimpleEntity {
%TypeHeaderCode
#include <tulip/GlSimpleEntity.h>
%End

  public:

    GlSimpleEntity();

    virtual ~GlSimpleEntity();

    virtual void draw(float lod, tlp::Camera* camera) = 0;

    virtual void acceptVisitor(tlp::GlSceneVisitor *visitor);

    virtual void setVisible(bool visible);
   
    bool isVisible();
   
    virtual void setStencil(int stencil);
    
    int getStencil();

    virtual tlp::BoundingBox getBoundingBox();

    tlp::GlComposite* getParent() const;

    void addParent(tlp::GlComposite *composite);

    void removeParent(tlp::GlComposite *composite);

    virtual void translate(const tlp::Coord &);

    virtual void getXML(std::string&) = 0;

    virtual void setWithXML(const std::string&, unsigned int&) = 0;

  };

};

