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
//@TLPGEOLICENCE#

#ifndef _TLP_GEO_ARRAY_H
#define _TLP_GEO_ARRAY_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <cassert>
#include <iostream>
#include <tulip/tulipconf.h>

namespace tlp {
  /**
   * @addtogroup basic
   */ 
  /*@{*/
  /**
   * @brief Fixed-size array encapsulation.
   * In debug mode, a bound check is performed at each access.
   * 
   * @author : David Auber auber@tulip-software.org
   * @version 0.0.1 24/01/2003 
   */
  template <typename Obj,unsigned int SIZE>
  struct TLP_SCOPE Array {
    Obj array[SIZE];
    inline Obj  operator[](const unsigned int i) const;
    inline Obj& operator[](const unsigned int i);
  };
  
  template <typename Obj,unsigned int SIZE>
  std::ostream& operator<<(std::ostream &os,const Array<Obj,SIZE> &array);
  template <typename Obj,unsigned int SIZE>
  std::istream& operator>>(std::istream &is, Array<Obj,SIZE> &);

  /*@}*/
}

#include "cxx/Array.cxx"

#endif
