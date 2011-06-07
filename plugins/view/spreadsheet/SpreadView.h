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

#ifndef Tulip_SPREADVIEW_H
#define Tulip_SPREADVIEW_H

#include <QtGui/QWidget>

#include <tulip/tulipconf.h>

#include <tulip/AbstractView.h>

#include <QtCore/QModelIndexList>


namespace Ui {
class SpreadViewWidget;
}

class TulipTableWidget;
namespace tlp {

class Graph;

class SpreadView: public AbstractView {
    Q_OBJECT
public:

    SpreadView();
    virtual ~SpreadView();

    QWidget *construct(QWidget *parent);
    void setData(Graph *graph, DataSet dataSet);
    void getData(Graph **graph, DataSet *dataSet);
    Graph *getGraph();

    void installInteractor(QAction*) {
    }

    QImage createPicture(int width, int height, bool center, int zoom = 1, int xOffset = 0, int yOffset = 0);


public slots :

    /**
    * @brief Show property menu
    **/
    void showPropertiesContextMenu(const QPoint& position);


    /**
    * @brief Show the context menu when user click on the table view.
    **/
    void showTableContextMenu(const QPoint& position);

    /**
    * @brief Called when user request the context menu on the elements header view.
    */
    void showElementsContextMenu(const QPoint& position);

    void draw();
    void refresh();
    void init();
    void setGraph(Graph *graph);

protected :

    void showPropertiesContextMenu(TulipTableWidget* tableWidget,int clickedColumn,const QPoint& position);
    void fillPropertiesContextMenu(QMenu& menu,TulipTableWidget* tableWidget,int clickedColumn);
    /**
      * @brief Execute the context menu for the selected row.
      **/
    void showElementsContextMenu(TulipTableWidget* tableWidget,int clickedRowIndex,const QPoint& position);
    void fillElementsContextMenu(QMenu& menu,TulipTableWidget* tableWidget,int clickedRowIndex);

    void deleteElements( const QModelIndexList& elements,TulipTableWidget *tableWidget ,bool delAll);

    /**
      * @brief Return the active table.
      **/
    TulipTableWidget* currentTable()const;
protected slots:    

    //Columns operations    
    void hideColumn();
    void createNewProperties();
    void copyColumnToOther();
    void setAllColumnValues();
    void resetColumn();
    void deleteColumn();

    //Rows operations
    void selectElements();
    void highlightElements();
    void deleteElements();
    //Nodes operations
    void copyNodes();
    void group();    
    void ungroup();

private:
    Ui::SpreadViewWidget *ui;
    tlp::Graph* _graph;
};

}

#endif
