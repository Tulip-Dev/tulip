#include "tulip/TulipSettings.h"

TulipSettings *TulipSettings::_instance = 0;

TulipSettings::TulipSettings(): QSettings("TulipSoftware","Tulip") {
}
