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

#include "EditColorScaleInteractor.h"
#include "GlLabelledColorScale.h"

#include <QMouseEvent>

#include <tulip/GlMainWidget.h>
#include <tulip/GlEntity.h>
#include <tulip/GlColorScale.h>
#include <tulip/ColorScaleConfigDialog.h>
#include <tulip/Camera.h>
#include <tulip/GlLayer.h>
#include <tulip/DoubleProperty.h>

#include "SOMView.h"
#include <SOMMap.h>

using namespace tlp;
using namespace std;

EditColorScaleInteractor::EditColorScaleInteractor() :
  currentProperty(NULL), colorScale(NULL), widthPercent(.80f), heightPercent(.1f), heightPosition(.1f),glMainWidgetWidth(0),glMainWidgetHeight(0),selectionLayer(new GlLayer("SelectionLayer")) {
}

EditColorScaleInteractor::~EditColorScaleInteractor() {
  selectionLayer->clear();
  delete selectionLayer;
  delete colorScale;
}

bool EditColorScaleInteractor::eventFilter(QObject *obj, QEvent *event) {
  tlp::GlMainWidget *glMainWidget = dynamic_cast<tlp::GlMainWidget*> (obj);

  if (glMainWidget == NULL)
    return false;

  if (event->type() == QEvent::MouseButtonDblClick) {
    QMouseEvent *me = (QMouseEvent*) event;
    GlMainWidget *glMainWidget = dynamic_cast<tlp::GlMainWidget*> (obj);

    if (!glMainWidget)
      return false;

    glMainWidget->getScene()->getGraphCamera().initGl();
    selectionLayer->set2DMode();
    glMainWidget->getScene()->addExistingLayer(selectionLayer);
    selectionLayer->getCamera().initGl();
    selectionLayer->addGlEntity(colorScale, "colorScale");

    std::vector<SelectedEntity> entities;
    glMainWidget->getScene()->selectEntities(RenderingSimpleEntities, me->pos().x(),
        me->pos().y(), 2, 2, selectionLayer, entities);
    bool foundGlColorScale = false;

    if (!entities.empty()) {
      for (vector<SelectedEntity>::iterator entityIterator = entities.begin(); entityIterator
           != entities.end(); ++entityIterator) {
        if ((*entityIterator).getSimpleEntity() == colorScale->getGlColorScale()) {
          ColorScaleConfigDialog dialog(colorScale->getGlColorScale()->getColorScale(),
                                        glMainWidget);
          foundGlColorScale = true;
          dialog.exec();
        }
      }

    }

    //Empty layer without destructing objects.
    selectionLayer->deleteGlEntity(colorScale);
    glMainWidget->getScene()->removeLayer(selectionLayer,false);

    return foundGlColorScale;
  }

  return false;
}
void EditColorScaleInteractor::viewChanged(View *view) {
  SOMView *somView = dynamic_cast<SOMView*> (view);

  if (somView != NULL) {
    assert(colorScale == NULL);
    GlMainWidget* glMainWidget = somView->getMapWidget();
    Size screenSize(glMainWidget->width() * widthPercent, glMainWidget->height()
                    * heightPercent);
    Coord bottomLeftScreenCoord((glMainWidget->width() - screenSize.getW()) * .5,
                                glMainWidget->height() * .1, 0);
    colorScale = new GlLabelledColorScale(bottomLeftScreenCoord, screenSize,
                                          somView->getColorScale(), 0, 0, false);

    propertyChanged(somView,somView->getSelectedProperty(), somView->getSelectedPropertyValues());
  }
}

bool EditColorScaleInteractor::compute(GlMainWidget *) {
  SOMView *somView = dynamic_cast<SOMView*> (view());
  assert(somView != NULL);

  screenSizeChanged(somView);
  return true;
}

bool EditColorScaleInteractor::draw(GlMainWidget *glMainWidget) {
  SOMView *somView = dynamic_cast<SOMView*> (view());
  assert(somView != NULL);

  if (colorScale) {
    DoubleProperty *newProperty = somView->getSelectedPropertyValues();

    if (currentProperty != newProperty) {
      propertyChanged(somView,somView->getSelectedProperty(), newProperty);
    }

    if (colorScale->isVisible()) {
      glMainWidget->getScene()->getGraphCamera().initGl();
      Camera camera2D = Camera(glMainWidget->getScene(), false);
      camera2D.setScene(glMainWidget->getScene());
      camera2D.initGl();

      map<string, GlSimpleEntity*> displays = colorScale->getGlEntities();

      for (map<string, GlSimpleEntity*>::iterator it = displays.begin(); it!= displays.end(); ++it) {
        it->second->draw(0, &camera2D);
      }
    }
  }

  return true;
}

void EditColorScaleInteractor::screenSizeChanged(SOMView* somView) {
  GlMainWidget* glMainWidget = somView->getMapWidget();

  if(glMainWidget->width()!=glMainWidgetWidth || glMainWidget->height()!=glMainWidgetHeight) {
    if (colorScale) {
      Size screenSize(glMainWidget->width() * widthPercent, glMainWidget->height()
                      * heightPercent);
      Coord bottomLeftScreenCoord((glMainWidget->width() - screenSize.getW()) * .5,
                                  glMainWidget->height() * .1, 0);
      colorScale->setPosition(bottomLeftScreenCoord);
      colorScale->setSize(screenSize);
      glMainWidgetWidth=glMainWidget->width();
      glMainWidgetHeight=glMainWidget->height();
    }
  }
}

void EditColorScaleInteractor::propertyChanged(SOMView* somView,const string& propertyName, DoubleProperty *propertyValues) {
  if (propertyValues) {
    colorScale->setVisible(true);
    //If the input samples are normalized we need to translate it to unormalized values before displaying it.
    double minValue = propertyValues->getNodeMin(somView->getSOM());
    double maxValue = propertyValues->getNodeMax(somView->getSOM());
    InputSample& inputSample = somView->getInputSample();
    colorScale->setMinValue(inputSample.isUsingNormalizedValues()?inputSample.unnormalize(minValue,propertyName):minValue);
    colorScale->setMaxValue(inputSample.isUsingNormalizedValues()?inputSample.unnormalize(maxValue,propertyName):maxValue);
  }
  else {
    colorScale->setVisible(false);
  }

  currentProperty = propertyValues;
}
