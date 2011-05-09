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

#ifndef Tulip_SPREADWIDGET_H
#define Tulip_SPREADWIDGET_H

#include <QtGui/QWidget>

#include <tulip/tulipconf.h>

#include <tulip/View.h>

#include "SpreadTable.h"
#include "ui_SpreadMainWidget.h"

namespace tlp {
  
  class Graph;
class SpreadView;
  class SpreadWidget : public QWidget, public Ui::SpreadMainWidgetData 
  {
    Q_OBJECT
      
  public:

    SpreadWidget(SpreadView* view,QWidget *parent=0);
    virtual ~SpreadWidget() {}

    void setData(Graph *graph,DataSet dataSet);
    DataSet getData();
    Graph *getGraph();

  protected:
    
    SpreadTable *activeSpread;
    QTableWidgetItem *currentCell;
    Graph *graph;
    
    bool editingLine;
    QString currentText;
    bool nodeTab;

  public slots :

    void redrawView();

  protected slots :
    
    void tableMouseReleasedSlot(SpreadTable*,QTableWidgetItem *);
    void returnPressedSlot();
    void beginEditLineSlot();
    void textChangedSlot(const QString &);
    void tabChanged(int);
    
  };

}

#endif
