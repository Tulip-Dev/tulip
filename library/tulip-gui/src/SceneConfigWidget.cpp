#include "tulip/SceneConfigWidget.h"

#include <QtCore/QDebug>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlMainWidget.h>
#include "ui_SceneConfigWidget.h"

using namespace tlp;

SceneConfigWidget::SceneConfigWidget(QWidget *parent): QWidget(parent), _ui(new Ui::SceneConfigWidget), _glMainWidget(NULL), _resetting(false) {
  _ui->setupUi(this);
  _ui->labelSizesSpanSlider->setHandleMovementMode(QxtSpanSlider::FreeMovement);
  _ui->labelsDisabledLabel->installEventFilter(this);
  _ui->labelsNoOverlapLabel->installEventFilter(this);
  _ui->labelsShowAllLabel->installEventFilter(this);
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

  _ui->labelsOrderingCombo->clear();
  _ui->scrollArea->setEnabled(_glMainWidget != NULL);

  if (_glMainWidget == NULL || _glMainWidget->getScene()->getGlGraphComposite() == NULL || _glMainWidget->getScene()->getGlGraphComposite()->getGraph() == NULL) {
    return;
  }

  Graph* graph = _glMainWidget->getScene()->getGlGraphComposite()->getGraph();
  GlGraphRenderingParameters* renderingParameters = _glMainWidget->getScene()->getGlGraphComposite()->getRenderingParametersPointer();

  // NODES
  _ui->labelsOrderingCombo->addItem(trUtf8("Disable ordering"));
  int i=1;
  std::string propName;
  forEach(propName,graph->getProperties()) {
    PropertyInterface* pi = graph->getProperty(propName);

    if (dynamic_cast<DoubleProperty*>(pi) != NULL) {
      _ui->labelsOrderingCombo->addItem(propName.c_str());

      if (renderingParameters->getElementOrderingProperty() == pi)
        _ui->labelsOrderingCombo->setCurrentIndex(i);

      i++;
    }
  }
  _ui->labelsFitCheck->setChecked(renderingParameters->isLabelScaled());
  _ui->labelsDensitySlider->setValue(renderingParameters->getLabelsDensity());
  _ui->labelSizesSpanSlider->setLowerValue(renderingParameters->getMinSizeOfLabel());
  _ui->labelSizesSpanSlider->setUpperValue(renderingParameters->getMaxSizeOfLabel());

  // EDGES
  _ui->edges3DCheck->setChecked(renderingParameters->isEdge3D());
  _ui->edgesArrowCheck->setChecked(renderingParameters->isViewArrow());
  _ui->edgesColorInterpolationCheck->setChecked(renderingParameters->isEdgeColorInterpolate());
  _ui->edgesSizeInterpolationCheck->setChecked(renderingParameters->isEdgeSizeInterpolate());

  QApplication::processEvents();
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
  if(!_glMainWidget->getScene()->getGlGraphComposite())
    return;

  GlGraphRenderingParameters* renderingParameters = _glMainWidget->getScene()->getGlGraphComposite()->getRenderingParametersPointer();

  // NODES
  DoubleProperty* orderingProperty = NULL;

  if (_ui->labelsOrderingCombo->currentIndex() != 0 && _glMainWidget->getScene()->getGlGraphComposite()->getGraph())
    orderingProperty = _glMainWidget->getScene()->getGlGraphComposite()->getGraph()->getProperty<DoubleProperty>(_ui->labelsOrderingCombo->currentText().toStdString());

  renderingParameters->setElementOrderingProperty(orderingProperty);
  renderingParameters->setLabelScaled(_ui->labelsFitCheck->isChecked());
  renderingParameters->setLabelsDensity(_ui->labelsDensitySlider->value());
  renderingParameters->setMinSizeOfLabel(_ui->labelSizesSpanSlider->lowerValue());
  renderingParameters->setMaxSizeOfLabel(_ui->labelSizesSpanSlider->upperValue());

  // EDGES
  renderingParameters->setEdge3D(_ui->edges3DCheck->isChecked());
  renderingParameters->setViewArrow(_ui->edgesArrowCheck->isChecked());
  renderingParameters->setEdgeColorInterpolate(_ui->edgesColorInterpolationCheck->isChecked());
  renderingParameters->setEdgeSizeInterpolate(_ui->edgesSizeInterpolationCheck->isChecked());

  _glMainWidget->draw();
  _ui->applyButton->setEnabled(false);
}
