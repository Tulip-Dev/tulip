#ifndef FINDSELECTIONWIDGET_H
#define FINDSELECTIONWIDGET_H

#include <tulip/tulipconf.h>
#include <tulip/Node.h>
#include <tulip/Edge.h>

#include "tulip/FindSelectionData.h"

namespace tlp {

class Graph;
class PropertyInterface;
class BooleanProperty;

class TLP_QT_SCOPE FindSelectionWidget : public QDialog,
  public Ui::FindSelectionData {
  Q_OBJECT
public:
  FindSelectionWidget(Graph *graph, std::string &currentProperty,
		      QWidget *parent = 0);
  // returns -1 if cancelled, the number of found items if accepted
  int exec();
  std::string getCurrentProperty();

private:
  Graph *graph;
  int nbItemsFound;
  void insertProperties(std::string& currentProperty);
  int getOperation();
  int getSource();
  int getMode();
  void evalNodes(PropertyInterface *, int, std::string, int, BooleanProperty *);
  void evalEdges(PropertyInterface *, int, std::string, int, BooleanProperty *);

  PropertyInterface *getProperty();
  void find(BooleanProperty *);
  
protected slots:
  virtual void propertyChanged( int no );
  virtual void accept();
};

}
#endif //FINDSELECTIONWIDGET_H
