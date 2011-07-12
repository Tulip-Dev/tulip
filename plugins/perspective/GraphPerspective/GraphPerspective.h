#ifndef GRAPHPERSPECTIVE_H
#define GRAPHPERSPECTIVE_H

#include <tulip/Perspective.h>

namespace Ui {
class GraphPerspectiveMainWindowData;
}

class GraphPerspective : public tlp::Perspective {
  Ui::GraphPerspectiveMainWindowData *_ui;

public:
  GraphPerspective(tlp::PerspectiveContext &c);
  virtual void construct();
};

#endif // GRAPHPERSPECTIVE_H
