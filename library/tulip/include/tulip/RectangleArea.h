#ifndef RECTANGLEAREA_H
#define RECTANGLEAREA_H

#include "Coord.h"
#include "Size.h"

namespace tlp {

/*
  This is used by some plugins:
  glyph/SquareBorder
  glyph/SquareBorderTextured
  layout/ImprovedWalker
  layout/SquarifiedTreeMap
*/


enum directionType {
    HORIZONTAL_DIRECTION,
    VERTICAL_DIRECTION
};

class TLP_SCOPE RectangleArea {
public:
    RectangleArea(float x = 0, float y = 0, float width = 0, float height = 0);
    explicit RectangleArea(const Coord&,const Size&);
    explicit RectangleArea(const Size&);
    explicit RectangleArea(const Coord&);
    void     set2D(float x, float y, float width, float height);

    float&   getX();
    float&   getY();
    float&   getVirtualX();
    float&   getVirtualY();
    float&   getWidth();
    float&   getHeight();
    float&   getVirtualWidth();
    float&   getVirtualHeight();  

    float    getX() const;
    float    getY() const;
    float    getVirtualX() const;
    float    getVirtualY() const;
    float    getWidth() const;
    float    getHeight() const;
    float    getVirtualWidth() const;
    float    getVirtualHeight() const;

    void     setDirection(directionType direction);
    directionType getDirection() const;

    float    getArea() const;    
    
    Coord    getCoord() const;
    Coord    getCoord2() const;
     
    Coord    getCenterCoord() const;
    Size     getSize() const;

protected:
    float x;
    float y;
    float width;
    float height; 

    directionType direction;
};

bool intersect(const RectangleArea& rect1, const RectangleArea& rect2);

}

#endif
