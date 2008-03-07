// Warning !!! this file does not have to be used
// when building Tulip with Qt 3
#ifndef GLGRAPHWIDGETPLUGIN_H
#define GLGRAPHWIDGETPLUGIN_H

#include <QtDesigner/QDesignerCustomWidgetInterface>
    
namespace tlp {

class GlMainWidgetPlugin : public QObject, public QDesignerCustomWidgetInterface
{
  Q_OBJECT
  Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
  GlMainWidgetPlugin(QObject *parent = 0);
    
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
