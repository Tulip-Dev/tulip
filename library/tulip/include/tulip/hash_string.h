#ifndef HASH_STRING_H
#define HASH_STRING_H

#ifndef DOXYGEN_NOTFOR_DEVEL
#include <string>

namespace stdext {
	/**
   * \addtogroup basic
   */ 
  /*@{*/
  /**
   * \brief provides string hash method 
   *
   *
   * Author : <a href="mailto:auber@tulip-software.org>David Auber</A>
   * Version 0.0.1 24/01/2003 
   */
  template<> struct hash<const std::string>{
    size_t operator()(const std::string &s) const {return hash<const char *>()(s.c_str()); }
  };
  template<> struct hash<std::string>{
    size_t operator()(const std::string &s) const {return hash<const char *>()(s.c_str()); }
  };
   /*@}*/
}

#endif //DOXYGEN_NOTFOR_DEVEL
#endif //HASH_STRING_H
