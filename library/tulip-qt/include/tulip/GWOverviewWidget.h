//-*-c++-*-
#ifndef GWOVERVIEW_H
#define GWOVERVIEW_H

#include "tulip/GWOverviewWidgetData.h"
#include <tulip/Camera.h>
#include <qcolor.h>
#include <string>
#include <qstring.h>
#include <qlayout.h>

namespace tlp {
  class GlGraph;
}

class GlMainWidget;
class QEvent;
class RectPosition;
class RenderingParametersDialogData;

/** \addtogroup Tulip_Widgets */ 
/*@{*/
class TLP_QT_SIMPLE_SCOPE GWOverviewWidget : public GWOverviewWidgetData { 
  Q_OBJECT
  
public:
  GWOverviewWidget(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
  ~GWOverviewWidget();
  bool eventFilter(QObject *, QEvent *);
  GlMainWidget *getObservedView();

public slots:
  void syncFromView();
  void setObservedView(GlMainWidget *);
  void updateView();
  void backColor(); //background color button
  void showRenderingParametersDialog();
  
private slots:
  //Used to catch graphRedrawn signal from view of which
  //we are showing an overview
  void draw(GlMainWidget *glWidget);
  //Used to catch the destroyed signal from view of which
  //we are showing an overview
  void observedViewDestroyed(QObject *glWidget);
  
private :
  GlMainWidget *_observedView;
  GlMainWidget *_view;
  bool _synchronizing;
  RectPosition *_glDraw;
  tlp::Camera *_initialCamera;
  RenderingParametersDialogData *paramDialog;
  void setBackgroundColor(QColor);
};
/*@}*/
#endif // GWOVERVIEW_H
