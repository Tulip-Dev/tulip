#ifndef FINDSELECTIONWIDGET_H
#define FINDSELECTIONWIDGET_H

#include <tulip/tulipconf.h>
#include <tulip/Node.h>
#include <tulip/Edge.h>

#if (QT_REL == 4)
#include "tulip/Qt3ForTulip.h"
#endif

#include "tulip/FindSelectionData.h"

namespace tlp {

class Graph;
class PropertyInterface;
class BooleanProperty;

class TLP_QT_SCOPE FindSelectionWidget : FindSelectionData {
  Q_OBJECT
    public:
  FindSelectionWidget(Graph *graph, std::string &currentProperty,
		      QWidget *parent = 0, const char *name = 0,
		      Qt::WFlags fl = 0);
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

  virtual void propertyChanged( int no );
  PropertyInterface *getProperty();
  void find(BooleanProperty *);
  
  protected slots:
    virtual void accept();
};

}
#endif //FINDSELECTIONWIDGET_H
