//-*-c++-*-
#ifndef GWOVERVIEW_H
#define GWOVERVIEW_H

#include "GWOverviewWidgetData.h"
#include <tulip/ObservableGlGraph.h>
#include <tulip/GlAugmentedDisplay.h>
#include <tulip/GlGraphWidget.h>
#include <qcolor.h>
#include <string>
#include <qstring.h>
#include <qlayout.h>

namespace tlp {
  class GlGraph;
}

class GlGraphWidget;
class QEvent;
class RectPosition;

/** \addtogroup Tulip_Widgets */ 
/*@{*/
class TLP_QT_SIMPLE_SCOPE GWOverviewWidget : public GWOverviewWidgetData { 
  Q_OBJECT
  
public:
  GWOverviewWidget(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
  ~GWOverviewWidget();
  bool eventFilter(QObject *, QEvent *);

public slots:
  void syncFromView();
  void updateView();
  void setObservedView(GlGraphWidget *);
  void backColor(); //background color button
  void showParameters(bool);
  void showExtendedParameters(bool);
  
private slots:
  //Used to catch graphRedrawn signal from view of which
  //we are showing an overview
  void draw(GlGraphWidget *glWidget);
  //Used to catch the destroyed signal from view of which
  //we are showing an overview
  void observedViewDestroyed(QObject *glWidget);
  
private :
  GlGraphWidget *_observedView;
  GlGraphWidget *_view;
  bool _synchronizing;
  RectPosition *_glDraw;
  Camera _initialCamera;
  bool _extendedViewIsVisible;
};
/*@}*/
#endif // GWOVERVIEW_H
