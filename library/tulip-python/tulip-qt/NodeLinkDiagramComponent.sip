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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (http://booggie.org/)                                                   |
// +-------------------------------------------------------------------------+

namespace tlp {
class NodeLinkDiagramComponent : tlp::GlMainView {
%TypeHeaderCode
#include <tulip/NodeLinkDiagramComponent.h>
%End

public:
	NodeLinkDiagramComponent();
	
	virtual ~NodeLinkDiagramComponent();

	virtual QWidget *construct(QWidget *parent);

	virtual void setData(tlp::Graph *graph, tlp::DataSet dataSet);
	
	virtual void getData(tlp::Graph **graph, tlp::DataSet *dataSet);

	virtual void specificEventFilter(QObject *object,QEvent *event);
	
	virtual void buildContextMenu(QObject *object, QContextMenuEvent *event,QMenu *contextMenu);
	
	virtual void computeContextMenuAction(QAction *action);
protected:
	
	void checkAlgorithmResult();

protected slots:
	void showDialog(QAction*);
	
	void gridOptions();

public slots:
	void centerView();
	
	void drawAfterRenderingParametersChange();
	
	void draw();
	
	void refresh();
	
	void init();
	
	void setGraph(tlp::Graph *graph);

	void elementSelectedSlot(unsigned int id,bool isNode);
};
};