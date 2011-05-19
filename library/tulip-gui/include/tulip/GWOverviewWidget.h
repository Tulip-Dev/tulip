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
#ifndef GWOVERVIEW_H
#define GWOVERVIEW_H

#include <tulip/Camera.h>
#include <tulip/ObservableGraph.h>
#include <tulip/GlMetaNodeRenderer.h>
#include <QtGui/qcolor.h>
#include <string>
#include <QtCore/qstring.h>
#include <QtGui/qlayout.h>
#include <QtGui/QWidget>

class QEvent;

namespace Ui {
class GWOverviewWidgetData;
}

namespace tlp {
  class GlGraph;

  class GlMainWidget;
  struct RectPosition;
  class GWOverviewWidget;

/** \addtogroup Tulip_Widgets */
/*@{*/
  class TLP_QT_SIMPLE_SCOPE GWOverviewWidget :
    public QWidget {
  Q_OBJECT

public:
  GWOverviewWidget(QWidget* parent = 0,bool drawIfNotVisible = false);
  ~GWOverviewWidget();
  bool eventFilter(QObject *, QEvent *);
  GlMainWidget *getObservedView();
  GlMainWidget *getView();

  void setDrawIfNotVisible(bool drawIfNotVisible){this->drawIfNotVisible=drawIfNotVisible;}

signals:

  void hideOverview(bool);

public slots:
  /*void syncFromView();*/
  void setObservedView(GlMainWidget *,GlSimpleEntity *entity);
  void updateView();
  /*void backColor(); //background color button
  void showRenderingParametersDialog();*/

  void show();

private slots:
  //Used to catch graphRedrawn signal from view of which
  //we are showing an overview
  void draw(GlMainWidget *glWidget,bool graphChanged=true);

  //Used to catch the destroyed signal from view of which
  //we are showing an overview
  void observedViewDestroyed(QObject *glWidget);

private :
  Ui::GWOverviewWidgetData *_ui;

  GlMainWidget *_observedView;
  GlMainWidget *_view;
  bool _synchronizing;
  RectPosition *_glDraw;
  bool _initialCameraIsInit;
  tlp::Camera *_initialCamera;
  GlMetaNodeRenderer metaNodeRenderer;
  bool drawIfNotVisible;
  //void setBackgroundColor(QColor);
};

}
/*@}*/
#endif // GWOVERVIEW_H
