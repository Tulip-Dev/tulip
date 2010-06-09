#include <QtGui/qlabel.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qheaderview.h>
#include <QtGui/qscrollbar.h>

#include <tulip/AbstractProperty.h>

#include "tulip/TulipTableWidget.h"
#include "tulip/ElementPropertiesWidget.h"

using namespace std;

namespace tlp {

  //==========================================
  ElementPropertiesWidget::ElementPropertiesWidget(Graph *graph,
						   const QStringList &nodeListedProperties,
						   const QStringList &edgeListedProperties,
						   QWidget *parent) :
    ElementPropertiesWidgetUI(parent),
    displayMode(NODE),
    nodeSet(false),
    edgeSet(false),
    graph(0)
  {
    setGraph(graph);
    setNodeListedProperties(nodeListedProperties);
    setEdgeListedProperties(edgeListedProperties);
    propertyTable->horizontalHeaderItem(0)->setText("Property");
    propertyTable->horizontalHeaderItem(1)->setText("Value");
    propertyTable->horizontalHeader()->setStretchLastSection(false);
    displayAllProperties=false;
    connect(propertyTable,SIGNAL(cellChanged(int,int)),
	    this, SLOT(propertyTableValueChanged(int,int)));
  }
  //==========================================
  ElementPropertiesWidget::ElementPropertiesWidget(QWidget *parent) :
    ElementPropertiesWidgetUI(parent),
    displayMode(NODE),
    nodeSet(false),
    edgeSet(false),
    graph(0)
  {
    setGraph(0);
    propertyTable->horizontalHeaderItem(0)->setText("Property");
    propertyTable->horizontalHeaderItem(1)->setText("Value");
    displayAllProperties=true;
    connect(propertyTable,SIGNAL(cellChanged(int,int)),
	    this,SLOT(propertyTableValueChanged(int,int)));
  }
  //==========================================
  ElementPropertiesWidget::~ElementPropertiesWidget() {
    if(graph)
      graph->removeGraphObserver(this);
  }
  //==========================================
  QStringList ElementPropertiesWidget::getCurrentListedProperties() const{
    switch(displayMode) {
    case NODE: return nodeListedProperties;
    case EDGE: return edgeListedProperties;
    default:
      std::string str(__PRETTY_FUNCTION__);
      str += ": current displayMode is invalid!";
      qWarning("%s",str.c_str());
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
    label->setText(QString("Node %1").arg(n.id));
    updateTable();
  }
  //==========================================
  void ElementPropertiesWidget::setCurrentEdge(Graph *sg,const edge &e) {
    if (graph!=sg) setGraph(sg);
    displayMode = EDGE;
    edgeSet = true;
    currentEdge = e;
    label->setText(QString("Edge %1").arg(e.id));
    updateTable();
  }
  //==========================================
  void ElementPropertiesWidget::setGraph(Graph *s, bool destroy) {
    if (graph!=0 && !destroy)
      graph->removeGraphObserver(this);
    graph = s;
    nodeSet = false;
    edgeSet = false;
    label->setText("No element selected");
    /*for (int i=0;i<propertyTable->rowCount();++i) {
      propertyTable->clearCell(i,0);
      propertyTable->clearCell(i,1);
      }*/
    propertyTable->setRowCount(0);
    if (graph!=0)
      graph->addGraphObserver(this);
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
    propertyTable->setRowCount(nodeListedProperties.size());
    updateTable();
  }
  //==========================================
  void ElementPropertiesWidget::setEdgeListedProperties(const QStringList &l) {
    edgeListedProperties = l;
    propertyTable->setRowCount(edgeListedProperties.size());
    updateTable();
  }
  //==========================================
  void ElementPropertiesWidget::updateTable() {
    if (graph == NULL) return;
    int i=0;
    disconnect(propertyTable, SIGNAL(cellChanged(int,int)),
	       this, SLOT(propertyTableValueChanged(int,int)));
    if (displayAllProperties) {
      Iterator<std::string> *it=graph->getLocalProperties();
      propertyTable->setRowCount(0);
    // check if currentNode or currentEdge is valid
    switch(displayMode) {
    case NODE:
      if (!nodeSet)
	return;
      break;
    case EDGE:
      if (!edgeSet)
	return;
    }
      // first count number properties
      // to avoid too much calls to setRowCount
      while(it->hasNext()) {
	it->next();
	++i;
      }
      delete it;
      it=graph->getInheritedProperties();
      while(it->hasNext()) {
	it->next();
	++i;
      }
      delete it;
      propertyTable->setRowCount(i);
      i = 0;
      it=graph->getLocalProperties();
      while(it->hasNext()) {
	std::string pname=it->next();
	QTableWidgetItem* nameItem = new QTableWidgetItem(pname.c_str());
	nameItem->setFlags(Qt::ItemIsEnabled);
	propertyTable->setItem(i, 0, nameItem);
	PropertyInterface *editedProperty = graph->getProperty(pname);
	switch(displayMode) {
	case NODE:
	    propertyTable->setTulipNodeItem(editedProperty, pname, currentNode, i, 1);
	  break;
	case EDGE:
	    propertyTable->setTulipEdgeItem(editedProperty, pname, currentEdge, i, 1);
	break;
      }
      ++i;
    } delete it;
    it=graph->getInheritedProperties();
    while(it->hasNext()) {
      std::string pname = it->next();
      QTableWidgetItem* nameItem = new QTableWidgetItem(pname.c_str());
      nameItem->setFlags(Qt::ItemIsEnabled);
      propertyTable->setItem(i, 0, nameItem);
      PropertyInterface *editedProperty = graph->getProperty(pname);
      switch(displayMode) {
      case NODE:
	    propertyTable->setTulipNodeItem(editedProperty, pname, currentNode, i, 1);
	  break;
	case EDGE:
	    propertyTable->setTulipEdgeItem(editedProperty, pname, currentEdge, i, 1);
	  break;
	}
	++i;
      }delete it;
    }
    else {
    // check if currentNode or currentEdge is valid
    switch(displayMode) {
    case NODE:
      if (!nodeSet)
	return;
      break;
    case EDGE:
      if (!edgeSet)
	return;
    }
      QStringList *listedProperties = NULL;
      switch(displayMode) {
      case NODE: listedProperties = &nodeListedProperties; break;
      case EDGE: listedProperties = &edgeListedProperties; break;
      }
      for (QStringList::const_iterator it=listedProperties->begin();
	   it != listedProperties->end();
	   ++it, ++i) {
	QTableWidgetItem* nameItem = new QTableWidgetItem(*it);
	nameItem->setFlags(Qt::ItemIsEnabled);
	propertyTable->setItem(i, 0, nameItem);
      string pname = (*it).toUtf8().data();
	if (graph->existProperty(pname)) {
	PropertyInterface *editedProperty = graph->getProperty(pname);
	  switch(displayMode) {
	  case NODE:
	  propertyTable->setTulipNodeItem(editedProperty, pname, currentNode, i, 1);
	    break;
	  case EDGE:
	  propertyTable->setTulipEdgeItem(editedProperty, pname, currentEdge, i, 1);
	  }
	}
      }
    }
    connect(propertyTable,SIGNAL(cellChanged(int,int)),
	    this, SLOT(propertyTableValueChanged(int,int)));
    // Ugly hack to avoid resizing
    if (propertyTable->isVisible()) {
      propertyTable->setColumnWidth(1, propertyTable->horizontalHeader()->length()
                    - propertyTable->columnWidth(0) - 5);
    }
  }
  //==========================================
  void ElementPropertiesWidget::propertyTableValueChanged(int row, int col) {
    if (col == 0)
      return;
    //  cerr << __PRETTY_FUNCTION__ << endl;
    bool *elementSet;
    QStringList *listedProperties;
    switch(displayMode) {
    case NODE: elementSet = &nodeSet; listedProperties = &nodeListedProperties; break;
    case EDGE: elementSet = &edgeSet; listedProperties = &edgeListedProperties; break;
    }

    QString property = propertyTable->item(row, 0)->text();
    QString value = ((TulipTableWidgetItem *)propertyTable->item(row, col))->textForTulip();
    PropertyInterface *editedProperty = graph->getProperty(property.toUtf8().data());
    if (editedProperty==0) return;
    //cerr << "Value :" << value.toUtf8().data() << endl;
    bool result=true;
    switch(displayMode) {
    case NODE:
    // allow to undo
    graph->push();
      result=editedProperty->setNodeStringValue(currentNode, value.toUtf8().data());
      break;
    case EDGE:
    // allow to undo
    graph->push();
      result=editedProperty->setEdgeStringValue(currentEdge, value.toUtf8().data());
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
  void ElementPropertiesWidget::nodePropertyChanged(Graph *s, node const &n, QString const &, QString const &) {
    if ((displayMode == NODE) && (graph == s) && (currentNode == n))
      updateTable();
  }
  //==========================================
  void ElementPropertiesWidget::edgePropertyChanged(Graph *s, edge const &e, QString const &, QString const &) {
    if ((displayMode == EDGE) && (graph == s) && (currentEdge == e))
      updateTable();
  }
  //==========================================
  void ElementPropertiesWidget::delNode(Graph *sg, node n) {
    if (sg!=graph && graph!=0 )
      graph->removeGraphObserver(this);
    if (displayMode==NODE && currentNode==n)
      setGraph(sg);
  }
  void ElementPropertiesWidget::delEdge(Graph *sg, edge e) {
    if (sg!=graph && graph!=0)
      graph->removeGraphObserver(this);
    if (displayMode==EDGE && currentEdge==e)
      setGraph(sg);
  }
  void ElementPropertiesWidget::destroy(Graph *) {
    setGraph(0, true);
  }

}
