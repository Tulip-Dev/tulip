#include "tulip/PreferenceManager.h"

#ifdef _WIN32
#ifdef DLL_EXPORT
tlp::PreferenceManager* tlp::PreferenceManager::inst=0;
#endif
#else
tlp::PreferenceManager* tlp::PreferenceManager::inst=0;
#endif

using namespace std;

namespace tlp
{
}
