#include "Orientation.h"

//====================================================================
orientationType maskFromPanel(bool reverseOrientation, bool reverseDirection) {
    if (reverseOrientation)
        if (reverseDirection)
           return orientationType (ORI_ROTATION_XY | ORI_INVERSION_HORIZONTAL);
        else
           return ORI_ROTATION_XY;
    else if (reverseDirection)
        return ORI_INVERSION_VERTICAL;
    else
        return ORI_DEFAULT;
}
