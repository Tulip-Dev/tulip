/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#ifndef Tulip_AUGMENTEDDISPLAYDIALOG_H
#define Tulip_AUGMENTEDDISPLAYDIALOG_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include "AugmentedDisplayDialogData.h"

namespace tlp {

  class Graph;

  class TLP_QT_SCOPE AugmentedDisplayDialog : public QDialog, public Ui::AugmentedDisplayDialogData {

    Q_OBJECT

  public:

    AugmentedDisplayDialog(QWidget* parent,Graph *graph,const std::string &viewType);

    void getRemovedList(std::vector<std::string> &toRemove);

  protected slots:

    void removeClicked();

  private :

    std::vector<std::string> removedList;

  };

}

#endif // Tulip_RENDERINGPARAMETERSDIALOG_H
