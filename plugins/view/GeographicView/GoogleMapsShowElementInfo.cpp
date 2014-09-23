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

#include "GoogleMapsShowElementInfo.h"


#include "ui_ElementInformationsWidget.h"
#include "GoogleMapsViewInteractors.h"
#include "../../utils/StandardInteractorPriority.h"
#include "../../utils/ViewNames.h"

#include <tulip/MouseInteractors.h>
#include <tulip/NodeLinkDiagramComponentInteractor.h>
#include <tulip/TulipItemDelegate.h>
#include <tulip/GraphElementModel.h>
#include <tulip/GlSimpleEntityItemModel.h>
#include <tulip/GlComplexPolygon.h>

#include <QPropertyAnimation>
#include <QLayout>
#include <QStringList>
#include <QVariantList>

using namespace std;
using namespace tlp;

// this class is needed to allow interactive settings
// of some GlComplexPolygon rendering properties
class tlp::GlComplexPolygonItemEditor :public GlSimpleEntityItemEditor {
public:

  GlComplexPolygonItemEditor(GlComplexPolygon* poly): GlSimpleEntityItemEditor(poly) {}

  // redefined inherited methods from GlSimpleEntityItemEditor
  QStringList propertiesNames() const {
    return QStringList() << "fillColor" << "outlineColor";
  }

  QVariantList propertiesQVariant() const  {
    return QVariantList() << QVariant::fromValue<Color>(((GlComplexPolygon *) entity)->getFillColor()) << QVariant::fromValue<Color>(((GlComplexPolygon *) entity)->getOutlineColor());
  }

  void setProperty(const QString &name, const QVariant &value) {
    if(name=="fillColor")
      ((GlComplexPolygon *) entity)->setFillColor(value.value<Color>());
    else if(name=="outlineColor")
      ((GlComplexPolygon *) entity)->setOutlineColor(value.value<Color>());
  }
};

class GoogleMapsInteractorGetInformation  : public NodeLinkDiagramComponentInteractor {

public:
  PLUGININFORMATION("GoogleMapsInteractorGetInformation", "Tulip Team", "06/2012", "Google Map Get Information Interactor", "1.0", "Information")
  /**
   * Default constructor
   */
  GoogleMapsInteractorGetInformation(const tlp::PluginContext*):NodeLinkDiagramComponentInteractor(":/tulip/gui/icons/i_select.png","Get information on nodes/edges") {
    setConfigurationWidgetText(QString("<h3>Get information interactor</h3>")+
                               "<b>Mouse left</b> click on an element to display its properties");
    setPriority(StandardInteractorPriority::GetInformation);
  }

  /**
   * Construct chain of responsibility
   */
  void construct() {
    push_back(new GoogleMapViewNavigator);
    push_back(new GoogleMapsShowElementInfo);
  }

  bool isCompatible(const string &viewName) const {
    return (viewName==ViewName::GoogleMapsViewName);
  }

};

PLUGIN(GoogleMapsInteractorGetInformation)

GoogleMapsShowElementInfo::GoogleMapsShowElementInfo(): _editor(NULL) {
  Ui::ElementInformationsWidget* ui = new Ui::ElementInformationsWidget;
  _informationsWidget=new QWidget();
  _informationsWidget->installEventFilter(this);
  ui->setupUi(_informationsWidget);
  tableView()->setItemDelegate(new TulipItemDelegate);
  _informationsWidgetItem=new QGraphicsProxyWidget();
  _informationsWidgetItem->setWidget(_informationsWidget);
  _informationsWidgetItem->setVisible(false);
}

void GoogleMapsShowElementInfo::clear() {
  dynamic_cast<GoogleMapsView*>(view())->getGoogleMapGraphicsView()->getGlMainWidget()->setCursor(QCursor());
  _informationsWidgetItem->setVisible(false);
}

QTableView* GoogleMapsShowElementInfo::tableView() const {
  return _informationsWidget->findChild<QTableView*>();
}

bool GoogleMapsShowElementInfo::eventFilter(QObject *widget, QEvent* e) {
  if (widget == _informationsWidget && (e->type() == QEvent::Wheel || e->type() == QEvent::MouseButtonPress))
    return true;

  if(_informationsWidget->isVisible() && e->type()==QEvent::Wheel) {
    _informationsWidgetItem->setVisible(false);
    return false;
  }

  QMouseEvent * qMouseEv = dynamic_cast<QMouseEvent *>(e);

  if(qMouseEv != NULL) {
    GoogleMapsView *googleMapsView=dynamic_cast<GoogleMapsView*>(view());
    SelectedEntity selectedEntity;

    if(e->type() == QEvent::MouseMove) {
      if (pick(qMouseEv->x(), qMouseEv->y(),selectedEntity)) {
        googleMapsView->getGoogleMapGraphicsView()->getGlMainWidget()->setCursor(Qt::WhatsThisCursor);
      }
      else {
        googleMapsView->getGoogleMapGraphicsView()->getGlMainWidget()->setCursor(QCursor());
      }

      return false;
    }
    else if (e->type() == QEvent::MouseButtonPress && qMouseEv->button() == Qt::LeftButton) {
      if(_informationsWidgetItem->isVisible()) {
        // Hide widget if we click outside it
        _informationsWidgetItem->setVisible(false);
      }

      if(!_informationsWidgetItem->isVisible()) {

        // Show widget if we click on node or edge
        if (pick(qMouseEv->x(), qMouseEv->y(),selectedEntity)) {
          if(selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED ||
              selectedEntity.getEntityType() == SelectedEntity::EDGE_SELECTED) {
            _informationsWidgetItem->setVisible(true);

            QLabel* title = _informationsWidget->findChild<QLabel*>();

            if(selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED) {
              title->setText(trUtf8("Node"));
              tableView()->setModel(new GraphNodeElementModel(_view->graph(),selectedEntity.getComplexEntityId(),_informationsWidget));
            }
            else {
              title->setText(trUtf8("Edge"));
              tableView()->setModel(new GraphEdgeElementModel(_view->graph(),selectedEntity.getComplexEntityId(),_informationsWidget));
            }

            title->setText(title->text() + " #" + QString::number(selectedEntity.getComplexEntityId()));

            QPoint position=qMouseEv->pos();

            if(position.x()+_informationsWidgetItem->rect().width()>_view->graphicsView()->sceneRect().width())
              position.setX(qMouseEv->pos().x()-_informationsWidgetItem->rect().width());

            if(position.y()+_informationsWidgetItem->rect().height()>_view->graphicsView()->sceneRect().height())
              position.setY(qMouseEv->pos().y()-_informationsWidgetItem->rect().height());

            _informationsWidgetItem->setPos(position);
            QPropertyAnimation *animation = new QPropertyAnimation(_informationsWidgetItem, "opacity");
            animation->setDuration(100);
            animation->setStartValue(0.);
            animation->setEndValue(1.);
            animation->start();

            return true;
          }
          else if(selectedEntity.getEntityType() == SelectedEntity::SIMPLE_ENTITY_SELECTED) {

            GlComplexPolygon *polygon=dynamic_cast<GlComplexPolygon*>(selectedEntity.getSimpleEntity());

            if(!polygon)
              return false;

            _informationsWidgetItem->setVisible(true);
            QLabel* title = _informationsWidget->findChild<QLabel*>();
            title->setText(selectedEntity.getSimpleEntity()->getParent()->findKey(selectedEntity.getSimpleEntity()).c_str());


            delete _editor;

            _editor = new GlComplexPolygonItemEditor(polygon);

            tableView()->setModel(new GlSimpleEntityItemModel(_editor, _informationsWidget));
            int size = title->height()+_informationsWidget->layout()->spacing()+tableView()->rowHeight(0)+tableView()->rowHeight(1)+10;
            _informationsWidget->setMaximumHeight(size);

            QPoint position=qMouseEv->pos();

            if(position.x()+_informationsWidgetItem->rect().width()>_view->graphicsView()->sceneRect().width())
              position.setX(qMouseEv->pos().x()-_informationsWidgetItem->rect().width());

            if(position.y()+_informationsWidgetItem->rect().height()>_view->graphicsView()->sceneRect().height())
              position.setY(qMouseEv->pos().y()-_informationsWidgetItem->rect().height());

            _informationsWidgetItem->setPos(position);
            QPropertyAnimation *animation = new QPropertyAnimation(_informationsWidgetItem, "opacity");
            animation->setDuration(100);
            animation->setStartValue(0.);
            animation->setEndValue(1.);
            animation->start();
          }
          else {
            return false;
          }
        }
      }
    }
  }

  return false;
}

bool GoogleMapsShowElementInfo::pick(int x, int y, SelectedEntity &selectedEntity) {
  GoogleMapsView *googleMapsView=dynamic_cast<GoogleMapsView*>(view());

  if(googleMapsView->getGoogleMapGraphicsView()->getGlMainWidget()->pickNodesEdges(x,y,selectedEntity))
    return true;

  vector<SelectedEntity> selectedEntities;

  if(googleMapsView->getGoogleMapGraphicsView()->getGlMainWidget()->pickGlEntities(x,y,selectedEntities)) {
    selectedEntity=selectedEntities[0];
    return true;
  }

  return false;
}

void GoogleMapsShowElementInfo::viewChanged(View * view) {
  if (view == NULL) {
    _view = NULL;
    return;
  }

  GoogleMapsView *googleMapsView=dynamic_cast<GoogleMapsView*>(view);
  _view=googleMapsView;
  connect(_view,SIGNAL(graphSet(tlp::Graph*)),_informationsWidgetItem,SLOT(close()));
  _view->getGoogleMapGraphicsView()->scene()->addItem(_informationsWidgetItem);
}
