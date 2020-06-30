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

#include <tulip/Interactor.h>
#include <tulip/NodeLinkDiagramComponentInteractor.h>
#include <tulip/NodeLinkDiagramComponent.h>

#include <QLabel>
#include <QIcon>

using namespace tlp;

NodeLinkDiagramComponentInteractor::NodeLinkDiagramComponentInteractor(const QString &iconPath,
                                                                       const QString &text,
                                                                       unsigned int priority)
    : GLInteractorComposite(QIcon(iconPath), text), _label(new QLabel), _priority(priority) {
  Interactor::setupConfigWidget(_label);
  _label->setWordWrap(true);
  _label->setAlignment(Qt::AlignTop);
  _label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

NodeLinkDiagramComponentInteractor::~NodeLinkDiagramComponentInteractor() {
  delete _label;
}

void NodeLinkDiagramComponentInteractor::setConfigurationWidgetText(const QString &text) {
  _label->setText(text);
}

QLabel *NodeLinkDiagramComponentInteractor::configurationDocWidget() const {
  return _label->text().isEmpty() ? nullptr : _label;
}

unsigned int NodeLinkDiagramComponentInteractor::priority() const {
  return _priority;
}
