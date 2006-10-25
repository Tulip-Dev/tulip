#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <typeinfo>

#if (QT_REL == 3)
#include <qlistview.h>
#include <qtable.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qinputdialog.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qlabel.h>
#include <qcolordialog.h>
#include <qtabwidget.h>
#else
#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qlistview.h>
#include <Qt3Support/q3table.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qinputdialog.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtGui/qlabel.h>
#include <QtGui/qcolordialog.h>
#include <QtGui/qtabwidget.h>
#include "tulip/Qt3ForTulip.h"
#endif

#include <tulip/Graph.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/GlGraph.h>
#include <tulip/PropertyWidget.h>
#include <tulip/SGHierarchyWidget.h>

#include "PropertyDialog.h"

using namespace std;
using namespace tlp;

//==================================================================================================
void PropertyDialog::setGlGraphWidget(GlGraph *glwidget) {
  glWidget=glwidget;
  tableNodes->selectNodeOrEdge(true);
  tableEdges->selectNodeOrEdge(false);
  //  tableEdges->filterSelection(_filterSelection);
  // tableNodes->filterSelection(_filterSelection);
}

PropertyDialog::PropertyDialog(QWidget* parent, const char* name, bool modal, Qt::WFlags fl)
#if (QT_REL == 3)
  : PropertyDialogData(parent, name, modal) {
#else
  : PropertyDialogData(parent, name, (Qt::WFlags) (fl | Qt::Widget)) {
#endif
  _filterSelection=false;
  glWidget=0;
  graph=0;
#if (QT_REL == 3)
  connect(localProperties, SIGNAL(selectionChanged(QListViewItem *)), SLOT(changePropertyName(QListViewItem *)));
  connect(inheritedProperties, SIGNAL(selectionChanged(QListViewItem *)), SLOT(changePropertyName(QListViewItem *)));
#else
  connect(localProperties, SIGNAL(selectionChanged(Q3ListViewItem *)), SLOT(changePropertyName(Q3ListViewItem *)));
  connect(inheritedProperties, SIGNAL(selectionChanged(Q3ListViewItem *)), SLOT(changePropertyName(Q3ListViewItem *)));
#endif
  connect(removeButton , SIGNAL(clicked()) , SLOT(removeProperty()) );
  connect(newButton,SIGNAL(clicked()),SLOT(newProperty()));
  connect(cloneButton,SIGNAL(clicked()),SLOT(cloneProperty()));

}
//=================================================
PropertyDialog::~PropertyDialog() {
}
//=================================================
void PropertyDialog::changePropertyName(QListViewItem *item) {
  if (item==0) {
    editedProperty=0;
    return;
  }
  string erreurMsg;

  tableNodes->selectNodeOrEdge(true);
  tableEdges->selectNodeOrEdge(false);
  tableNodes->changeProperty(graph,item->text(0).ascii());
  tableEdges->changeProperty(graph,item->text(0).ascii());

  PropertyInterface *tmpProxy=graph->getProperty(item->text(0).ascii());
  editedProperty=tmpProxy;
  editedPropertyName=item->text(0).ascii();
  propertyName->setText(item->text(0));
  
  if (graph->existLocalProperty(item->text(0).ascii()))
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
  if (tabWidget->currentPageIndex()==0)
    tableNodes->setAll();
  else
    tableEdges->setAll();
  setAllButton->setDown(false);
}
//=================================================
void PropertyDialog::setGraph(Graph *sg) {
  graph=sg;
  editedProperty=0;

  localProperties->clear();
  inheritedProperties->clear();
  propertyName->setText(QString("Select a Property"));
  //Build the property list
  tableNodes->setGraph(sg);
  tableEdges->setGraph(sg);
  tableEdges->filterSelection(_filterSelection);
  tableNodes->filterSelection(_filterSelection);
  if (sg==0) return;

  Iterator<string> *it=sg->getLocalProperties();
  while (it->hasNext()) {
    string tmp=it->next();
    QListViewItem *tmpItem=new QListViewItem(localProperties);
    tmpItem->setText(0,QString(tmp.c_str()));
  } delete it;
  it=sg->getInheritedProperties();
  while (it->hasNext()) {
    string tmp=it->next();
    QListViewItem *tmpItem=new QListViewItem(inheritedProperties);
    tmpItem->setText(0,QString(tmp.c_str()));
  } delete it;
}
//=================================================
void PropertyDialog::newProperty() {
  if (!graph) return;
  QStringList lst;
  lst << "Color" << "Integer" << "Layout" << "Metric" << "Selection" << "Size" << "String";
  bool ok = FALSE;
  QString res = QInputDialog::getItem( "Property type","Please select the property type", lst, 3, FALSE, &ok, this );
  if ( ok ) {
      QString text = QInputDialog::getText("Property name", "Please enter the property name", QLineEdit::Normal, QString::null, &ok, this );
      if (ok) {
	string erreurMsg;
	if (strcmp(res.ascii(),"Selection")==0) graph->getLocalProperty<BooleanProperty>(text.ascii());
	if (strcmp(res.ascii(),"Metric")==0) graph->getLocalProperty<DoubleProperty>(text.ascii());
	if (strcmp(res.ascii(),"Layout")==0) graph->getLocalProperty<LayoutProperty>(text.ascii());
	if (strcmp(res.ascii(),"String")==0) graph->getLocalProperty<StringProperty>(text.ascii());
	if (strcmp(res.ascii(),"Integer")==0) graph->getLocalProperty<IntegerProperty>(text.ascii());
	if (strcmp(res.ascii(),"Size")==0) graph->getLocalProperty<SizeProperty>(text.ascii());
	if (strcmp(res.ascii(),"Color")==0) graph->getLocalProperty<ColorProperty>(text.ascii());
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
  StringProperty *labels=graph->getProperty<StringProperty>("viewLabel");
  if (tabWidget->currentPageIndex()==0) {
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
    Observable::unholdObservers();
}
//=================================================
void PropertyDialog::removeProperty() {
  if (editedProperty==0) return;
  if(graph->existLocalProperty(editedPropertyName)) {
    graph->delLocalProperty(editedPropertyName);
    setGraph(graph);
    editedProperty=0;
  }
  else
    QMessageBox::critical( 0, "Tulip Property Editor Remove Failed",
			   QString("You cannot remove an inherited property,\n")
			   );
}
//=================================================
void PropertyDialog::cloneProperty() {
  if (!graph) return;
  bool ok=false;
  QString text = QInputDialog::getText( "Property name" ,  "Please enter the property name" , QLineEdit::Normal,QString::null, &ok, this );
  if (ok) {
    Observable::holdObservers();
    if (graph->existProperty(text.ascii())) {
      if (typeid(*graph->getProperty(text.ascii()))!=typeid(*editedProperty)) {
	QMessageBox::critical( 0, "Tulip Warning" ,"Property are not of the same type");
	return;
      }
    }
    string erreurMsg;
    if (typeid((*editedProperty)) == typeid(DoubleProperty))
      {*graph->getLocalProperty<DoubleProperty>(text.ascii())=*((DoubleProperty*)editedProperty);}
    if (typeid((*editedProperty)) == typeid(LayoutProperty))
      {*graph->getLocalProperty<LayoutProperty>(text.ascii())=*((LayoutProperty*)editedProperty);}
    if (typeid((*editedProperty)) == typeid(StringProperty))
      {*graph->getLocalProperty<StringProperty>(text.ascii())=*((StringProperty*)editedProperty);}
    if (typeid((*editedProperty)) == typeid(BooleanProperty))
      {*graph->getLocalProperty<BooleanProperty>(text.ascii())=*((BooleanProperty*)editedProperty);}
    if (typeid((*editedProperty)) == typeid(IntegerProperty))
      {*graph->getLocalProperty<IntegerProperty>(text.ascii())=*((IntegerProperty*)editedProperty);}
    if (typeid((*editedProperty)) == typeid(ColorProperty))
      {*graph->getLocalProperty<ColorProperty>(text.ascii())=*((ColorProperty*)editedProperty);}
    if (typeid((*editedProperty)) == typeid(SizeProperty))
      {*graph->getLocalProperty<SizeProperty>(text.ascii())=*((SizeProperty*)editedProperty);}
    setGraph(graph);
    Observable::unholdObservers();
  }
}
//=================================================
