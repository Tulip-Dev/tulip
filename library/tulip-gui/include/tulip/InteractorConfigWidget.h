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
///@cond DOXYGEN_HIDDEN

#ifndef INTERACTORCONFIGWIDGET_H
#define INTERACTORCONFIGWIDGET_H

#include <QDialog>
#include <unordered_map>

namespace Ui {
class InteractorConfigWidget;
}

class QHideEvent;
class QShowEvent;

namespace tlp {
class Interactor;

class InteractorConfigWidget : public QDialog {

  Ui::InteractorConfigWidget *_ui;
  Interactor *_interactor;
  // current index registration per interactor
  std::unordered_map<std::string, int> lastIndex;

public:
  explicit InteractorConfigWidget(QWidget *parent = nullptr);
  ~InteractorConfigWidget();
  bool setWidgets(Interactor *interactor);

  void showEvent(QShowEvent *) override;
  void hideEvent(QHideEvent *) override;
};

} // namespace tlp

#endif // INTERACTORCONFIGWIDGET_H
