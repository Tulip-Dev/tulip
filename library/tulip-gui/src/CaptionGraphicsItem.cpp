/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QApplication>
#include <QMenu>
#include <QComboBox>
#include <QPushButton>

#include <tulip/DoubleProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/CaptionItem.h>
#include <tulip/CaptionGraphicsItem.h>
#include <tulip/CaptionGraphicsSubItems.h>
#include <tulip/View.h>

using namespace std;

namespace tlp {

CaptionGraphicsItem::CaptionGraphicsItem(View *view):_view(view) {

  _rondedRectItem=new CaptionGraphicsBackgroundItem(QRect(QPoint(0,0),QSize(130,260)));
  _rondedRectItem->setBrush(QBrush(QColor(255,255,255,180)));
  connect(_rondedRectItem,SIGNAL(filterChanged(float,float)),this,SLOT(filterChangedSlot(float,float)));

  _confPropertySelectionWidget = new QPushButton();
  _confPropertySelectionWidget->resize(QSize(120,25));
  _confPropertySelectionItem = new QGraphicsProxyWidget(_rondedRectItem);
  _confPropertySelectionItem->setWidget(_confPropertySelectionWidget);
  _confPropertySelectionItem->setPos(5,230);
  _confPropertySelectionItem->setZValue(2);
  _nodesEdgesTextItem = new QGraphicsSimpleTextItem(_rondedRectItem);
  connect(_confPropertySelectionWidget,SIGNAL(clicked()),this,SLOT(selectPropertyButtonClicked()));
}

void CaptionGraphicsItem::loadConfiguration() {
  constructConfigWidget();
}

void CaptionGraphicsItem::setType(unsigned int type) {
  if(type==1 || type==2)
    _nodesEdgesTextItem->setText("on nodes");
  else
    _nodesEdgesTextItem->setText("on edges");

  _nodesEdgesTextItem->setPos(35,210);
}

void CaptionGraphicsItem::generateColorCaption(const QGradient &activeGradient, const QGradient &hideGradient, const string &propertyName, double minValue, double maxValue) {
  _rondedRectItem->generateColorCaption(activeGradient,hideGradient,propertyName,minValue,maxValue);
}

void CaptionGraphicsItem::generateSizeCaption(const vector< pair <double,float> > &metricToSizeFilteredList,const string &propertyName, double minValue, double maxValue) {
  _rondedRectItem->generateSizeCaption(metricToSizeFilteredList,propertyName,minValue,maxValue);
}

void CaptionGraphicsItem::constructConfigWidget() {

  if(_view->graph()==NULL) {
    _confPropertySelectionWidget->setText("");
    _confPropertySelectionWidget->setToolTip("");
    return;
  }

  QString oldName = _confPropertySelectionWidget->text();
  QString selectedProp = QString::null;
  std::string piName;
  bool findViewMetric = false;
  QString firstDoubleMetricName = QString::null;
  forEach(piName, _view->graph()->getProperties()) {
    if (_view->graph()->getProperty(piName)->getTypename() != "double")
      continue;

    if(firstDoubleMetricName.isNull())
      firstDoubleMetricName=piName.c_str();

    if (oldName == piName.c_str())
      selectedProp = piName.c_str();

    if(piName == "viewMetric")
      findViewMetric = true;
  }

  if(selectedProp.isNull()) {
    if(findViewMetric)
      selectedProp = "viewMetric";
    else
      selectedProp = firstDoubleMetricName;
  }

  _confPropertySelectionWidget->setText(wrappedPropName(selectedProp));
  _confPropertySelectionWidget->setToolTip(selectedProp);
}

string CaptionGraphicsItem::usedProperty() {
  return _confPropertySelectionWidget->toolTip().toStdString();
}

CaptionGraphicsItem::~CaptionGraphicsItem() {
}

void CaptionGraphicsItem::filterChangedSlot(float begin,float end) {
  emit filterChanged(begin,end);
}

void CaptionGraphicsItem::selectPropertyButtonClicked() {
  if (_view->graph() == NULL)
    return;

  QMenu menu;
  std::string piName;
  forEach(piName, _view->graph()->getProperties()) {
    if (_view->graph()->getProperty(piName)->getTypename() != "double")
      continue;

    QAction* action =
      menu.addAction(piName.c_str(),this,SLOT(propertySelectedSlot()));

    if (_confPropertySelectionWidget->text() == QString(piName.c_str()))
      menu.setActiveAction(action);

  }
  // set a combo like stylesheet
  QPalette palette = QComboBox().palette();
  menu.setStyleSheet(QString("QMenu::item {color: %1; background-color: %2;} QMenu::item:selected {color: %3; background-color: %4}").arg(palette.color(QPalette::Active, QPalette::Text).name()).arg(palette.color(QPalette::Active, QPalette::Base).name()).arg(palette.color(QPalette::Active, QPalette::HighlightedText).name()).arg(palette.color(QPalette::Active, QPalette::Highlight).name()));

  // compute a combo like position
  // to popup the menu
  QWidget* pViewport = QApplication::widgetAt(QCursor::pos());
  QWidget* pView = pViewport->parentWidget();
  QGraphicsView* pGraphicsView = qobject_cast<QGraphicsView*>(pView);
  QPoint popupPos = pGraphicsView->mapToGlobal(pGraphicsView->mapFromScene(_confPropertySelectionItem->mapToScene(_confPropertySelectionItem->subWidgetRect(_confPropertySelectionWidget).bottomLeft())));

  menu.exec(popupPos);
}

void CaptionGraphicsItem::propertySelectedSlot() {
  QAction* action = static_cast<QAction*>(sender());
  _confPropertySelectionWidget->setText(wrappedPropName(action->text()));
  _confPropertySelectionWidget->setToolTip(action->text());
  emit selectedPropertyChanged(action->text().toStdString());
}

QString CaptionGraphicsItem::wrappedPropName(const QString &originalName) const {
  return originalName;
}

}
