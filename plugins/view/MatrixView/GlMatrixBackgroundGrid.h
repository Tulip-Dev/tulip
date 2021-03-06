/**
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

#ifndef GLMATRIXBACKGROUNDGRID_H
#define GLMATRIXBACKGROUNDGRID_H

#include <tulip/GlSimpleEntity.h>
namespace tlp {
class MatrixView;

class GlMatrixBackgroundGrid : public tlp::GlSimpleEntity {
public:
  GlMatrixBackgroundGrid(MatrixView *view);
  tlp::BoundingBox getBoundingBox() override;
  void draw(float lod, tlp::Camera *camera) override;

  void getXML(std::string &) override {}
  void setWithXML(const std::string &, unsigned int &) override {}

private:
  MatrixView *_view;
};
} // namespace tlp
#endif // GLMATRIXBACKGROUNDGRID_H
