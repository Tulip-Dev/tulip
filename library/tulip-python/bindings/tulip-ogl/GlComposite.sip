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

class GlComposite : tlp::GlSimpleEntity {
%TypeHeaderCode
#include <tulip/GlComposite.h>
%End

  public:
  
    GlComposite(bool deleteComponentsInDestructor=true);
    
    ~GlComposite();

    virtual void addLayerParent(tlp::GlLayer *layer);

    virtual void removeLayerParent(tlp::GlLayer *layer);

    void reset(bool deleteElems);

    void addGlEntity(tlp::GlSimpleEntity *entity, const std::string &key);

    void deleteGlEntity(const std::string &key,bool informTheEntity=true);

    void deleteGlEntity(tlp::GlSimpleEntity *entity,bool informTheEntity=true);

    std::string findKey(tlp::GlSimpleEntity *entity);

    tlp::GlSimpleEntity* findGlEntity(const std::string &key);
    
    void notifyModified(tlp::GlSimpleEntity *entity);

    virtual void draw(float, tlp::Camera *);

    virtual void setStencil(int stencil);

    virtual void acceptVisitor(tlp::GlSceneVisitor *visitor);
    
    void setDeleteComponentsInDestructor(bool deleteComponentsInDestructor);

    virtual void translate(const tlp::Coord &mouvement);
};
};
