/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#ifndef _Tulip_BASEGRAPHICSVIEWCOMPONENTMACRO_H
#define _Tulip_BASEGRAPHICSVIEWCOMPONENTMACRO_H

#include "tulip/BaseGraphicsViewComponent.h"


namespace tlp {

#if (QT_VERSION >= QT_VERSION_CHECK(4, 4, 0))
#define GRAPHICSVIEWEXTENSION(C,N,BN,A,D,I,R)	\
class C :public BaseGraphicsViewComponent { \
public : \
C():BaseGraphicsViewComponent(BN){} \
virtual ~C() {} \
}; \
VIEWPLUGIN(C,N,A,D,I,R)
#else
#define GRAPHICSVIEWEXTENSION(C,N,BN,A,D,I,R)
#endif


}

#endif






