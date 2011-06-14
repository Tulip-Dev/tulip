#include "tulip/Perspective.h"

namespace tlp {

Perspective::Perspective(PerspectiveContext &c) {
  _mainWindow = c.mainWindow;
  _project = c.project;
  _externalFile = c.externalFile;
  _parameters = c.parameters;
}

Perspective::~Perspective() {
  delete _project;
  delete _mainWindow;
}

bool Perspective::isCompatible(tlp::TulipProject *) {
  return false;
}

}
