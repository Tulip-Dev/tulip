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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

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

    GlMainView *mainView;
    GlMainWidget* observedMainWidget;
    bool holdUpdateView;

  public:
    RenderingParametersDialog(QWidget* parent=0);

    void setGlMainView(GlMainView *view);

  public slots:

    void updateView();
    void backColor();
    void selectionColor();
    void selectionSaveAtDefaultColor();
    void setButtonColor(QColor tmp,QPushButton *button);

  signals :

    void viewNeedDraw();

  };

}

#endif // Tulip_RENDERINGPARAMETERSDIALOG_H
