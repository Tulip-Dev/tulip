/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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

#ifndef PARALLELCOORDSELEMENTSHOWINFOS_H_
#define PARALLELCOORDSELEMENTSHOWINFOS_H_

#include <QCursor>
#include <QMouseEvent>

#include <tulip/MouseShowElementInfos.h>
#include <tulip/GlScene.h>

namespace tlp {

class ParallelCoordsElementShowInfos : public MouseShowElementInfos {
public :
  bool pick(int x, int y, SelectedEntity &selectedEntity);
};

}
#endif /* PARALLELCOORDSELEMENTSHOWINFOS_H_ */
