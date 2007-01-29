#if (QT_REL == 3)
#include <qmessagebox.h>
#include <qlabel.h>
#else
#include <QtGui/qlabel.h>
#include <QtGui/qmessagebox.h>
#include "tulip/Qt3ForTulip.h"
#endif

#include <tulip/AbstractProperty.h>

#include "tulip/TulipTableWidget.h"
#include "tulip/ElementPropertiesWidget.h"

using namespace tlp;
//==========================================
ElementPropertiesWidget::ElementPropertiesWidget(Graph *graph,
                                               const QStringList &nodeListedProperties,
                                               const QStringList &edgeListedProperties,
                                               QWidget *parent, const char *name, Qt::WFlags fl) :
  ElementPropertiesWidgetData(parent, name, fl), 
  displayMode(NODE), 
  nodeSet(false), 
  edgeSet(false),
  graph(0)
{
  setGraph(graph);
  setNodeListedProperties(nodeListedProperties);
  setEdgeListedProperties(edgeListedProperties);
  propertyTable->horizontalHeader()->setLabel(0, tr("Property"));
  propertyTable->horizontalHeader()->setLabel(1, tr("Value"));
  propertyTable->setColumnStretchable(1, false);
  displayAllProperties=false;
  connect(propertyTable,SIGNAL(valueChanged(int,int)),this,SLOT(propertyTableValueChanged(int,int)));
}
//==========================================
ElementPropertiesWidget::ElementPropertiesWidget(QWidget *parent, const char *name, Qt::WFlags fl) :
  ElementPropertiesWidgetData(parent, name, fl), 
  displayMode(NODE), 
  nodeSet(false), 
  edgeSet(false),
  graph(0)
{
  setGraph(0);
  propertyTable->horizontalHeader()->setLabel(0, tr("Property"));
  propertyTable->horizontalHeader()->setLabel(1, tr("Value"));
  displayAllProperties=true;
  connect(propertyTable,SIGNAL(valueChanged(int,int)),this,SLOT(propertyTableValueChanged(int,int)));
}
//==========================================
ElementPropertiesWidget::~ElementPropertiesWidget() {
}
//==========================================
QStringList ElementPropertiesWidget::getCurrentListedProperties() const{
  switch(displayMode) {
  case NODE: return nodeListedProperties;
  case EDGE: return edgeListedProperties;
  default:
    qWarning(QString(__PRETTY_FUNCTION__) + ": current displayMode is invalid!");
    return QStringList();
  }
}
//==========================================
QStringList ElementPropertiesWidget::getNodeListedProperties() const {
  return nodeListedProperties;
}
//==========================================
QStringList ElementPropertiesWidget::getEdgeListedProperties() const {
  return edgeListedProperties;
}
//==========================================
Graph *ElementPropertiesWidget::getGraph() const {
  return graph;
}
//==========================================
node ElementPropertiesWidget::getCurrentNode() const {
  return currentNode;
}
//==========================================
edge ElementPropertiesWidget::getCurrentEdge() const {
  return currentEdge;
}
//==========================================
tlp::ElementType ElementPropertiesWidget::getDisplayMode() const {
  return displayMode;
}
//==========================================
bool ElementPropertiesWidget::isElementDisplayed() const {
  switch(displayMode) {
  case NODE: return nodeSet; break;
  case EDGE: return edgeSet; break;
  }
  return true; //never reached, but suppresses a compiler warning
}
//==========================================
void ElementPropertiesWidget::setDisplayMode(const tlp::ElementType mode) {
  switch(mode) {
  case NODE:
  case EDGE:
    nodeSet = false;
    edgeSet = false;
    displayMode = mode;
    updateTable();
    break;
  }
}
//==========================================
void ElementPropertiesWidget::setCurrentNode(Graph *sg, const node &n) {
  if (graph!=sg) setGraph(sg);
  displayMode = NODE;
  nodeSet = true;
  currentNode = n;
  this->setCaption(QString("Node %1").arg(n.id));
  label->setText(QString("Node %1").arg(n.id));
  updateTable();
}
//==========================================
void ElementPropertiesWidget::setCurrentEdge(Graph *sg,const edge &e) {
  if (graph!=sg) setGraph(sg);
  displayMode = EDGE;
  edgeSet = true;
  currentEdge = e;
  this->setCaption(QString("Edge %1").arg(e.id));
  label->setText(QString("Edge %1").arg(e.id));
  updateTable();
}
//==========================================
void ElementPropertiesWidget::setGraph(Graph *s, bool destroy) {
  if (graph!=0 && !destroy)
    graph->removeObserver(this);
  graph = s;
  nodeSet = false;
  edgeSet = false;
  this->setCaption("");
  label->setText("No element selected");
  for (int i=0;i<propertyTable->numRows();++i) {
    propertyTable->clearCell(i,0);
    propertyTable->clearCell(i,1);
  }
  propertyTable->setNumRows(0);
  if (graph!=0)
    graph->addObserver(this);
}
//==========================================
void ElementPropertiesWidget::setCurrentListedProperties(const QStringList &l) {
  switch(displayMode) {
  case NODE: return setNodeListedProperties(l);
  case EDGE: return setEdgeListedProperties(l);
  }
}
//==========================================
void ElementPropertiesWidget::setNodeListedProperties(const QStringList &l) {
  nodeListedProperties = l;
  propertyTable->setNumRows(nodeListedProperties.size());
  updateTable();
}
//==========================================
void ElementPropertiesWidget::setEdgeListedProperties(const QStringList &l) {
  edgeListedProperties = l;
  propertyTable->setNumRows(edgeListedProperties.size());
  updateTable();
}
//==========================================
void ElementPropertiesWidget::updateTable() {
  if (graph == NULL) return;
  int i=0;
  if (displayAllProperties) {
    propertyTable->setNumRows(0);    
    Iterator<std::string> *it=graph->getLocalProperties();
    while(it->hasNext()) {
      propertyTable->setNumRows(i + 1);
      std::string pname=it->next();
      propertyTable->setText(i, 0, QString(pname.c_str()));
      PropertyInterface *editedProperty = graph->getProperty(pname);
      switch(displayMode) {
      case NODE:
	if (nodeSet) {
	  propertyTable->setTulipNodeItem(editedProperty, pname, currentNode, i, 1);
	}
	break;
      case EDGE:
	if (edgeSet) {
	  propertyTable->setTulipEdgeItem(editedProperty, pname, currentEdge, i, 1);
	}
	break;
      }
      ++i;
    } delete it;
    it=graph->getInheritedProperties();
    while(it->hasNext()) {
      propertyTable->setNumRows(i + 1);
      std::string pname=it->next();
      propertyTable->setText(i, 0, QString(pname.c_str()));
      PropertyInterface *editedProperty = graph->getProperty(pname);
      switch(displayMode) {
      case NODE:
	if (nodeSet) {
	  propertyTable->setTulipNodeItem(editedProperty, pname, currentNode, i, 1);
	}
	break;
      case EDGE:
	if (edgeSet) {
	  PropertyInterface *editedProperty = graph->getProperty(pname);
	  propertyTable->setTulipEdgeItem(editedProperty, pname, currentEdge, i, 1);
	}
	break;
      }
      ++i;
    }delete it;
  }
  else {
    QStringList *listedProperties = NULL;
    switch(displayMode) {
    case NODE: listedProperties = &nodeListedProperties; break;
    case EDGE: listedProperties = &edgeListedProperties; break;
    }
    propertyTable->setNumRows(listedProperties->count());
    for (QStringList::const_iterator it=listedProperties->begin();
	 it != listedProperties->end();
	 ++it, ++i) {
      propertyTable->setText(i, 0, *it);
      if (graph->existProperty((*it).latin1())) {
        switch(displayMode) {
        case NODE:
          if (nodeSet) {
            PropertyInterface *editedProperty = graph->getProperty((*it).latin1());
            propertyTable->setTulipNodeItem(editedProperty, (*it).latin1(), currentNode, i, 1);
          }
          break;
        case EDGE:
          if (edgeSet) {
            PropertyInterface *editedProperty = graph->getProperty((*it).latin1());
            propertyTable->setTulipEdgeItem(editedProperty, (*it).latin1(), currentEdge, i, 1);
          }
        }
      }
    }
  }
  propertyTable->adjustColumn(0);
  propertyTable->adjustColumn(1);
}
using namespace std;
//==========================================
void ElementPropertiesWidget::propertyTableValueChanged(int row, int col) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  bool *elementSet;
  QStringList *listedProperties;
  switch(displayMode) {
  case NODE: elementSet = &nodeSet; listedProperties = &nodeListedProperties; break;
  case EDGE: elementSet = &edgeSet; listedProperties = &edgeListedProperties; break;
  }

  QString property = propertyTable->text(row, 0);
  QString value = propertyTable->text(row, col);
  PropertyInterface *editedProperty = graph->getProperty(property.latin1());
  if (editedProperty==0) return;
  std::string tmp = value.latin1();
  //  cerr << "Value :" << value << endl;
  bool result=true;
  switch(displayMode) {
  case NODE:
    result=editedProperty->setNodeStringValue( currentNode, tmp);
    break;
  case EDGE:
    result=editedProperty->setEdgeStringValue( currentEdge, tmp);
    break;
  }
  
  if (!result) {
    QMessageBox::critical( 0, "Tulip Property Editor Change Failed",
			   QString("The value entered for this property is not correct,\n"
				   "The change won't be applied\n"
				   "Modify the entered value to apply the changes.")
			   );
  }
  else {
    switch(displayMode) {
    case NODE:
      emit tulipNodePropertyChanged(graph, currentNode, property, value);
      break;
    case EDGE:
      emit tulipEdgePropertyChanged(graph, currentEdge, property, value);
      break;
    }
  }
  //}
}
//==========================================
void ElementPropertiesWidget::nodePropertyChanged(Graph *s, node const &n, QString const &property, QString const &value) {
  if ((displayMode == NODE) && (graph == s) && (currentNode == n))
    updateTable();
}
//==========================================
void ElementPropertiesWidget::edgePropertyChanged(Graph *s, edge const &e, QString const &property, QString const &value) {
  if ((displayMode == EDGE) && (graph == s) && (currentEdge == e))
    updateTable();
}
//==========================================
void ElementPropertiesWidget::delNode(Graph *sg, node n) {
  if (sg!=graph && graph!=0 )
    graph->removeObserver(this);
  if (displayMode==NODE && currentNode==n)
    setGraph(sg);
}
void ElementPropertiesWidget::delEdge(Graph *sg, edge e) {
  if (sg!=graph && graph!=0)
    graph->removeObserver(this);
  if (displayMode==EDGE && currentEdge==e)
    setGraph(sg);
}
void ElementPropertiesWidget::destroy(Graph *) {
  setGraph(0, true);
}
