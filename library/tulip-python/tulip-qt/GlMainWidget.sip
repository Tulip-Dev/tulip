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
class GlMainWidget : QGLWidget {
%TypeHeaderCode
#include <tulip/GlMainWidget.h>
%End

%ConvertToSubClassCode
	if (dynamic_cast<tlp::GlMainWidget *>(sipCpp)) {
		sipType = sipType_tlp_GlMainWidget;
	} else {
		sipType = NULL;
	}
%End

public:
	GlMainWidget(QWidget *parent, tlp::AbstractView *view=NULL);
	
	~GlMainWidget();

	void setData(tlp::Graph *graph, tlp::DataSet dataSet);

	tlp::DataSet getData();

	void setGraph(tlp::Graph *graph);

	tlp::Graph *getGraph();

	tlp::View *getView();

	tlp::GlScene* getScene();

	void doSelect(const int x, const int y,
			const int width, const int height,
			std::vector<tlp::node> &sNode /Out/, std::vector<tlp::edge> &sEdge /Out/,
			tlp::GlLayer* layer=NULL);
	
	bool doSelect(const int x, const int y,
			tlp::ElementType &type /Out/,
			tlp::node & /Out/, tlp::edge & /Out/,
			tlp::GlLayer* layer=NULL);
	
	
	bool selectGlEntities(const int x, const int y,
			const int width, const int height,
			std::vector<tlp::GlSimpleEntity *>&pickedEntities /Out/, tlp::GlLayer* layer=NULL);
	

	bool selectGlEntities(const int x, const int y,
			std::vector<tlp::GlSimpleEntity *>&pickedEntities /Out/, tlp::GlLayer* layer=NULL); 
	
	
	virtual QImage grabFrameBuffer(bool withAlpha = false);

	void setViewLabel(bool viewLabel);

	virtual void makeCurrent();

public slots:
	void draw(bool graphChanged=true);

	void redraw();
	
	void closeEvent(QCloseEvent *e);
	
	protected slots:
	void paintEvent( QPaintEvent* );
	
signals:
	void closing(tlp::GlMainWidget *, QCloseEvent *);
	void graphRedrawn(tlp::GlMainWidget *glWidget,bool graphChanged);
	
public :
	static QGLWidget* getFirstQGLWidget();
};
};
