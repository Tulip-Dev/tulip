/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#include <algorithm>

#include "OrientableLayout.h"

using namespace tlp;

//====================================================================
OrientableLayout::OrientableLayout(LayoutProperty* layoutParam,
                                   orientationType mask) {
    layout = layoutParam;
    setOrientation(mask);
}

//====================================================================
void OrientableLayout::setOrientation(orientationType mask) {
    orientation = mask;

    readX  = &OrientableCoord::getInvertedX;
    readY  = &OrientableCoord::getInvertedY;
    readZ  = &Coord::getZ;
    writeX = &OrientableCoord::setInvertedX;
    writeY = &OrientableCoord::setInvertedY;
    writeZ = &Coord::setZ;

    if (orientation & ORI_INVERSION_HORIZONTAL) {
        readX   = &Coord::getX;
        writeX  = &Coord::setX;
    }
        
    if (orientation & ORI_INVERSION_VERTICAL) {
        readY   = &Coord::getY;
        writeY  = &Coord::setY;
    }

    if (orientation & ORI_INVERSION_Z) {
        readZ   = &OrientableCoord::getInvertedZ;
        writeZ  = &OrientableCoord::setInvertedZ;
    }

    if (orientation & ORI_ROTATION_XY) {
        std::swap(readX, readY);
        std::swap(writeX, writeY);
    }    
}

//====================================================================
OrientableCoord OrientableLayout::createCoord(const float x, const float y,
                                              const float z) {
    return OrientableCoord(this, x, y, z);
}

//====================================================================
OrientableCoord OrientableLayout::createCoord(const Coord& v) {
    return OrientableCoord(this, v);
}

//====================================================================
std::vector<OrientableCoord> OrientableLayout::
                             convertEdgeLinetype(const std::vector<Coord>& v) {
    std::vector<OrientableCoord>  orientableLine;
    CoordLineType::const_iterator itCoordLineType = v.begin();
    for ( ; itCoordLineType < v.end() ; itCoordLineType++ )  {
        OrientableCoord newOrientableCoord(this, *itCoordLineType);
        orientableLine.push_back(newOrientableCoord);
    }
    return orientableLine;
}

//====================================================================
void OrientableLayout::setAllNodeValue(const PointType& v) {
    layout->setAllNodeValue(v);
}

//====================================================================
void OrientableLayout::setNodeValue(node n, const PointType& v) {
    layout->setNodeValue(n, v);
}

//====================================================================
OrientableLayout::PointType OrientableLayout::getNodeValue(const node n) {
    return OrientableCoord(this, layout->getNodeValue(n));
}

//====================================================================
OrientableLayout::PointType OrientableLayout::getNodeDefaultValue() {
    return OrientableCoord(this, layout->getNodeDefaultValue());
}

//====================================================================
void OrientableLayout::setAllEdgeValue(const LineType& v) {
    CoordLineType vecCoord(v.begin(), v.end());
    layout->setAllEdgeValue(vecCoord);
}

//====================================================================
void OrientableLayout::setEdgeValue(const edge e, const LineType& v) {
    CoordLineType vecCoord(v.begin(), v.end());
    layout->setEdgeValue(e, vecCoord);
}

//====================================================================
OrientableLayout::LineType OrientableLayout::getEdgeValue(const edge e) {
    return convertEdgeLinetype(layout->getEdgeValue(e));
}

//====================================================================
OrientableLayout::LineType OrientableLayout::getEdgeDefaultValue() {
    return convertEdgeLinetype(layout->getEdgeDefaultValue());
}
