#ifndef %ProjectName:u%_H
#define %ProjectName:u%_H

#include <tulip/%BaseView%.h>

@if "%BaseView%" == "View"
#include <QGraphicsView>
@endif

class %ProjectName:c%: public tlp::%BaseView% {
Q_OBJECT

@if "%BaseView%" == "View"
  QGraphicsView* _graphicsView;

@endif

public:
PLUGININFORMATION("%PluginName%", "%Author%", "%Date%", "%Informations%", "%Version%", "%Group%")

public:
  %ProjectName:c%(const tlp::PluginContext* context = NULL);
  virtual ~%ProjectName:c%();

  virtual QList<QWidget*> configurationWidgets() const;
  virtual void applySettings();

  virtual tlp::DataSet state() const;
  virtual void setState(const tlp::DataSet&);

  @if "%BaseView%" == "View"
  virtual void setupUi()=0;

  virtual QGraphicsView* graphicsView() const=0;
  virtual QGraphicsItem* centralItem() const;
  @endif

  @if "%BaseView%" == "ViewWidget"
  virtual void setupWidget();

  @endif

  @if "%BaseView%" != "GlMainView"
  virtual void centerView();

  virtual void draw(tlp::PluginProgress* pluginProgress);
  virtual void refresh(tlp::PluginProgress* pluginProgress);
  virtual QPixmap snapshot(const QSize& outputSize=QSize()) const;
  @endif

protected:
  virtual void interactorsInstalled(const QList<tlp::Interactor*>& interactors);
  virtual void currentInteractorChanged(tlp::Interactor*);

  virtual void graphChanged(tlp::Graph*)=0;

  @if "%BaseView%" == "View"
  virtual void graphDeleted(tlp::Graph* parentGraph);

  @endif

  virtual void fillContextMenu(QMenu*,const QPointF &);
};

#endif // %ProjectName:u%_H
