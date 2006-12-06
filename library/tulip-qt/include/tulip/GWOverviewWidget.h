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
  bool eventFilter(QObject *, QEvent *);

public slots:
  void syncFromView();
  void updateView();
  void setObservedView(tlp::GlGraph *);
  void backColor(); //background color button
  void showParameters(bool);
  void showExtendedParameters(bool);
  
private:

  //implements GlGraphObserver
  void draw(tlp::GlGraph *);
  void destroy(tlp::GlGraph *);

private :
  tlp::GlGraph  *_observedView;
  GlGraphWidget *_view;
  bool _synchronizing;
  RectPosition *_glDraw;
  Camera _initialCamera;
  bool _extendedViewIsVisible;
};
/*@}*/
#endif // GWOVERVIEW_H
