#ifndef CIRCLEGLYPH_H
#define CIRCLEGLYPH_H

#include <tulip/Glyph.h>
#include <tulip/TulipViewSettings.h>

namespace tlp {

class TLP_GLES_SCOPE CircleGlyph : public Glyph {

public:

  GLYPHINFORMATION("2D - Circle", "2D - Circle Extremity", "Antoine Lambert", "19/05/2016", "Circle", "1.0", NodeShape::Circle)

  CircleGlyph();

  void getIncludeBoundingBox(BoundingBox &boundingBox);

};

}

#endif // CIRCLEGLYPH_H
