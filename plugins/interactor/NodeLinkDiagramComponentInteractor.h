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
#ifndef NODELINKDIAGRAMCOMPONENTINTERACTOR_H
#define NODELINKDIAGRAMCOMPONENTINTERACTOR_H

#include <QtGui/QLabel>

#include <tulip/GLInteractor.h>
#include <tulip/NodeLinkDiagramComponent.h>

namespace tlp {

/** \brief Tulip interactor abstract class for NodeLinkDiagramComponent
 *
 */
class NodeLinkDiagramComponentInteractor  : public GLInteractorComposite {
  QLabel* _label;
  unsigned int _priority;

public :

  NodeLinkDiagramComponentInteractor(const QString &iconPath, const QString &text):GLInteractorComposite(QIcon(iconPath),text), _label(NULL), _priority(0) {
  }

  virtual ~NodeLinkDiagramComponentInteractor() {
    _label->deleteLater();
  }

  void setConfigurationWidgetText(const QString& text) {
    _label = new QLabel(text);
    _label->setWordWrap(true);
    _label->setAlignment(Qt::AlignTop);
    _label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  }

  virtual bool isCompatible(const std::string &viewName) const {
    return (viewName==NodeLinkDiagramComponent::viewName);
  }

  virtual QWidget* configurationWidget() const {
    return _label;
  }

  unsigned int priority() const {
    return _priority;
  }

  void setPriority(unsigned int p) {
    _priority = p;
  }

};
}

#endif // NODELINKDIAGRAMCOMPONENTINTERACTOR_H
