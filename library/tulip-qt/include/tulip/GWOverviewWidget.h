//-*-c++-*-
#ifndef GWOVERVIEW_H
#define GWOVERVIEW_H

#include "GWOverviewWidgetData.h"
#include <tulip/ObservableGlGraph.h>
#include <tulip/GlAugmentedDisplay.h>
#include <qcolor.h>
#include <string>
#include <qstring.h>


namespace tlp {
class GlGraph;
}
class QEvent;

struct RectPosition : public tlp::GlAugmentedDisplay {
  void draw(tlp::GlGraph *);
  RectPosition(GlGraphWidget *, tlp::GlGraph *);
  tlp::GlGraph *observedView;
private :
  GlGraphWidget *view;
};

class GWOverviewWidget : public GWOverviewWidgetData, public tlp::GlGraphObserver { 
  Q_OBJECT
  
public:
  GWOverviewWidget(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
  ~GWOverviewWidget();
  void draw(tlp::GlGraph *);
  bool eventFilter(QObject *, QEvent *);
  void showParameters(bool);

public slots:
  void updateView();
  void setObservedView(tlp::GlGraph *);
  void syncFromView();
  void backColor();
  void showExtendedParameters(bool);
  
private:
  void destroy(tlp::GlGraph *);
  GlGraph *observedView;
  bool synchronizing;
  RectPosition *glDraw;
  Camera initialCamera;
  bool extendedViewIsVisible;
};

#endif // GWOVERVIEW_H
