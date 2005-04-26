//-*-c++-*-

#include <cassert>
#include <cmath>
#include <iostream>

#include "tulip/Coord.h"

//float Coord::dist(const Coord &c) const {return sqrt(((*this)[0]-c[0])*((*this)[0]-c[0])+((*this)[1]-c[1])*((*this)[1]-c[1])+((*this)[2]-c[2])*((*this)[2]-c[2]));}
//float Coord::norm() const {return sqrt((*this)[0]*(*this)[0]+(*this)[1]*(*this)[1]+(*this)[2]*(*this)[2]);}

/*
std::ostream & operator<< (std::ostream &os, const Coord &c) {
  os << '(';
  os << c.v[0];
  for (unsigned int i=1;i<3;++i)
    os << "," <<  c.v[i];
  os << ')';
  return os;
}

std::istream & operator>> (std::istream &is, Coord & outC) {
  char c;
  int pos = is.tellg();
  is.clear();

  if( !(is >> c) || c!='(' )
  	goto failed;

  for( unsigned int i=0;i<3;++i ) {
	  if( i>0 && ( !(is >> c) || c!=',' ) )
	  	goto failed;
	  if( !(is >> outC.v[i]) )
    	goto failed;
  }

  if( !(is >> c) || c!=')' )
  	goto failed;

  return is;  

failed:

  is.seekg(pos);
  is.setstate( std::ios::failbit );
  return is;  
}
*/

