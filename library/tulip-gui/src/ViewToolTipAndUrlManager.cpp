/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <tulip/ViewToolTipAndUrlManager.h>
#include <QGraphicsView>
#include <QToolTip>
#include <QKeyEvent>
#include <QUrl>
#include <QDesktopServices>
#include <QBuffer>
#include <QImage>

#include <tulip/TlpQtTools.h>
#include <tulip/GraphModel.h>
#include <tulip/StringProperty.h>
#include "ui_ManageGraphEltTooltipDialog.h"

using namespace tlp;
using namespace std;

#define TOOLTIPS_ENABLED "display tooltips"
#define IMAGE_TOOLTIP_PROPERTY "img tooltip property"
#define IMAGE_TOOLTIP_MAX_DIM "img tooltip w/h max"
#define URL_TOOLTIP_PROPERTY "url tooltip property"

ViewToolTipAndUrlManager::ViewToolTipAndUrlManager(tlp::View *view, QWidget *widget)
    : _view(view), _widget(widget) {
  connect(_view, SIGNAL(graphSet(tlp::Graph *)), this, SLOT(viewGraphSet(tlp::Graph *)));
}

void ViewToolTipAndUrlManager::viewGraphSet(Graph *graph) {
  if (graph) {
    bool tooltips = false;
    graph->getAttribute(TOOLTIPS_ENABLED, tooltips);
    if (tooltips)
      _view->graphicsView()->viewport()->installEventFilter(this);
    else
      _view->graphicsView()->viewport()->removeEventFilter(this);
  } else
    _view->graphicsView()->viewport()->removeEventFilter(this);
}

void ViewToolTipAndUrlManager::setState(const tlp::DataSet &) {}

void ViewToolTipAndUrlManager::state(DataSet &) const {}

StringProperty *ViewToolTipAndUrlManager::getAttributeProperty(const std::string &attribute) {
  StringProperty *prop = nullptr;
  std::string propName;
  Graph *graph = _view->graph();
  if (graph->getAttribute(attribute, propName)) {
    if (!(prop = dynamic_cast<StringProperty *>(graph->getProperty(propName))))
      // force attribute removal
      graph->removeAttribute(attribute);
  }
  return prop;
}

void ViewToolTipAndUrlManager::fillContextMenu(QMenu *menu, node n) {
  StringProperty *urlProp = getAttributeProperty(URL_TOOLTIP_PROPERTY);
  if (!urlProp)
    return;

  _contextMenuUrl = urlProp->getNodeValue(n);
  if (_contextMenuUrl.empty())
    return;

  menu->addSeparator();
  QAction *action = menu->addAction(QString("Open ").append(tlpStringToQString(_contextMenuUrl)),
                                    this, SLOT(openUrl()));
  action->setToolTip(action->text().append(" in the default browser"));
}

void ViewToolTipAndUrlManager::fillContextMenu(QMenu *menu, edge e) {
  StringProperty *urlProp = getAttributeProperty(URL_TOOLTIP_PROPERTY);
  if (!urlProp)
    return;

  _contextMenuUrl = urlProp->getEdgeValue(e);
  if (_contextMenuUrl.empty())
    return;

  menu->addSeparator();

  QAction *action = menu->addAction(QString("Open ").append(tlpStringToQString(_contextMenuUrl)),
                                    this, SLOT(openUrl()));
  action->setToolTip(action->text().append(" in the default browser"));
}

void ViewToolTipAndUrlManager::openUrl() {
  // open the current url
  QDesktopServices::openUrl(QUrl(tlpStringToQString(_contextMenuUrl)));
}

void ViewToolTipAndUrlManager::fillContextMenu(QMenu *menu) {
  QAction *action = menu->addAction("Tooltips display parameters");
  action->setToolTip(
      QString("Enable/disable the display of a tooltip showing "
              "information\nabout the graph element located under the mouse pointer"));
  connect(action, SIGNAL(triggered(bool)), this, SLOT(manageToolTips()));
}

void ViewToolTipAndUrlManager::manageToolTips() {
  Graph *graph = _view->graph();

  // get all existing StringProperty with non default valuated nodes or edges
  std::set<std::string> props;
  for (auto inheritedProp : graph->getInheritedObjectProperties()) {
    StringProperty *prop = dynamic_cast<StringProperty *>(inheritedProp);
    if (prop != nullptr) {
      if (prop->hasNonDefaultValuatedNodes(graph) || prop->hasNonDefaultValuatedEdges(graph))
        props.insert(prop->getName());
    }
  }

  for (auto localProp : graph->getLocalObjectProperties()) {
    StringProperty *prop = dynamic_cast<StringProperty *>(localProp);
    if (prop != nullptr) {
      if (prop->hasNonDefaultValuatedNodes(graph) || prop->hasNonDefaultValuatedEdges(graph))
        props.insert(prop->getName());
    }
  }

  QDialog ttDialog(_view->graphicsView()->viewport());
  Ui::ManageGraphEltTooltipDialog ui;
  ui.setupUi(&ttDialog);
#ifdef __APPLE__
  ui.imgPropCB->setMinimumContentsLength(21);
  ui.urlPropCB->setMinimumContentsLength(21);
#endif

  // configure ui
  bool tooltips = false;
  graph->getAttribute(TOOLTIPS_ENABLED, tooltips);
  ui.tooltipsGroupBox->setChecked(tooltips);
  // configure potential img props
  ui.imgPropCB->addItem(" None ");
  ui.imgPropCB->setCurrentIndex(0);
  std::string imgPropName;
  {
    StringProperty *imgProp = getAttributeProperty(IMAGE_TOOLTIP_PROPERTY);
    if (imgProp)
      imgPropName = imgProp->getName();
  }
  // insert the StringProperty props found
  for (auto propName : props) {
    // among the view... properties only viewLabel and viewTexture are allowed
    if (propName.find("view") != 0 || propName == "viewLabel" || propName == "viewTexture") {
      auto qs = tlpStringToQString(propName);
      ui.imgPropCB->addItem(qs);
      if (imgPropName == propName)
        ui.imgPropCB->setCurrentText(qs);
    }
  }
  // configure max dim
  int imgMaxDim = 200;
  graph->getAttribute(IMAGE_TOOLTIP_MAX_DIM, imgMaxDim);
  ui.imgMaxDimSB->setMaximum(10000);
  ui.imgMaxDimSB->setValue(imgMaxDim);
  // configure potential url props
  ui.urlPropCB->addItem(" None ");
  ui.urlPropCB->setCurrentIndex(0);
  std::string urlPropName;
  {
    StringProperty *urlProp = getAttributeProperty(URL_TOOLTIP_PROPERTY);
    if (urlProp)
      urlPropName = urlProp->getName();
  }
  // insert the StringProperty props found
  for (auto propName : props) {
    // among the view... properties only viewLabel and viewTexture are allowed
    if (propName.find("view") != 0 || propName == "viewLabel") {
      auto qs = tlpStringToQString(propName);
      ui.urlPropCB->addItem(qs);
      if (urlPropName == propName)
        ui.urlPropCB->setCurrentText(qs);
    }
  }

  // show dialog
  ttDialog.setModal(true);
  if (ttDialog.exec()) {
    // configure tooltips parameters
    if ((tooltips = ui.tooltipsGroupBox->isChecked()))
      _view->graphicsView()->viewport()->installEventFilter(this);
    else
      _view->graphicsView()->viewport()->removeEventFilter(this);
    graph->setAttribute(TOOLTIPS_ENABLED, tooltips);
    imgPropName = QStringToTlpString(ui.imgPropCB->currentText());
    if (graph->existProperty(imgPropName))
      graph->setAttribute(IMAGE_TOOLTIP_PROPERTY, imgPropName);
    else
      graph->removeAttribute(IMAGE_TOOLTIP_PROPERTY);
    imgMaxDim = ui.imgMaxDimSB->value();
    graph->setAttribute(IMAGE_TOOLTIP_MAX_DIM, imgMaxDim);
    urlPropName = QStringToTlpString(ui.urlPropCB->currentText());
    if (graph->existProperty(urlPropName))
      graph->setAttribute(URL_TOOLTIP_PROPERTY, urlPropName);
    else
      graph->removeAttribute(URL_TOOLTIP_PROPERTY);
  }
}

bool ViewToolTipAndUrlManager::eventFilter(QObject *, QEvent *event) {
  // clear url if tooltip is no longer visible
  if (!_url.empty() && !QToolTip::isVisible())
    _url.clear();

  Graph *graph = _view->graph();

  if (graph == nullptr)
    return false;

  bool tooltips = false;
  graph->getAttribute(TOOLTIPS_ENABLED, tooltips);
  // get the property holding the img associated to graph elements
  StringProperty *imgProp = getAttributeProperty(IMAGE_TOOLTIP_PROPERTY);

  // get the property holding the urls associated to graph elements
  StringProperty *urlProp = getAttributeProperty(URL_TOOLTIP_PROPERTY);

  if (event->type() == QEvent::ToolTip && (tooltips == true || urlProp != nullptr)) {
    QHelpEvent *he = static_cast<QHelpEvent *>(event);

    node tmpNode;
    edge tmpEdge;
    std::string img;
    if (_view->getNodeOrEdgeAtViewportPos(he->pos().x(), he->pos().y(), tmpNode, tmpEdge)) {
      QString ttip;

      if (tmpNode.isValid()) {
        if (urlProp)
          _url = urlProp->getNodeValue(tmpNode);
        if (tooltips)
          ttip = NodesGraphModel::getNodeTooltip(graph, tmpNode);
        if (imgProp)
          img = imgProp->getNodeValue(tmpNode);
      } else if (tmpEdge.isValid()) {
        if (urlProp)
          _url = urlProp->getEdgeValue(tmpEdge);
        if (tooltips)
          ttip = EdgesGraphModel::getEdgeTooltip(graph, tmpEdge);
        if (imgProp)
          img = imgProp->getEdgeValue(tmpEdge);
      }
      // only http urls are valid
      if (!_url.empty() && _url.find("http://") != 0 && _url.find("https://"))
        _url.insert(0, "http://");
      if (!_url.empty()) {
        // warn user that there is a web page associated to the current
        // graph element which can be opened with a space key press
        ttip.append(QString(ttip.isEmpty() ? "" : "\n\n"))
            .append(QString("hit &lt;SPACE&gt; bar to open <b>"))
            .append(tlpStringToQString(_url))
            .append("</b>");
        // give the focus to the parent widget
        // to ensure to catch the space key press
        _view->graphicsView()->viewport()->parentWidget()->setFocus();
      }
      if (!ttip.isEmpty()) {
        // preserve current formatting
        ttip = QString("<p style='white-space:pre'><font size='-1'>")
                   .append(ttip)
                   .append(QString("</font></p>"));
        // show image if needed
        if (!img.empty()) {
          // read image
          QImage qimg(img.c_str());
          if (!qimg.isNull()) {
            // get graph attribute value for IMAGE_TOOLTIP_MAX_DIM
            int dimMax = 200; // default value
            graph->getAttribute(IMAGE_TOOLTIP_MAX_DIM, dimMax);
            // scale qimg to fit in a dimMax max square
            // dimMax = 0 means no constraint
            if (dimMax) {
              auto dim = qimg.width();
              if (dim > dimMax)
                qimg = qimg.scaledToWidth(dimMax, Qt::SmoothTransformation);
              dim = qimg.height();
              if (dim > dimMax)
                qimg = qimg.scaledToHeight(dimMax, Qt::SmoothTransformation);
            }
            QByteArray bytes;
            QBuffer buf(&bytes);
            qimg.save(&buf, "png", 100);
            ttip = QString("<img src='data:image/png;base64, %0'/>")
                       .arg(QString(bytes.toBase64()))
                       .append(ttip);
          }
        }
        QToolTip::showText(he->globalPos(), ttip, _widget);
        return true;
      }
    } else {
      // be sure to hide the tooltip if the mouse cursor
      // is not under a node or an edge
      QToolTip::hideText();
      event->ignore();
    }
  }

  // if there is a current url to open, check for a space key press
  if (!_url.empty() && (event->type() == QEvent::KeyPress) &&
      (static_cast<QKeyEvent *>(event)->key() == Qt::Key_Space)) {
    // open the current url
    QDesktopServices::openUrl(QUrl(tlpStringToQString(_url)));
    _url.clear();
    return true;
  }

  return false;
}
