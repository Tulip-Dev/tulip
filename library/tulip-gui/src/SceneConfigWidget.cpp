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

#include "tulip/SceneConfigWidget.h"

#include <QtCore/QDebug>
#include <QtGui/QMainWindow>
#include <QtGui/QApplication>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlMainWidget.h>
#include <tulip/Perspective.h>
#include <tulip/TulipSettings.h>
#include "ui_SceneConfigWidget.h"
#include <tulip/GraphPropertiesModel.h>

using namespace tlp;

SceneConfigWidget::SceneConfigWidget(QWidget *parent): QWidget(parent), _ui(new Ui::SceneConfigWidget), _glMainWidget(NULL), _resetting(false) {
  _ui->setupUi(this);

  if (Perspective::instance()->mainWindow() != NULL) {
    _ui->selectionColorButton->setDialogParent(Perspective::instance()->mainWindow());
    _ui->backgroundColorButton->setDialogParent(Perspective::instance()->mainWindow());
  }

  _ui->labelSizesSpanSlider->setHandleMovementMode(QxtSpanSlider::FreeMovement);
  _ui->labelsDisabledLabel->installEventFilter(this);
  _ui->labelsNoOverlapLabel->installEventFilter(this);
  _ui->labelsShowAllLabel->installEventFilter(this);
}

SceneConfigWidget::~SceneConfigWidget() {
  delete _ui;
}

void SceneConfigWidget::setGlMainWidget(tlp::GlMainWidget* glMainWidget) {
  if (_glMainWidget != NULL)
    disconnect(_glMainWidget,SIGNAL(graphChanged()),this,SLOT(resetChanges()));

  _glMainWidget = glMainWidget;

  if (_glMainWidget != NULL)
    connect(_glMainWidget,SIGNAL(graphChanged()),this,SLOT(resetChanges()));

  resetChanges();
}

void SceneConfigWidget::resetChanges() {
  _resetting = true;

  _ui->scrollArea->setEnabled(_glMainWidget != NULL);

  if (_glMainWidget == NULL || _glMainWidget->getScene()->getGlGraphComposite() == NULL || _glMainWidget->getScene()->getGlGraphComposite()->getGraph() == NULL)
    return;

  Graph* graph = _glMainWidget->getScene()->getGlGraphComposite()->getGraph();
  GlGraphRenderingParameters* renderingParameters = _glMainWidget->getScene()->getGlGraphComposite()->getRenderingParametersPointer();

  // NODES
  GraphPropertiesModel<DoubleProperty>* model = new GraphPropertiesModel<DoubleProperty>(trUtf8("Disable ordering"),graph);
  _ui->labelsOrderingCombo->setModel(model);

  if (renderingParameters->getElementOrderingProperty() == NULL)
    _ui->labelsOrderingCombo->setCurrentIndex(0);
  else
    _ui->labelsOrderingCombo->setCurrentIndex(model->rowOf(renderingParameters->getElementOrderingProperty()));

  _ui->labelsFitCheck->setChecked(renderingParameters->isLabelScaled());
  _ui->labelsDensitySlider->setValue(renderingParameters->getLabelsDensity());
  _ui->labelSizesSpanSlider->setLowerValue(renderingParameters->getMinSizeOfLabel());
  _ui->labelSizesSpanSlider->setUpperValue(renderingParameters->getMaxSizeOfLabel());

  // EDGES
  _ui->edges3DCheck->setChecked(renderingParameters->isEdge3D());
  _ui->edgesArrowCheck->setChecked(renderingParameters->isViewArrow());
  _ui->edgesColorInterpolationCheck->setChecked(renderingParameters->isEdgeColorInterpolate());
  _ui->edgesSizeInterpolationCheck->setChecked(renderingParameters->isEdgeSizeInterpolate());

  // SCENE
  _ui->backgroundColorButton->setTulipColor(_glMainWidget->getScene()->getBackgroundColor());
  _ui->selectionColorButton->setTulipColor(renderingParameters->getSelectionColor());

//  QApplication::processEvents();
  _resetting = false;
}

bool SceneConfigWidget::eventFilter(QObject* obj, QEvent* ev) {
  if (ev->type() == QEvent::MouseButtonPress) {
    if (obj == _ui->labelsDisabledLabel)
      _ui->labelsDensitySlider->setValue(-100);
    else if (obj == _ui->labelsNoOverlapLabel)
      _ui->labelsDensitySlider->setValue(0);
    else if (obj == _ui->labelsShowAllLabel)
      _ui->labelsDensitySlider->setValue(100);

    return true;
  }

  return false;
}

void SceneConfigWidget::settingsChanged() {
  _ui->applyButton->setEnabled(!_resetting);
}

void SceneConfigWidget::applySettings() {
  if(_resetting || !_glMainWidget->getScene()->getGlGraphComposite())
    return;

  GlGraphRenderingParameters* renderingParameters = _glMainWidget->getScene()->getGlGraphComposite()->getRenderingParametersPointer();

  // NODES
  if (_ui->labelsOrderingCombo->currentIndex()==0)
    renderingParameters->setElementOrderingProperty(NULL);
  else {
    GraphPropertiesModel<DoubleProperty>* model = static_cast<GraphPropertiesModel<DoubleProperty> *>(_ui->labelsOrderingCombo->model());
    renderingParameters->setElementOrderingProperty(dynamic_cast<DoubleProperty*>(model->index(_ui->labelsOrderingCombo->currentIndex(),0).data(TulipModel::PropertyRole).value<PropertyInterface*>()));
  }

  renderingParameters->setLabelScaled(_ui->labelsFitCheck->isChecked());
  renderingParameters->setLabelsDensity(_ui->labelsDensitySlider->value());
  renderingParameters->setMinSizeOfLabel(_ui->labelSizesSpanSlider->lowerValue());
  renderingParameters->setMaxSizeOfLabel(_ui->labelSizesSpanSlider->upperValue());

  // EDGES
  renderingParameters->setEdge3D(_ui->edges3DCheck->isChecked());
  renderingParameters->setViewArrow(_ui->edgesArrowCheck->isChecked());
  renderingParameters->setEdgeColorInterpolate(_ui->edgesColorInterpolationCheck->isChecked());
  renderingParameters->setEdgeSizeInterpolate(_ui->edgesSizeInterpolationCheck->isChecked());

  // SCENE
  renderingParameters->setSelectionColor(_ui->selectionColorButton->tulipColor());
  _glMainWidget->getScene()->setBackgroundColor(_ui->backgroundColorButton->tulipColor());

  _glMainWidget->draw();
  _ui->applyButton->setEnabled(false);
  emit settingsApplied();
}
