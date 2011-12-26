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
#ifndef Tulip_GLMAINVIEW_H
#define Tulip_GLMAINVIEW_H

#include <tulip/ViewWidget.h>
#include "tulip/GlOverviewGraphicsItem.h"

namespace tlp {

class GlMainWidget;

class TLP_QT_SCOPE GlMainView: public tlp::ViewWidget {
  Q_OBJECT

  tlp::GlMainWidget* _glMainWidget;
public:
  GlMainView();
  virtual ~GlMainView();

  tlp::GlMainWidget* getGlMainWidget() const;
  GlOverviewGraphicsItem *overview;

public slots:
  virtual void draw(tlp::PluginProgress* pluginProgress);
  virtual void drawOverview();
  virtual void centerView();

protected:
  virtual void setupWidget();
};
}

#endif /* GLMAINVIEW_H_ */
