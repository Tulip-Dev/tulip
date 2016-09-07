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

#include "tulip/SceneConfigWidget.h"

#include <QDebug>
#include <QMainWindow>
#include <QApplication>

#include <tulip/GlGraph.h>
#include <tulip/GlMainWidget.h>
#include <tulip/Perspective.h>
#include <tulip/TulipSettings.h>
#include <tulip/GraphPropertiesModel.h>

// Disable GCC pedantic warnings when including "ui_SceneConfigWidget.h"
// because it includes qxtspanslider.h which raises a warning
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-pedantic"
#endif
#include "ui_SceneConfigWidget.h"
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

using namespace tlp;

SceneConfigWidget::SceneConfigWidget(QWidget *parent): QWidget(parent), _ui(new Ui::SceneConfigWidget), _glMainWidget(nullptr), _resetting(false) {
  _ui->setupUi(this);

  connect(_ui->dynamicFontSizeRB, SIGNAL(toggled(bool)), this, SLOT(dynamicFontRBToggled(bool)));
  connect(_ui->labelSizesSpanSlider, SIGNAL(lowerValueChanged(int)), this, SLOT(updateSliderRangeLabels()));
  connect(_ui->labelSizesSpanSlider, SIGNAL(upperValueChanged(int)), this, SLOT(updateSliderRangeLabels()));

  _ui->selectionColorButton->setDialogTitle("Choose the color of selected nodes or edges");
  _ui->backgroundColorButton->setDialogTitle("Choose the background color");

  if (Perspective::instance() != nullptr && Perspective::instance()->mainWindow() != nullptr) {
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
  if (_glMainWidget != nullptr) {
    disconnect(_glMainWidget,SIGNAL(graphChanged()),this,SLOT(resetChanges()));
    disconnect(_glMainWidget,SIGNAL(viewDrawn(GlMainWidget *,bool)),this,SLOT(resetChanges()));
  }

  _glMainWidget = glMainWidget;

  if (_glMainWidget != nullptr) {
    connect(_glMainWidget,SIGNAL(graphChanged()),this,SLOT(resetChanges()));
    // we assume that if an outside action causes an update of one
    // of the rendering parameters managed in this widget, necessarily
    // a call to _glMainWidget->draw will follow
    connect(_glMainWidget,SIGNAL(viewDrawn(GlMainWidget *,bool)),this,SLOT(resetChanges()));
  }

  resetChanges();
}

void SceneConfigWidget::resetChanges() {
  _resetting = true;

  _ui->scrollArea->setEnabled(_glMainWidget != nullptr);

  if (_glMainWidget == nullptr || _glMainWidget->getScene()->getMainGlGraph() == nullptr || _glMainWidget->getScene()->getMainGlGraph()->getGraph() == nullptr)
    return;

  Graph* graph = _glMainWidget->getScene()->getMainGlGraph()->getGraph();
  GlGraphRenderingParameters& renderingParameters = _glMainWidget->getScene()->getMainGlGraph()->getRenderingParameters();

  // NODES
  delete _ui->labelsOrderingCombo->model();
  GraphPropertiesModel<NumericProperty>* model = new GraphPropertiesModel<NumericProperty>(trUtf8("Disable ordering"),graph);
  _ui->labelsOrderingCombo->setModel(model);

  if (renderingParameters.elementsOrderingProperty() == nullptr)
    _ui->labelsOrderingCombo->setCurrentIndex(0);
  else
    _ui->labelsOrderingCombo->setCurrentIndex(model->rowOf(renderingParameters.elementsOrderingProperty()));

  _ui->descendingCB->setChecked(renderingParameters.elementsOrderedDescending());

  _ui->labelsFitCheck->setChecked(renderingParameters.labelsScaled());
  _ui->fixedFontSizeRB->setChecked(renderingParameters.labelsFixedFontSize());
  _ui->dynamicFontSizeRB->setChecked(!renderingParameters.labelsFixedFontSize());
  _ui->labelsDensitySlider->setValue(renderingParameters.labelsDensity());
  _ui->labelSizesSpanSlider->setLowerValue(renderingParameters.minSizeOfLabels());
  _ui->labelSizesSpanSlider->setUpperValue(renderingParameters.maxSizeOfLabels());

  // EDGES
  _ui->edges3DCheck->setChecked(renderingParameters.edges3D());
  _ui->edgesArrowCheck->setChecked(renderingParameters.displayEdgesExtremities());
  _ui->edgesColorInterpolationCheck->setChecked(renderingParameters.interpolateEdgesColors());
  _ui->edgesSizeInterpolationCheck->setChecked(renderingParameters.interpolateEdgesSizes());

  // COLORS
  _ui->backgroundColorButton->setTulipColor(_glMainWidget->getScene()->getBackgroundColor());
  _ui->selectionColorButton->setTulipColor(renderingParameters.selectionColor());

  // PROJECTION
  if (_glMainWidget->getScene()->isViewOrtho())
    _ui->orthoRadioButton->setChecked(true);
  else
    _ui->centralRadioButton->setChecked(true);

  // GRAPH CHANGING
  if (_glMainWidget->keepScenePointOfViewOnSubgraphChanging())
    _ui->keepSceneRadioButton->setChecked(true);
  else
    _ui->centerSceneRadioButton->setChecked(true);

  _resetting = false;
}

bool SceneConfigWidget::eventFilter(QObject* obj, QEvent* ev) {
  if (ev->type() == QEvent::MouseButtonPress) {
    if (obj == _ui->labelsDisabledLabel) {
      _ui->labelsDensitySlider->setValue(-100);
      applySettings();
    }
    else if (obj == _ui->labelsNoOverlapLabel) {
      _ui->labelsDensitySlider->setValue(0);
      applySettings();
    }
    else if (obj == _ui->labelsShowAllLabel) {
      _ui->labelsDensitySlider->setValue(100);
      applySettings();
    }

    return true;
  }

  return false;
}

void SceneConfigWidget::applySettings() {
  if(_resetting || !_glMainWidget->getScene()->getMainGlGraph())
    return;

  GlGraphRenderingParameters& renderingParameters = _glMainWidget->getScene()->getMainGlGraph()->getRenderingParameters();

  // NODES
  if (_ui->labelsOrderingCombo->currentIndex()==0)
    renderingParameters.setElementsOrderingProperty(nullptr);
  else {
    GraphPropertiesModel<NumericProperty>* model = static_cast<GraphPropertiesModel<NumericProperty> *>(_ui->labelsOrderingCombo->model());
    renderingParameters.setElementsOrderingProperty(dynamic_cast<NumericProperty*>(model->index(_ui->labelsOrderingCombo->currentIndex(),0).data(TulipModel::PropertyRole).value<PropertyInterface*>()));
  }

  renderingParameters.setElementOrderedDescending(_ui->descendingCB->isChecked());
  renderingParameters.setLabelsFixedFontSize(_ui->fixedFontSizeRB->isChecked());
  renderingParameters.setLabelsScaled(_ui->labelsFitCheck->isChecked());
  renderingParameters.setLabelsDensity(_ui->labelsDensitySlider->value());
  renderingParameters.setMinSizeOfLabels(_ui->labelSizesSpanSlider->lowerValue());
  renderingParameters.setMaxSizeOfLabels(_ui->labelSizesSpanSlider->upperValue());

  // EDGES
  renderingParameters.setEdges3D(_ui->edges3DCheck->isChecked());
  renderingParameters.setDisplayEdgesExtremities(_ui->edgesArrowCheck->isChecked());
  renderingParameters.setInterpolateEdgesColors(_ui->edgesColorInterpolationCheck->isChecked());
  renderingParameters.setInterpolateEdgesSizes(_ui->edgesSizeInterpolationCheck->isChecked());

  // COLORS
  renderingParameters.setSelectionColor(_ui->selectionColorButton->tulipColor());
  _glMainWidget->getScene()->setBackgroundColor(_ui->backgroundColorButton->tulipColor());

  // PROJECTION
  _glMainWidget->getScene()->setViewOrtho(_ui->orthoRadioButton->isChecked());

  // GRAPH CHANGINS
  _glMainWidget->setKeepScenePointOfViewOnSubgraphChanging(_ui->keepSceneRadioButton->isChecked());

  _glMainWidget->draw();
  emit settingsApplied();
}

void SceneConfigWidget::dynamicFontRBToggled(bool state) {
  _ui->sizeLimitsGB->setEnabled(state);
}

void SceneConfigWidget::updateSliderRangeLabels() {
  _ui->labelsMinSizeLabel->setText(QString::number(_ui->labelSizesSpanSlider->lowerValue()));
  _ui->labelsMaxSizeLabel->setText(QString::number(_ui->labelSizesSpanSlider->upperValue()));
}
