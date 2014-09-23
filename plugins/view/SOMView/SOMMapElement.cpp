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

#include "SOMMapElement.h"
#include <SOMMap.h>

#include <tulip/GlRect.h>
#include <tulip/GlCircle.h>
#include <tulip/ColorProperty.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>

#define DEGTORAD(x) (M_PI/180.f) * x

using namespace tlp;

SOMMapElement::SOMMapElement(tlp::Coord position, tlp::Size size, SOMMap *map,
                             tlp::ColorProperty *colorProperty) :
  som(map), position(position), size(size) {
  buildMainComposite(position, size, map);

  if (colorProperty)
    updateColors(colorProperty);

  computeNodeAreaSize();
}

SOMMapElement::~SOMMapElement() {
  reset(true);
}

void SOMMapElement::setData(SOMMap* map, tlp::ColorProperty* colorProperty) {
  som = map;
  reset(true);
  nodesMap.clear();
  buildMainComposite(position, size, som);

  if (colorProperty)
    updateColors(colorProperty);

  computeNodeAreaSize();
}

float SOMMapElement::computeMaximizedRadiusForHexagone(unsigned int width,
    unsigned int height, tlp::Size& size) {
  float ry = (4 * size.getH()) / (3 * height + 1) / 2;
  float rx = (size.getW() / (cos(DEGTORAD(30)) * width)) / 2;

  //Take the min
  return rx > ry ? ry : rx;
}

void SOMMapElement::buildMainComposite(tlp::Coord basePos, tlp::Size gridSize,
                                       SOMMap* map) {
  reset(true);

  SOMMap::SOMMapConnectivity conn = map->getConnectivity();
  tlp::node n;
  std::ostringstream oss;
  oss.str("");

  if (conn == SOMMap::six) {

    //Compute the best radius
    float r = computeMaximizedRadiusForHexagone(map->getWidth(),
              map->getHeight(), gridSize);

    float h = r / 2;
    float ri = cos(DEGTORAD(30)) * r;

    float top = basePos.getY() + gridSize.getH();

    for (unsigned int i = 0; i < map->getHeight(); ++i) {
      for (unsigned int j = 0; j < map->getWidth(); ++j) {

        float x = (j * ri * 2) + ri;
        float y = ((i + 1) * ((2 * r) - h)) - h;

        Coord center(basePos.getX() + x, top - y, 0);

        if (i % 2 != 0) {
          center.setX(center.getX() + ri);
        }

        n = map->getNodeAt(j, i);
        Color c = Color(255, 255, 255, 0);
        tlp::GlCircle *hex = new tlp::GlCircle(center, r, c, c, true, false,
                                               M_PI / 2.0f, 6);
        oss.str("");
        oss << j << "," << i;
        addGlEntity(hex, oss.str());
        nodesMap[n] = hex;

      }
    }
  }
  else {

    Coord elementSize(gridSize.getW() / map->getWidth(), gridSize.getH()
                      / map->getHeight(), 0);

    for (unsigned int i = 0; i < map->getHeight(); ++i) {
      for (unsigned int j = 0; j < map->getWidth(); ++j) {
        Coord topLeft(j * elementSize.getX(), (map->getHeight() - i)
                      * elementSize.getY(), 0);
        topLeft += basePos;
        Coord bottomRight(topLeft.getX() + elementSize.getX(), topLeft.getY()
                          - elementSize.getY(), 0);

        assert(topLeft.getX() < bottomRight.getX() && topLeft.getY()> bottomRight.getY());
        tlp::GlRect *rec = NULL;
        n = map->getNodeAt(j, i);
        Color c = Color(255, 255, 255, 0);
        rec = new tlp::GlRect(topLeft, bottomRight, c, c);
        oss.str("");
        oss << j << "," << i;
        addGlEntity(rec, oss.str());
        nodesMap[n] = rec;
      }
    }
  }
}

void SOMMapElement::updateColors(ColorProperty *newColor) {
  node n;
  SOMMap::SOMMapConnectivity connect = som->getConnectivity();
  forEach(n,som->getNodes()) {
    if (connect == SOMMap::six) {
      GlCircle *hex = (GlCircle*) nodesMap[n];
      hex->setFillColor(newColor->getNodeValue(n));
    }
    else {
      GlRect *rect = (GlRect*) nodesMap[n];
      rect->setBottomRightColor(newColor->getNodeValue(n));
      rect->setTopLeftColor(newColor->getNodeValue(n));
    }
  }
}

tlp::Coord SOMMapElement::getTopLeftPositionForElement(unsigned int x,
    unsigned int y) {
  Coord pos;

  if (som->getConnectivity() == SOMMap::six) {

    float r = computeMaximizedRadiusForHexagone(som->getWidth(),
              som->getHeight(), size);

    float h = r / 2;
    float ri = cos(DEGTORAD(30)) * r;

    if (y % 2 == 0)
      pos.setX(x * ri * 2);
    else
      pos.setX(ri * (x * 2 + 1));

    float ys = ((y + 1) * ((2 * r) - h)) - r;

    pos.setX(position.getX() + pos.getX());

    float top = position.getY() + size.getH();
    pos.setY(top - ys);

  }
  else {
    pos.set(x * (size.getW() / som->getWidth()), (som->getHeight() - y)
            * (size.getH() / som->getHeight()), 0);
    pos += position;
  }

  return pos;
}
tlp::Size SOMMapElement::getNodeAreaSize() {
  return nodeAreaSize;
}

void SOMMapElement::computeNodeAreaSize() {
  if (som->getConnectivity() == SOMMap::six) {
    //Compute the best radius
    float r = computeMaximizedRadiusForHexagone(som->getWidth(),
              som->getHeight(), size);
    float ri = cos(DEGTORAD(30)) * r;

    nodeAreaSize = Size(2 * ri, r, 0);
  }
  else {
    nodeAreaSize.set(size.getW() / som->getWidth(), size.getH()
                     / som->getHeight(), 0);
  }
}
