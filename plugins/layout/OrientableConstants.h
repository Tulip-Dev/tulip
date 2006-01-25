#ifndef ORIENTABLECONSTANT_H
#define ORIENTABLECONSTANT_H

enum orientationType {
    ORI_DEFAULT                     = 0,
    ORI_INVERSION_HORIZONTAL        = 1,
    ORI_INVERSION_VERTICAL          = 2,
    ORI_INVERSION_Z                 = 4,
    ORI_ROTATION_XY                 = 8
};

enum {
    UP_DOWN     = 0,
    DOWN_UP     = 1,
    RIGHT_LEFT  = 2,
    LEFT_RIGHT  = 3
};

orientationType maskFromPanel(bool reverseOrientation, bool reverseDirection);

#endif
