#ifndef PROPERTYDIALOG_H
#define PROPERTYDIALOG_H
#include <string>

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <tulip/Graph.h>
#include <tulip/Observable.h>

#include "PropertyDialogData.h"

class QListViewItem;
class QStringList;
class QScrollBar;

namespace tlp {
class PropertyInterface;
}

class GlMainWidget;

class PropertyDialog : public PropertyDialogData
{ 
  Q_OBJECT;

public:
  PropertyDialog(QWidget *parent = 0, const char *name = 0, bool modal = FALSE, WFlags fl = 0);
  ~PropertyDialog();
  void setGlMainWidget(GlMainWidget *);

private:
  tlp::Graph *graph;
  GlMainWidget *glWidget;
  tlp::PropertyInterface *editedProperty;
  std::string editedPropertyName;
  bool _filterSelection;
  void updateNbElements();
  
public slots:  
  ///Manages observers change notification.
  void update();
  void selectNode(tlp::node);
  void selectEdge(tlp::edge);
  void changePropertyName(Q3ListViewItem *);
  void newProperty();
  void removeProperty();
  void cloneProperty();
  void toStringProperty();
  void filterSelection(bool);
  void setAllValue();
  
signals:
  void tulipAllNodePropertyChanged(tlp::Graph *, const QString &property, const QString &value);
  void tulipAllEdgePropertyChanged(tlp::Graph *, const QString &property, const QString &value);
};

#endif // PROPERTYDIALOG_H
