//-*-c++-*-
#ifndef GWOVERVIEW_H
#define GWOVERVIEW_H

#include "tulip/GWOverviewWidgetData.h"
#include "tulip/RenderingParametersDialogData.h"
#include <tulip/Camera.h>
#include <tulip/ObservableGraph.h>
#include <tulip/GlMetaNodeRenderer.h>
#include <QtGui/qcolor.h>
#include <string>
#include <QtCore/qstring.h>
#include <QtGui/qlayout.h>

class QEvent;

namespace tlp {
  class GlGraph;

  class GlMainWidget;
  class RectPosition;
  class GWOverviewWidget;

/** \addtogroup Tulip_Widgets */
/*@{*/
  class TLP_QT_SIMPLE_SCOPE GWOverviewWidget :
    public QWidget,public Ui::GWOverviewWidgetData {
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

private slots:
  //Used to catch graphRedrawn signal from view of which
  //we are showing an overview
  void draw(GlMainWidget *glWidget,bool graphChanged=true);

  //Used to catch the destroyed signal from view of which
  //we are showing an overview
  void observedViewDestroyed(QObject *glWidget);

private :
  GlMainWidget *_observedView;
  GlMainWidget *_view;
  bool _synchronizing;
  RectPosition *_glDraw;
  tlp::Camera *_initialCamera;
  GlMetaNodeRenderer metaNodeRenderer;
  bool drawIfNotVisible;
  //void setBackgroundColor(QColor);
};

}
/*@}*/
#endif // GWOVERVIEW_H
