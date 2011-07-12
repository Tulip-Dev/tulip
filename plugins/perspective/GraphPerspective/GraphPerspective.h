#ifndef GRAPHPERSPECTIVE_H
#define GRAPHPERSPECTIVE_H

#include <tulip/Perspective.h>

class GraphPerspective : public tlp::Perspective {
public:
  GraphPerspective(tlp::PerspectiveContext &c);
  virtual void construct();
};

#endif // GRAPHPERSPECTIVE_H
