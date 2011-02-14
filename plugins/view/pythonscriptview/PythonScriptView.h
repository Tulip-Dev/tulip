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
// | Tulip Python View                                                       |
// | Author:  Antoine Lambert 												 |
// | Contact:  antoine.lambert@labri.fr 								     |
// +-------------------------------------------------------------------------+
// | License:                                                                |
// |                                                                         |
// | Tulip Python View is free software; you can redistribute it             |
// | and/or modify  it under the terms of the GNU General Public License     |
// | as published by the Free Software Foundation; either version 2 of the   |
// | License, or (at your option) any later version.                         |
// |                                                                         |
// | Tulip Python View is distributed in the hope that it will be            |
// | useful, but WITHOUT ANY WARRANTY; without even the implied warranty of  |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
// | GNU General Public License for more details.                            |
// |                                                                         |
// | You should have received a copy of the GNU General Public License       |
// | along with this program.  If not, see <http://www.gnu.org/licenses/>.   |
// |                                                                         |
// +-------------------------------------------------------------------------+

#ifndef PYTHONSCRIPTVIEW_H_
#define PYTHONSCRIPTVIEW_H_

#include "PythonScriptViewWidget.h"

#include <QtCore/QObject>
#include <QtWebKit/QWebView>

#include <tulip/AbstractView.h>

class PythonInterpreter;

namespace tlp {
class SGHierarchyWidget;
}

class PythonScriptView : public tlp::AbstractView {

	Q_OBJECT

public :

	PythonScriptView();
	~PythonScriptView();

	QWidget *construct(QWidget *parent);

	void setData(tlp::Graph *graph, tlp::DataSet dataSet);

	void getData(tlp::Graph **graph, tlp::DataSet *dataSet);

	tlp::Graph* getGraph() {return graph;}
	void createPicture(const std::string&, int, int) {}
	void setGraph(tlp::Graph* graph) {this->graph = graph;}
	void draw() {}
	void refresh() {}
	void init() {}

	bool eventFilter(QObject *obj, QEvent *event);

private slots :

	void newScript();
	void loadScript();
	void saveScript();
	void executeCurrentScript();
	void stopCurrentScript();
	void showApiDocumentation();
	void newStringModule();
	void newFileModule();
	void loadModule();
	void saveModuleToFile();
	void mainScriptTextChanged();
	void moduleScriptTextChanged();
	void closeModuleTabRequested(int tab);
	void decreaseFontSize();
	void increaseFontSize();

private :

	bool loadScript(const QString &fileName);
	bool loadModule(const QString &fileName);
	bool loadModuleFromSrcCode(const std::string &moduleName, const std::string &moduleSrcCode);
	void saveModule();
	void saveModule(int tabIdx);
	void saveAllModules();
	void reloadAllModules();
	void commentSelectedCode(QObject *obj);
	void uncommentSelectedCode(QObject *obj);


	PythonScriptViewWidget *viewWidget;
	PythonInterpreter *pythonInterpreter;
	tlp::Graph *graph;
	QDialog *browserDialog;
	QWebView *browser;

	std::string mainScriptFileName;
	std::map<int, std::string> editedModules;
	int fontZoom;

	tlp::SGHierarchyWidget *clusterTreeWidget;

};

#endif /* PYTHONSCRIPTVIEW_H_ */
