#include "tulip/Perspective.h"

namespace tlp {

Perspective::Perspective(PerspectiveContext &c) {
  _mainWindow = c.mainWindow;
}

bool Perspective::isCompatible(tlp::TulipProject *) {
  return false;
}

}
