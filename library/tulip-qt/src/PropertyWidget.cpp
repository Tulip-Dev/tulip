#include <iostream>
#include <sstream>
#include <string>

#if (QT_REL == 3)
#include <qstring.h>
#include <qpushbutton.h>
#include <qfiledialog.h>
#include <qcolordialog.h>
#include <qpainter.h>
#include <qfontmetrics.h>
#include <qapplication.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <qvalidator.h>
#include <qsizepolicy.h>
#include <qmessagebox.h>
#include <qinputdialog.h>
#else
#include <QtCore/qstring.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qfiledialog.h>
#include <QtGui/qcolordialog.h>
#include <QtGui/qpainter.h>
#include <QtGui/qfontmetrics.h>
#include <QtGui/qapplication.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtCore/qdir.h>
#include <QtCore/qfileinfo.h>
#include <QtGui/qvalidator.h>
#include <QtGui/qsizepolicy.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qinputdialog.h>
#include "tulip/Qt3ForTulip.h"
#endif

#include <tulip/Graph.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/GlGraph.h>
#include <tulip/Glyph.h>

#include "tulip/PropertyWidget.h"

#define TABLEBUFSIZE 100

using namespace std;
using namespace tlp;

//================================================================================
PropertyWidget::PropertyWidget(QWidget *parent, const char *name) :
  TulipTableWidget(parent, name), graph(0) {
  resetBackColor1();
  resetBackColor2();
  editedProperty=0;
  vScroll=verticalScrollBar();
  setColumnReadOnly(0, true);
  horizontalHeader()->setLabel(0, tr("Undefined"));
  connect(vScroll,SIGNAL(valueChanged(int)),SLOT(scroll(int)));
  connect(this,SIGNAL(valueChanged(int,int)),SLOT(changePropertyValue(int,int)));
  vScrollPos=0;
}

PropertyWidget::~PropertyWidget() {}

Graph *PropertyWidget::getGraph() const {return graph;}
void PropertyWidget::setGraph(Graph *s) {
  editedProperty=0;
  for (int i=0;i<numRows();++i) {
    clearCell(i,0);
    clearCell(i,1);
  }
  setNumRows(0);
  vScrollPos = 0;
  graph = s;
  update();
}

void PropertyWidget::changeProperty(Graph *sg,const std::string &name) {
  graph=sg;
  if (!graph->existProperty(name)) {
    editedProperty=0;
  } 
  else {
    editedProperty=graph->getProperty(name);
    editedPropertyName=name;
  }
  setNumCols(2);
  setColumnStretchable(1, true);
  updateNbElements();
  update();
}

void PropertyWidget::selectNodeOrEdge(bool b) {
  displayNode=b;
}

void PropertyWidget::changePropertyValue(int i,int j) {
  if (displayNode)
    changePropertyNodeValue(i,j);
  else
    changePropertyEdgeValue(i,j);
}

void PropertyWidget::changePropertyEdgeValue(int i,int j) {  
  //  cerr << __PRETTY_FUNCTION__ << endl;
  if (editedProperty==0) return;
  Observable::holdObservers();
  bool result=true;
  string str=this->text(i,j).ascii();
  BooleanProperty *tmpSel=graph->getProperty<BooleanProperty>("viewSelection");  
  Iterator<edge> *it=graph->getEdges();
  edge tmp;
  for (int pos=0; it->hasNext();) {
    tmp=it->next();
    if (!_filterSelection || tmpSel->getEdgeValue(tmp)) {
      if (pos==i) {
        result=editedProperty->setEdgeStringValue(tmp,str);
        break;
      }
      ++pos;
    }
  } delete it;
  
  if (!result) {
    QMessageBox::critical( 0, "Tulip Property Editor Change Failed",
                           QString("The value entered for this edge is not correct,\n"
                                   "The change won't be applied\n"
                                   "Modify the entered value to apply the changes.")
                           );
  }
  else {
    emit tulipEdgePropertyChanged(graph, tmp, editedPropertyName.c_str(), str.c_str());
  }
  
  this->setColumnWidth(1, this->horizontalHeader()->width() - this->columnWidth(0));
  Observable::unholdObservers();
}

void PropertyWidget::changePropertyNodeValue(int i,int j) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  if (editedProperty==0) return;
  Observable::holdObservers();
  bool result=true;
  string str=(this->text(i,j).latin1());
  
  BooleanProperty *tmpSel=graph->getProperty<BooleanProperty>("viewSelection");  
  Iterator<node> *it=graph->getNodes();
  node tmp;
  for (int pos=0; it->hasNext();) {
      tmp=it->next();
      if (!_filterSelection || tmpSel->getNodeValue(tmp)) {
        if (pos==i) {
          result=editedProperty->setNodeStringValue(tmp,str);
          break;
        }
        ++pos;
      }
  } delete it;
  
  if (!result) {
    QMessageBox::critical( 0, "Tulip Property Editor Change Failed",
                           QString("The value entered for this node is not correct,\n"
                                   "The change won't be applied\n"
                                   "Modify the entered value to apply the changes.")
                           );
  }
  else {
    emit tulipNodePropertyChanged(graph, tmp, editedPropertyName.c_str(), str.c_str());
  }  
  this->setColumnWidth(1, this->horizontalHeader()->width() - this->columnWidth(0));
  Observable::unholdObservers();
}

void PropertyWidget::filterSelection(bool b) {
  _filterSelection=b;
  updateNbElements();
  update();
}

void PropertyWidget::scroll(int i) {
  if (editedProperty==0) return;
  int curId=i/20;
  bool toUpdate=false;
  if (curId>vScrollPos+TABLEBUFSIZE/4) {
    if (vScrollPos+TABLEBUFSIZE/2!=nbElement) {
      vScrollPos=curId;
      if (curId+TABLEBUFSIZE/2>nbElement) curId=nbElement-TABLEBUFSIZE/2;
      toUpdate=true;
    }
  }
  if (curId<vScrollPos-TABLEBUFSIZE/4) {
    if (vScrollPos-TABLEBUFSIZE/2!=0) {
      vScrollPos=curId;
      if (curId-TABLEBUFSIZE/2<0) curId=TABLEBUFSIZE/2-1;
      toUpdate=true;
    }
  }
  if (toUpdate){update();}
}

void PropertyWidget::update() {
  if (graph==0) return;
  if (displayNode)
    updateNodes();
  else
    updateEdges();
  QTable::repaint();
}

void PropertyWidget::updateEdges() {
  if (editedProperty==0) {
    return;
  }
  BooleanProperty *tmpSel=graph->getProperty<BooleanProperty>("viewSelection");
  setNumRows(nbElement);
  Iterator<edge> *it=graph->getEdges();
  for (int i=0; it->hasNext();) {
    char buf[16];
    edge tmp=it->next();
    if (!_filterSelection || tmpSel->getEdgeValue(tmp)) {
      if ((i>=vScrollPos-TABLEBUFSIZE/2) && (i<=vScrollPos+TABLEBUFSIZE/2)) {
	sprintf (buf,"%d", tmp.id );
	setText(i,0,buf );
	setTulipEdgeItem(editedProperty, editedPropertyName, tmp, i, 1);
      }
      else if (i>vScrollPos+TABLEBUFSIZE/2) break;
      ++i;
    }
  } delete it;
  adjustColumn(0);
  setColumnWidth(1, horizontalHeader()->width() - columnWidth(0));
}

void PropertyWidget::updateNodes() {
  if (editedProperty==0) return;
  BooleanProperty *tmpSel=graph->getProperty<BooleanProperty>("viewSelection");
  setNumRows(nbElement);
  Iterator<node> *it=graph->getNodes();
  for (int i=0; it->hasNext();) {
    char buf[16];
    node tmp=it->next();
    if (!_filterSelection || tmpSel->getNodeValue(tmp)) {
      if ((i>=vScrollPos-TABLEBUFSIZE/2) && (i<=vScrollPos+TABLEBUFSIZE/2)) {
	sprintf (buf,"%d", tmp.id );
	setText(i,0,buf );
	setTulipNodeItem(editedProperty, editedPropertyName, tmp, i, 1);
      }
      else if (i>vScrollPos+TABLEBUFSIZE/2) break;
      ++i;
    }
  } delete it;
  adjustColumn(0);
  setColumnWidth(1, horizontalHeader()->width() - columnWidth(0));
}

void PropertyWidget::updateNbElements() {
  if (graph==0) return;
  unsigned int nbNode,nbEdge;
  if (!_filterSelection) {
    nbNode=graph->numberOfNodes();
    nbEdge=graph->numberOfEdges();
  }
  else {
    BooleanProperty *tmpSel=graph->getProperty<BooleanProperty>("viewSelection");
    nbNode=0;
    nbEdge=0;
    Iterator<node> *it=graph->getNodes();
    while (it->hasNext())
      if (tmpSel->getNodeValue(it->next())) nbNode++;
    delete it;
    Iterator<edge> *itE=graph->getEdges();
    while (itE->hasNext())
      if (tmpSel->getEdgeValue(itE->next())) nbEdge++;
    delete itE;
  }
  if (displayNode) nbElement=nbNode; else nbElement=nbEdge;
}

void PropertyWidget::setAll() {
  if (displayNode)
   setAllNodeValue();
  else
   setAllEdgeValue();
  QTable::update();
}

void PropertyWidget::setAllNodeValue() {
  if (editedProperty==0) return;
  Observable::holdObservers();
  bool ok=false;
  string tmpStr;

  if (editedPropertyName == "viewShape") {
    QStringList tmp;
    GlyphFactory::initFactory();
    Iterator<string> *itS=GlyphFactory::factory->availablePlugins();
    while (itS->hasNext()){
      tmp.append(QString(itS->next().c_str()));
    }delete itS;
    
    QString shapeName = QInputDialog::getItem(string("Property \"" + editedPropertyName + "\": set all node value").c_str(),
                                              "Please choose a shape",
                                              tmp, 0, false, &ok, this);
    if (ok) {
      stringstream ss;
      ss << GlGraph::glyphId(shapeName.ascii());
      tmpStr = ss.str();
    }
  } else if (editedPropertyName == "viewLabelPosition") {
    QStringList tmp;
    for (int i = 0; i < 5; i++)
      tmp.append(QString(GlGraph::labelPositionName(i).c_str()));
    
    QString labelPosName = QInputDialog::getItem(string("Property \"" + editedPropertyName + "\": set all node value").c_str(),
                                              "Please choose a position",
                                              tmp, 0, false, &ok, this);
    if (ok) {
      stringstream ss;
      ss << GlGraph::labelPositionId(labelPosName.ascii());
      tmpStr = ss.str();
    }
  }
  else if (typeid((*editedProperty)) == typeid(DoubleProperty)) {
    double d = QInputDialog::getDouble(string("Property \"" + editedPropertyName + "\": set all node value").c_str(),
                                       "Please enter your value",
                                       0, -2147483647, 2147483647, 10,
                                       &ok, this);
    if (ok) {
      stringstream ss;
      ss << d;
      tmpStr = ss.str();
    }
  }
  else if (typeid(*editedProperty) == typeid(ColorProperty)) {
    QRgb col = QColorDialog::getRgba(qRgba(0,0,0,200), &ok);
    if (ok) {
      stringstream ss;
      ss << "(" << qRed(col) << "," << qGreen(col) << "," << qBlue(col) << "," << qAlpha(col) << ")";
      tmpStr = ss.str();
    }
  }
  else {
    QString text = QInputDialog::getText(string("Property \"" + editedPropertyName + "\": set all node value").c_str(),
                                         "Please enter your value",
                                         QLineEdit::Normal,QString::null, &ok, this);
    if (ok) tmpStr = text.ascii();
    else ok = false;
  }
  
  if (ok) {
    bool result=true;
    BooleanProperty *tmpSel=graph->getProperty<BooleanProperty>("viewSelection");

    if (!_filterSelection && graph->existLocalProperty(editedPropertyName)) {
      result=editedProperty->setAllNodeStringValue(tmpStr);
    }
    else {
      Iterator<node> *it=graph->getNodes();
      for (int nbNode=0; it->hasNext();) {
        node tmp=it->next();
        if (!_filterSelection || tmpSel->getNodeValue(tmp)) {
          result=editedProperty->setNodeStringValue(tmp,tmpStr);
          if (!result) break;
        }
        if (_filterSelection && tmpSel->getNodeValue(tmp)) {
          this->setText(nbNode, 1, tmpStr.c_str());
          nbNode++;
        }
      }
      delete it;
    }

    if (!result) {
      QMessageBox::critical(0, "Tulip Property Editor : set all node value Failed",
                            QString("The value entered for the nodes is not correct,\n"
                                    "The change won't be applied\n")
                            );
    }
    else {
      this->update();
    }
  }
  
  Observable::unholdObservers();
}

void  PropertyWidget::setAllEdgeValue() {
  if (editedProperty==0) return;
  Observable::holdObservers();
  bool ok=false;
  string tmpStr;

  if (typeid(*editedProperty) == typeid(ColorProperty)) {
    QRgb col = QColorDialog::getRgba(qRgba(255,0,0,200), &ok);
    if (ok) {
      stringstream ss;
      ss << "(" << qRed(col) << "," << qGreen(col) << "," << qBlue(col) << "," << qAlpha(col) << ")";
      tmpStr = ss.str();
    }
  }
  else if (editedPropertyName == "viewShape") {
    QStringList tmp;
    for (int i = 0; i < GlGraph::edgeShapesCount; i++)
      tmp.append(QString(GlGraph::edgeShapeName(GlGraph::edgeShapeIds[i]).c_str()));
    
    QString shapeName = QInputDialog::getItem(string("Property \"" + editedPropertyName + "\": set all node value").c_str(),
                                              "Please choose a shape",
                                              tmp, 0, false, &ok, this);
    if (ok) {
      stringstream ss;
      ss << GlGraph::edgeShapeId(shapeName.ascii());
      tmpStr = ss.str();
    }
  }
  else {
    QString text = QInputDialog::getText(string("Property \"" + editedPropertyName + "\": set all edge value").c_str(),
                                         "Please enter your value",
                                         QLineEdit::Normal, QString::null, &ok, this);
    if (ok) tmpStr = text.ascii();
    else ok = false;
  }
  
  if (ok) {
    bool result=true;
    BooleanProperty *tmpSel=graph->getProperty<BooleanProperty>("viewSelection");
    if (!_filterSelection && graph->existLocalProperty(editedPropertyName)) {
      result=editedProperty->setAllEdgeStringValue(tmpStr);
    }
    else {
      Iterator<edge> *itE=graph->getEdges();
      for (int nbEdge=0; itE->hasNext();) {
        edge tmp=itE->next();
        if (!_filterSelection || tmpSel->getEdgeValue(tmp)) {
          result=editedProperty->setEdgeStringValue(tmp, tmpStr);
          if (!result) break;
        }
        if (_filterSelection && tmpSel->getEdgeValue(tmp)) {
          this->setText(nbEdge, 1, tmpStr.c_str());
          nbEdge++;
        }
      }
      delete itE;
    }
    
    if (!result) {
      QMessageBox::critical(0, "Tulip Property Editor set all node value Failed",
                            QString("The value entered for the edges is not correct,\n"
                                    "change won't be applied\n")
                            );
    }
    else {
      this->update();
    }
  }
  Observable::unholdObservers();
}
