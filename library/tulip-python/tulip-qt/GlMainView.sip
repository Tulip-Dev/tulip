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

class GlMainView : tlp::AbstractView {
%TypeHeaderCode
#include <tulip/GlMainView.h>
%End

public:
	GlMainView();

	virtual QWidget *construct(QWidget *parent);

	tlp::Graph *getGraph();
	
	tlp::GlMainWidget *getGlMainWidget();
	
	virtual void createPicture(const std::string &pictureName,int width, int height);

	virtual void buildContextMenu(QObject *object,QContextMenuEvent *event,QMenu *contextMenu);

public slots:

	virtual void draw();

	virtual void refresh();

	virtual void hideOverview(bool);

	virtual void showDialog(QAction* action);

};
};