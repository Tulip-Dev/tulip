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

namespace tlp {
  
  class Graph;

  class TLP_QT_SCOPE SpreadLineEdit : public QLineEdit 
  {
    Q_OBJECT

  public :
    
    SpreadLineEdit(QWidget *parent):QLineEdit(parent){}

  signals:

    void widgetActivate();

  protected :
    
    void mousePressEvent(QMouseEvent *event) {
      emit widgetActivate();
      QLineEdit::mousePressEvent(event);
    }
  };

  class TLP_QT_SCOPE SpreadView : public ViewCreator {
  public :
    virtual View* create(QWidget *parent);
  };

  class TLP_QT_SCOPE SpreadWidget : public View
  {
    Q_OBJECT
      
  public:

    SpreadWidget(QWidget *parent=0);
    virtual ~SpreadWidget() {}

    void setData(Graph *graph,std::string *in=NULL);
    Graph *getGraph();

    QWidget *getWidget() {return this;}
      
    void addNode(Graph *,const node );
    void addEdge(Graph *,const edge );
    void delNode(Graph *,const node );
    void delEdge(Graph *,const edge );
    void destroy(Graph *);

  protected:
    
    QVBoxLayout *vboxLayout;
    QLineEdit *lineEdit;
    QTabWidget *tabWidget;
    SpreadTable *spreadNodesTable;
    SpreadTable *spreadEdgesTable;
    SpreadTable *activeSpread;
    QTableWidgetItem *currentCell;
    Graph *graph;
    
    bool editingLine;
    QString currentText;
    bool nodeTab;

  protected slots :
    
    void tableMouseReleasedSlot(SpreadTable*);
    void returnPressedSlot();
    void beginEditLineSlot();
    void textChangedSlot(const QString &);
    void tabChanged(int);
    
  };

}

#endif
