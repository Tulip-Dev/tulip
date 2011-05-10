/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include <iostream>
#include <sstream>
#include <string>

#include <QtCore/qstring.h>
#include <QtGui/qpushbutton.h>
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
#include <QtGui/qlabel.h>
#include <QtGui/qmenu.h>
#include <QtGui/qheaderview.h>
#include <QtGui/qscrollbar.h>

#include <tulip/Graph.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/GlyphManager.h>
#include <tulip/GlGraphStaticData.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/EdgeExtremityGlyphManager.h>
#include <tulip/GlTextureManager.h>

#include "tulip/TlpQtTools.h"
#include "tulip/PropertyWidget.h"
#include "tulip/TextEditDialog.h"

#define TABLEBUFSIZE 100

using namespace std;
using namespace tlp;

//================================================================================
PropertyWidget::PropertyWidget(QWidget *parent, const char *name) :
  TulipTableWidget(parent, name), graph(0), vScrollPos(0),displayNode(true),inUpdate(false){
  resetBackColor1();
  resetBackColor2();
  editedProperty=0;
  setRowCount(0);
  setColumnCount(2);
  setHorizontalHeaderItem(0, new QTableWidgetItem("Id"));
  setHorizontalHeaderItem(1, new QTableWidgetItem(""));
  horizontalHeader()->setStretchLastSection(true);
  showProperties = false;
  connect(verticalScrollBar(), SIGNAL(valueChanged(int)), SLOT(scroll(int)));
  connect(this,SIGNAL(cellChanged(int,int)),this,SLOT(changePropertyValue(int,int)));
  // enable context menu
  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this,SIGNAL(customContextMenuRequested (const QPoint &)),
	  SLOT(showContextMenu(const QPoint&)));
}

PropertyWidget::~PropertyWidget() {}

Graph *PropertyWidget::getGraph() const {return graph;}
void PropertyWidget::setGraph(Graph *s) {
  editedProperty=0;
  clearContents();
  setRowCount(0);
  setColumnCount(2);
  horizontalHeaderItem(0)->setText("Id");
  horizontalHeaderItem(1)->setText("Property");
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
  setColumnCount(2);
  horizontalHeaderItem(0)->setText("Id");
  horizontalHeaderItem(1)->setText(QString::fromUtf8(name.c_str()));
  updateNbElements();
  update();
}

void PropertyWidget::selectNodeOrEdge(bool b) {
  displayNode=b;
}

void PropertyWidget::changePropertyValue(int i,int j) {
  if(inUpdate)
    return;
  if (displayNode)
    changePropertyNodeValue(i,j);
  else
    changePropertyEdgeValue(i,j);
}

void PropertyWidget::changePropertyEdgeValue(int i,int j) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  if (editedProperty == NULL) return;
  Observable::holdObservers();
  bool result=true;
  string str=((TulipTableWidgetItem *)item(i,j))->textForTulip().toUtf8().data();
  BooleanProperty *tmpSel=graph->getProperty<BooleanProperty>("viewSelection");
  Iterator<edge> *it=graph->getEdges();
  edge tmp;
  // allow to undo
  graph->push();
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
                           QString("The input value for this edge is not correct,\n"
                                   "The change won't be applied.")
                           );
    // restore old value
    disconnect(this,SIGNAL(cellChanged(int,int)), this, SLOT(changePropertyValue(int,int)));
    setTulipEdgeItem(editedProperty, editedPropertyName, tmp, i, 1);
    connect(this,SIGNAL(cellChanged(int,int)), this, SLOT(changePropertyValue(int,int)));
  }
  else {
    emit tulipEdgePropertyChanged(graph, tmp, editedPropertyName.c_str(), str.c_str());
  }

  this->setColumnWidth(1, horizontalHeader()->length() - columnWidth(0));
  Observable::unholdObservers();
}

void PropertyWidget::changePropertyNodeValue(int i, int j) {
  if (editedProperty == NULL) return;
  Observable::holdObservers();
  bool result=true;
  string str = ((TulipTableWidgetItem*)item(i, j))->textForTulip().toUtf8().data();
  BooleanProperty *tmpSel=graph->getProperty<BooleanProperty>("viewSelection");
  Iterator<node> *it=graph->getNodes();
  node tmp;
  // allow to undo
  graph->push();
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
                           QString("The input value for this node is not correct,\n"
                                   "The change won't be applied.")
                           );
    // restore old value
    disconnect(this,SIGNAL(cellChanged(int,int)), this, SLOT(changePropertyValue(int,int)));
    setTulipNodeItem(editedProperty, editedPropertyName, tmp, i, 1);
    connect(this,SIGNAL(cellChanged(int,int)), this, SLOT(changePropertyValue(int,int)));
  }
  else {
    emit tulipNodePropertyChanged(graph, tmp, editedPropertyName.c_str(), str.c_str());
  }
  this->setColumnWidth(1, this->horizontalHeader()->width() - this->columnWidth(0));
  Observable::unholdObservers();
}

void PropertyWidget::filterSelection(bool b) {
  _filterSelection=b;
  if (!b)
    updateNbElements();
  //update();
}

void PropertyWidget::scroll(int i) {
  if (editedProperty == NULL) return;
  int curId = i;
  bool toUpdate = false;
  if (curId > (vScrollPos + TABLEBUFSIZE/2)) {
    if ((vScrollPos + TABLEBUFSIZE+1) != (int) nbElement) {
      if (curId + TABLEBUFSIZE >= (int) nbElement)
        curId=nbElement-TABLEBUFSIZE-1;
      vScrollPos=curId;
      toUpdate=true;
    }
  }
  if (curId<vScrollPos) {
    if (vScrollPos!=0) {
      vScrollPos=curId;
      toUpdate=true;
    }
  }

  if (toUpdate){
    update();
  }
}

void PropertyWidget::update() {
  if (graph == NULL) return;
  // check editedProperty
  if (editedProperty && !graph->existProperty(editedPropertyName))
    editedProperty = NULL;

  bool lastUpdateStatus=inUpdate;
  inUpdate=true;
  clearContents();
  if (displayNode)
    updateNodes();
  else
    updateEdges();
  inUpdate=lastUpdateStatus;
  horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
  QTableWidget::repaint();
}

void PropertyWidget::updateEdges() {
  if (editedProperty == NULL) {
    return;
  }
  updateNbElements();
  BooleanProperty *tmpSel=graph->getProperty<BooleanProperty>("viewSelection");
  setRowCount(nbElement);
  Iterator<edge> *it=graph->getEdges();
  for (int i=0; it->hasNext();) {
    char buf[16];
    edge tmp=it->next();
    if (!_filterSelection || tmpSel->getEdgeValue(tmp)) {
      if ((i>=vScrollPos) && (i<=vScrollPos+TABLEBUFSIZE)) {
        sprintf (buf,"%d", tmp.id );
        QTableWidgetItem* idItem = new QTableWidgetItem(buf);
        idItem->setFlags(Qt::ItemIsEnabled);
        setItem(i, 0, idItem);
        setTulipEdgeItem(editedProperty, editedPropertyName, tmp, i, 1);
      }
      else if (i>vScrollPos+TABLEBUFSIZE) break;
      ++i;
    }
  } delete it;
  //adjustColumn(0);
  setColumnWidth(1, horizontalHeader()->length() - columnWidth(0));
}

void PropertyWidget::updateNodes() {
  if (editedProperty == NULL) return;
  updateNbElements();
  BooleanProperty *tmpSel=graph->getProperty<BooleanProperty>("viewSelection");
  setRowCount(nbElement);
  Iterator<node> *it=graph->getNodes();
  for (int i=0; it->hasNext();) {
    char buf[16];
    node tmp=it->next();
    if (!_filterSelection || tmpSel->getNodeValue(tmp)) {
      if ((i>=vScrollPos) && (i<=vScrollPos+TABLEBUFSIZE)) {
        sprintf (buf,"%d", tmp.id );
        QTableWidgetItem* idItem = new QTableWidgetItem(buf);
        idItem->setFlags(Qt::ItemIsEnabled);
        setItem(i, 0, idItem);
        setTulipNodeItem(editedProperty, editedPropertyName, tmp, i, 1);
      }
      else if (i>vScrollPos+TABLEBUFSIZE) break;
      ++i;
    }
  } delete it;
  //adjustColumn(0);
  setColumnWidth(1, horizontalHeader()->length() - columnWidth(0));
}

void PropertyWidget::updateNbElements() {
  if (graph == NULL) return;
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
  QTableWidget::update();
}

void PropertyWidget::setAllNodeValue() {
  if (editedProperty == NULL) return;
  Observable::holdObservers();
  bool ok=false;
  string tmpStr;

  if (editedPropertyName == "viewShape") {
    QStringList tmp;
    Iterator<string> *itS=GlyphPluginManager::availablePlugins();
    while (itS->hasNext()){
      tmp.append(QString(itS->next().c_str()));
    }delete itS;

    QString shapeName = QInputDialog::getItem(this, string("Property \"" + editedPropertyName + "\": set all node value").c_str(),
                                              "Please choose a shape",
                                              tmp, 0, false, &ok);
    if (ok) {
      stringstream ss;
      ss << GlyphManager::getInst().glyphId(shapeName.toUtf8().data());
      tmpStr = ss.str();
    }
  } else if (editedPropertyName == "viewFont") {
    ChooseFileNameDialog dialog(QString("Font (*.ttf)"),this);
    if(dialog.exec()==QDialog::Accepted){
      tmpStr=dialog.getText().toStdString();
      ok=true;
    }
  } else if (editedPropertyName == "viewTexture") {
    ChooseFileNameDialog dialog(QString("Images (*.png *.jpeg *.jpg *.bmp)"),TulipBitmapDir.c_str(),this);
    if(dialog.exec()==QDialog::Accepted){
      tmpStr=dialog.getText().toStdString();
      GlTextureManager::getInst().clearErrorVector();
      ok=true;
    }
  } else if (editedPropertyName == "viewLabelPosition") {
    QStringList tmp;
    for (int i = 0; i < 5; i++)
      tmp.append(QString(GlGraphStaticData::labelPositionName(i).c_str()));

    QString labelPosName = QInputDialog::getItem(this, string("Property \"" + editedPropertyName + "\": set all node value").c_str(),
                                              "Please choose a position",
                                              tmp, 0, false, &ok);
    if (ok) {
      stringstream ss;
      ss << GlGraphStaticData::labelPositionId(labelPosName.toUtf8().data());
      tmpStr = ss.str();
    }
  }
  else if (typeid((*editedProperty)) == typeid(DoubleProperty)) {
    double d = QInputDialog::getDouble(this, QString::fromUtf8(string("Property \"" + editedPropertyName + "\": set all node value").c_str()),
                                       "Please enter your value",
                                       0, -2147483647, 2147483647, 10,
                                       &ok);
    if (ok) {
      stringstream ss;
      ss << d;
      tmpStr = ss.str();
    }
  }
  else if (typeid(*editedProperty) == typeid(ColorProperty)) {
    QColor col;
    if (getColorDialog(qRgba(255,0,0,200),NULL,"Color chooser",col)) {
      ok=true;
      stringstream ss;
      ss << "(" << col.red() << "," << col.green() << "," << col.blue() << "," << col.alpha() << ")";
      tmpStr = ss.str();
    }
  }
  else if(editedPropertyName == "viewLabel"){
    TextEditDialog textEditDialog("");
    QString text;
    if(textEditDialog.exec()){
      text=textEditDialog.getText();
      ok=true;
    }
    if (ok) tmpStr = text.toUtf8().data();
    else ok = false;
  }
  else {
    QString text = QInputDialog::getText(this, QString::fromUtf8(string("Property \"" + editedPropertyName + "\": set all node value").c_str()),
                                         "Please enter your value",
                                         QLineEdit::Normal,QString::null, &ok);
    if (ok) tmpStr = text.toUtf8().data();
    else ok = false;
  }

  // allow to undo
  graph->push();
  if (ok) {
    bool result=true;
    BooleanProperty *tmpSel=graph->getProperty<BooleanProperty>("viewSelection");

    if (!_filterSelection && graph->existLocalProperty(editedPropertyName)) {
      result=editedProperty->setAllNodeStringValue(tmpStr);
    }
    else {
      Iterator<node> *it=graph->getNodes();
      while (it->hasNext()) {
        node tmp=it->next();
        if (!_filterSelection || tmpSel->getNodeValue(tmp)) {
          result = editedProperty->setNodeStringValue(tmp,tmpStr);
          if (!result) break;
        }
      } delete it;
    }

    if (!result) {
      QMessageBox::critical(0, "Tulip Property Editor : set all node value Failed",
                            QString("The input value for the nodes is not correct,\n"
                                    "The change won't be applied.")
                            );
    }
    else {
      this->update();
    }
  }

  Observable::unholdObservers();
}

void  PropertyWidget::setAllEdgeValue() {
  if (editedProperty == NULL) return;
  Observable::holdObservers();
  bool ok=false;
  string tmpStr;

  if (typeid(*editedProperty) == typeid(ColorProperty)) {
    QColor col;
    if (getColorDialog(qRgba(0,0,0,200),NULL,"Color chooser",col)) {
      ok=true;
      stringstream ss;
      ss << "(" << col.red() << "," << col.green() << "," << col.blue() << "," << col.alpha() << ")";
      tmpStr = ss.str();
    }
  }
  else if (editedPropertyName == "viewShape") {
    QStringList tmp;
    for (int i = 0; i < GlGraphStaticData::edgeShapesCount; i++)
      tmp.append(QString(GlGraphStaticData::edgeShapeName(GlGraphStaticData::edgeShapeIds[i]).c_str()));

    QString shapeName = QInputDialog::getItem(this, string("Property \"" + editedPropertyName + "\": set all node value").c_str(),
                                              "Please choose a shape",
                                              tmp, 0, false, &ok);
    if (ok) {
      stringstream ss;
      ss << GlGraphStaticData::edgeShapeId(shapeName.toUtf8().data());
      tmpStr = ss.str();
    }
  }
  else if(editedPropertyName == "viewSrcAnchorShape" || editedPropertyName == "viewTgtAnchorShape"){
	  QStringList tmp;
	  tmp.append("NONE");
	  Iterator<string> *itS=EdgeExtremityGlyphPluginManager::availablePlugins();
	  while (itS->hasNext()){
		  tmp.append(QString(itS->next().c_str()));
	  }delete itS;

	  QString shapeName = QInputDialog::getItem(this, string("Property \"" + editedPropertyName + "\": set all edge value").c_str(),
	                                                "Please choose a shape",
	                                                tmp, 0, false, &ok);
	  if(ok){
		  stringstream ss;
		  ss << EdgeExtremityGlyphManager::getInst().glyphId(shapeName.toUtf8().data());
		  tmpStr = ss.str();
	  }
  } else if (editedPropertyName == "viewTexture") {
    ChooseFileNameDialog dialog(QString("Images (*.png *.jpeg *.jpg *.bmp)"),TulipBitmapDir.c_str(),this);
    if(dialog.exec()==QDialog::Accepted){
      tmpStr=dialog.getText().toStdString();
      GlTextureManager::getInst().clearErrorVector();
      ok=true;
    }
  } else {
    QString text = QInputDialog::getText(this, QString::fromUtf8(string("Property \"" + editedPropertyName + "\": set all edge value").c_str()),
                                         "Please enter your value",
                                         QLineEdit::Normal, QString::null, &ok);
    if (ok) tmpStr = text.toUtf8().data();
    else ok = false;
  }

  // allow to undo
  graph->push();
  if (ok) {
    bool result=true;
    BooleanProperty *tmpSel=graph->getProperty<BooleanProperty>("viewSelection");
    if (!_filterSelection && graph->existLocalProperty(editedPropertyName)) {
      result=editedProperty->setAllEdgeStringValue(tmpStr);
    }
    else {
      Iterator<edge> *itE=graph->getEdges();
      while (itE->hasNext()) {
        edge tmp=itE->next();
        if (!_filterSelection || tmpSel->getEdgeValue(tmp)) {
          result = editedProperty->setEdgeStringValue(tmp, tmpStr);
          if (!result) break;
        }
      } delete itE;
    }

    if (!result) {
      QMessageBox::critical(0, "Tulip Property Editor set all node value Failed",
                            QString("The input value for the edges is not correct,\n"
                                    "change won't be applied.")
                            );
    }
    else {
      this->update();
    }
  }
  Observable::unholdObservers();
}

void PropertyWidget::connectNotify ( const char * signal ) {
  if (string(signal).find("showElementProperties") != string::npos)
    showProperties = true;
}

void PropertyWidget::showContextMenu(const QPoint & pos) {
  QModelIndex index = indexAt(pos);
  int row = index.row();
  if ((unsigned int) row < nbElement) {
    std::string textId(item(row, 0)->text().toUtf8().data());
    if (textId.size() && (textId.find_first_not_of("0123456789") == string::npos)) {
      selectRow(row);
      QMenu contextMenu(this);
      string title;
      title += displayNode ? "Node " : "Edge ";
      title += textId;
      contextMenu.addAction(tr(title.c_str()))->setEnabled(false);
      contextMenu.addSeparator();
      contextMenu.addAction(tr("Add to/Remove from selection"));
      QAction* selectAction = contextMenu.addAction(tr("Select"));
      QAction* deleteAction = contextMenu.addAction(tr("Delete"));
      QAction* propAction = NULL;
      if (showProperties) {
	contextMenu.addSeparator();
	propAction = contextMenu.addAction(tr("Properties"));
      }
      QAction* action = contextMenu.exec(mapToGlobal(pos), selectAction);
      clearSelection();
      if (!action)
	return;
      unsigned int itemId = (unsigned int) atoi(textId.c_str());
      Observable::holdObservers();
      if (action == deleteAction) { // Delete
	// delete graph item
	if (displayNode)
	  graph->delNode(node(itemId));
	else
	  graph->delEdge(edge(itemId));
      }
      if (showProperties && action == propAction) {
	emit showElementProperties(itemId, displayNode);
      }	else {
	BooleanProperty *elementSelected = graph->getProperty<BooleanProperty>("viewSelection");
	if (action == selectAction) { // Select
	  // empty selection
	  elementSelected->setAllNodeValue(false);
	  elementSelected->setAllEdgeValue(false);
	}
	// select graph item
	if (displayNode) {
	  node tmpNode(itemId);
	  elementSelected->setNodeValue(tmpNode, !elementSelected->getNodeValue(tmpNode));
	} else {
	  edge tmpEdge(itemId);
	  elementSelected->setEdgeValue(tmpEdge, !elementSelected->getEdgeValue(tmpEdge));
	}
      }
      Observable::unholdObservers();
    }
  }
}
