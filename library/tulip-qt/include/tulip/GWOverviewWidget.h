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
class TLP_QT_SIMPLE_SCOPE GWOverviewWidget : public GWOverviewWidgetData, public tlp::GlGraphObserver { 
  Q_OBJECT
  
public:
  GWOverviewWidget(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
  ~GWOverviewWidget();
  void draw(tlp::GlGraph *);
  bool eventFilter(QObject *, QEvent *);
  void showParameters(bool);
  GlGraphWidget* view;

public slots:
  void updateView();
  void setObservedView(tlp::GlGraph *);
  void syncFromView();
  void backColor();
  void showExtendedParameters(bool);
  
private:
  void destroy(tlp::GlGraph *);
  tlp::GlGraph *observedView;
  bool synchronizing;
  RectPosition *glDraw;
  Camera initialCamera;
  bool extendedViewIsVisible;
};
/*@}*/
#endif // GWOVERVIEW_H
