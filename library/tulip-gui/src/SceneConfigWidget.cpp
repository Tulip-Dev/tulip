#include "tulip/SceneConfigWidget.h"

#include <QtCore/QDebug>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlMainWidget.h>
#include "ui_SceneConfigWidget.h"

using namespace tlp;

SceneConfigWidget::SceneConfigWidget(QWidget *parent): QWidget(parent), _ui(new Ui::SceneConfigWidget), _glMainWidget(NULL) {
  _ui->setupUi(this);
}

void SceneConfigWidget::setGlMainWidget(tlp::GlMainWidget* glMainWidget) {
  if (_glMainWidget != NULL)
    disconnect(_glMainWidget,SIGNAL(graphChanged(tlp::Graph*)),this,SLOT(resetChanges()));
  _glMainWidget = glMainWidget;
  if (_glMainWidget != NULL)
    connect(_glMainWidget,SIGNAL(graphChanged(tlp::Graph*)),this,SLOT(resetChanges()));
  resetChanges();
}

void SceneConfigWidget::resetChanges() {
  _ui->labelsOrderingCombo->clear();
  _ui->scrollArea->setEnabled(_glMainWidget != NULL);
  if (_glMainWidget == NULL || _glMainWidget->getGraph() == NULL) {
    return;
  }
  Graph* graph = _glMainWidget->getGraph();
  GlGraphRenderingParameters* renderingParameters = _glMainWidget->getScene()->getGlGraphComposite()->getRenderingParametersPointer();

  // Metric ordering combo
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
  qWarning() << renderingParameters->getMinSizeOfLabel() << " - " << renderingParameters->getMaxSizeOfLabel();
  _ui->labelSizesSpanSlider->setMinimum(renderingParameters->getMinSizeOfLabel());
  _ui->labelSizesSpanSlider->setMaximum(renderingParameters->getMaxSizeOfLabel());
}
