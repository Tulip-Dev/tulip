/*
 * Authors: Jérémy Compostella, Jean Darracq, Benjamin Muller,
 *          Fabrice Rochambeau, Fabiani Simplice, Jyl Cristoff Zobeide
 * 
 * Email : jcompost@etu.u-bordeaux1.fr, jdarracq@etu.u-bordeaux1.fr,
 *         bmuller@etu.u-bordeaux1.fr, frochamb@etu.u-bordeaux1.fr,
 *         fsimplic@etu.u-bordeaux1.fr, jczobeid@etu.u-bordeaux1.fr.
 *
 * $Id $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation; either version 2 of the License, or     
 * (at your option) any later version.
 */

#ifndef BMDLINK_H
#define BMDLINK_H

#ifndef DOXYGEN_NOTFOR_USER

template <typename TYPE> struct BmdList;
template <typename TYPE> struct BmdListTest;

namespace tlp {
  template <typename TYPE>
  class BmdLink {
    
  friend struct BmdList<TYPE>;
  friend struct BmdListTest<TYPE>;
  public:
    TYPE getData() {
      return data;
    }
    BmdLink *prev(){
      return pre;
    }
    BmdLink *succ(){
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
