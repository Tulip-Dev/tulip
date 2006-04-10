#ifndef PROPERTYDIALOG_H
#define PROPERTYDIALOG_H
#include <string>

#include <tulip/SuperGraph.h>
#include <tulip/Observable.h>

#include "PropertyDialogData.h"

class QListViewItem;
class QStringList;

class PProxy;
class GlGraph;
class QScrollBar;

class PropertyDialog : public PropertyDialogData
{ 
  Q_OBJECT;

public:
  PropertyDialog(QWidget *parent = 0, const char *name = 0, bool modal = FALSE, WFlags fl = 0);
  ~PropertyDialog();
  void setGlGraphWidget(GlGraph *);

private:
  SuperGraph *supergraph;
  GlGraph *glWidget;
  PProxy *editedProperty;
  std::string editedPropertyName;
  bool _filterSelection;
  void updateNbElements();
  
public slots:  
  ///Manages observers change notification.
  void update();
  void selectNode(node);
  void selectEdge(edge);
  void setSuperGraph(SuperGraph *);
  void changePropertyName(QListViewItem *);
  void newProperty();
  void removeProperty();
  void cloneProperty();
  void toStringProperty();
  void filterSelection(bool);
  void setAllValue();
  
signals:
  void tulipAllNodePropertyChanged(SuperGraph *, const QString &property, const QString &value);
  void tulipAllEdgePropertyChanged(SuperGraph *, const QString &property, const QString &value);
};

#endif // PROPERTYDIALOG_H
