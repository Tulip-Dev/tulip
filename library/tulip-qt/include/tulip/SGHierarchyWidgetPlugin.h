// Warning !!! this file does not have to be used
// when building Tulip with Qt 3
#ifndef CLUSTERTREEPLUGIN_H
#define CLUSTERTREEPLUGIN_H

#include <QtDesigner/QDesignerCustomWidgetInterface>
#include "tulip/Graph.h"

namespace tlp {

class SGHierarchyWidgetPlugin : public QObject, public QDesignerCustomWidgetInterface
{
  Q_OBJECT
  Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
  SGHierarchyWidgetPlugin(QObject *parent = 0);
  ~SGHierarchyWidgetPlugin();
    
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
  Graph *graph;
};

}
#endif
