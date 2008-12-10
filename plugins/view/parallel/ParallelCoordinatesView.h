//-*-c++-*-
/*
 Author: Antoine Lambert

 Email : antoine.lambert@labri.fr

 Last modification : 12/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef Tulip_PARALLELVIEW_H
#define Tulip_PARALLELVIEW_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include "ParallelCoordinatesDrawing.h"
#include "ParallelCoordinatesConfigDialog.h"
#include "QuantitativeParallelAxis.h"

#include <QtGui/QMainWindow>

#include <tulip/GlMainView.h>
#include <tulip/ObservableGraph.h>

#include <set>

class QMenu;

namespace tlp {

  // Parallel Coordinates main class which acts as a mediator between :
  //    -> the data (the nodes or the edges of a graph),
  //    -> the parallel coordinates drawing (updated each times the data and their properties changed or when the drawing parameters are modified by the user)
  //    -> the view config dialog
  //    -> the OpenGL rendering widget
  class TLP_QT_SCOPE ParallelCoordinatesView : public GlMainView {

    Q_OBJECT;

  public :

    ParallelCoordinatesView();
    ~ParallelCoordinatesView();

    // View methods
    QWidget *construct(QWidget *parent);
    void setData(Graph *graph, DataSet dataSet);
    void getData(Graph **graph, DataSet *dataSet);
    Graph *getGraph();
    void installInteractor(QAction *);
    void constructInteractorsMap();
    void constructInteractorsActionList();
    void specificEventFilter(QObject *object,QEvent *event);
    void buildContextMenu(QObject *object,QMouseEvent *event,QMenu *contextMenu);
    void computeContextMenuAction(QAction *action);

    // methods called by interactors
    void setDataUnderPointerSelectFlag(const int x, const int y, const bool selectFlag);
    void setDataInRegionSelectFlag(const int x, const int y, const unsigned int width, const unsigned int height, const bool selectFlag);
    void resetSelection();
    void deleteDataUnderPointer(const int x, const int y);
    void showDataUnderPointerProperties(const int x, const int y);
    void highlightDataUnderPointer(const int x, const int y, const bool addEltToMagnifyFlag);
    void highlightDataInRegion(const int x, const int y, const int width, const int height, const bool addEltToMagnifyFlag);
    void resetHighlightedElements();
    bool highlightedElementsSet() const;
    ParallelAxis *getAxisUnderPointer(const int x, const int y) const;
    void swapAxis(ParallelAxis *firstAxis, ParallelAxis *secondAxis);
    void removeAxis(ParallelAxis *axis);
    void addAxis(ParallelAxis *axis);
    std::vector<ParallelAxis *> getAllAxis();

    void updateWithAxisSlidersRange(ParallelAxis *axis);
    void updateAxisSlidersPosition();

    void highlightDataInAxisBoxPlotRange(QuantitativeParallelAxis *axis);


  public slots:

    void draw();
    void centerView();
    void refresh();
    void init();
    void setGraph(Graph *graph);
    void elementSelectedSlot(unsigned int id, bool isNode) {
          emit elementSelected(id,isNode);
    }

  private :

	std::set<unsigned int> mapGlEntitiesInRegionToData(const int x, const int y, const unsigned int width, const unsigned int height);
	void initGlWidget();
	void buildMenuEntries();
	void showConfigDialog();
	void showAxisConfigDialog(ParallelAxis *axis);

	void setUpAndDrawView();
	void updateWithProgressBar();
	void updateWithoutProgressBar();

	viewType getViewType() const;
	void setViewType(const viewType vType);

	void cleanup();

    QMenu *dialogMenu;
    QMenu *viewSetupMenu;
    QMenu *optionsMenu;
    QMenu *exportImageMenu;
    QAction *view2d;
    QAction *view2dSpline;
    QAction *showToolTips;
    QAction *addRemoveDataFromSelection;
    QAction *selectData;
    QAction *deleteData;
    QAction *showDataProperties;

    unsigned int dataUnderMousePointer;
    GlLayer *mainLayer;

    GlGraphComposite *glGraphComposite;
    DataSet *dataSet;

    ParallelCoordinatesConfigDialog *configDialog;
    ParallelCoordinatesGraphProxy *graphProxy;
    ParallelCoordinatesDrawing *parallelCoordsDrawing;
    ParallelAxis *axisUnderPointer;

    bool center;
  };

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_PARALLELVIEW_H
