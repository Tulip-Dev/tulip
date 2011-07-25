#ifndef GRAPHPERSPECTIVE_H
#define GRAPHPERSPECTIVE_H

#include <tulip/Perspective.h>

class GraphPerspectiveProject;

namespace Ui {
class GraphPerspectiveMainWindowData;
}

class GraphPerspective : public tlp::Perspective {
  Ui::GraphPerspectiveMainWindowData *_ui;

  GraphPerspectiveProject *_perspectiveProject;

public:
  GraphPerspective(tlp::PerspectiveContext &c);
  virtual void construct(tlp::PluginProgress *);
};

#endif // GRAPHPERSPECTIVE_H
