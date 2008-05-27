//-*-c++-*-
#ifndef GWOVERVIEW_H
#define GWOVERVIEW_H

#include "tulip/GWOverviewWidgetData.h"
#include "tulip/RenderingParametersDialogData.h"
#include <tulip/Camera.h>
#include <QtGui/qcolor.h>
#include <string>
#include <QtCore/qstring.h>
#include <QtGui/qlayout.h>

namespace tlp {
  class GlGraph;
}

class GlMainWidget;
class QEvent;
class RectPosition;
class GWOverviewWidget;

class RenderingParametersDialog : public QDialog,
				  public Ui::RenderingParametersDialogData {
  Q_OBJECT

  GWOverviewWidget *overview;
public:
  RenderingParametersDialog(GWOverviewWidget* parent);
  void windowActivationChange(bool oldActive);
public slots:
  void updateView();
  void backColor();
};

/** \addtogroup Tulip_Widgets */ 
/*@{*/
class TLP_QT_SIMPLE_SCOPE GWOverviewWidget : public QWidget,
					     public Ui::GWOverviewWidgetData { 
  Q_OBJECT
  
public:
  GWOverviewWidget(QWidget* parent = 0);
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
  RenderingParametersDialog* paramDialog;
  void setBackgroundColor(QColor);
};
/*@}*/
#endif // GWOVERVIEW_H
