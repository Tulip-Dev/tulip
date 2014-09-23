/*
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
///@cond DOXYGEN_HIDDEN

#ifndef TULIPWORKSPACE_H
#define TULIPWORKSPACE_H

#include <QWidget>

#include <tulip/tulipconf.h>

namespace tlp {

class TLP_QT_SCOPE TulipWorkspace: public QWidget {
  Q_OBJECT

public:
  explicit TulipWorkspace(QWidget* parent=NULL);
  virtual ~TulipWorkspace();
};

}

#endif // TULIPWORKSPACE_H
///@endcond
