#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <vector>

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qlistview.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qinputdialog.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtGui/qlabel.h>
#include <QtGui/qcolordialog.h>
#include <QtGui/qtabwidget.h>

#include <tulip/Graph.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/PropertyWidget.h>
#include <tulip/SGHierarchyWidget.h>
#include <tulip/GlMainWidget.h>
#include <tulip/ForEach.h>
#include <tulip/hash_string.h>

#include "tulip/PropertyDialog.h"
#include "tulip/CopyPropertyDialog.h"

using namespace std;

namespace tlp {

  //==================================================================================================
  PropertyDialog::PropertyDialog(QWidget* parent)
    : QWidget(parent) {
    setupUi(this);
    _filterSelection=false;
    graph=0;
    connect(localProperties, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), SLOT(changePropertyName(QListWidgetItem *, QListWidgetItem *)));
    connect(inheritedProperties, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), SLOT(changePropertyName(QListWidgetItem *, QListWidgetItem *)));
    connect(removeButton , SIGNAL(clicked()) , SLOT(removeProperty()) );
    connect(newButton,SIGNAL(clicked()),SLOT(newProperty()));
    connect(cloneButton,SIGNAL(clicked()),SLOT(cloneProperty()));

  }
  //=================================================
  PropertyDialog::~PropertyDialog() {
  }
  //=================================================
  void PropertyDialog::changePropertyName(QListWidgetItem *item, QListWidgetItem *) {
    if (item==0) {
      editedProperty=0;
      return;
    }
    string erreurMsg;

    tableNodes->selectNodeOrEdge(true);
    tableEdges->selectNodeOrEdge(false);
    tableNodes->changeProperty(graph,item->text().toAscii().data());
    tableEdges->changeProperty(graph,item->text().toAscii().data());

    PropertyInterface *tmpProxy=graph->getProperty(item->text().toAscii().data());
    editedProperty=tmpProxy;
    editedPropertyName=item->text().toAscii().data();
    //propertyName->setText(item->text());
  
    if (graph->existLocalProperty(item->text().toAscii().data()))
      inheritedProperties->clearSelection();
    else
      localProperties->clearSelection();

    tableEdges->update();
    tableNodes->update();
  }
  //=================================================
  void PropertyDialog::filterSelection(bool b) {
    _filterSelection= b;
    tableEdges->filterSelection(b);
    tableNodes->filterSelection(b);
    update();
  }
  //=================================================
  void PropertyDialog::update() {
    tableEdges->update();
    tableNodes->update();
  }
  //=================================================
  void PropertyDialog::selectNode(node n){}
  //=================================================
  void PropertyDialog::selectEdge(edge e){}
  //=================================================
  void PropertyDialog::setAllValue() {
    if (tabWidget->currentIndex()==0)
      tableNodes->setAll();
    else
      tableEdges->setAll();
    setAllButton->setDown(false);
  }
  //=================================================
  void PropertyDialog::setGraph(Graph *graph) {
    this->graph=graph;
    editedProperty=0;

    localProperties->clear();
    inheritedProperties->clear();
    //Build the property list
    tableNodes->selectNodeOrEdge(true);
    tableEdges->selectNodeOrEdge(false);
    tableNodes->setGraph(graph);
    tableEdges->setGraph(graph);
    tableEdges->filterSelection(_filterSelection);
    tableNodes->filterSelection(_filterSelection);
    if (graph==0) return;

    Iterator<string> *it=graph->getLocalProperties();
    while (it->hasNext()) {
      string tmp=it->next();
      QListWidgetItem* tmpItem = new QListWidgetItem(localProperties);
      tmpItem->setText(QString(tmp.c_str()));
    } delete it;
    it=graph->getInheritedProperties();
    while (it->hasNext()) {
      string tmp=it->next();
      QListWidgetItem *tmpItem = new QListWidgetItem(inheritedProperties);
      tmpItem->setText(QString(tmp.c_str()));
    } delete it;
  }
  //=================================================
  void PropertyDialog::newProperty() {
    if (!graph) return;
    QStringList lst;
    lst << "Color" << "Integer" << "Layout" << "Metric" << "Selection" << "Size" << "String";
    bool ok = false;
    QString res = QInputDialog::getItem(this, "Property type",
					"Please select the property type",
					lst, 3, false, &ok);
    if ( ok ) {
      QString text = QInputDialog::getText(this, "Property name",
					   "Please enter the property name",
					   QLineEdit::Normal, QString::null,
					   &ok);
      if (ok) {
	string erreurMsg;
	if (strcmp(res.toAscii().data(),"Selection")==0) graph->getLocalProperty<BooleanProperty>(text.toAscii().data());
	if (strcmp(res.toAscii().data(),"Metric")==0) graph->getLocalProperty<DoubleProperty>(text.toAscii().data());
	if (strcmp(res.toAscii().data(),"Layout")==0) graph->getLocalProperty<LayoutProperty>(text.toAscii().data());
	if (strcmp(res.toAscii().data(),"String")==0) graph->getLocalProperty<StringProperty>(text.toAscii().data());
	if (strcmp(res.toAscii().data(),"Integer")==0) graph->getLocalProperty<IntegerProperty>(text.toAscii().data());
	if (strcmp(res.toAscii().data(),"Size")==0) graph->getLocalProperty<SizeProperty>(text.toAscii().data());
	if (strcmp(res.toAscii().data(),"Color")==0) graph->getLocalProperty<ColorProperty>(text.toAscii().data());
	setGraph(graph);
      }
    }
  }
  //=================================================
  void PropertyDialog::toStringProperty() {
    if (editedProperty==0) return;
    string name=editedPropertyName;
    if (name == "viewLabel") return;
    Observable::holdObservers();
    PropertyInterface *newLabels=graph->getProperty(name);
    StringProperty *labels=graph->getLocalProperty<StringProperty>("viewLabel");
    if (tabWidget->currentIndex()==0) {
      labels->setAllNodeValue( newLabels->getNodeDefaultStringValue() );
      Iterator<node> *itN=graph->getNodes();
      while(itN->hasNext()) {
	node itn=itN->next();
	labels->setNodeValue(itn,  newLabels->getNodeStringValue(itn) );
      } delete itN;
    }
    else {
      labels->setAllEdgeValue( newLabels->getEdgeDefaultStringValue() );
      Iterator<edge> *itE=graph->getEdges();
      while(itE->hasNext()) {
	edge ite=itE->next();
	labels->setEdgeValue(ite,newLabels->getEdgeStringValue(ite));
      } delete itE;
    }
    /*glWidget->getScene()->getGlGraphComposite()->getInputData()->reloadLabelProperty();
      glWidget->draw();*/
    Observable::unholdObservers();
  }
  //=================================================
  void PropertyDialog::removeProperty() {
    if (editedProperty==0) return;
    if(graph->existLocalProperty(editedPropertyName)) {
      graph->delLocalProperty(editedPropertyName);
      //setGlMainWidget(glWidget);
      editedProperty=0;
    }
    else
      QMessageBox::critical( 0, "Tulip Property Editor Remove Failed",
			     QString("You cannot remove an inherited property,\n")
			     );
  }
  //=================================================
  void PropertyDialog::cloneProperty() {
    if (!graph || !editedProperty) return;
    CopyPropertyDialog dialog(parentWidget());
    vector<string> localProps;
    vector<string> inheritedProps;
    string prop;
    Graph *parent = graph->getSuperGraph();
    if (parent == graph)
      parent = 0;
    forEach(prop, graph->getLocalProperties()) {
      if (typeid(*graph->getProperty(prop)) == typeid(*editedProperty)) {
	if (prop != editedPropertyName)
	  localProps.push_back(prop);
	if (parent && parent->existProperty(prop))
	  inheritedProps.push_back(prop);
      }
    }
    forEach(prop, graph->getInheritedProperties()) {
      if ((prop != editedPropertyName) &&
	  (typeid(*graph->getProperty(prop)) == typeid(*editedProperty)))
	inheritedProps.push_back(prop);
    }
    dialog.setProperties(editedPropertyName, localProps, inheritedProps);
    CopyPropertyDialog::destType type;
    std::string text = dialog.getDestinationProperty(type);
    if (text.size() > 0) {
      if (type != CopyPropertyDialog::INHERITED) {
	if (graph->existProperty(text)) {
	  if (typeid(*graph->getProperty(text))!=typeid(*editedProperty)) {
	    QMessageBox::critical(parentWidget(), "Tulip Warning" ,"Properties are not of the same type.");
	    return;
	  }
	  if (type == CopyPropertyDialog::NEW &&
	      graph->existLocalProperty(text)) {
	    if (text == editedPropertyName) {
	      QMessageBox::critical(parentWidget(), "Tulip Warning",
				    "Properties are the same.");
	      return;
	    } else if (QMessageBox::question(parentWidget(),
					     "Copy confirmation",
					     (std::string("Property ") + text + " already exists,\ndo you really want to overwrite it ?").c_str(),
					     QMessageBox::Ok,
					     QMessageBox::Cancel)
		       == QDialog::Rejected)
	      return;
	  }
	}
	Observable::holdObservers();
	if (typeid((*editedProperty)) == typeid(DoubleProperty))
	  {*graph->getLocalProperty<DoubleProperty>(text)=*((DoubleProperty*)editedProperty);}
	if (typeid((*editedProperty)) == typeid(LayoutProperty))
	  {*graph->getLocalProperty<LayoutProperty>(text)=*((LayoutProperty*)editedProperty);}
	if (typeid((*editedProperty)) == typeid(StringProperty))
	  {*graph->getLocalProperty<StringProperty>(text)=*((StringProperty*)editedProperty);}
	if (typeid((*editedProperty)) == typeid(BooleanProperty))
	  {*graph->getLocalProperty<BooleanProperty>(text)=*((BooleanProperty*)editedProperty);}
	if (typeid((*editedProperty)) == typeid(IntegerProperty))
	  {*graph->getLocalProperty<IntegerProperty>(text)=*((IntegerProperty*)editedProperty);}
	if (typeid((*editedProperty)) == typeid(ColorProperty))
	  {*graph->getLocalProperty<ColorProperty>(text)=*((ColorProperty*)editedProperty);}
	if (typeid((*editedProperty)) == typeid(SizeProperty))
	  {*graph->getLocalProperty<SizeProperty>(text)=*((SizeProperty*)editedProperty);}
	//}
      } else {
	Graph *parent = graph->getSuperGraph();
	Observable::holdObservers();
	if (typeid((*editedProperty)) == typeid(DoubleProperty))
	  {*parent->getProperty<DoubleProperty>(text)=*((DoubleProperty*)editedProperty);}
	if (typeid((*editedProperty)) == typeid(LayoutProperty))
	  {*parent->getProperty<LayoutProperty>(text)=*((LayoutProperty*)editedProperty);}
	if (typeid((*editedProperty)) == typeid(StringProperty))
	  {*parent->getProperty<StringProperty>(text)=*((StringProperty*)editedProperty);}
	if (typeid((*editedProperty)) == typeid(BooleanProperty))
	  {*parent->getProperty<BooleanProperty>(text)=*((BooleanProperty*)editedProperty);}
	if (typeid((*editedProperty)) == typeid(IntegerProperty))
	  {*parent->getProperty<IntegerProperty>(text)=*((IntegerProperty*)editedProperty);}
	if (typeid((*editedProperty)) == typeid(ColorProperty))
	  {*parent->getProperty<ColorProperty>(text)=*((ColorProperty*)editedProperty);}
	if (typeid((*editedProperty)) == typeid(SizeProperty))
	  {*parent->getProperty<SizeProperty>(text)=*((SizeProperty*)editedProperty);}
      }
      setGraph(graph);
      Observable::unholdObservers();
    }
  }
  //=================================================

}
