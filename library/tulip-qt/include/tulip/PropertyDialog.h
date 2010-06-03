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

class QStringList;
class QScrollBar;

namespace tlp {
class GlMainWidget;
class PropertyInterface;

class TLP_QT_SCOPE PropertyDialog: public QWidget, public Ui::PropertyDialogData {
Q_OBJECT
  ;

public:
  PropertyDialog(QWidget *parent = 0);
  ~PropertyDialog();
  void setGraph(Graph *);

private:
  Graph *graph;
  //GlMainWidget *glWidget;
  PropertyInterface *editedProperty;
  std::string editedPropertyName;
  bool _filterSelection;
  void updateNbElements();

public slots:
  ///Manages observers change notification.
  void update();
  void selectNode(node);
  void selectEdge(edge);
  void newProperty();
  void removeProperty();
  void cloneProperty();
  void toStringProperty();
  void filterSelection(bool);
  void setAllValue();
  void importCSVData();
private slots:
  void propertySelectionChanged();
  void propertyTypeFilterChanged();
  void nameFilterChanged(const QString& text);
signals:
  void tulipAllNodePropertyChanged(Graph *, const QString &property, const QString &value);
  void tulipAllEdgePropertyChanged(Graph *, const QString &property, const QString &value);
  void newPropertySignal(Graph *, const std::string &);
  void removePropertySignal(Graph *, const std::string &);
};

}
#endif // PROPERTYDIALOG_H
