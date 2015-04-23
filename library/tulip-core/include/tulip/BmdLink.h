/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
///@cond DOXYGEN_HIDDEN


#ifndef BMDLINK_H
#define BMDLINK_H

#ifndef DOXYGEN_NOTFOR_USER

template <typename TYPE> class BmdList;
template <typename TYPE> class BmdListTest;

namespace tlp {
template <typename TYPE>
class BmdLink {

  friend class BmdList<TYPE>;
  friend class BmdListTest<TYPE>;
public:
  TYPE getData() {
    return data;
  }
  BmdLink *prev() {
    return pre;
  }
  BmdLink *succ() {
    return suc;
  }
  //protected:
  TYPE data;
  BmdLink *pre;
  BmdLink *suc;
  BmdLink(TYPE a, BmdLink<TYPE> *pre, BmdLink<TYPE> *suc);
};
}
#include <tulip/cxx/BmdLink.cxx>

#endif
#endif
///@endcond
