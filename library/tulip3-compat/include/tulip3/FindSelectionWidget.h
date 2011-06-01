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
#ifndef FINDSELECTIONWIDGET_H
#define FINDSELECTIONWIDGET_H

#include <tulip/tulipconf.h>
#include <tulip/Node.h>
#include <tulip/Edge.h>

#include "ui_FindSelection.h"

namespace tlp {

class Graph;
class PropertyInterface;
class BooleanProperty;

class TLP3_COMPAT_SCOPE FindSelectionWidget : public QDialog,
  public Ui::FindSelectionData {
  Q_OBJECT
public:
  FindSelectionWidget(Graph *graph, std::string &currentProperty,
		      QWidget *parent = 0);
  // returns -1 if cancelled, the number of found items if accepted
  int exec();
  std::string getCurrentProperty();

private:
  Graph *graph;
  int nbItemsFound;
  void insertProperties(std::string& currentProperty);
  int getOperation();
  int getSource();
  int getMode();
  void evalNodes(PropertyInterface *property, int mode, std::string fv, int op, BooleanProperty *selection);
  void evalEdges(PropertyInterface *property, int mode, std::string fv, int op, BooleanProperty *selection);

  PropertyInterface *getProperty();
  void find(BooleanProperty *);
  
protected slots:
  virtual void propertyChanged( int no );
  virtual void accept();
};

}
#endif //FINDSELECTIONWIDGET_H
