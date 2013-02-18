#include "EnclosingCircleHighlighter.h"

using namespace tlp;
using namespace std;

#include <tulip/GlScene.h>
#include <tulip/Circle.h>
#include <tulip/ForEach.h>
#include <tulip/GlCircle.h>
#include <tulip/GlMainView.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/GlMainWidget.h>

#include <QtGui/qwidget.h>
#include <QtGui/qcolordialog.h>

#include "../PathFinderTools.h"

Color getInverseColor(const Color &c) {
  return Color(255 - c.getR(), 255 - c.getG(), 255 - c.getB(), c.getA());
}

//******************************************************
void EnclosingCircleHighlighter::highlight(const PathFinder*, GlMainWidget *glMainWidget, BooleanProperty *selection, node, node) {
  GlGraphInputData *inputData(getInputData(glMainWidget));
  GlScene *scene = glMainWidget->getScene();
  LayoutProperty *layout = inputData->getElementLayout();
  vector<Circlef > circles;
  node n;
  float minDepth=-.5; // We'll draw the circle beyond the deeper node. Will work in most cases.
  forEach(n, selection->getNodesEqualTo(true)) {
    Circlef c;
    minDepth=min(minDepth, layout->getNodeValue(n).getZ());

    if (getNodeEnclosingCircle(c, inputData, n))
      circles.push_back(c);
  }
  edge e;
  forEach(e, selection->getEdgesEqualTo(true)) {
    Circlef c;

    if (getEdgeEnclosingCircle(c, inputData, e))
      circles.push_back(c);
  }
  Circlef enclosing(enclosingCircle<float> (circles));

  Color inside, outline;

  if (inversedColor) {
    inside = getInverseColor(glMainWidget->getScene()->getBackgroundColor());
    outline = inside;
  }
  else {
    inside = circleColor;
    outline = outlineColor;
  }

  inside.setA(alpha);

  GlCircle *glCircle = new GlCircle(Coord(enclosing[0], enclosing[1], minDepth), enclosing.radius, outline, inside, true, true, 0, 256);
  addGlEntity(scene, glCircle, true, "PathFinderCircle");
}

void EnclosingCircleHighlighter::draw(GlMainWidget*) {
}

PathHighlighter *EnclosingCircleHighlighter::clone() {
  return new EnclosingCircleHighlighter;
}

bool EnclosingCircleHighlighter::isConfigurable() {
  return true;
}

QWidget *EnclosingCircleHighlighter::getConfigurationWidget() {
  configurationWidget = new EnclosingCircleConfigurationWidget;

  if (inversedColor) {
    configurationWidget->inverseColorRadio->setChecked(true);
    configurationWidget->circleColorBtn->setDisabled(true);
  }
  else
    configurationWidget->solidColorRadio->setChecked(true);

  configurationWidget->alphaSlider->setValue(alpha);
  connect(configurationWidget->solidColorRadio, SIGNAL(clicked(bool)), this, SLOT(solidColorRadioChecked(bool)));
  connect(configurationWidget->inverseColorRadio, SIGNAL(clicked(bool)), this, SLOT(inverseColorRadioChecked(bool)));
  connect(configurationWidget->circleColorBtn, SIGNAL(clicked(bool)), this, SLOT(colorButtonClicked()));
  connect(configurationWidget->alphaSlider, SIGNAL(valueChanged(int)), this, SLOT(alphaChanged(int)));
  return configurationWidget;
}

void EnclosingCircleHighlighter::solidColorRadioChecked(bool) {
  configurationWidget->circleColorBtn->setDisabled(false);
  inversedColor=false;
}

void EnclosingCircleHighlighter::inverseColorRadioChecked(bool) {
  configurationWidget->circleColorBtn->setDisabled(true);
  inversedColor=true;
}

void EnclosingCircleHighlighter::colorButtonClicked() {
  QColor initial(circleColor.getR(), circleColor.getG(), circleColor.getB(), circleColor.getA());
  QColor c(QColorDialog::getColor(initial, configurationWidget));
  outlineColor = Color(0,0,0);
  circleColor = Color(c.red(), c.green(), c.blue(), c.alpha());
}

void EnclosingCircleHighlighter::alphaChanged(int a) {
  alpha = a;
}
