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
#include "tulip3/FindSelectionWidget.h"

#include <tulip/Graph.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/IntegerProperty.h>
#include <QtCore/qregexp.h>

using namespace tlp;

FindSelectionWidget::FindSelectionWidget(Graph *graph, std::string& currentProperty,
					 QWidget *parent) : 
  QDialog(parent),
  graph(graph){
  setupUi(this);
  insertProperties(currentProperty);
  propertyChanged(-1);
}

static bool IsEvaluableProxy( PropertyInterface *p) {
  return dynamic_cast<DoubleProperty*>(p) || dynamic_cast<StringProperty*>(p) ||
    dynamic_cast<BooleanProperty*>(p) || dynamic_cast<IntegerProperty*>(p);
}

struct TLP3_COMPAT_SCOPE GItem {
  GItem( const node & inN ): n(inN), isnode(true) {}
  GItem( const edge & inE ): e(inE), isnode(false) {}
  node n;
  edge e;
  bool isnode;
};

static bool EvalProxy( PropertyInterface *p, const GItem &gi, std::string value, int mode) {
  assert( IsEvaluableProxy(p) );
  DoubleProperty  * metp = dynamic_cast<DoubleProperty*>(p);
  StringProperty  * strp = dynamic_cast<StringProperty*>(p);
  BooleanProperty * selp = dynamic_cast<BooleanProperty*>(p);
  IntegerProperty * intp = dynamic_cast<IntegerProperty*>(p);
  
  if( metp ) {
    // DoubleProperty 
    DoubleType::RealType v = gi.isnode ? metp->getNodeValue( gi.n ) : metp->getEdgeValue( gi.e );
    DoubleType::RealType v0 = atof( value.c_str() );
    if( mode == 0 )		return v < v0;
    if( mode == 1 )		return v <= v0;
    if( mode == 2 )		return v == v0;
    if( mode == 3 )		return v >= v0;
    if( mode == 4 )		return v > v0;
    else return v != v0;
  }
  
  if( strp ) {
    // StringProperty 
    std::string v = gi.isnode ? strp->getNodeValue( gi.n ) : strp->getEdgeValue( gi.e );
    std::string v0 = value.c_str();
    QRegExp rexp( v0.c_str() );
    if( mode == 2 ) return rexp.exactMatch( v.c_str() );
    else return !rexp.exactMatch( v.c_str() );
  }
  
  if( selp ) {
    // BooleanProperty 
    bool v  = gi.isnode ? selp->getNodeValue( gi.n )
      : selp->getEdgeValue( gi.e );
    bool v0 = !( value.size() == 0 || value == "False" || value == "false" || value == "0" );
    if( mode == 2 ) return v == v0;
    else return v != v0;
  }

  if( intp ) {
    // IntegerProperty 
    IntegerType::RealType v = gi.isnode ? intp->getNodeValue( gi.n ) : intp->getEdgeValue( gi.e );
    IntegerType::RealType v0 = atoi( value.c_str() );
    if( mode == 0 )		return v < v0;
    if( mode == 1 )		return v <= v0;
    if( mode == 2 )		return v == v0;
    if( mode == 3 )		return v >= v0;
    if( mode == 4 )		return v > v0;
    else return v != v0;
  }
  
  return false;
}

void FindSelectionWidget::propertyChanged(int) {
  PropertyInterface * p = getProperty();
  if( dynamic_cast<DoubleProperty*>(p) ) {
    filterOp->clear();
    filterOp->addItem( "<" );
    filterOp->addItem( "<=" );
    filterOp->addItem( "=" );
    filterOp->addItem( ">=" );
    filterOp->addItem( ">" );
    filterOp->addItem( "!=" );
    filterValue->setText( QString() );
    filterValue->setValidator( new QDoubleValidator(this) );
    filterValue->show();
  }
  else   if( dynamic_cast<IntegerProperty*>(p) ) {
    filterOp->clear();
    filterOp->addItem( "<" );
    filterOp->addItem( "<=" );
    filterOp->addItem( "=" );
    filterOp->addItem( ">=" );
    filterOp->addItem( ">" );
    filterOp->addItem( "!=" );
    filterValue->setText( QString() );
    filterValue->setValidator( new QIntValidator(this) );
    filterValue->show();
  }
  else if( dynamic_cast<StringProperty*>(p) ) {
    filterOp->clear();
    filterOp->addItem( "=" );
    filterOp->addItem( "!=" );
    filterValue->setValidator( 0 );
    filterValue->show();
  }
  else if( dynamic_cast<BooleanProperty*>(p) ) {
    filterOp->clear();
    filterOp->addItem( "False" );
    filterOp->addItem( "True" );
    filterValue->hide();
    filterValue->setValidator( 0 );
      filterValue->setText( QString() );
  }
}

int FindSelectionWidget::getMode() {
  if( filterOp->count() == 2 )
    // == -> 2 & != -> 5
    return filterOp->currentIndex() == 0 ? 2 : 5;
  else
    return filterOp->currentIndex();
}

std::string FindSelectionWidget::getCurrentProperty() {
  return std::string(inputProp->currentText().toUtf8().data());
}

PropertyInterface * FindSelectionWidget::getProperty() {
  return graph->getProperty(getCurrentProperty());
}

int FindSelectionWidget::getOperation() {
  if(setToSelectionOpt->isChecked())	return 0;
  if(addToSelectionOpt->isChecked())	return 1;
  if(rmvFromSelectionOpt->isChecked())	return 2;
  return 3;
}

int FindSelectionWidget::getSource() {
  return srcOpt->currentIndex();
}

void FindSelectionWidget::insertProperties(std::string &currentProperty) {
  Iterator<std::string> * propIt = graph->getProperties();
  while( propIt->hasNext() ) {
    std::string n = propIt->next();
    PropertyInterface* p = graph->getProperty( n );
    if (IsEvaluableProxy(p)) {
      inputProp->addItem(QString::fromUtf8(n.c_str()));
      if (currentProperty == n)
	inputProp->setCurrentIndex(inputProp->count() - 1);
    }
  } delete propIt;
    
}

int FindSelectionWidget::exec() {
  nbItemsFound = -1;
  QDialog::exec();
  return nbItemsFound;
}

void FindSelectionWidget::evalNodes(PropertyInterface *p, int mode,
				    std::string fv, int operation,
				    BooleanProperty *selection) {
  Iterator<node> * nodeIt = graph->getNodes();
  while( nodeIt->hasNext() ) {
    node n = nodeIt->next();
    bool v = EvalProxy( p, GItem(n), fv, mode );
    if (v)
      ++nbItemsFound;
    if( operation == 0 ) {
      selection->setNodeValue( n, v );
    } else if( operation == 1 ) {
      if( v )
	selection->setNodeValue( n, true );
    } else if( operation == 2 )  {
      if( v )
	selection->setNodeValue( n, false );
    } else {
      if( !v )
	selection->setNodeValue( n, false );
    }
  }
  delete nodeIt;
}

void FindSelectionWidget::evalEdges(PropertyInterface *p, int mode,
				    std::string fv, int operation, 
				    BooleanProperty *selection) {
  Iterator<edge> * edgeIt = graph->getEdges();
  while( edgeIt->hasNext() ) {
    edge e = edgeIt->next();
    bool v = EvalProxy( p, GItem(e), fv, mode );
    if (v)
      ++nbItemsFound;
      if( operation == 0 ) {
	selection->setEdgeValue( e, v );
      } else if( operation == 1 ) {
	if( v )
	  selection->setEdgeValue( e, true );
      } else if( operation == 2 ) {
	if( v )
	  selection->setEdgeValue( e, false );
      } else {
	if( !v )
	  selection->setEdgeValue( e, false );
      }
  }
  delete edgeIt;
}

void FindSelectionWidget::find(BooleanProperty *selP) {
  PropertyInterface * p = getProperty();
  int mode  = getMode();
  std::string fv = filterValue->text().toUtf8().data();
  int op = getOperation();
  nbItemsFound = 0;
  if( (getSource()+1) & 1 ) 
      evalNodes(p, mode, fv, op, selP);
  if( (getSource()+1) & 2 )
    evalEdges(p, mode, fv, op, selP);
}

void FindSelectionWidget::accept() {
  BooleanProperty * selP = graph->getProperty<BooleanProperty>("viewSelection");
  if (!selP) return;
  find(selP);
  close();
}
