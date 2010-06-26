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
#include "SpreadView.h"

#include <iostream>

#include <tulip/Graph.h>
#include "SpreadConfigurationWidget.h"

using namespace std;

static void tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ") {
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	string::size_type pos = str.find_first_of(delimiters, lastPos);
	while (string::npos != pos || string::npos != lastPos) {
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		lastPos = str.find_first_not_of(delimiters, pos);
		pos = str.find_first_of(delimiters, lastPos);
	}
}

namespace tlp {

SpreadView::SpreadView() :
	AbstractView(),configurationWidget(NULL) {
}
SpreadView::~SpreadView() {
  if(configurationWidget)
    delete configurationWidget;
}

QWidget *SpreadView::construct(QWidget *parent) {
	QWidget *widget = AbstractView::construct(parent);
	mainWidget = new SpreadWidget(this, widget);
	setCentralWidget(mainWidget);

	configurationWidget = new SpreadConfigurationWidget();
	connect(configurationWidget, SIGNAL(updated()), this, SLOT(updated()));

	return widget;
}

void SpreadView::setData(Graph *graph, DataSet dataSet) {
	configurationWidget->setGraph(graph);
	if (dataSet.exist("data")) {
		DataSet data;
		dataSet.get("data", data);
		if (data.exist("displayedProperties")) {
			string properties;
			data.get("displayedProperties", properties);
			vector<string> propertiesVector;
			tokenize(properties, propertiesVector, ";");
			configurationWidget->setSelectedProperties(propertiesVector);
		}
	}
	mainWidget->setData(graph, dataSet);
}

void SpreadView::getData(Graph **graph, DataSet *dataSet) {
	DataSet data;

	if (!configurationWidget->allPropertiesSelected()) {
		ostringstream oss;
		vector<string> selectedProperties = configurationWidget->getSelectedProperties();
		for (vector<string>::iterator it = selectedProperties.begin(); it
				!= selectedProperties.end(); ++it) {
			oss << *it << ";";
		}
		data.set("displayedProperties", oss.str());
	}
	dataSet->set<DataSet> ("data", data);
	*graph = mainWidget->getGraph();
}

Graph* SpreadView::getGraph() {
	return mainWidget->getGraph();
}

void SpreadView::setGraph(Graph *graph) {
	mainWidget->setData(graph, DataSet());
}

void SpreadView::draw() {
	mainWidget->redrawView();
}

void SpreadView::refresh() {
	draw();
}

void SpreadView::init() {
	draw();
}

void SpreadView::createPicture(const string &pictureName, int width, int height) {
	cout << "createPicture not implement yet for SpreadView" << endl;
}

list<pair<QWidget *, string> > SpreadView::getConfigurationWidget() {
	list<pair<QWidget *, string> > widgets;
	widgets.push_back(make_pair(configurationWidget, "Properties Selection"));
	return widgets;
}

std::vector<std::string> SpreadView::getSelectedProperties() const {
	return configurationWidget->getSelectedProperties();
}

void SpreadView::updated() {
	//Force to reload
	mainWidget->redrawView();
}
VIEWPLUGIN(SpreadView, "Table view", "Tulip Team", "16/04/2008", "Spreadsheet view", "1.0")
;

}
