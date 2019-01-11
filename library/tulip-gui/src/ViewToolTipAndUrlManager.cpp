/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <QActionGroup>
#include <QGraphicsView>
#include <QHelpEvent>
#include <QToolTip>
#include <QString>
#include <QKeyEvent>
#include <QUrl>
#include <QDesktopServices>

#include <tulip/TlpTools.h>
#include <tulip/TlpQtTools.h>
#include <tulip/Graph.h>
#include <tulip/GraphModel.h>
#include <tulip/StringProperty.h>

using namespace tlp;
using namespace std;

ViewToolTipAndUrlManager::ViewToolTipAndUrlManager(tlp::View *view, tlp::GlMainWidget *widget)
    : _view(view), _glMainWidget(widget), _tooltips(false) {}

void ViewToolTipAndUrlManager::setState(const tlp::DataSet &data) {
  data.get("Tooltips", _tooltips);
  data.get("Url property", _urlPropName);
}

void ViewToolTipAndUrlManager::state(DataSet &data) const {
  data.set("Tooltips", _tooltips);
  data.set("Url property", _urlPropName);
}

void ViewToolTipAndUrlManager::fillContextMenu(QMenu *menu, node n) {
  if (_urlPropName.empty())
    return;

  Graph *graph = _view->graph();

  _contextMenuUrl =
      dynamic_cast<StringProperty *>(graph->getProperty(_urlPropName))->getNodeValue(n);

  if (_contextMenuUrl.empty())
    return;

  menu->addSeparator();
  QAction *action = menu->addAction(QString("Open ").append(tlpStringToQString(_contextMenuUrl)),
                                    this, SLOT(openUrl()));
  action->setToolTip(action->text().append(" in the default browser"));
}

void ViewToolTipAndUrlManager::fillContextMenu(QMenu *menu, edge e) {
  if (_urlPropName.empty())
    return;

  Graph *graph = _view->graph();

  _contextMenuUrl =
      dynamic_cast<StringProperty *>(graph->getProperty(_urlPropName))->getEdgeValue(e);
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
  Graph *graph = _view->graph();

  QAction *action = menu->addAction("Tooltips");
  action->setToolTip(QString("When moving the mouse pointer, a tooltip is displayed with some "
                             "information about the graph element located under the pointer"));
  action->setCheckable(true);
  action->setChecked(_tooltips);
  connect(action, SIGNAL(triggered(bool)), this, SLOT(displayToolTips(bool)));

  // add submenu to manage the url property choice
  QMenu *urlPropMenu;
  if (graph->existProperty(_urlPropName))
    urlPropMenu = menu->addMenu(
        QString("Url property").append(" (").append(tlpStringToQString(_urlPropName)).append(")"));
  else {
    urlPropMenu = menu->addMenu("Url property");
    _urlPropName.clear();
  }
  urlPropMenu->setToolTip(
      QString("Choose the property giving the web page associated with a graph element"));

  QActionGroup *urlPropGroup = new QActionGroup(urlPropMenu);
  urlPropGroup->setExclusive(true);
  connect(urlPropMenu, SIGNAL(triggered(QAction *)), this, SLOT(setUrlProp(QAction *)));
  action = urlPropMenu->addAction(" None ");
  action->setCheckable(true);
  urlPropGroup->addAction(action);
  if (_urlPropName.empty())
    action->setChecked(true);
  action->setToolTip(QString("The graph elements have no associated web page"));
  // get all existing StringProperty
  std::set<std::string> props;
  for (auto inheritedProp : graph->getInheritedObjectProperties()) {
    StringProperty *prop = dynamic_cast<StringProperty *>(inheritedProp);

    if (prop != nullptr)
      props.insert(prop->getName());
  }

  for (auto localProp : graph->getLocalObjectProperties()) {
    StringProperty *prop = dynamic_cast<StringProperty *>(localProp);
    if (prop != nullptr)
      props.insert(prop->getName());
  }

  // insert the StringProperty found
  for (auto propName : props) {
    // among the view... properties only viewLabel is allowed
    if (propName.find("view") != 0 || propName == "viewLabel") {
      action = urlPropMenu->addAction(tlpStringToQString(propName));
      action->setToolTip(
          QString("The url of the web page associated with a graph element is given by the \"")
              .append(tlpStringToQString(propName))
              .append(QString("\" property value")));
      urlPropGroup->addAction(action);
      action->setCheckable(true);
      if (_urlPropName == propName)
        action->setChecked(true);
    }
  }
}

void ViewToolTipAndUrlManager::displayToolTips(bool display) {
  if ((_tooltips = display))
    _view->graphicsView()->viewport()->installEventFilter(this);
  else
    _view->graphicsView()->viewport()->removeEventFilter(this);
}

void ViewToolTipAndUrlManager::setUrlProp(QAction *action) {
  _urlPropName = QStringToTlpString(action->text());
  if (!_view->graph()->existProperty(_urlPropName))
    _urlPropName.clear();
}

bool ViewToolTipAndUrlManager::eventFilter(QObject *, QEvent *event) {
  Graph *graph = _view->graph();

  if (graph == nullptr)
    return false;

  // clear url if tooltip is no longer visible
  if (!_url.empty() && !QToolTip::isVisible())
    _url.clear();

  // get the property holding the urls associated to graph elements
  StringProperty *urlProp = _urlPropName.empty()
                                ? nullptr
                                : dynamic_cast<StringProperty *>(graph->getProperty(_urlPropName));

  if (event->type() == QEvent::ToolTip && (_tooltips == true || urlProp != nullptr)) {
    SelectedEntity type;
    QHelpEvent *he = static_cast<QHelpEvent *>(event);

    if (_glMainWidget->pickNodesEdges(he->x(), he->y(), type)) {
      QString ttip;
      node tmpNode = type.getNode();

      if (tmpNode.isValid()) {
        if (urlProp)
          _url = urlProp->getNodeValue(tmpNode);
        if (_tooltips)
          ttip = NodesGraphModel::getNodeTooltip(graph, tmpNode);
      } else {
        edge tmpEdge = type.getEdge();

        if (tmpEdge.isValid()) {
          if (urlProp)
            _url = urlProp->getEdgeValue(tmpEdge);
          if (_tooltips)
            ttip = EdgesGraphModel::getEdgeTooltip(graph, tmpEdge);
        }
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
        ttip = QString("<p style='white-space:pre'><font size=\"-1\">")
                   .append(ttip)
                   .append(QString("</font></p>"));
        QToolTip::showText(he->globalPos(), ttip, _glMainWidget);
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
