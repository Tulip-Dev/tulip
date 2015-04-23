#ifndef %ProjectName:u%_H
#define %ProjectName:u%_H

#include <QIcon>

@if "%BaseInteractor%" == "GLInteractor"
#include <tulip/GLInteractor.h>
@endif
@if "%BaseInteractor%" == "Interactor"
#include <tulip/InteractorComposite.h>
@endif

class %ProjectName:c%Composite: public tlp::%BaseInteractor%Composite {
  Q_OBJECT
public:
  PLUGININFORMATION("%PluginName%", "%Author%", "%Date%", "%Informations%", "%Version%", "%Group%")

  %ProjectName:c%Composite(const tlp::PluginContext*);
  virtual ~%ProjectName:c%Composite();

  virtual void construct();

  virtual bool isCompatible(const std::string& viewName);
  virtual QWidget* configurationWidget() const;
  virtual unsigned int priority() const;
  virtual QCursor cursor() const;

public slots:
  virtual void undoIsDone();

signals:
};

class %ProjectName:c%Component: public tlp::%BaseInteractor%Component {
  Q_OBJECT
public:

  virtual void init();
  virtual void clear();
  virtual void viewChanged(tlp::View *);
  virtual bool eventFilter(QObject*, QEvent*);

@if "%BaseInteractor%" == "GLInteractor"
  virtual bool draw(tlp::GlMainWidget*);
  virtual bool compute(tlp::GlMainWidget*);
@endif

public slots:

signals:

};



#endif // %ProjectName:u%_H
