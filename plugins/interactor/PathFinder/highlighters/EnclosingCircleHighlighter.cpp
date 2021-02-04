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

#include <tulip/GlScene.h>
#include <tulip/Circle.h>
#include <tulip/GlCircle.h>
#include <tulip/GlMainView.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/GlMainWidget.h>
#include <tulip/ColorButton.h>

#include <QWidget>

#include "EnclosingCircleHighlighter.h"
#include "../PathFinderTools.h"
#include "ui_EnclosingCircleConfiguration.h"

using namespace tlp;
using namespace std;

EnclosingCircleConfigurationWidget::EnclosingCircleConfigurationWidget(Color &circleColor,
                                                                       QWidget *parent)
    : QWidget(parent), _ui(new Ui::EnclosingCircleConfigurationData) {
  _ui->setupUi(this);
  connect(_ui->solidColorRadio, SIGNAL(clicked(bool)), this, SIGNAL(solidColorRadioChecked(bool)));
  connect(_ui->inverseColorRadio, SIGNAL(clicked(bool)), this,
          SIGNAL(inverseColorRadioChecked(bool)));
  connect(_ui->circleColorBtn, SIGNAL(clicked()), this, SIGNAL(colorButtonClicked()));
  connect(_ui->alphaSlider, SIGNAL(valueChanged(int)), this, SIGNAL(alphaChanged(int)));
  _ui->circleColorBtn->setDialogTitle("Choose the enclosing circle color");
  _ui->circleColorBtn->setTulipColor(circleColor);
}

EnclosingCircleConfigurationWidget::~EnclosingCircleConfigurationWidget() {
  delete _ui;
}

void EnclosingCircleConfigurationWidget::circleColorBtnDisabled(const bool disabled) {
  _ui->circleColorBtn->setDisabled(disabled);
}

void EnclosingCircleConfigurationWidget::inverseColorRadioCheck(const bool checked) {
  _ui->inverseColorRadio->setChecked(checked);
}

void EnclosingCircleConfigurationWidget::alphaSliderSetValue(const int val) {
  _ui->alphaSlider->setValue(val);
}

void EnclosingCircleConfigurationWidget::solidColorRadioCheck(const bool checked) {
  _ui->solidColorRadio->setChecked(checked);
}

Color EnclosingCircleConfigurationWidget::getCircleColor() const {
  return _ui->circleColorBtn->tulipColor();
}

Color getInverseColor(const Color &c) {
  return Color(255 - c.getR(), 255 - c.getG(), 255 - c.getB(), c.getA());
}

//******************************************************
EnclosingCircleHighlighter::EnclosingCircleHighlighter()
    : PathHighlighter("Enclosing circle"), circleColor(200, 200, 200), outlineColor(0, 0, 0),
      alpha(128), inversedColor(false), configurationWidget(nullptr) {}

void EnclosingCircleHighlighter::highlight(const PathFinder *, GlMainWidget *glMainWidget,
                                           BooleanProperty *selection, node, node) {
  GlGraphInputData *inputData(getInputData(glMainWidget));
  GlScene *scene = glMainWidget->getScene();
  LayoutProperty *layout = inputData->getElementLayout();
  vector<Circlef> circles;
  float minDepth = -.5; // We'll draw the circle beyond the deeper node. Will work in most cases.
  for (auto n : selection->getNodesEqualTo(true)) {
    Circlef c;
    minDepth = min(minDepth, layout->getNodeValue(n).getZ());

    if (getNodeEnclosingCircle(c, inputData, n))
      circles.push_back(c);
  }
  for (auto e : selection->getEdgesEqualTo(true)) {
    Circlef c;

    if (getEdgeEnclosingCircle(c, inputData, e))
      circles.push_back(c);
  }
  Circlef enclosing(enclosingCircle<float>(circles));

  Color inside, outline;

  if (inversedColor) {
    inside = getInverseColor(glMainWidget->getScene()->getBackgroundColor());
    outline = inside;
  } else {
    inside = circleColor;
    outline = outlineColor;
  }

  inside.setA(alpha);

  GlCircle *glCircle = new GlCircle(Coord(enclosing[0], enclosing[1], minDepth), enclosing.radius,
                                    outline, inside, true, true, 0, 256);
  addGlEntity(scene, glCircle, true, "PathFinderCircle");
}

void EnclosingCircleHighlighter::draw(GlMainWidget *) {}

bool EnclosingCircleHighlighter::isConfigurable() const {
  return true;
}

EnclosingCircleHighlighter::~EnclosingCircleHighlighter() {
  // no need to delete the configurationWidget. Qt will do it well.
  // delete configurationWidget;
}

QWidget *EnclosingCircleHighlighter::getConfigurationWidget() {
  configurationWidget = new EnclosingCircleConfigurationWidget(circleColor);

  if (inversedColor) {
    configurationWidget->inverseColorRadioCheck(true);
    configurationWidget->circleColorBtnDisabled(true);
  } else
    configurationWidget->solidColorRadioCheck(true);

  configurationWidget->alphaSliderSetValue(alpha);
  connect(configurationWidget, SIGNAL(solidColorRadioChecked(bool)), this,
          SLOT(solidColorRadioChecked(bool)));
  connect(configurationWidget, SIGNAL(inverseColorRadioChecked(bool)), this,
          SLOT(inverseColorRadioChecked(bool)));
  connect(configurationWidget, SIGNAL(colorButtonClicked()), this, SLOT(colorButtonClicked()));
  connect(configurationWidget, SIGNAL(alphaChanged(int)), this, SLOT(alphaChanged(int)));
  return configurationWidget;
}

void EnclosingCircleHighlighter::solidColorRadioChecked(bool) {
  configurationWidget->circleColorBtnDisabled(false);
  inversedColor = false;
}

void EnclosingCircleHighlighter::inverseColorRadioChecked(bool) {
  configurationWidget->circleColorBtnDisabled(true);
  inversedColor = true;
}

void EnclosingCircleHighlighter::colorButtonClicked() {
  outlineColor = Color(0, 0, 0);
  circleColor = configurationWidget->getCircleColor();
}

void EnclosingCircleHighlighter::alphaChanged(int a) {
  alpha = a;
}
