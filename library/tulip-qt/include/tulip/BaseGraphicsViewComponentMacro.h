//-*-c++-*-
/**
 Author: Antoine Lambert Morgan Mathiaut
 Last modification : 08/12/2008
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
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






