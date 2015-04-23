/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#include <tulip/Color.h>

#include <QWidget>
#include <QObject>

namespace Ui {
class EnclosingCircleConfigurationData;
}

namespace tlp {

class EnclosingCircleConfigurationWidget: public QWidget {

  Q_OBJECT

  Ui::EnclosingCircleConfigurationData* _ui;
public:
  EnclosingCircleConfigurationWidget(QWidget *parent=0);
  ~EnclosingCircleConfigurationWidget();

  void circleColorBtnDisabled(const bool disabled);
  void inverseColorRadioCheck(const bool checked);
  void solidColorRadioCheck(const bool checked);
  void alphaSliderSetValue(const int val);

signals:
  void solidColorRadioChecked(bool);
  void inverseColorRadioChecked(bool);
  void colorButtonClicked();
  void alphaChanged(int);
};

class EnclosingCircleHighlighter: public QObject, public PathHighlighter {
  Q_OBJECT

public:
  EnclosingCircleHighlighter();
  ~EnclosingCircleHighlighter();
  void highlight(const PathFinder *parent, GlMainWidget *glMainWidget, BooleanProperty *selection, tlp::node src, tlp::node tgt);
  void draw(GlMainWidget *glMainWidget);
  bool isConfigurable() const;
  QWidget *getConfigurationWidget();

public slots:
  void inverseColorRadioChecked(bool checked);
  void solidColorRadioChecked(bool checked);
  void colorButtonClicked();
  void alphaChanged(int);

private:
  Color circleColor, outlineColor;
  int alpha;
  bool inversedColor;

  EnclosingCircleConfigurationWidget *configurationWidget;
};
}
#endif /* ENCLOSINGCIRCLEHIGHLIGHTER_H_ */
