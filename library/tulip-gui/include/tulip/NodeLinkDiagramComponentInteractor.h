/*
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
#ifndef NODELINKDIAGRAMCOMPONENTINTERACTOR_H
#define NODELINKDIAGRAMCOMPONENTINTERACTOR_H

#include <tulip/GLInteractor.h>

class QLabel;

namespace tlp {

/** \brief Tulip interactor abstract class for NodeLinkDiagramComponent
 *
 */
class TLP_QT_SCOPE NodeLinkDiagramComponentInteractor : public GLInteractorComposite {
  QLabel *_label;
  unsigned int _priority;

public:
  NodeLinkDiagramComponentInteractor(const QString &iconPath, const QString &text,
                                     unsigned int priority = 0);

  ~NodeLinkDiagramComponentInteractor() override;

  void setConfigurationWidgetText(const QString &text);

  QWidget *configurationWidget() const override;

  unsigned int priority() const override;

  void setPriority(const unsigned int p);
};
}

#endif // NODELINKDIAGRAMCOMPONENTINTERACTOR_H
