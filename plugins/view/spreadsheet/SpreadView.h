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

#ifndef Tulip_SPREADVIEW_H
#define Tulip_SPREADVIEW_H

#include <QtGui/QWidget>

#include <tulipconf.h>

#include <tulip/AbstractView.h>

#include "SpreadWidget.h"

namespace tlp {
  
  class Graph;

  class TLP_QT_SCOPE SpreadViewCreator : public ViewCreator {
  public :
    virtual View* create(const std::string &pluginName,QWidget *parent);
  };

  class TLP_QT_SCOPE SpreadView : public AbstractView
  {
    Q_OBJECT
      
  public:

    SpreadView(const std::string &pluginName,QWidget *parent=0);
    virtual ~SpreadView() {}

    void setData(Graph *graph,DataSet dataSet);
    void getData(Graph **graph,DataSet *dataSet);
    Graph *getGraph();

  protected :
    
    SpreadWidget *mainWidget;
    
  public slots :

    void draw();
    void reinitAndDraw();
    void setGraph(Graph *graph);

  };

}

#endif
