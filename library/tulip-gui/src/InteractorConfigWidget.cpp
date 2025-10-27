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

#include <tulip/InteractorConfigWidget.h>
#include "ui_InteractorConfigWidget.h"

#include <QLabel>

#include <tulip/TlpQtTools.h>
#include <tulip/Interactor.h>

using namespace tlp;

InteractorConfigWidget::InteractorConfigWidget(QWidget *parent)
    : QDialog(parent), _ui(new Ui::InteractorConfigWidget), _interactor(nullptr) {
  _ui->setupUi(this);
  resize(500, 600);
}

InteractorConfigWidget::~InteractorConfigWidget() {
  // removes current config widgets from the layout
  // It is up to the interactor developer to delete its config widget
  _ui->interactorConfigWidgetDoc->takeWidget();
  _ui->interactorConfigWidgetOptions->takeWidget();

  delete _ui;
}

bool InteractorConfigWidget::setWidgets(Interactor *interactor) {
  // removes current config widgets from the layout
  // It is up to the interactor developer to delete its config widget
  _ui->interactorConfigWidgetDoc->takeWidget();
  _ui->interactorConfigWidgetOptions->takeWidget();

  QWidget *optionsWidget(interactor->configurationOptionsWidget());
  QLabel *docWidget(interactor->configurationDocWidget());

  if ((docWidget == nullptr) && (optionsWidget == nullptr)) {
    _interactor = nullptr;
    hide();
    return false;
  } else {
    setWindowTitle(tlpStringToQString(interactor->info()));

    if (docWidget != nullptr) {
      _ui->interactorConfigWidgetDoc->setWidget(docWidget);
      // fix display of QCheckBox and QRadioButton children
      tlpFixCBRBs(docWidget);
      _ui->tabWidget->setTabEnabled(0, true); // in case it was previously set to false
      if (optionsWidget != nullptr) {
        auto idx = lastIndex.find(interactor->info());
        if (idx != lastIndex.end())
          // restore current tab index
          _ui->tabWidget->setCurrentIndex(idx->second);
        else
          _ui->tabWidget->setCurrentIndex(0);
      } else
        _ui->tabWidget->setCurrentIndex(0);
    } else {
      _ui->tabWidget->setTabEnabled(0, false);
      _ui->tabWidget->setCurrentIndex(1);
    }

    if (optionsWidget != nullptr) {
      _ui->interactorConfigWidgetOptions->setWidget(optionsWidget);
      // fix display of QCheckBox and QRadioButton children
      tlpFixCBRBs(optionsWidget);
      _ui->tabWidget->setTabEnabled(1, true); // in case it was previously set to false
    } else
      _ui->tabWidget->setTabEnabled(1, false);

    _interactor = interactor;
  }
  return true;
}

void InteractorConfigWidget::showEvent(QShowEvent *ev) {
  QDialog::showEvent(ev);

  if (parentWidget())
    move(parentWidget()->window()->frameGeometry().topLeft() +
         parentWidget()->window()->rect().center() - rect().center());
}

void InteractorConfigWidget::hideEvent(QHideEvent *ev) {
  if (_ui->tabWidget->isTabEnabled(0) && _ui->tabWidget->isTabEnabled(1))
    // register current tab index
    lastIndex[_interactor->info()] = _ui->tabWidget->currentIndex();
  QDialog::hideEvent(ev);
}
