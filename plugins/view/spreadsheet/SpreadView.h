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

#include "SpreadWidget.h"

namespace tlp {

class Graph;

class SpreadConfigurationWidget;

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

	virtual void createPicture(const std::string&, int, int);

	std::list<std::pair<QWidget *, std::string> > getConfigurationWidget();

	std::vector<std::string> getSelectedProperties() const;
protected:


	SpreadWidget *mainWidget;
	SpreadConfigurationWidget *configurationWidget;

public slots :

	void draw();
	void refresh();
	void init();
	void setGraph(Graph *graph);


	protected slots:
	void updated();
};

}

#endif
