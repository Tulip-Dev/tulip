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

#include <tulip/GlTools.h>
#include <tulip/GlLabel.h>

#include "ParallelAxis.h"

using namespace std;

namespace tlp {

void drawComposite(GlComposite *composite, float lod, Camera *camera) {
  map<string, GlSimpleEntity*> glEntities(composite->getGlEntities());
  map<string, GlSimpleEntity*>::iterator it2;

  for (it2 = glEntities.begin(); it2 != glEntities.end() ; ++it2) {
    GlSimpleEntity *entity = it2->second;
    GlComposite *compositeEntity = dynamic_cast<GlComposite *>(entity);

    if (compositeEntity != NULL) {
      drawComposite(compositeEntity, lod, camera);
    }
    else {
      entity->draw(lod, camera);
    }
  }
}

ParallelAxis::ParallelAxis(GlAxis *glAxis, const float axisAreaWidth, const float rotationAngle, const GlAxis::CaptionLabelPosition captionPosition) : glAxis(glAxis), axisAreaWidth(axisAreaWidth), slidersActivated(false), rotationAngle(rotationAngle), hidden(false) {
  setStencil(1);
  glAxis->addCaption(captionPosition, 20.0f, true, axisAreaWidth / 2.0f, glAxis->getAxisLength() / 18.0f);
  glAxis->updateAxis();
  BoundingBox axisBB(glAxis->getBoundingBox());
  emptyRect = new GlRect(Coord(axisBB[0][0], axisBB[1][1] + glAxis->getAxisLength() / 10.0f), Coord(axisBB[1][0], axisBB[0][1] - glAxis->getAxisLength() / 15.0f), Color(0,0,0,0), Color(0,0,0,0), true, false);
  enableTrickForSelection();
  glAxis->updateAxis();
  resetSlidersPosition();
}

ParallelAxis::~ParallelAxis() {
  delete glAxis;
}

void ParallelAxis::setAxisHeight(const float height) {
  float resizeFactor = height / getAxisHeight();
  glAxis->setAxisLength(height);
  Coord baseCoord(glAxis->getAxisBaseCoord());
  bottomSliderCoord = baseCoord + Coord(0.0f ,(bottomSliderCoord.getY() - baseCoord.getY()) * resizeFactor);
  topSliderCoord = baseCoord + Coord(0.0f ,(topSliderCoord.getY() - baseCoord.getY()) * resizeFactor);
}

void ParallelAxis::setRotationAngle(const float rotationAngle) {
  this->rotationAngle = rotationAngle;
}

void ParallelAxis::setCaptionPosition(const GlAxis::CaptionLabelPosition captionPosition) {
  glAxis->addCaption(captionPosition, 20.0f, true, axisAreaWidth / 2.0f, glAxis->getAxisLength() / 18.0f);
  glAxis->updateAxis();
}

void ParallelAxis::translate(const Coord &c) {
  glAxis->translate(c);
  topSliderCoord += c;
  bottomSliderCoord += c;
}

void ParallelAxis::setBaseCoord(const Coord &baseCoord) {
  translate(baseCoord - glAxis->getAxisBaseCoord());
}

void ParallelAxis::draw(float lod,Camera *camera) {
  if (rotationAngle != 0.0f) {
    glPushMatrix();
    glRotatef(rotationAngle, 0.0f , 0.0f, 1.0f);
    GlLabel *captionLabel = NULL;
    GlComposite *axisCaptionComposite = dynamic_cast<GlComposite *>(glAxis->findGlEntity("caption composite"));

    if (axisCaptionComposite != NULL) {
      string captionLabelId = glAxis->getAxisName() + " axis caption";
      captionLabel = dynamic_cast<GlLabel *>(axisCaptionComposite->findGlEntity(captionLabelId));
    }

    if (captionLabel != NULL) {
      if (rotationAngle > -270.0f && rotationAngle < -90.0f) {
        captionLabel->rotate(0.0f, 0.0f, -180.0f);
      }
      else {
        captionLabel->rotate(0.0f, 0.0f, 0.0f);
      }
    }
  }

  drawComposite(glAxis, lod, camera);

  if (rotationAngle != 0.0f) {
    glPopMatrix();
  }
}

void ParallelAxis::redraw() {
  glAxis->updateAxis();
}

void ParallelAxis::resetSlidersPosition() {
  Coord baseCoord(glAxis->getAxisBaseCoord());
  bottomSliderCoord = baseCoord;
  topSliderCoord = baseCoord + Coord(0.0f, getAxisHeight());
}

void ParallelAxis::enableTrickForSelection() {
  glAxis->addGlEntity(emptyRect, "empty rect");
}

void ParallelAxis::disableTrickForSelection() {
  glAxis->deleteGlEntity(emptyRect);
}

BoundingBox ParallelAxis::getBoundingBox() {
  BoundingBox glAxisBB(glAxis->getBoundingBox());

  if (rotationAngle != 0.0f) {
    Coord bbBL(glAxisBB[0]);
    Coord bbTR(glAxisBB[1]);
    Coord bbBR(glAxisBB[1][0], glAxisBB[0][1]);
    Coord bbTL(glAxisBB[0][0], glAxisBB[1][1]);
    rotateVector(bbBL, rotationAngle, Z_ROT);
    rotateVector(bbTR, rotationAngle, Z_ROT);
    rotateVector(bbBR, rotationAngle, Z_ROT);
    rotateVector(bbTL, rotationAngle, Z_ROT);
    glAxisBB = BoundingBox();
    glAxisBB.expand(bbBL);
    glAxisBB.expand(bbTR);
    glAxisBB.expand(bbBR);
    glAxisBB.expand(bbTL);
  }

  return glAxisBB;
}

Array<Coord, 4> ParallelAxis::getBoundingPolygonCoords() const {
  Array<Coord, 4> ret;
  BoundingBox glAxisBB(glAxis->getBoundingBox());
  Coord bbBL(glAxisBB[0]);
  Coord bbTR(glAxisBB[1]);
  Coord bbBR(glAxisBB[1][0], glAxisBB[0][1]);
  Coord bbTL(glAxisBB[0][0], glAxisBB[1][1]);

  if (rotationAngle != 0.0f) {
    rotateVector(bbBL, rotationAngle, Z_ROT);
    rotateVector(bbTR, rotationAngle, Z_ROT);
    rotateVector(bbBR, rotationAngle, Z_ROT);
    rotateVector(bbTL, rotationAngle, Z_ROT);
  }

  ret[0] = bbTL;
  ret[1] = bbTR;
  ret[2] = bbBR;
  ret[3] = bbBL;
  return ret;
}

Coord ParallelAxis::getBaseCoord() const {
  return glAxis->getAxisBaseCoord();
}

Coord ParallelAxis::getTopCoord() const {
  return glAxis->getAxisBaseCoord() + Coord(0, getAxisHeight(), 0.0f);

}

}
