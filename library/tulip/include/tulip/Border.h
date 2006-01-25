#ifndef BORDER_H
#define BORDER_H

#include "tulip/RectangleArea.h"

const float BORDERWIDTH_LEVEL_FACTOR = .75;
const float BORDERWIDTH_ROOT = 15;

bool isBorderedGlyph(int glyph);

float evaluateBorderSize(int nodeLevel);
float evaluateBorderSize(int nodeLevel, const RectangleArea& NodeArea,
                         int maxLevel = 0);
                         
#endif
