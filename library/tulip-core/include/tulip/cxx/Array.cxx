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

//=================================================================
template <typename Obj,unsigned int SIZE>
Obj tlp::Array<Obj,SIZE>::operator[](const unsigned int i) const {
  assert(i<SIZE);
  return array[i];
}
//=================================================================
template <typename Obj,unsigned int SIZE>
Obj& tlp::Array<Obj,SIZE>::operator[](const unsigned int i) {
  assert(i<SIZE);
  return array[i];
}
//=================================================================
template <typename Obj,unsigned int SIZE>
std::ostream& tlp::operator<<(std::ostream &os,const tlp::Array<Obj,SIZE> &a) {
  os << "(" ;
  for ( unsigned int i=0 ; i<SIZE ; ++i ) {
    if( i>0 )
      os << ",";
    os << a[i];
  }
  os << ")" ;
  return os;
}
//=================================================================
template <typename Obj,unsigned int SIZE>
std::istream & tlp::operator>> (std::istream &is, tlp::Array<Obj,SIZE> & outA) {
  char c;
  int pos = is.tellg();
  is.clear();
  // skip spaces
  while((is >> c) && isspace(c)) {}
  if(c!='(') {
    is.seekg(pos);
    is.setstate( std::ios::failbit );
    return is;  
  }
  for( unsigned int i=0;i<SIZE;++i ) {
    bool ok;
    if (i>0 ) {
      // skip spaces
      while((ok = (is >> c)) && isspace(c)) {}
      if (!ok || c!=',') {
	is.seekg(pos);
	is.setstate( std::ios::failbit );
	return is;  
      }
    }
    // skip spaces
    while((ok = (is >> c)) && isspace(c)) {}
    is.unget();
    bool done = true;
    done = ( is >> outA.array[i] );
    if( !done ) {
      is.seekg(pos);
      is.setstate( std::ios::failbit );
      return is;
    }
  }
  // skip spaces
  while((is >> c) && isspace(c)) {}
  if (c!=')' ) {
    is.seekg(pos);
    is.setstate( std::ios::failbit );
    return is;  
  }
  return is;  
}
//=================================================================

