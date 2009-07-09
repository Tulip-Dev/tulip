#include "tulip/GlewManager.h"

#include <iostream>

//====================================================
#ifdef _WIN32
#ifdef DLL_EXPORT
tlp::GlewManager* tlp::GlewManager::inst=0;
#endif
#else
tlp::GlewManager* tlp::GlewManager::inst=0;
#endif

using namespace std;

namespace tlp
{
}
