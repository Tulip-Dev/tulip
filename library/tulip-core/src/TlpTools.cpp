/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>
#include <sstream>
#include <clocale>
#include <cerrno>
#include <random>
#include <chrono>

#ifndef _WIN32
#include <sys/stat.h>
#include <unistd.h>
#include <gzstream.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <utf8.h>
// msys2 build hack
#define __CRT__NO_INLINE
#include <gzstream.h>
#undef __CRT__NO_INLINE

#ifdef _MSC_VER
#include <dbghelp.h>
#endif
#ifdef __GLIBCXX__
#include <ext/stdio_filebuf.h>
#endif
#else
#include <dirent.h>
#include <dlfcn.h>
#endif

#include <tulip/TulipException.h>
#include <tulip/TlpTools.h>
#include <tulip/Plugin.h>
#include <tulip/PluginLoader.h>
#include <tulip/PropertyTypes.h>
#include <tulip/TulipRelease.h>

using namespace std;
using namespace tlp;

#ifndef __EMSCRIPTEN__
static const char *TULIP_PLUGINS_PATH_VARIABLE = "TLP_PLUGINS_PATH";
#endif

// the relative path (a string), from the install dir
// of the directory where the tulip libraries are installed
#define TULIP_INSTALL_LIBDIR_STR STRINGIFY(TULIP_INSTALL_LIBDIR)

string tlp::TulipLibDir;
string tlp::TulipPluginsPath;
string tlp::TulipBitmapDir;
string tlp::TulipShareDir;
// indicates that the end of the program has been reached
bool tlp::TulipProgramExiting = false;

// must be called at exit to ensure a clean exit
static void atExit() {
  tlp::TulipProgramExiting = true;
}

// as the exit handlers are called in reverse order
// of their registration our own has to be registered after
// this shared lib has been loaded (see initTulipLib) and
// after the plugins are loaded (see PluginLibraryLoader::loadPlugins...)
void tlp::registerTulipExitHandler() {
  atexit(atExit);
}

#ifdef _WIN32
const char tlp::PATH_DELIMITER = ';';
#else
const char tlp::PATH_DELIMITER = ':';
#endif

#ifndef __EMSCRIPTEN__
// A function that retrieves the Tulip libraries directory based on
// the path of the loaded shared library libtulip-core-X.Y.[dll, so, dylib]
extern "C" {
char *getTulipLibDir(char *buf) {
  std::string tulipLibDir;
  std::string libTulipName;

#ifdef _WIN32
#ifdef __MINGW32__
  libTulipName =
      "libtulip-core-" + getMajor(TULIP_VERSION) + "." + getMinor(TULIP_VERSION) + ".dll";
#else
  libTulipName = "tulip-core-" + getMajor(TULIP_VERSION) + "_" + getMinor(TULIP_VERSION) + ".dll";
#endif
  HMODULE hmod = GetModuleHandle(libTulipName.c_str());

  if (hmod != nullptr) {
    TCHAR szPath[512 + 1];
    DWORD dwLen = GetModuleFileName(hmod, szPath, 512);

    if (dwLen > 0) {
      std::string tmp = szPath;
      std::replace(tmp.begin(), tmp.end(), '\\', '/');
      tulipLibDir = tmp.substr(0, tmp.rfind('/') + 1) + "../" + TULIP_INSTALL_LIBDIR_STR;
    }
  }

#else
#ifdef __APPLE__
  libTulipName =
      "libtulip-core-" + getMajor(TULIP_VERSION) + "." + getMinor(TULIP_VERSION) + ".dylib";
#else
  libTulipName = "libtulip-core-" + getMajor(TULIP_VERSION) + "." + getMinor(TULIP_VERSION) + ".so";
#endif
  void *ptr = dlopen(libTulipName.c_str(), RTLD_LAZY);

  if (ptr != nullptr) {
    void *symbol = dlsym(ptr, "getTulipLibDir");

    if (symbol != nullptr) {
      Dl_info info;
      if (dladdr(symbol, &info)) {
        std::string tmp = info.dli_fname;
        tulipLibDir = tmp.substr(0, tmp.rfind('/') + 1);
        tulipLibDir.append("../").append(TULIP_INSTALL_LIBDIR_STR);
      }
    }
    dlclose(ptr);
  }

#endif
  return strcpy(buf, tulipLibDir.c_str());
}
}

// throw an exception if an expected directory does not exist
static void checkDirectory(std::string dir, bool tlpDirSet, bool throwEx) {
  // remove ending / separator if any
  // bug detected on Windows
  if (dir[dir.length() - 1] == '/')
    dir.erase(dir.length() - 1);

  if (!pathExist(dir)) {
    std::stringstream ess;
    ess << "Error - " << dir << ":" << std::endl << strerror(errno) << std::endl;
    if (tlpDirSet)
      ess << std::endl << "Check your TLP_DIR environment variable";
    if (throwEx)
      throw TulipException(ess.str());
    else if ( // output only if not in a python installed wheel
        (dir.find("/tulip/native/") == string::npos) &&
        // and not building tulip-core bindings
        (dir.find("library/tulip-core/src") == string::npos))
      tlp::error() << ess.str();
  }
}

//=========================================================
void tlp::initTulipLib(const char *appDirPath) {
  if (!TulipShareDir.empty()) // already initialized
    return;

  char *getEnvTlp;
  string::size_type pos;
  // we use curDir to ease debugging
  // Tulip..Dir global variables may be invisible
  // when using gdb
  std::string curDir;

  getEnvTlp = getenv("TLP_DIR");

  if (getEnvTlp == nullptr) {
    if (appDirPath) {
#ifdef _WIN32
      curDir = std::string(appDirPath) + "/../" + TULIP_INSTALL_LIBDIR_STR;
#else
      // one dir up to initialize the lib dir
      curDir.append(appDirPath, strlen(appDirPath) - strlen(strrchr(appDirPath, '/') + 1));
      curDir.append(TULIP_INSTALL_LIBDIR_STR);

#endif
    } else {
      char buf[1024];
      // if no appDirPath is provided, retrieve dynamically the Tulip lib dir
      curDir = getTulipLibDir(buf);
    }
  } else
    curDir = string(getEnvTlp);

#ifdef _WIN32
  // ensure it is a unix-style path
  pos = curDir.find('\\', 0);

  while (pos != string::npos) {
    curDir[pos] = '/';
    pos = curDir.find('\\', pos);
  }

#endif

  // ensure it is '/' terminated
  if (curDir[curDir.length() - 1] != '/')
    curDir += '/';

  // check that TulipLibDir exists
  bool tlpDirSet = (getEnvTlp != nullptr);
  bool throwExOnCheck = appDirPath != nullptr;

  checkDirectory(TulipLibDir = curDir, tlpDirSet, throwExOnCheck);

  getEnvTlp = getenv(TULIP_PLUGINS_PATH_VARIABLE);

  if (getEnvTlp != nullptr) {
    curDir = string(getEnvTlp);
#ifdef _WIN32
    // ensure it is a unix-style path
    pos = curDir.find('\\', 0);

    while (pos != string::npos) {
      curDir[pos] = '/';
      pos = curDir.find('\\', pos);
    }

#endif
    curDir = TulipLibDir + "tulip" + PATH_DELIMITER + curDir;
  } else
    curDir = TulipLibDir + "tulip";
  TulipPluginsPath = curDir;

  // one dir up to initialize the share dir
  pos = TulipLibDir.length() - 2;
  pos = TulipLibDir.rfind("/", pos);
  curDir = TulipLibDir.substr(0, pos + 1) + "share/tulip/";

#ifndef _WIN32
  // special case for Debian when Tulip install prefix is /usr
  // as libraries are installed in <prefix>/lib/<arch>
  if (!pathExist(curDir)) {
    pos = TulipLibDir.rfind("/", pos - 1);
    curDir = TulipLibDir.substr(0, pos + 1) + "share/tulip/";
  }
#endif

  // register our exit handler enabling exit detection
  registerTulipExitHandler();

  // check that TulipShareDir exists
  checkDirectory(TulipShareDir = curDir, tlpDirSet, throwExOnCheck);

  curDir = TulipShareDir + "bitmaps/";

  // check that TulipBitmapDir exists
  checkDirectory(TulipBitmapDir = curDir, tlpDirSet, throwExOnCheck);

  // initialize serializers
  initTypeSerializers();

  // initialize pseudo random number generator seed
  initRandomSequence();
}
//=========================================================

// tlp class names demangler
#if defined(__GNUC__)
#include <cxxabi.h>
std::string tlp::demangleClassName(const char *className, bool hideTlp) {
  static char demangleBuffer[1024];
  int status;
  size_t length = 1024;
  abi::__cxa_demangle(className, demangleBuffer, &length, &status);

  // skip tlp::
  if (hideTlp && strstr(demangleBuffer, "tlp::") == demangleBuffer)
    return std::string(demangleBuffer + 5);

  return std::string(demangleBuffer);
}
#elif defined(_MSC_VER)
// With Visual Studio, typeid(tlp::T).name() does not return a mangled type name
// but a human readable type name in the form "class tlp::T"
// so just remove the first 11 characters to return T
std::string tlp::demangleClassName(const char *className, bool hideTlp) {
  char *clName = const_cast<char *>(className);

  if (strstr(className, "class ") == className)
    clName += 6;

  if (hideTlp && strstr(clName, "tlp::") == clName)
    return std::string(clName + 5);

  return std::string(clName);
}
#else
#error define symbols demangling function
#endif

#else // __EMSCRIPTEN__

void initTulipLib(const char *) {}

std::string tlp::demangleClassName(const char *className, bool) {
  return std::string(className);
}

#endif // __EMSCRIPTEN__

//=========================================================
std::istream *tlp::getIgzstream(const std::string &name, int open_mode) {
#if defined(WIN32) && ZLIB_VERNUM >= 0x1270
  std::wstring utf16name;
  utf8::utf8to16(name.begin(), name.end(), std::back_inserter(utf16name));
  return new igzstream(utf16name.c_str(), open_mode);
#else
  return new igzstream(name.c_str(), open_mode);
#endif
}

std::ostream *tlp::getOgzstream(const std::string &name, int open_mode) {
#if defined(WIN32) && ZLIB_VERNUM >= 0x1270
  std::wstring utf16name;
  utf8::utf8to16(name.begin(), name.end(), std::back_inserter(utf16name));
  return new ogzstream(utf16name.c_str(), open_mode);
#else
  return new ogzstream(name.c_str(), open_mode);
#endif
}

// random sequence management
//=========================================================

static unsigned int randomSeed = UINT_MAX;
// uniformly-distributed integer random number generator that produces non-deterministic random
// numbers
static std::random_device rd;
// Mersenne Twister pseudo-random generator of 32-bit numbers
static std::mt19937 mt;

void tlp::setSeedOfRandomSequence(unsigned int seed) {
  randomSeed = seed;
}

unsigned int tlp::getSeedOfRandomSequence() {
  return randomSeed;
}

void tlp::initRandomSequence() {
  // init seed from random sequence with std::random_device
  if (randomSeed == UINT_MAX) {
#ifndef __MINGW32__
    mt.seed(rd());
#else
    // std::random_device implementation is deterministic in MinGW so initialize seed with current
    // time (microsecond precision)
    mt.seed(uint(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
#endif
  } else {
    mt.seed(randomSeed);
  }
}

int tlp::randomInteger(int max) {
  if (max == 0) {
    return 0;
  } else if (max > 0) {
    std::uniform_int_distribution<int> dist(0, max);
    return dist(mt);
  } else {
    std::uniform_int_distribution<int> dist(max, 0);
    return dist(mt);
  }
}

unsigned int tlp::randomUnsignedInteger(unsigned int max) {
  if (max == 0) {
    return 0;
  } else {
    std::uniform_int_distribution<unsigned int> dist(0, max);
    return dist(mt);
  }
}

double tlp::randomDouble(double max) {
  std::uniform_real_distribution<double> dist(0, std::nextafter(max, DBL_MAX));
  return dist(mt);
}

//=========================================================

bool tlp::pathExist(const std::string &pathname) {
#ifndef WIN32
  struct stat info;
  return stat(pathname.c_str(), &info) == 0;
#else
  struct _stat info;
  std::wstring utf16pathname;
  utf8::utf8to16(pathname.begin(), pathname.end(), std::back_inserter(utf16pathname));
  return _wstat(utf16pathname.c_str(), &info) == 0;
#endif
}

//=========================================================

#if defined(WIN32) && defined(__GLIBCXX__)
// function to get the string representation of the bitwise combination of std::ios_base::openmode
// flags
static std::wstring openmodeToWString(std::ios_base::openmode mode) {
  std::wstring ret;
  bool testb = (mode & std::ios_base::binary) == std::ios_base::binary;
  bool testi = (mode & std::ios_base::in) == std::ios_base::in;
  bool testo = (mode & std::ios_base::out) == std::ios_base::out;
  bool testt = (mode & std::ios_base::trunc) == std::ios_base::trunc;
  bool testa = (mode & std::ios_base::app) == std::ios_base::app;

  if (!testi && testo && !testt && !testa)
    ret = L"w";

  if (!testi && testo && !testt && testa)
    ret = L"a";

  if (!testi && testo && testt && !testa)
    ret = L"w";

  if (testi && !testo && !testt && !testa)
    ret = L"r";

  if (testi && testo && !testt && !testa)
    ret = L"r+";

  if (testi && testo && testt && !testa)
    ret = L"w+";

  if (testb)
    ret += L"b";

  return ret;
}

// class to open a file for reading whose path contains non ascii characters (MinGW only)
class wifilestream : public std::istream {
public:
  wifilestream(const std::wstring &wfilename, std::ios_base::openmode mode)
      : fp(nullptr), buffer(nullptr) {
    fp = _wfopen(wfilename.c_str(), openmodeToWString(mode).c_str());

    if (fp) {
      buffer = new __gnu_cxx::stdio_filebuf<char>(fp, mode);
    }

    init(buffer);
  }
  ~wifilestream() {
    delete buffer;

    if (fp) {
      fclose(fp);
    }
  }

private:
  FILE *fp;
  __gnu_cxx::stdio_filebuf<char> *buffer;
};

// class to open a file for writing whose path contains non ascii characters (MinGW only)
class wofilestream : public std::ostream {
public:
  wofilestream(const std::wstring &wfilename, std::ios_base::openmode open_mode)
      : fp(nullptr), buffer(nullptr) {
    fp = _wfopen(wfilename.c_str(), openmodeToWString(open_mode).c_str());

    if (fp) {
      buffer = new __gnu_cxx::stdio_filebuf<char>(fp, open_mode);
    }

    init(buffer);
  }
  ~wofilestream() {
    delete buffer;

    if (fp) {
      fclose(fp);
    }
  }

private:
  FILE *fp;
  __gnu_cxx::stdio_filebuf<char> *buffer;
};
#endif

//=========================================================

std::istream *tlp::getInputFileStream(const std::string &filename, std::ios_base::openmode mode) {
#ifndef WIN32
  // On Linux and Mac OS, UTF-8 encoded paths are supported by std::ifstream
  return new std::ifstream(filename.c_str(), mode);
#else
  // On Windows, the path name (possibly containing non ascii characters) has to be converted to
  // UTF-16 in order to open a stream
  std::wstring utf16filename;
  utf8::utf8to16(filename.begin(), filename.end(), std::back_inserter(utf16filename));
#ifdef __GLIBCXX__
  // With MinGW, it's a little bit tricky to get an input stream
  return new wifilestream(utf16filename, mode);
#elif defined(_MSC_VER)
  // Visual Studio has wide char version of std::ifstream
  return new std::ifstream(utf16filename.c_str(), mode);
#else
  // Fallback
  return new std::ifstream(filename.c_str(), mode);
#endif
#endif
}

//=========================================================

std::ostream *tlp::getOutputFileStream(const std::string &filename,
                                       std::ios_base::openmode open_mode) {
#ifndef WIN32
  // On Linux and Mac OS, UTF-8 encoded paths are supported by std::ofstream
  return new std::ofstream(filename.c_str(), open_mode);
#else
  // On Windows, the path name (possibly containing non ascii characters) has to be converted to
  // UTF-16 in order to open a stream
  std::wstring utf16filename;
  utf8::utf8to16(filename.begin(), filename.end(), std::back_inserter(utf16filename));
#ifdef __GLIBCXX__
  // With MinGW, it's a little bit tricky to get an output stream
  return new wofilestream(utf16filename, open_mode);
#elif defined(_MSC_VER)
  // Visual Studio has wide char version of std::ofstream
  return new std::ofstream(utf16filename.c_str(), open_mode);
#else
  // Fallback
  return new std::ofstream(filename.c_str(), open_mode);
#endif
#endif
}

//=========================================================

// Gui Test mode
static bool GuiTestingMode = false;

bool tlp::inGuiTestingMode() {
  return GuiTestingMode;
}

void tlp::setGuiTestingMode(bool enabled) {
  GuiTestingMode = enabled;
}
