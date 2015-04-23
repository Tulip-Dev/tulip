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

#include <tulip/NodeLinkDiagramComponentInteractor.h>
#include <tulip/NodeLinkDiagramComponent.h>

#include <QLabel>
#include <QtGui/QIcon>

using namespace tlp;

NodeLinkDiagramComponentInteractor::NodeLinkDiagramComponentInteractor(const QString &iconPath, const QString &text, unsigned int priority):GLInteractorComposite(QIcon(iconPath),text), _label(NULL), _priority(priority) {
}

NodeLinkDiagramComponentInteractor::~NodeLinkDiagramComponentInteractor() {
  delete _label;
}

void NodeLinkDiagramComponentInteractor::setConfigurationWidgetText(const QString& text) {
  assert(_label == NULL);
  _label = new QLabel(text);
  _label->setWordWrap(true);
  _label->setAlignment(Qt::AlignTop);
  _label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

QWidget* NodeLinkDiagramComponentInteractor::configurationWidget() const {
  return _label;
}

unsigned int NodeLinkDiagramComponentInteractor::priority() const {
  return _priority;
}

void NodeLinkDiagramComponentInteractor::setPriority(const unsigned int p) {
  _priority = p;
}
