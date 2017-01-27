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

#include "GeographicViewShowElementInfo.h"

#include "ui_ElementInformationWidget.h"
#include "GeographicViewInteractors.h"
#include "../../utils/StandardInteractorPriority.h"
#include "../../utils/PluginNames.h"

#include <tulip/MouseInteractors.h>
#include <tulip/NodeLinkDiagramComponentInteractor.h>
#include <tulip/TulipItemDelegate.h>
#include <tulip/GraphElementModel.h>
#include <tulip/GlEntityItemModel.h>
#include <tulip/GlConcavePolygon.h>
#include <tulip/MouseShowElementInfo.h>
#include <tulip/GlComposite.h>

#include <QPropertyAnimation>
#include <QLayout>
#include <QStringList>
#include <QVariantList>

using namespace std;
using namespace tlp;

// this class is needed to allow interactive settings
// of some GlConcavePolygon rendering properties
class tlp::GlConcavePolygonItemEditor : public GlEntityItemEditor {
public:
  GlConcavePolygonItemEditor(GlConcavePolygon *poly) : GlEntityItemEditor(poly) {
  }

  // redefined inherited methods from GlEntityItemEditor
  QStringList propertiesNames() const {
    return QStringList() << "fillColor"
                         << "outlineColor";
  }

  QVariantList propertiesQVariant() const {
    return QVariantList() << QVariant::fromValue<Color>(((GlConcavePolygon *)entity)->getFillColor())
                          << QVariant::fromValue<Color>(((GlConcavePolygon *)entity)->getOutlineColor());
  }

  void setProperty(const QString &name, const QVariant &value) {
    if (name == "fillColor")
      ((GlConcavePolygon *)entity)->setFillColor(value.value<Color>());
    else if (name == "outlineColor")
      ((GlConcavePolygon *)entity)->setOutlineColor(value.value<Color>());
  }
};

class GeographicViewInteractorGetInformation : public NodeLinkDiagramComponentInteractor {

public:
  PLUGININFORMATION("GeographicViewInteractorGetInformation", "Tulip Team", "06/2012", "Geographic View Get Information Interactor", "1.0",
                    "Information")
  /**
   * Default constructor
   */
  GeographicViewInteractorGetInformation(const tlp::PluginContext *)
      : NodeLinkDiagramComponentInteractor(MouseShowElementInfo::getInteractorIcon(), "Get information on nodes/edges") {
    setConfigurationWidgetText(QString("<h3>Get information interactor</h3>") + "<b>Mouse left</b> click on an element to display its properties");
    setPriority(StandardInteractorPriority::GetInformation);
  }

  /**
   * Construct chain of responsibility
   */
  void construct() {
    push_back(new GeographicViewNavigator);
    push_back(new GeographicViewShowElementInfo);
  }

  bool isCompatible(const string &viewName) const {
    return (viewName == ViewName::GeographicViewName);
  }
};

PLUGIN(GeographicViewInteractorGetInformation)

GeographicViewShowElementInfo::GeographicViewShowElementInfo() : _editor(NULL) {
  Ui::ElementInformationWidget ui;
  _informationWidget = new QWidget();
  _informationWidget->installEventFilter(this);
  ui.setupUi(_informationWidget);
  tableView()->setItemDelegate(new TulipItemDelegate(tableView()));
  _informationWidgetItem = new QGraphicsProxyWidget();
  _informationWidgetItem->setWidget(_informationWidget);
  _informationWidgetItem->setVisible(false);
}

void GeographicViewShowElementInfo::clear() {
  dynamic_cast<GeographicView *>(view())->getGeographicViewGraphicsView()->getGlMainWidget()->setCursor(QCursor());
  _informationWidgetItem->setVisible(false);
}

QTableView *GeographicViewShowElementInfo::tableView() const {
  return _informationWidget->findChild<QTableView *>();
}

bool GeographicViewShowElementInfo::eventFilter(QObject *widget, QEvent *e) {
  if (widget == _informationWidget && (e->type() == QEvent::Wheel || e->type() == QEvent::MouseButtonPress))
    return true;

  if (_informationWidget->isVisible() && e->type() == QEvent::Wheel) {
    _informationWidgetItem->setVisible(false);
    return false;
  }

  QMouseEvent *qMouseEv = dynamic_cast<QMouseEvent *>(e);

  if (qMouseEv != nullptr) {
    GeographicView *geoView = dynamic_cast<GeographicView *>(view());
    SelectedEntity selectedEntity;

    if (e->type() == QEvent::MouseMove) {
      if (pick(qMouseEv->x(), qMouseEv->y(), selectedEntity)) {
        geoView->getGeographicViewGraphicsView()->getGlMainWidget()->setCursor(Qt::WhatsThisCursor);
      } else {
        geoView->getGeographicViewGraphicsView()->getGlMainWidget()->setCursor(QCursor());
      }

      return false;
    } else if (e->type() == QEvent::MouseButtonPress && qMouseEv->button() == Qt::LeftButton) {
      if (_informationWidgetItem->isVisible()) {
        // Hide widget if we click outside it
        _informationWidgetItem->setVisible(false);
      }

      if (!_informationWidgetItem->isVisible()) {

        // Show widget if we click on node or edge
        if (pick(qMouseEv->x(), qMouseEv->y(), selectedEntity)) {
          if (selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED || selectedEntity.getEntityType() == SelectedEntity::EDGE_SELECTED) {
            _informationWidgetItem->setVisible(true);

            QLabel *title = _informationWidget->findChild<QLabel *>();

            if (selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED) {
              title->setText(trUtf8("Node"));
              tableView()->setModel(new GraphNodeElementModel(_view->graph(), selectedEntity.getNode().id, _informationWidget));
            } else {
              title->setText(trUtf8("Edge"));
              tableView()->setModel(new GraphEdgeElementModel(_view->graph(), selectedEntity.getEdge().id, _informationWidget));
            }

            QPoint position = qMouseEv->pos();

            if (position.x() + _informationWidgetItem->rect().width() > _view->graphicsView()->sceneRect().width() - 5)
              position.setX(_view->graphicsView()->sceneRect().width() - _informationWidgetItem->rect().width() - 5);

            if (position.y() + _informationWidgetItem->rect().height() > _view->graphicsView()->sceneRect().height())
              position.setY(_view->graphicsView()->sceneRect().height() - _informationWidgetItem->rect().height() - 5);

            _informationWidgetItem->setPos(position);
            QPropertyAnimation *animation = new QPropertyAnimation(_informationWidgetItem, "opacity");
            animation->setDuration(100);
            animation->setStartValue(0.);
            animation->setEndValue(1.);
            animation->start();

            return true;
          } else if (selectedEntity.getEntityType() == SelectedEntity::ENTITY_SELECTED) {

            GlConcavePolygon *polygon = dynamic_cast<GlConcavePolygon *>(selectedEntity.getGlEntity());

            if (!polygon)
              return false;

            _informationWidgetItem->setVisible(true);
            QLabel *title = _informationWidget->findChild<QLabel *>();
            title->setText(selectedEntity.getGlEntity()->getParent()->findKey(selectedEntity.getGlEntity()).c_str());

            delete _editor;

            _editor = new GlConcavePolygonItemEditor(polygon);

            tableView()->setModel(new GlEntityItemModel(_editor, _informationWidget));
            int size = title->height() + _informationWidget->layout()->spacing() + tableView()->rowHeight(0) + tableView()->rowHeight(1) + 10;
            _informationWidget->setMaximumHeight(size);

            QPoint position = qMouseEv->pos();

            if (position.x() + _informationWidgetItem->rect().width() > _view->graphicsView()->sceneRect().width())
              position.setX(qMouseEv->pos().x() - _informationWidgetItem->rect().width());

            if (position.y() + _informationWidgetItem->rect().height() > _view->graphicsView()->sceneRect().height())
              position.setY(qMouseEv->pos().y() - _informationWidgetItem->rect().height());

            _informationWidgetItem->setPos(position);
            QPropertyAnimation *animation = new QPropertyAnimation(_informationWidgetItem, "opacity");
            animation->setDuration(100);
            animation->setStartValue(0.);
            animation->setEndValue(1.);
            animation->start();
          } else {
            return false;
          }
        }
      }
    }
  }

  return false;
}

bool GeographicViewShowElementInfo::pick(int x, int y, SelectedEntity &selectedEntity) {
  GeographicView *geoView = dynamic_cast<GeographicView *>(view());

  if (geoView->getGeographicViewGraphicsView()->getGlMainWidget()->pickNodesEdges(x, y, selectedEntity))
    return true;

  vector<SelectedEntity> selectedEntities;

  if (geoView->getGeographicViewGraphicsView()->getGlMainWidget()->pickGlEntities(x, y, selectedEntities)) {
    selectedEntity = selectedEntities[0];
    return true;
  }

  return false;
}

void GeographicViewShowElementInfo::viewChanged(View *view) {
  if (view == nullptr) {
    _view = nullptr;
    return;
  }

  GeographicView *geoView = dynamic_cast<GeographicView *>(view);
  _view = geoView;
  connect(_view, SIGNAL(graphSet(tlp::Graph *)), _informationWidgetItem, SLOT(close()));
  _view->getGeographicViewGraphicsView()->scene()->addItem(_informationWidgetItem);
}
