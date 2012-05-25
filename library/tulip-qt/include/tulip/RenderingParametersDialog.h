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
#ifndef Tulip_RENDERINGPARAMETERSDIALOG_H
#define Tulip_RENDERINGPARAMETERSDIALOG_H

#include <tulip/tulipconf.h>

#include <tulip/GlGraphRenderingParameters.h>
#include "tulip/RenderingParametersDialogData.h"

#include <QtGui/QTreeWidgetItem>

namespace tlp {

class GlScene;
class GlLayer;
class GlComposite;
class GlGraphComposite;
class GlMainView;
class GlMainWidget;

class TLP_QT_SCOPE RenderingParametersDialog : public QWidget, public Ui::RenderingParametersDialogData {
  Q_OBJECT

  GlMainWidget* glWidget;
  bool holdUpdateView;

public:
  RenderingParametersDialog(QWidget* parent=0);

  void setGlMainWidget(GlMainWidget *glWidget);

public slots:

  void updateView();
  void backColor();
  void selectionColor();
  void selectionSaveAtDefaultColor();
  void setButtonColor(QColor tmp,QPushButton *button);
  void labelSizeChanged(int);
  void updateOrderingProperty(QString propertyName);
  void setInitViewOnSetGraph();

signals :

  void viewNeedDraw();

protected :

  void recreateOrderingPropertyCombobox(GlGraphRenderingParameters& param);
  void updateDensityLabels();

};

}

#endif // Tulip_RENDERINGPARAMETERSDIALOG_H
