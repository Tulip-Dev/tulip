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

#include "ThresholdInteractor.h"
#include "SOMView.h"
#include "GlLabelledColorScale.h"
#include <SOMMap.h>

#include <tulip/GlLabel.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/ForEach.h>
#include <tulip/QGlPixelBufferManager.h>
#include <tulip/GlTextureManager.h>
#include <tulip/ColorScale.h>
#include <tulip/GlQuad.h>
#include <tulip/GlRect.h>
#include <tulip/GlLayer.h>
#include <tulip/GlMainWidget.h>
#include <tulip/BooleanProperty.h>

#include <QToolTip>
#include <QMouseEvent>
#include <QGLPixelBuffer>
#include <QPicture>

#include <stdint.h>

using namespace tlp;
using namespace std;

static Coord basePos[4] = { Coord(0, 0, 0), Coord(0, 0, 0), Coord(0, 0, 0), Coord(0, 0, 0) };
void drawComposite(GlComposite *composite, float lod, Camera *camera) {

  map<string, GlSimpleEntity*> glEntities = composite->getGlEntities();

  map<string, GlSimpleEntity*>::iterator it2;

  for (it2 = glEntities.begin(); it2 != glEntities.end(); ++it2) {
    it2->second->draw(lod, camera);
  }
}

ColorScaleSlider::ColorScaleSlider(SliderWay way, Size size, GlLabelledColorScale *colorScale,
                                   const string& textureName) :
  way(way), size(size), linkedSlider(NULL), linkedScale(colorScale), currentShift(0) {

  buildComposite(textureName);
  linkedScale->getGlColorScale()->getColorScale()->addObserver(this);

}

double ColorScaleSlider::getValue() {
  return linkedScale->getMinValue() + currentShift
         * (linkedScale->getMaxValue() - linkedScale->getMinValue());
}

ColorScaleSlider::~ColorScaleSlider() {
  linkedScale->getGlColorScale()->getColorScale()->removeObserver(this);
  reset(true);
}

void ColorScaleSlider::buildComposite(const std::string& textureName) {
  ostringstream oss;
  Coord colorScaleCoord = linkedScale->getGlColorScale()->getBaseCoord();
  float Ypos = colorScaleCoord.getY() - linkedScale->getGlColorScale()->getThickness() * .5;

  if (way == ToLeft) {
    oss << linkedScale->getMaxValue();
    position.set(colorScaleCoord.getX() + linkedScale->getGlColorScale()->getLength(), Ypos,
                 colorScaleCoord.getZ());
    currentShift = 1;
  }
  else {
    oss << linkedScale->getMinValue();
    position.set(colorScaleCoord.getX(), Ypos, colorScaleCoord.getZ());
    currentShift = 0;
  }

  float arrowLen = size.getW() * 0.25;

  Size labelSize(size.getW(), size.getH());
  vector<Color> fillColors;
  fillColors.insert(fillColors.begin(), 3,
                    linkedScale->getGlColorScale()->getColorAtPos(position));

  vector<Coord> points;
  points.push_back(position);
  points.push_back(Coord(position.getX() - (size.getW() * 0.5), position.getY() - arrowLen));
  points.push_back(Coord(position.getX() + (size.getW() * 0.5), position.getY() - arrowLen));

  Coord p1 = Coord(points[2].getX(), position.getY() - size.getH(), 0);
  Coord p2 = Coord(points[1].getX(), position.getY() - size.getH(), 0);

  rect = new GlQuad(p1, p2, points[1], points[2], Color(255, 255, 255));
  Coord labelPosition(position.getX(), p1.getY() + (points[1].getY()
                      - p1.getY()) * 0.5);
  rect->setTextureName(textureName);
  arrow = new GlPolygon(points, fillColors, fillColors, true, false);
  addGlEntity(arrow, "arrow");
  addGlEntity(rect, "frame");
  label = new GlLabel(labelPosition, labelSize, Color(0, 0, 0));
  addGlEntity(label, "label");
  label->setText(oss.str());

  computeBoundingBox();

}

void ColorScaleSlider::setLinkedSlider(ColorScaleSlider* linkedSlider) {
  if (!linkedSlider)
    this->linkedSlider = NULL;
  else {
    if (way == ToLeft) {
      if (linkedSlider->getBasePosition().getX() <= position.getX())
        this->linkedSlider = linkedSlider;
      else {
        this->linkedSlider = NULL;
        std::cerr << __PRETTY_FUNCTION__ << ":" << __LINE__ << " "
                  << "Invalid linked slider bad coordinates" << std::endl;
      }
    }
    else {
      if (linkedSlider->getBasePosition().getX() >= position.getX())
        this->linkedSlider = linkedSlider;
      else {
        this->linkedSlider = NULL;
        std::cerr << __PRETTY_FUNCTION__ << ":" << __LINE__ << " "
                  << "Invalid linked slider bad coordinates" << std::endl;
      }
    }
  }
}

void ColorScaleSlider::computeBoundingBox() {
  tlp::GlBoundingBoxSceneVisitor visitor(NULL);
  acceptVisitor(&visitor);
  boundingBox = visitor.getBoundingBox();
}

void ColorScaleSlider::setColor(Color c) {
  arrow->setFillColor(c);
}

float ColorScaleSlider::getLeftBound() {
  if (way == ToRight || linkedSlider == NULL)
    return 0;
  else
    return linkedSlider->getCurrentShift();
}
float ColorScaleSlider::getRightBound() {
  if (way == ToLeft || linkedSlider == NULL)
    return 1;
  else
    return linkedSlider->getCurrentShift();
}

void ColorScaleSlider::setValue(double value) {
  if (value >= linkedScale->getMinValue() && value <= linkedScale->getMaxValue()) {
    currentShift = (value - linkedScale->getMinValue()) / (linkedScale->getMaxValue()
                   - linkedScale->getMinValue());
    updatePosition();
  }
}

void ColorScaleSlider::shift(float shift) {
  currentShift += shift;

  if (currentShift < getLeftBound()) {
    currentShift = getLeftBound();
  }

  if (currentShift > getRightBound())
    currentShift = getRightBound();

  updatePosition();
}

void ColorScaleSlider::updatePosition() {
  float xPos = linkedScale->getPosition().getX() + currentShift * linkedScale->getSize().getW();
  float xShift = xPos - position.getX();

  if (xShift != 0) {
    Coord mouvement(xShift, 0, 0);
    arrow->translate(mouvement);
    label->translate(mouvement);
    rect->translate(mouvement);
    setColor(linkedScale->getGlColorScale()->getColorAtPos(Coord(xPos,0,0)));
    ostringstream oss;
    oss << getValue();
    label->setText(oss.str());
    position.setX(xPos);
  }
}

void ColorScaleSlider::beginShift() {
}

void ColorScaleSlider::endShift() {
}
void ColorScaleSlider::draw(float lod, Camera *camera) {
  arrow->draw(lod, camera);
  rect->draw(lod, camera);
  label->draw(lod, camera);
}

void ColorScaleSlider::update(std::set<Observable *>::iterator,
                              std::set<Observable *>::iterator) {
  float xPos = linkedScale->getPosition().getX() + currentShift * linkedScale->getSize().getW();
  setColor(linkedScale->getGlColorScale()->getColorAtPos(Coord(xPos,0,0)));
}
void ColorScaleSlider::observableDestroyed(Observable *) {
}

SliderBar::SliderBar(ColorScaleSlider* left, ColorScaleSlider* right, const string& textureName) :
  GlSimpleEntity(), left(left), right(right), texture(textureName),isVisible(false) {
}

SliderBar::~SliderBar() {
}

float SliderBar::getLeftBound() {
  return left->getLeftBound();
}
float SliderBar::getRightBound() {
  return right->getRightBound();
}

void SliderBar::beginShift() {
  isVisible = true;
  right->beginShift();
  left->beginShift();
}
void SliderBar::shift(float shift) {
  float combinedShift = shift;

  if (left->getCurrentShift() + shift < left->getLeftBound())
    combinedShift = left->getLeftBound() - left->getCurrentShift();

  if (right->getCurrentShift() + shift > right->getRightBound())
    combinedShift = right->getRightBound() - right->getCurrentShift();

  right->shift(combinedShift);
  left->shift(combinedShift);
}
void SliderBar::endShift() {
  right->endShift();
  left->endShift();
  isVisible = false;
}
void SliderBar::draw(float lod, tlp::Camera *camera) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Coord lPos = left->getBasePosition();
  Size lSize = left->getSize();
  Coord rPos = right->getBasePosition();
  Size rSize = right->getSize();

  Coord topLeft(lPos.getX() + lSize.getW() * 0.5, lPos.getY() - lSize.getH(), lPos.getZ());
  Coord bottomRight(rPos.getX() - rSize.getW() * .5, rPos.getY() - rSize.getH() * .25,rPos.getZ());

  //Sliders are overlapping don't draw the slider bar
  if(bottomRight.getX() - topLeft.getX() > 0) {
    GlRect rect(topLeft,bottomRight,Color(255,255,255),Color(255,255,255));

    if(isVisible) {
      rect.setTextureName(texture);
    }
    else {
      rect.setTopLeftColor(Color(255,255,255,0));
      rect.setBottomRightColor(Color(255,255,255,0));
    }

    rect.draw(lod,camera);
  }

  boundingBox = BoundingBox();
  boundingBox.expand(topLeft);
  boundingBox.expand(bottomRight);

  glDisable(GL_BLEND);
}
ThresholdInteractor::ThresholdInteractor() :
  layer(new GlLayer("Threshold")), mouvingSlider(NULL),rSlider(NULL),lSlider(NULL), startDrag(false), textureName("") {
}

ThresholdInteractor::~ThresholdInteractor() {
  if (!textureName.empty()) {
    static_cast<SOMView*>(view())->getMapWidget()->deleteTexture(textureId);
    GlTextureManager::getInst().deleteTexture(textureName);
  }

  layer->getComposite()->reset(true);
  delete layer;
}

void ThresholdInteractor::setView(View *view) {
  EditColorScaleInteractor::setView(view);

  if (currentProperty)
    buildSliders(static_cast<SOMView*>(view));

  view->refresh();
}

bool ThresholdInteractor::draw(GlMainWidget *glMainWidget) {
  EditColorScaleInteractor::draw(glMainWidget);

  if (layer->isVisible()) {
    glMainWidget->getScene()->getGraphCamera().initGl();
    Camera camera2D(glMainWidget->getScene(), false);
    camera2D.setScene(glMainWidget->getScene());
    camera2D.initGl();
    drawComposite(layer->getComposite(), 0, &camera2D);
  }

  return true;
}
bool ThresholdInteractor::eventFilter(QObject * widget, QEvent * event) {

  GlMainWidget *glMainWidget = dynamic_cast<GlMainWidget*>(widget);
  SOMView *somView = dynamic_cast<SOMView*>(view());

  if (event->type() == QEvent::MouseButtonPress && ((QMouseEvent*) event)->button()
      == Qt::LeftButton) {

    QMouseEvent *e = static_cast<QMouseEvent*>(event);
    vector<SelectedEntity> selectedEntities;
    set<Slider*> finalSelectedEntities;

    //Update Camera for selection
    layer->set2DMode();
    glMainWidget->getScene()->addExistingLayer(layer);
    glMainWidget->getScene()->selectEntities(RenderingSimpleEntities, e->x(), e->y(), 0, 0, layer,
        selectedEntities);
    glMainWidget->getScene()->removeLayer(layer,false);

    if (!selectedEntities.empty()) {

      map<string, GlSimpleEntity*> displays = layer->getGlEntities();

      for (vector<SelectedEntity>::iterator itPE = selectedEntities.begin(); itPE
           != selectedEntities.end(); ++itPE) {
        for (map<string, GlSimpleEntity*>::iterator itDisplay = displays.begin(); itDisplay
             != displays.end(); ++itDisplay) {
          GlComposite *composite = dynamic_cast<GlComposite*> (itDisplay->second);

          if (composite && !composite->findKey(itPE->getSimpleEntity()).empty()) {

            Slider *slider = dynamic_cast<Slider*> (composite);

            if (slider) {
              finalSelectedEntities.insert(slider);
            }

            break;
          }
          else {
            if (itDisplay->second == (itPE->getSimpleEntity())) {
              Slider *slider = dynamic_cast<Slider*> (itDisplay->second);

              if (slider) {
                finalSelectedEntities.insert(slider);
              }
            }
          }
        }
      }

      assert(!finalSelectedEntities.empty());

      if (!startDrag) {
        glMainWidget->setMouseTracking(true);
        startDrag = true;
        mouvingSlider = *finalSelectedEntities.begin();
        mouvingSlider->beginShift();
        XPosCursor = e->x();
        glMainWidget->getScene()->getGraphCamera().initGl();

        layer->setVisible(false);
        colorScale->setVisible(false);
        somView->drawMapWidget();
        colorScale->setVisible(true);
        layer->setVisible(true);
        somView->refresh();
      }
    }

    return true;
  }

  if (event->type() == QEvent::MouseMove) {
    if (startDrag) {
      QMouseEvent *e = (QMouseEvent*) event;
      float xShift = e->x() - XPosCursor;
      XPosCursor = e->x();

      if (xShift == 0) {
        return true;
      }

      mouvingSlider->shift(xShift / (colorScale->getGlColorScale()->getLength()));
      somView->refresh();
    }

    return true;
  }

  if (event->type() == QEvent::MouseButtonRelease && startDrag) {
    QMouseEvent *me = (QMouseEvent*) event;
    SOMMap *som = somView->getSOM();
    assert(mouvingSlider != NULL);
    glMainWidget->setMouseTracking(false);
    startDrag = false;
    mouvingSlider->endShift();
    mouvingSlider = NULL;
    Qt::KeyboardModifiers systMod;
#if defined(__APPLE__)
    systMod = Qt::AltModifier;
#else
    systMod = Qt::ControlModifier;
#endif

    if (me->modifiers() == systMod) {
      if (somView->getMask())
        performSelection(somView, somView->getMask()->getNodesEqualTo(true, som));
      else
        performSelection(somView, som->getNodes());
    }
    else
      performSelection(somView, som->getNodes());

    return true;
  }

  EditColorScaleInteractor::eventFilter(widget, event);
  return false;
}

void ThresholdInteractor::performSelection(SOMView *view, tlp::Iterator<node> *it) {
  BooleanProperty* selection = view->graph()->getProperty<BooleanProperty> ("viewSelection");
  set<node> mask;
  map<node, set<node> > &mappingTab = view->getMappingTab();
  Observable::holdObservers();
  selection->setAllNodeValue(false);

  //If we are using normalized values we display unnormalized values, to compare them we need to renormalize them
  InputSample &inputSample = view->getInputSample();
  unsigned int propertyIndex = inputSample.findIndexForProperty(view->getSelectedProperty());
  double rightSliderRealValue = inputSample.isUsingNormalizedValues()?inputSample.normalize(rSlider->getValue(),propertyIndex):rSlider->getValue();
  double leftSliderRealValue = inputSample.isUsingNormalizedValues()?inputSample.normalize(lSlider->getValue(),propertyIndex):lSlider->getValue();

  node n;
  forEach(n,it) {
    double nodeValue = currentProperty->getNodeValue(n);

    if (nodeValue <=  rightSliderRealValue && nodeValue >= leftSliderRealValue) {
      if (mappingTab.find(n) != mappingTab.end()) {
        for (set<node>::iterator it = mappingTab[n].begin(); it != mappingTab[n].end(); ++it) {
          selection->setNodeValue(*it, true);
        }
      }

      mask.insert(n);
    }
  }
  view->setMask(mask);
  Observable::unholdObservers();
}

void ThresholdInteractor::screenSizeChanged(SOMView* somView) {
  EditColorScaleInteractor::screenSizeChanged(somView);
  clearSliders();

  if (currentProperty)
    buildSliders(somView);
}

void ThresholdInteractor::propertyChanged(SOMView* somView,const string& propertyName, DoubleProperty *newProperty) {
  EditColorScaleInteractor::propertyChanged(somView,propertyName, newProperty);

  if (newProperty) {
    clearSliders();
    buildSliders(somView);
    layer->setVisible(true);
  }
  else
    layer->setVisible(false);

}
void ThresholdInteractor::buildSliders(SOMView* somView) {
  BooleanProperty *mask = somView->getMask();
  SOMMap *som = somView->getSOM();
  assert(som);
  Size sliderSize(colorScale->getSize().getH(), colorScale->getSize().getH());

  double minValue, maxValue, intervalMinValue, intervalMaxValue;
  //Get the minimum and the maximum values.
  minValue = currentProperty->getNodeMin(somView->getSOM());
  maxValue = currentProperty->getNodeMax(somView->getSOM());

  if (mask) {
    intervalMinValue = maxValue;
    intervalMaxValue = minValue;
    node n;
    forEach (n,mask->getNodesEqualTo(true,som)) {
      double nodeValue = currentProperty->getNodeValue(n);

      if (nodeValue < intervalMinValue)
        intervalMinValue = nodeValue;

      if (nodeValue > intervalMaxValue)
        intervalMaxValue = nodeValue;
    }
  }
  else {
    intervalMinValue = minValue;
    intervalMaxValue = maxValue;
  }

  InputSample& inputSample = somView->getInputSample();
  unsigned int propertyIndex = inputSample.findIndexForProperty(somView->getSelectedProperty());

  if (textureName.empty())
    generateSliderTexture(somView->getMapWidget());

  lSlider = new ColorScaleSlider(ColorScaleSlider::ToRight, sliderSize, colorScale, textureName);

  if (intervalMinValue != minValue) {
    //If we use normalized values we need to translate them to unnormalized for user comprehension
    double intervalMinDisplayValue = inputSample.isUsingNormalizedValues()?inputSample.unnormalize(intervalMinValue,propertyIndex):intervalMinValue;
    lSlider->setValue(intervalMinDisplayValue);
  }

  layer->addGlEntity(lSlider, "Left");

  rSlider = new ColorScaleSlider(ColorScaleSlider::ToLeft, sliderSize, colorScale, textureName);

  if (intervalMaxValue != maxValue) {
    //If we use normalized values we need to translate them to unnormalized for user comprehension
    double intervalMaxDisplayValue = inputSample.isUsingNormalizedValues()?inputSample.unnormalize(intervalMaxValue,propertyIndex):intervalMaxValue;
    rSlider->setValue(intervalMaxDisplayValue);
  }

  layer->addGlEntity(rSlider, "Right");

  lSlider->setLinkedSlider(rSlider);
  rSlider->setLinkedSlider(lSlider);

  SliderBar *bar = new SliderBar(lSlider, rSlider, textureName);
  layer->addGlEntity(bar, "sliderBar");
}
void ThresholdInteractor::clearSliders() {
  if (layer)
    layer->getComposite()->reset(true);

  rSlider = NULL;
  lSlider = NULL;
}

void ThresholdInteractor::generateSliderTexture(GlMainWidget* widget) {
  uintptr_t id = reinterpret_cast<uintptr_t>(this);
  ostringstream oss;
  oss << "ThresholdInteractorSliderTexture" << id;
  widget->makeCurrent();
  textureId = widget->bindTexture(QPixmap(":/sliderTexture.png"));
  textureName = oss.str();
  GlTextureManager::getInst().registerExternalTexture(textureName, textureId);
}
