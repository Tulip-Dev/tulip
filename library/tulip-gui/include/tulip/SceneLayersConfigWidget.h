/*
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
///@cond DOXYGEN_HIDDEN


#ifndef SCENELAYERSCONFIGWIDGET_H
#define SCENELAYERSCONFIGWIDGET_H

#include <QWidget>

#include <tulip/tulipconf.h>

namespace Ui {
class SceneLayersConfigWidget;
}

namespace tlp {

class GlMainWidget;

class TLP_QT_SCOPE SceneLayersConfigWidget : public QWidget {
  Q_OBJECT

  Ui::SceneLayersConfigWidget* _ui;
  tlp::GlMainWidget* _glMainWidget;
public:
  explicit SceneLayersConfigWidget(QWidget *parent = NULL);
  virtual ~SceneLayersConfigWidget();

  void setGlMainWidget(tlp::GlMainWidget* glMainWidget);

public slots:

  void resizeFirstColumn();

signals:
  void drawNeeded();


};
}

#endif // SCENELAYERSCONFIGWIDGET_H
///@endcond
