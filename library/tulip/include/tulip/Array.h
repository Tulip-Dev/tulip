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
   * \addtogroup basic
   */ 
  /*@{*/
  /**
   * \brief class for array encapsulation
   *
   * Enables to create an Array of Obj with a
   * limited size. Out of bound accesses are only checked
   * in debug mode.
   * 
   * \author : David Auber auber@tulip-software.org
   * \version 0.0.1 24/01/2003 
   */
  template <typename Obj,unsigned int SIZE>
  struct TLP_SCOPE Array {
    Obj array[SIZE];
    inline Obj  operator[](const unsigned int i) const;
    inline Obj& operator[](const unsigned int i);
  };
  /**
   * Write array in a stream
   */
  template <typename Obj,unsigned int SIZE>
  std::ostream& operator<<(std::ostream &os,const Array<Obj,SIZE> &);
  template <typename Obj,unsigned int SIZE>
  std::istream& operator>>(std::istream &is, Array<Obj,SIZE> &);

  /*@}*/
}

#include "./cxx/Array.cxx"

#endif


