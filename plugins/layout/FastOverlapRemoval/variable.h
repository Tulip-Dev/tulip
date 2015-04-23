/**
 *
 * Authors:
 *   Tim Dwyer <tgdwyer@gmail.com>
 *
 * Copyright (C) 2005 Authors
 *
 * Released under GNU LGPL.  Read the file 'COPYING' for more information.
 */
#ifndef SEEN_REMOVEOVERLAP_VARIABLE_H
#define SEEN_REMOVEOVERLAP_VARIABLE_H

#include <vector>
#include <iostream>
#include "block.h"

namespace vpsc {

class Constraint;
typedef std::vector<Constraint*> Constraints;
class Variable {
  friend std::ostream& operator <<(std::ostream &os, const Variable &v);
public:
  double desiredPosition;
  const double weight;
  double offset;
  Block *block;
  bool visited;
  Constraints in;
  Constraints out;
  char *toString();
  inline Variable(const double desiredPos = 0, const double weight = 1)
    : desiredPosition(desiredPos)
    , weight(weight)
    , offset(0)
    , block(NULL)
    , visited(false) {
  }
  inline double position() const {
    return block->posn+offset;
  }
  //double position() const;
  ~Variable(void) {
    in.clear();
    out.clear();
  }

  size_t id() const {
    return (size_t) this;
  }
};
}
#endif // SEEN_REMOVEOVERLAP_VARIABLE_H
