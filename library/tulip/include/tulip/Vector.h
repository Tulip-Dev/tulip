//@TLPGEOLICENCE#

#ifndef _TLP_GEO_VECTOR_H
#define _TLP_GEO_VECTOR_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cassert>
#include <tulip/Array.h>
#include <tulip/tulipconf.h>

namespace tlp {
  /**
   * \addtogroup basic
   */ 
  /*@{*/
  /**
   * \brief class for mathematical vector
   *
   * Enables to create a Vector of Obj with a
   * Fixed size and provide Mathematical operation. Mathematical 
   * operators must be defined for Obj. 
   *
   * \author : David Auber auber@tulip-software.org
   * \version 0.0.1 24/01/2003 
   */
#define VECTOR Vector<Obj,SIZE>
#define TEMPLATEVECTOR template <typename Obj,unsigned int SIZE>

  TEMPLATEVECTOR
    class TLP_SCOPE Vector:public Array<Obj,SIZE> {
    public:
    /*      Vector(const VECTOR &v):Array<Obj,SIZE>(v) {}
	    Vector() {}*/
    inline VECTOR & operator*=(const Obj &);
    inline VECTOR & operator*=(const VECTOR &);
    inline VECTOR & operator/=(const Obj &);
    inline VECTOR & operator/=(const VECTOR &);
    inline VECTOR & operator+=(const Obj &);
    inline VECTOR & operator+=(const VECTOR &);
    inline VECTOR & operator-=(const Obj &);
    inline VECTOR & operator-=(const VECTOR &);
    inline VECTOR & operator^=(const VECTOR &);
    
    inline VECTOR operator+(const VECTOR &) const;
    inline VECTOR operator+(const Obj &) const;
    inline VECTOR operator-(const VECTOR &) const;
    inline VECTOR operator-(const Obj &) const;
    inline VECTOR operator*(const VECTOR &) const;
    inline VECTOR operator*(const Obj &) const ;
    inline VECTOR operator/(const VECTOR &) const;
    inline VECTOR operator/(const Obj &) const;
    inline VECTOR operator^(const VECTOR &) const;

    inline bool operator!=(const VECTOR &) const;
    inline bool operator==(const VECTOR &) const;
    inline VECTOR & fill(const Obj &obj);
    inline Obj norm () const;
    inline Obj dist (const VECTOR &) const;
    inline Obj dotProduct(const VECTOR &) const;
  };
  /*@}*/
}


#include "./cxx/Vector.cxx"
#endif
