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
#include <tulip/ObservableProperty.h>
#include <tulip/MutableContainer.h>

#include <QtCore/QModelIndexList>


namespace Ui {
class SpreadViewWidget;
}

class GraphTableWidget;
namespace tlp {
class Graph;
class BooleanProperty;
}

class SpreadView: public tlp::AbstractView , public tlp::PropertyObserver, public tlp::Observable{
    Q_OBJECT
public:

    SpreadView();
    virtual ~SpreadView();

    QWidget *construct(QWidget *parent);
    void setData(tlp::Graph *graph, tlp::DataSet dataSet);
    void getData(tlp::Graph **graph, tlp::DataSet *dataSet);
    tlp::Graph *getGraph();

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
    void setGraph(tlp::Graph *graph);

    void treatEvent(const tlp::Event &);
    void treatEvents(const std::vector<tlp::Event> &events);

protected :

    void showPropertiesContextMenu(GraphTableWidget* tableWidget,int clickedColumn,const QPoint& position);
    void fillPropertiesContextMenu(QMenu& menu,GraphTableWidget* tableWidget,int clickedColumn);
    /**
      * @brief Execute the context menu for the selected row.
      **/
    void showElementsContextMenu(GraphTableWidget* tableWidget,int clickedRowIndex,const QPoint& position);
    void fillElementsContextMenu(QMenu& menu,GraphTableWidget* tableWidget,int clickedRowIndex);

    void deleteElements( const QModelIndexList& elements,GraphTableWidget *tableWidget ,bool delAll);

    /**
      * @brief Return the active table.
      **/
    GraphTableWidget* currentTable()const;
protected slots:    

    void updateElementVisibility(int state);
    void showOnlySelectedElements(bool show);
    void filterElements();
    void filterElements(const QString& pattern);

    //Columns operations    
    void hideColumn();
    void createNewProperties();
    void copyColumnToOther();
    void setAllColumnValues();
    void resetColumn();
    void deleteColumn();

    //Rows operations
    void selectElements();
    void addToSelection();
    void removeFromSelection();
    void highlightElements();
    void deleteElements();
    //Nodes operations
    void copyNodes();
    void group();    
    void ungroup();

    void columnsInserted(const QModelIndex & parent, int first, int last);
    void columnsDeleted(const QModelIndex & parent, int first, int last);
    void rowsInserted(const QModelIndex & parent, int first, int last);
    void dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);
    void modelReset();

private:

    bool displayOnlySelectedElements()const;
    QRegExp elementValuesFilter()const;

    void updateSelectionForHighlightedElements(GraphTableWidget* tableWidget,bool select,bool clearOldSelection=false);

    void updateFilters();

    Ui::SpreadViewWidget *ui;
    tlp::Graph* _graph;

    tlp::MutableContainer<bool> _updatedNodes;
    tlp::MutableContainer<bool> _updatedEdges;
    tlp::BooleanProperty* _selectionProperty;
    bool _reloadSelectionProperty;

};


#endif
