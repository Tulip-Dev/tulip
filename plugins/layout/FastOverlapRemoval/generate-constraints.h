/**
 * \brief Functions to automatically generate constraints for the
 * rectangular node overlap removal problem.
 *
 * Authors:
 *   Tim Dwyer <tgdwyer@gmail.com>
 *
 * Copyright (C) 2005 Authors
 *
 * Released under GNU LGPL.  Read the file 'COPYING' for more information.
 */
#ifndef SEEN_REMOVEOVERLAP_GENERATE_CONSTRAINTS_H
#define SEEN_REMOVEOVERLAP_GENERATE_CONSTRAINTS_H
#include <iostream>

namespace vpsc {
class Rectangle {
  friend std::ostream& operator <<(std::ostream &os, const Rectangle &r);
public:
  Rectangle(double x, double X, double y, double Y,
            const double& xBorder, const double& yBorder);
  double getMaxX() const {
    return maxX+xBorder;
  }
  double getMaxY() const {
    return maxY+yBorder;
  }
  double getMinX() const {
    return minX;
  }
  double getMinY() const {
    return minY;
  }
  double getMinD(unsigned const d) const {
    return ( d == 0 ? getMinX() : getMinY() );
  }
  double getMaxD(unsigned const d) const {
    return ( d == 0 ? getMaxX() : getMaxY() );
  }
  double getCentreX() const {
    return minX+width()/2.0;
  }
  double getCentreY() const {
    return minY+height()/2.0;
  }
  double width() const {
    return getMaxX()-minX;
  }
  double height() const {
    return getMaxY()-minY;
  }
  void moveCentreX(double x) {
    moveMinX(x-width()/2.0);
  }
  void moveCentreY(double y) {
    moveMinY(y-height()/2.0);
  }
  void moveMinX(double x) {
    maxX=x+width()-xBorder;
    minX=x;
  }
  void moveMinY(double y) {
    maxY=y+height()-yBorder;
    minY=y;
  }
  inline double overlapX(Rectangle *r) const {
    if (getCentreX() <= r->getCentreX() && r->minX < getMaxX())
      return getMaxX() - r->minX;

    if (r->getCentreX() <= getCentreX() && minX < r->getMaxX())
      return r->getMaxX() - minX;

    return 0;
  }
  inline double overlapY(Rectangle *r) const {
    if (getCentreY() <= r->getCentreY() && r->minY < getMaxY())
      return getMaxY() - r->minY;

    if (r->getCentreY() <= getCentreY() && minY < r->getMaxY())
      return r->getMaxY() - minY;

    return 0;
  }
private:
  double minX,maxX,minY,maxY;
  const double& xBorder;
  const double& yBorder;
};


class Variable;
class Constraint;
class Node;

typedef enum {Open, Close} EventType;
struct Event {
  EventType type;
  Node *v;
  double pos;
  Event(EventType t, Node *v, double p) : type(t),v(v),pos(p) {};
  void init(EventType t, Node *n, double p) {
    type = t;
    v = n;
    pos = p;
  }
};

class ConstraintsGenerator {
  Event **events;
  const unsigned int n;

public:

  ConstraintsGenerator(unsigned int nbRectangles): n(nbRectangles) {
    events = new Event*[2*n];
  }

  ~ConstraintsGenerator() {
    delete [] events;
  }

  // returns number of constraints generated
  int generateXConstraints(Rectangle** rs, Variable** vars, Constraint** &cs, const bool useNeighbourLists);
  int generateYConstraints(Rectangle** rs, Variable** vars, Constraint** &cs);
};

}

#endif // SEEN_REMOVEOVERLAP_GENERATE_CONSTRAINTS_H
