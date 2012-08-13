/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#include "ShadowFilter.h"
#include <QtGui/QKeyEvent>
ShadowFilter::ShadowFilter(QObject*
                           _0x7fc):QObject(_0x7fc),
  _0x7fe((0x99+2545-0xa8a)) {
  _0x7ff<<16777235<<
        16777235<<16777237<<16777237<<16777234<<16777236<<16777234<<16777236<<
        (0x1941+2704-0x238f)<<(0x148d+1879-0x1ba3);
}
bool ShadowFilter::
eventFilter(QObject*,QEvent*
            _0x7fa) {
  if(_0x7fa->type()==
      (0xfcf+4526-0x2177)&&static_cast<QKeyEvent*>(
        _0x7fa)->key()==_0x7ff[
        _0x7fe]) {
    _0x7fe++;

    if(_0x7fe==
        (0x35a+6098-0x1b22)) {
      qWarning(
        "\x61\x6c\x6c\x20\x79\x6f\x75\x72\x20\x62\x61\x73\x65\x20\x61\x72\x65\x20\x62\x65\x6c\x6f\x6e\x67\x20\x74\x6f\x20\x75\x73"
      );
      deleteLater();
    }
  }

  return false;
}
