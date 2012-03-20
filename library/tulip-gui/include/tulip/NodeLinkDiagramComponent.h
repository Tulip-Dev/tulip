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
#ifndef _Tulip_NODELINKDIAGRAMCOMPONENT_H
#define _Tulip_NODELINKDIAGRAMCOMPONENT_H

#include <tulip/GlMainView.h>

namespace tlp {
class GlGrid;

class TLP_QT_SCOPE NodeLinkDiagramComponent: public tlp::GlMainView {
  Q_OBJECT

  tlp::GlGrid* _grid;
  QDialog* _gridOptions;

  void registerTriggers();
  void updateGrid();
public:
  PLUGININFORMATIONS("Node Link Diagram view", "Tulip Team", "16/04/2008", "Node link diagram", "1.0", "relational")

  NodeLinkDiagramComponent(const tlp::PluginContext* context = NULL);

  void setState(const tlp::DataSet &);
  tlp::DataSet state() const;
  bool isLayoutProperty(tlp::PropertyInterface *) const;

public slots:
  void draw(PluginProgress *pluginProgress);

protected slots:
  void redraw();
  void setZOrdering(bool);
  void showGridControl();

protected:
  void graphChanged(tlp::Graph *);

};
}

#endif /* NODELINKDIAGRAMCOMPONENT_H_ */






