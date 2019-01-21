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

#include <tulip/TlpQtTools.h>
#include <tulip/Interactor.h>


using namespace tlp;

InteractorConfigWidget::InteractorConfigWidget(QWidget *parent) : QDialog(parent),_ui(new Ui::InteractorConfigWidget), _interactor(nullptr)
{
    _ui->setupUi(this);
}

InteractorConfigWidget::~InteractorConfigWidget(){
    delete _ui;
}

void InteractorConfigWidget::clearWidget(){
    //removes widget from the layout to not delete the object and give back parenthood. It is up to the interactor developer to delete its config widget
        if(_interactor!=nullptr) {
            _interactor->configurationWidget()->hide();
            _ui->scrollArea->takeWidget();
            _interactor=nullptr;
        }
}

void InteractorConfigWidget::setWidget(Interactor *interactor) {
    setWindowTitle(tlpStringToQString(interactor->name()));
    //removes widget from the layout to not delete the object and give back parenthood. It is up to the interactor developer to delete its config widget
    if(_interactor!=nullptr) {
        if(_interactor->configurationWidget()!=interactor->configurationWidget()) {
            _ui->scrollArea->takeWidget();
            _interactor->configurationWidget()->hide();
        }
    }

    _ui->scrollArea->setWidget(interactor->configurationWidget());
    _interactor = interactor;
}
