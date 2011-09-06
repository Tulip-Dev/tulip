#ifndef SIZEPROPERTYANIMATION_H_
#define SIZEPROPERTYANIMATION_H_

#include "tulip/VectorPropertyAnimation.h"
#include <tulip/SizeProperty.h>

namespace tlp {
typedef VectorPropertyAnimation<tlp::SizeProperty, tlp::Size, float, 3> SizePropertyAnimation;
}

#endif /* SIZEPROPERTYANIMATION_H_ */
