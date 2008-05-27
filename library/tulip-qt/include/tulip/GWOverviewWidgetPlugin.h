#ifndef GWOVERVIEWWIDGETPLUGIN_H
#define GWOVERVIEWWIDGETPLUGIN_H

#include <customwidget.h>

namespace tlp {

class GWOverviewWidgetPlugin : public QObject, public QDesignerCustomWidgetInterface
{
  Q_OBJECT
  Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
  GWOverviewWidgetPlugin(QObject *parent = 0);
  ~GWOverviewWidgetPlugin();
    
  bool isContainer() const;
  bool isInitialized() const;
  QIcon icon() const;
  QString codeTemplate() const;
  QString domXml() const;
  QString group() const;
  QString includeFile() const;
  QString name() const;
  QString toolTip() const;
  QString whatsThis() const;
  QWidget *createWidget(QWidget *parent);
  void initialize(QDesignerFormEditorInterface *core);

private:
  bool initialized;
};

}

#endif
