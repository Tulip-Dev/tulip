/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
///@cond DOXYGEN_HIDDEN

#ifndef Tulip_GLDRAWSCENEVISITOR_H
#define Tulip_GLDRAWSCENEVISITOR_H

#include <unordered_map>
#include <tulip/GlSceneVisitor.h>

namespace tlp {

class GlGraphInputData;

class TLP_GL_SCOPE GlDrawSceneVisitor : public GlSceneVisitor {

public:
  GlDrawSceneVisitor(std::unordered_map<unsigned int, float> *lod, GlGraphInputData *inputData)
      : inputData(inputData) {
    lodMap = lod;
  }

  virtual void visit(GlSimpleEntity *entity);
  virtual void visit(GlComplexeEntity *entity);
  virtual void visit(GlLayout *layout);

private:
  std::unordered_map<unsigned int, float> *lodMap;
  GlGraphInputData *inputData;
};
} // namespace tlp

#endif // Tulip_GLDRAWSCENEVISITOR_H
///@endcond
