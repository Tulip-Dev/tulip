#ifndef BORDER_H
#define BORDER_H

#include "tulip/RectangleArea.h"

namespace tlp {

const float BORDERWIDTH_LEVEL_FACTOR = .75;
const float BORDERWIDTH_ROOT = 15;

bool TLP_SCOPE isBorderedGlyph(int glyph);

float TLP_SCOPE evaluateBorderSize(int nodeLevel);
float TLP_SCOPE evaluateBorderSize(int nodeLevel, const RectangleArea& NodeArea,
				   int maxLevel = 0);
                         
}
#endif
