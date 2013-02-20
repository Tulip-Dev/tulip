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

#ifndef ENCLOSINGCIRCLEHIGHLIGHTER_H_
#define ENCLOSINGCIRCLEHIGHLIGHTER_H_

#include "PathHighlighter.h"

#include <QtGui/qwidget.h>

#include "ui_EnclosingCircleConfiguration.h"

class EnclosingCircleConfigurationWidget: public QWidget, public Ui::EnclosingCircleConfigurationData {
public:
  EnclosingCircleConfigurationWidget(QWidget *parent=0): QWidget(parent) {
    setupUi(this);
  }
};

class EnclosingCircleHighlighter: public QObject, public PathHighlighter {
  Q_OBJECT
public:
  inline EnclosingCircleHighlighter(): PathHighlighter("Enclosing circle"), circleColor(200,200,200), outlineColor(0,0,0), alpha(128), inversedColor(false) {}
  void highlight(const PathFinder *parent, tlp::GlMainWidget *glMainWidget, tlp::BooleanProperty *selection, tlp::node src, tlp::node tgt);
  void draw(tlp::GlMainWidget *glMainWidget);
  PathHighlighter *clone();
  bool isConfigurable();
  QWidget *getConfigurationWidget();

public slots:
  void inverseColorRadioChecked(bool checked);
  void solidColorRadioChecked(bool checked);
  void colorButtonClicked();
  void alphaChanged(int);

private:
  tlp::Color circleColor;
  tlp::Color outlineColor;
  int alpha;
  bool inversedColor;

  EnclosingCircleConfigurationWidget *configurationWidget;
};

#endif /* ENCLOSINGCIRCLEHIGHLIGHTER_H_ */
