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

#include <tulip/InteractorConfigWidget.h>
#include "ui_InteractorConfigWidget.h"

#include <QScrollArea>
#include <QLabel>
#include <QShowEvent>

#include <tulip/TlpQtTools.h>
#include <tulip/Interactor.h>

using namespace tlp;

InteractorConfigWidget::InteractorConfigWidget(QWidget *parent)
    : QDialog(parent), _ui(new Ui::InteractorConfigWidget), _interactor(nullptr) {
  _ui->setupUi(this);
  resize(500, 600);
}

InteractorConfigWidget::~InteractorConfigWidget() {
  delete _ui;
}

void InteractorConfigWidget::clearWidgets() {
  // removes widget from the layout to not delete the object and give back parenthood. It is up to
  // the interactor developer to delete its config widget
  if (_interactor != nullptr) {
    // take all widgets
    QWidget *oldConfig(_interactor->configurationWidget());
    // if old config is present and is only a QLabel => Documentation tab, else Options tab
    QWidget *DocWidget = nullptr;
    QWidget *OptionsWidget = nullptr;
    bool isOldDocConfigWidget(false), isOldOptionsConfigWidget(false);
    if (oldConfig != nullptr) {
      if (dynamic_cast<QLabel *>(oldConfig) != nullptr) {
        DocWidget = oldConfig;
        isOldDocConfigWidget = true;
      } else {
        OptionsWidget = oldConfig;
        isOldOptionsConfigWidget = true;
      }
    } else {
      DocWidget = _interactor->configurationDocWidget();
      OptionsWidget = _interactor->configurationOptionsWidget();
    }
    if ((isOldDocConfigWidget && _interactor->configurationWidget() != DocWidget) ||
        (_interactor->configurationDocWidget() != DocWidget)) {
      _ui->interactorConfigWidgetDoc->widget()->hide();
      _ui->interactorConfigWidgetDoc->takeWidget();
    }
    if ((isOldOptionsConfigWidget && _interactor->configurationWidget() != OptionsWidget) ||
        (_interactor->configurationOptionsWidget() != OptionsWidget)) {
      _ui->interactorConfigWidgetOptions->widget()->hide();
      _ui->interactorConfigWidgetOptions->takeWidget();
    }
    _interactor = nullptr;
  }
}

bool InteractorConfigWidget::setWidgets(Interactor *interactor) {
  // take all widgets
  QWidget *oldConfig(interactor->configurationWidget());
  // if old config is present and is only a QLabel => Documentation tab, else Options tab
  QWidget *DocWidget = nullptr;
  QWidget *OptionsWidget = nullptr;
  bool isOldDocConfigWidget(false), isOldOptionsConfigWidget(false);
  if (oldConfig != nullptr) {
    if (dynamic_cast<QLabel *>(oldConfig) != nullptr) {
      DocWidget = oldConfig;
      isOldDocConfigWidget = true;
    } else {
      OptionsWidget = oldConfig;
      isOldOptionsConfigWidget = true;
    }
  } else {
    DocWidget = interactor->configurationDocWidget();
    OptionsWidget = interactor->configurationOptionsWidget();
  }

  if ((DocWidget == nullptr) && (OptionsWidget == nullptr)) {
    clearWidgets();
    hide();
    return false;
  } else {
    setWindowTitle(tlpStringToQString(interactor->info()));
    // removes widget from the layout to not delete the object and give back parenthood. It is up to
    // the interactor developer to delete its config widget
    if (_interactor != nullptr) {
      if ((isOldDocConfigWidget && _interactor->configurationWidget() != DocWidget) ||
          (_interactor->configurationDocWidget() != DocWidget)) {
        if (_ui->tabWidget->isTabEnabled(0)) {
          _ui->interactorConfigWidgetDoc->widget()->hide();
          _ui->interactorConfigWidgetDoc->takeWidget();
        }
      }
      if ((isOldOptionsConfigWidget && _interactor->configurationWidget() != OptionsWidget) ||
          (_interactor->configurationOptionsWidget() != OptionsWidget)) {
        if (_ui->tabWidget->isTabEnabled(1)) {
          _ui->interactorConfigWidgetOptions->widget()->hide();
          _ui->interactorConfigWidgetOptions->takeWidget();
        }
      }
    }

    if (DocWidget != nullptr) {
      _ui->interactorConfigWidgetDoc->setWidget(DocWidget);
      _ui->tabWidget->setTabEnabled(0, true); // in case it was previously set to false
    } else
      _ui->tabWidget->setTabEnabled(0, false);

    if (OptionsWidget != nullptr) {
      _ui->interactorConfigWidgetOptions->setWidget(OptionsWidget);
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
