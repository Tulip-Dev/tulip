// +-------------------------------------------------------------------------+
// | Tulip Software                                                          |
// | Author:  Morgan Mathiaut                                                |
// +-------------------------------------------------------------------------+
// | License:                                                                |
// |                                                                         |
// | Tulip is free software; you can redistribute it and/or modify           |
// | it under the terms of the GNU General Public License as published by    |
// | the Free Software Foundation; either version 2 of the License, or       |
// | (at your option) any later version.                                     |
// |                                                                         |
// | Tulip is distributed in the hope that it will be useful,                |
// | but WITHOUT ANY WARRANTY; without even the implied warranty of          |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
// | GNU General Public License for more details.                            |
// |                                                                         |
// | You should have received a copy of the GNU General Public License       |
// | along with QSimpleSheet; if not, write to the Free Software             |
// | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA|
// |                                                                         |
// +-------------------------------------------------------------------------+

#ifndef Tulip_SPREADWIDGET_H
#define Tulip_SPREADWIDGET_H

#include <QtGui/QWidget>

#include <tulipconf.h>

#include <tulip/View.h>

#include "SpreadTable.h"
#include "SpreadMainWidgetData.h"

namespace tlp {
  
  class Graph;

  class TLP_QT_SCOPE SpreadWidget : public QWidget, public Ui::SpreadMainWidgetData 
  {
    Q_OBJECT
      
  public:

    SpreadWidget(QWidget *parent=0);
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
    
    void tableMouseReleasedSlot(SpreadTable*);
    void returnPressedSlot();
    void beginEditLineSlot();
    void textChangedSlot(const QString &);
    void tabChanged(int);
    
  };

}

#endif
