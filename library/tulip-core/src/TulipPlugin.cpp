#include "tulip/TulipPlugin.h"

#include <tulip/TulipRelease.h>

using namespace tlp;

QString TulipPlugin::tulipRelease() const {
  return TULIP_MM_RELEASE;
}
