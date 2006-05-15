using namespace tlp;

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
  if( !(is >> c) || c!='(' ) {
    is.seekg(pos);
    is.setstate( std::ios::failbit );
    return is;  
  }
  for( unsigned int i=0;i<SIZE;++i ) {
    if( i>0 && ( !(is >> c) || c!=',' ) ) {
      is.seekg(pos);
      is.setstate( std::ios::failbit );
      return is;  
    }
    bool done = true;
    done = ( is >> outA.array[i] );
    if( !done ) {
      is.seekg(pos);
      is.setstate( std::ios::failbit );
      return is;
    }
  }
  if( !(is >> c) || c!=')' ) {
    is.seekg(pos);
    is.setstate( std::ios::failbit );
    return is;  
  }
  return is;  
}
//=================================================================

