#include <algorithm>
#include "tulip/RectangleArea.h"
#include "tulip/Border.h"
using namespace std;

const float BORDERWIDTH_LIMIT = 2.2;

const int BORDEREDGLYPH[] ={100, 101};
const int BORDEREDGLYPHSIZE = sizeof(BORDEREDGLYPH) / sizeof(int);

//====================================================================
float tlp::evaluateBorderSize(int nodeLevel) {
    return BORDERWIDTH_ROOT * pow(BORDERWIDTH_LEVEL_FACTOR, nodeLevel);
}

//====================================================================
float tlp::evaluateBorderSize(int nodeLevel, const RectangleArea& NodeArea, int) {
    float limitSize  = min(NodeArea.getWidth()  / BORDERWIDTH_LIMIT,
                           NodeArea.getHeight() / BORDERWIDTH_LIMIT);
    float borderSize = tlp::evaluateBorderSize(nodeLevel);
    return min(limitSize, borderSize);
}

//====================================================================
bool tlp::isBorderedGlyph(int glyph) {
    return binary_search(BORDEREDGLYPH, BORDEREDGLYPH + BORDEREDGLYPHSIZE,
                         glyph);
}






