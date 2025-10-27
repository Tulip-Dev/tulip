/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#ifndef COMPOSITE_H
#define COMPOSITE_H

#include <map>
#include <list>
#include <string>

#include <tulip/GlSimpleEntity.h>
#include <tulip/tulipconf.h>

namespace tlp {

/**
 * @ingroup OpenGL
 * \brief GlSimpleEntity used to aggregate other GlEntity
 *
 * This class provide basic container to manage other GlEntity
 * @see GlSimpleEntity
 */
class TLP_GL_SCOPE GlComposite : public GlSimpleEntity {

public:
  /**
   * @brief Constructor
   * @param deleteComponentsInDestructor if true : call delete on components when the GlComposite is
   * delete
   */
  GlComposite(bool deleteComponentsInDestructor = true);

  /**
   * @brief Destructor
   */
  ~GlComposite() override;

  /**
   * @brief Clear the composite
   *
   * If deleteElems is true, composite's entities are delete
   */
  void reset(bool deleteElems);
  /**
   * @brief Add new entity with name : key.
   *
   * The composite does not takes the entity's ownership, i.e. it is not its responsibility to
   * delete it.
   */
  void addGlEntity(GlSimpleEntity *entity, const std::string &key);
  /**
   * @brief Remove entity with name : key
   *
   * The entity is not deleted
   */
  void deleteGlEntity(const std::string &key, bool informTheEntity = true);
  /**
   * @brief Remove given entity
   *
   * The entity is not deleted
   */
  void deleteGlEntity(GlSimpleEntity *entity, bool informTheEntity = true);
  /**
   * @brief Find name of given entity
   */
  std::string findKey(GlSimpleEntity *entity);
  /**
   * @brief Find entity with name : key
   */
  GlSimpleEntity *findGlEntity(const std::string &key);
  /**
   * @brief Return map of entities in composite
   */
  const std::map<std::string, GlSimpleEntity *> &getGlEntities() const {
    return elements;
  }

  /**
   * @brief Set stencil number for all composite's children
   *
   * For more information on stencil  :
   * @see GlSimpleEntity
   */
  void setStencil(int stencil) override {
    this->stencil = stencil;

    for (std::list<GlSimpleEntity *>::iterator it = _sortedElements.begin();
         it != _sortedElements.end(); ++it) {
      (*it)->setStencil(stencil);
    }
  }

  /**
   * @brief Set if at the destruction of composite, components well be deleted
   */
  void setDeleteComponentsInDestructor(bool deleteComponentsInDestructor) {
    this->deleteComponentsInDestructor = deleteComponentsInDestructor;
  }

  /**
   * @brief translate the composite with children
   */
  void translate(const Coord &mouvement) override;

  /**
   * @brief Function to export data in outString (in XML format)
   */
  void getXML(std::string &outString) override;

  /**
   * @brief Function to set data with inString (in XML format)
   */
  void setWithXML(const std::string &inString, unsigned int &currentPosition) override;

  ///@cond DOXYGEN_HIDDEN

  /**
   * Function used to visit composite's children
   */
  void acceptVisitor(GlSceneVisitor *visitor) override {
    // visitor->visit(this);
    for (auto entity : _sortedElements) {
      if (entity->isVisible()) {

#ifndef NDEBUG
        GlComposite *composite = dynamic_cast<GlComposite *>(entity);

        if (!composite && !entity->getBoundingBox().isValid()) {
          for (auto &itE : elements) {
            if (itE.second == entity) {
              tlp::warning() << "Invalid bounding box for entity: " << itE.first << std::endl;
              assert(false);
            }
          }
        }

#endif

        entity->acceptVisitor(visitor);
      }
    }
  }

  /**
   * Add a layer parent from this entity
   */
  virtual void addLayerParent(GlLayer *layer);

  /**
   * Remove a layer parent from this entity
   */
  virtual void removeLayerParent(GlLayer *layer);

  /**
   * Call when a child of the composite is modified
   */
  void notifyModified(GlSimpleEntity *entity);

  /**
   * \warning This function does nothing, GlComposite is a GlSimpleEntity
   * so the draw member function must be defined
   */
  void draw(float, Camera *) override {}

  ///@endcond

protected:
  std::map<std::string, GlSimpleEntity *> elements;
  std::list<GlSimpleEntity *>
      _sortedElements; // necessary to enable ordering of elements (for alpha blending)
  std::vector<GlLayer *> layerParents;
  bool deleteComponentsInDestructor;
};
} // namespace tlp
#endif
