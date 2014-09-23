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

#include <tulip/GlGraphComposite.h>
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

SceneConfigWidget::SceneConfigWidget(QWidget *parent): QWidget(parent), _ui(new Ui::SceneConfigWidget), _glMainWidget(NULL), _resetting(false) {
  _ui->setupUi(this);

  connect(_ui->dynamicFontSizeRB, SIGNAL(toggled(bool)), this, SLOT(dynamicFontRBToggled(bool)));
  connect(_ui->labelSizesSpanSlider, SIGNAL(lowerValueChanged(int)), this, SLOT(updateSliderRangeLabels()));
  connect(_ui->labelSizesSpanSlider, SIGNAL(upperValueChanged(int)), this, SLOT(updateSliderRangeLabels()));

  _ui->selectionColorButton->setDialogTitle("Choose the color of selected nodes or edges");
  _ui->backgroundColorButton->setDialogTitle("Choose the color of the backagroud");

  if (Perspective::instance() != NULL && Perspective::instance()->mainWindow() != NULL) {
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
  if (_glMainWidget != NULL) {
    disconnect(_glMainWidget,SIGNAL(graphChanged()),this,SLOT(resetChanges()));
    disconnect(_glMainWidget,SIGNAL(viewDrawn(GlMainWidget *,bool)),this,SLOT(resetChanges()));
  }

  _glMainWidget = glMainWidget;

  if (_glMainWidget != NULL) {
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

  _ui->scrollArea->setEnabled(_glMainWidget != NULL);

  if (_glMainWidget == NULL || _glMainWidget->getScene()->getGlGraphComposite() == NULL || _glMainWidget->getScene()->getGlGraphComposite()->getGraph() == NULL)
    return;

  Graph* graph = _glMainWidget->getScene()->getGlGraphComposite()->getGraph();
  GlGraphRenderingParameters* renderingParameters = _glMainWidget->getScene()->getGlGraphComposite()->getRenderingParametersPointer();

  // NODES
  delete _ui->labelsOrderingCombo->model();
  GraphPropertiesModel<NumericProperty>* model = new GraphPropertiesModel<NumericProperty>(trUtf8("Disable ordering"),graph);
  _ui->labelsOrderingCombo->setModel(model);

  if (renderingParameters->getElementOrderingProperty() == NULL)
    _ui->labelsOrderingCombo->setCurrentIndex(0);
  else
    _ui->labelsOrderingCombo->setCurrentIndex(model->rowOf(renderingParameters->getElementOrderingProperty()));

  _ui->labelsFitCheck->setChecked(renderingParameters->isLabelScaled());
  _ui->labelsBillboardedCheck->setChecked(renderingParameters->getLabelsAreBillboarded());
  _ui->fixedFontSizeRB->setChecked(renderingParameters->isLabelFixedFontSize());
  _ui->dynamicFontSizeRB->setChecked(!renderingParameters->isLabelFixedFontSize());
  _ui->labelsDensitySlider->setValue(renderingParameters->getLabelsDensity());
  _ui->labelSizesSpanSlider->setLowerValue(renderingParameters->getMinSizeOfLabel());
  _ui->labelSizesSpanSlider->setUpperValue(renderingParameters->getMaxSizeOfLabel());

  // EDGES
  _ui->edges3DCheck->setChecked(renderingParameters->isEdge3D());
  _ui->edgesArrowCheck->setChecked(renderingParameters->isViewArrow());
  _ui->edgesColorInterpolationCheck->setChecked(renderingParameters->isEdgeColorInterpolate());
  _ui->edgesSizeInterpolationCheck->setChecked(renderingParameters->isEdgeSizeInterpolate());

  // COLORS
  _ui->backgroundColorButton->setTulipColor(_glMainWidget->getScene()->getBackgroundColor());
  _ui->selectionColorButton->setTulipColor(renderingParameters->getSelectionColor());

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

//  QApplication::processEvents();
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
  if(_resetting || !_glMainWidget->getScene()->getGlGraphComposite())
    return;

  GlGraphRenderingParameters* renderingParameters = _glMainWidget->getScene()->getGlGraphComposite()->getRenderingParametersPointer();

  // NODES
  if (_ui->labelsOrderingCombo->currentIndex()==0)
    renderingParameters->setElementOrderingProperty(NULL);
  else {
    GraphPropertiesModel<NumericProperty>* model = static_cast<GraphPropertiesModel<NumericProperty> *>(_ui->labelsOrderingCombo->model());
    renderingParameters->setElementOrderingProperty(dynamic_cast<NumericProperty*>(model->index(_ui->labelsOrderingCombo->currentIndex(),0).data(TulipModel::PropertyRole).value<PropertyInterface*>()));
  }

  renderingParameters->setLabelScaled(_ui->labelsFitCheck->isChecked());
  renderingParameters->setLabelsAreBillboarded(_ui->labelsBillboardedCheck->isChecked());
  renderingParameters->setLabelFixedFontSize(_ui->fixedFontSizeRB->isChecked());
  renderingParameters->setLabelsDensity(_ui->labelsDensitySlider->value());
  renderingParameters->setMinSizeOfLabel(_ui->labelSizesSpanSlider->lowerValue());
  renderingParameters->setMaxSizeOfLabel(_ui->labelSizesSpanSlider->upperValue());

  // EDGES
  renderingParameters->setEdge3D(_ui->edges3DCheck->isChecked());
  renderingParameters->setViewArrow(_ui->edgesArrowCheck->isChecked());
  renderingParameters->setEdgeColorInterpolate(_ui->edgesColorInterpolationCheck->isChecked());
  renderingParameters->setEdgeSizeInterpolate(_ui->edgesSizeInterpolationCheck->isChecked());

  // COLORS
  renderingParameters->setSelectionColor(_ui->selectionColorButton->tulipColor());
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
