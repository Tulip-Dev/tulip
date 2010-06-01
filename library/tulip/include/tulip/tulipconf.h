#ifndef TULIPCONF_H
#define TULIPCONF_H

#if (__GNUC__ < 3)
#  define stdext std
#  define _DEPRECATED
#  include <stl_hash_fun.h>
#else
#    define _DEPRECATED __attribute__ ((deprecated))
#    define stdext __gnu_cxx
#  if  (__GNUC_MINOR__ < 4 && __GNUC__ < 4)
#    include <ext/stl_hash_fun.h>
#  elif (__GNUC_MINOR__ < 3)
#    include <ext/hash_fun.h>
#  endif
#endif

#ifdef _WIN32
# 	ifdef DLL_TULIP
#		define TLP_SCOPE         __declspec(dllexport)
# 	else
#		define TLP_SCOPE       __declspec(dllimport)
# 	endif
#endif
#ifndef TLP_SCOPE
#define TLP_SCOPE           
#endif

#ifdef _WIN32
# 	ifdef DLL_TULIP_GL
#		define TLP_GL_SCOPE         __declspec(dllexport)
# 	else
#		define TLP_GL_SCOPE       __declspec(dllimport)
# 	endif
#endif
#ifndef TLP_GL_SCOPE
#define TLP_GL_SCOPE           
#endif

#ifdef _WIN32
# 	ifdef DLL_TULIP_QT2
#		define TLP_QT_SCOPE       __declspec(dllimport)
#	else
# 		ifdef DLL_TULIP_QT
#			define TLP_QT_SCOPE         __declspec(dllexport)
# 		else
#			define TLP_QT_SCOPE       __declspec(dllimport)
# 		endif
#	endif
#endif
#ifndef TLP_QT_SCOPE
#define TLP_QT_SCOPE           
#endif

#ifdef _WIN32
#   ifdef DLL_TULIP_QT2
#		define TLP_QT_SIMPLE_SCOPE 
#	else 
# 		ifdef DLL_TULIP_QT
#			define TLP_QT_SIMPLE_SCOPE  __declspec(dllexport)
# 		else
#			define TLP_QT_SIMPLE_SCOPE       
# 		endif
#	endif
#endif
#ifndef TLP_QT_SIMPLE_SCOPE
#define TLP_QT_SIMPLE_SCOPE           
#endif

#ifdef _WIN32
# 	ifdef DLL_TULIP_PLUGINSMANAGER
#		define TLP_PLUGINSMANAGER_SCOPE         __declspec(dllexport)
# 	else
#		define TLP_PLUGINSMANAGER_SCOPE       __declspec(dllimport)
# 	endif
#endif
#ifndef TLP_PLUGINSMANAGER_SCOPE
#define TLP_PLUGINSMANAGER_SCOPE           
#endif

#endif //TULIPCONF_H
