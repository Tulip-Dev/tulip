 //-*-c++-*-
/*
 Author: Ludwig Fiolka
 Email : ludwig.fiolka@inria.fr
 Last modification : Oct 26, 2009
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 */

#ifndef ZOOMANDPANHIGHLIGHTER_H_
#define ZOOMANDPANHIGHLIGHTER_H_

#include "PathHighlighter.h"

class ZoomAndPanHighlighter: public PathHighlighter, public QObject {
public:
	inline ZoomAndPanHighlighter(): PathHighlighter("Zoom and pan") {}
  void highlight(const PathFinder *parent, tlp::GlMainWidget *glMainWidget, tlp::BooleanProperty *selection, tlp::node src, tlp::node tgt);
	void draw(tlp::GlMainWidget *glMainWidget);
	PathHighlighter *clone();
	bool isConfigurable();
	QWidget *getConfigurationWidget();
};

#endif /* ZOOMANDPANHIGHLIGHTER_H_ */
