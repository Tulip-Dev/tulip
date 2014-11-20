/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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

#include <cstring>
#include <ctime>
#include <string>
#include <sstream>
#include <locale.h>
#include <errno.h>

#ifndef _WIN32
#include <unistd.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <utf8.h>
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

#include <gzstream.h>
#include <tulip/TulipException.h>
#include <tulip/TlpTools.h>
#include <tulip/PluginLoader.h>
#include <tulip/PropertyTypes.h>
#include <tulip/TulipRelease.h>

using namespace std;
using namespace tlp;

#ifndef _TULIP_LIB_DIR
#ifdef _WIN32
#define _TULIP_LIB_DIR "c:\\Tulip\\lib\\"
#else
#define _TULIP_LIB_DIR "/usr/local/lib/"
#endif
#endif

static const char *TULIP_PLUGINS_PATH_VARIABLE="TLP_PLUGINS_PATH";

string tlp::TulipLibDir;
string tlp::TulipPluginsPath;
string tlp::TulipBitmapDir;
string tlp::TulipShareDir;
#ifdef _WIN32
const char tlp::PATH_DELIMITER = ';';
#else
const char tlp::PATH_DELIMITER = ':';
#endif

#ifndef EMSCRIPTEN
// A function that retrieves the Tulip libraries directory based on
// the path of the loaded shared library libtulip-core-X.Y.[dll, so, dylib]
extern "C" {
  char* getTulipLibDir(char* buf) {
    std::string tulipLibDir;
    std::string libTulipName;

#ifdef _WIN32
#ifdef __MINGW32__
    libTulipName = "libtulip-core-" + getMajor(TULIP_VERSION) + "." + getMinor(TULIP_VERSION) + ".dll";
#else
    libTulipName = "tulip-core-" + getMajor(TULIP_VERSION) + "_" + getMinor(TULIP_VERSION) + ".dll";
#endif
    HMODULE hmod = GetModuleHandle(libTulipName.c_str());

    if (hmod != NULL) {
      TCHAR szPath[512 + 1];
      DWORD dwLen = GetModuleFileName(hmod, szPath, 512);

      if (dwLen > 0) {
        std::string tmp = szPath;
        std::replace(tmp.begin(), tmp.end(), '\\', '/');
        tulipLibDir = tmp.substr(0, tmp.rfind('/')+1) + "../lib";
      }
    }

#else
#ifdef __APPLE__
    libTulipName = "libtulip-core-" + getMajor(TULIP_VERSION) + "." + getMinor(TULIP_VERSION) + ".dylib";
#else
    libTulipName = "libtulip-core-" + getMajor(TULIP_VERSION) + "." + getMinor(TULIP_VERSION) + ".so";
#endif
    void *ptr;
    Dl_info info;

    ptr = dlopen(libTulipName.c_str(), RTLD_LAZY);

    if (ptr != NULL) {
      void* symbol = dlsym(ptr, "getTulipLibDir");

      if (symbol != NULL) {
        if (dladdr(symbol, &info)) {
          std::string tmp = info.dli_fname;
          tulipLibDir = tmp.substr(0, tmp.rfind('/')+1) + "../lib";
        }
      }
    }

#endif
    return strcpy(buf, tulipLibDir.c_str());
  }

}

// throw an exception if an expected directory does not exist
static void checkDirectory(std::string dir) {
  // remove ending / separator if any
  // bug detected on Windows
  if (dir[dir.length() - 1] == '/')
    dir.erase(dir.length() - 1);

  tlp_stat_t infoEntry;

  if (statPath(dir,&infoEntry) != 0) {
    std::stringstream ess;
    ess << "Error - " << dir << ": " << std::endl << strerror(errno);
    ess << std::endl << "Check your TLP_DIR environment variable";

    throw TulipException(ess.str());
  }
}

//=========================================================
void tlp::initTulipLib(const char* appDirPath) {
  // first we must ensure that the parsing of float or double
  // doest not depend on locale
  setlocale(LC_NUMERIC, "C");

  if (!TulipShareDir.empty()) // already initialized
    return;

  char *getEnvTlp;
  string::size_type pos;

  getEnvTlp=getenv("TLP_DIR");

  if (getEnvTlp==0) {
    if (appDirPath) {
#ifdef _WIN32
      TulipLibDir = std::string(appDirPath) + "/../lib";
#else
      // one dir up to initialize the lib dir
      TulipLibDir.append(appDirPath,
                         strlen(appDirPath) -
                         strlen(strrchr(appDirPath, '/') + 1));
#ifdef I64
      // check for lib64
      string tlpPath64 = TulipLibDir + "lib64/tulip";
      tlp_stat_t statInfo;

      if (statPath(tlpPath64, &statInfo) == 0)
        TulipLibDir.append("lib64");
      else
#endif
        TulipLibDir.append("lib");

#endif
    }
    else {
      char buf[1024];
      // if no appDirPath is provided, retrieve dynamically the Tulip lib dir
      TulipLibDir = getTulipLibDir(buf);

      // if no results (should not happen with a clean Tulip install), fall back in the default value provided during compilation
      if (TulipLibDir.empty())
        TulipLibDir = string(_TULIP_LIB_DIR);
    }
  }
  else
    TulipLibDir=string(getEnvTlp);

#ifdef _WIN32
  // ensure it is a unix-style path
  pos = TulipLibDir.find('\\', 0);

  while(pos != string::npos) {
    TulipLibDir[pos] = '/';
    pos = TulipLibDir.find('\\', pos);
  }

#endif

  // ensure it is '/' terminated
  if (TulipLibDir[TulipLibDir.length() - 1] != '/')
    TulipLibDir+='/';

  // check TulipLibDir exists
  bool tlpDirSet = (getEnvTlp!=NULL);

  if (tlpDirSet)
    checkDirectory(TulipLibDir);

  getEnvTlp=getenv(TULIP_PLUGINS_PATH_VARIABLE);

  if (getEnvTlp!=NULL) {
    TulipPluginsPath=string(getEnvTlp);
#ifdef _WIN32
    // ensure it is a unix-style path
    pos = TulipPluginsPath.find('\\', 0);

    while(pos != string::npos) {
      TulipPluginsPath[pos] = '/';
      pos = TulipPluginsPath.find('\\', pos);
    }

#endif
    TulipPluginsPath= TulipLibDir + "tulip" + PATH_DELIMITER + TulipPluginsPath;
  }
  else
    TulipPluginsPath= TulipLibDir + "tulip";


  // one dir up to initialize the share dir
  pos = TulipLibDir.length() - 2;
  pos = TulipLibDir.rfind("/", pos);
  TulipShareDir=TulipLibDir.substr(0, pos + 1)+"share/tulip/";

  // check it exists
  if (tlpDirSet)
    checkDirectory(TulipShareDir);

  TulipBitmapDir=TulipShareDir+"bitmaps/";

  // check it exists
  if (tlpDirSet)
    checkDirectory(TulipBitmapDir);

  // initialize serializers
  initTypeSerializers();
}
//=========================================================

// tlp class names demangler
#if defined(__GNUC__)
#include <cxxabi.h>
std::string tlp::demangleClassName(const char* className,
                                   bool hideTlp) {
  static char demangleBuffer[1024];
  int status;
  size_t length = 1024;
  abi::__cxa_demangle((char *) className, (char *) demangleBuffer,
                      &length, &status);

  // skip tlp::
  if (hideTlp && strstr(demangleBuffer, "tlp::") == demangleBuffer)
    return std::string(demangleBuffer + 5);

  return std::string(demangleBuffer);
}
#elif defined(_MSC_VER)
// With Visual Studio, typeid(tlp::T).name() does not return a mangled type name
// but a human readable type name in the form "class tlp::T"
// so just remove the first 11 characters to return T
std::string tlp::demangleClassName(const char* className,
                                   bool hideTlp) {
  char* clName = const_cast<char*>(className);

  if (strstr(className, "class ") == className)
    clName += 6;

  if (hideTlp && strstr(clName, "tlp::") == clName)
    return std::string(clName + 5);

  return std::string(clName);
}
#else
#error define symbols demangling function
#endif

#else // EMSCRIPTEN

void initTulipLib(const char*) {}

std::string tlp::demangleClassName(const char* className, bool) {
  return std::string(className);
}

#endif // EMSCRIPTEN

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

//=========================================================

// random sequence management
static unsigned int randomSeed = UINT_MAX;
void tlp::setSeedOfRandomSequence(unsigned int seed) {
  randomSeed = seed;
}

unsigned int tlp::getSeedOfRandomSequence() {
  return randomSeed;
}

void tlp::initRandomSequence() {
  if (randomSeed == UINT_MAX)
    srand((unsigned int) time(NULL));
  else
    srand(randomSeed);
}

//=========================================================

int tlp::statPath(const std::string &pathname, tlp_stat_t *buf) {
#ifndef WIN32
  return stat(pathname.c_str(), buf);
#else
  std::wstring utf16pathname;
  utf8::utf8to16(pathname.begin(), pathname.end(), std::back_inserter(utf16pathname));
  return _wstat(utf16pathname.c_str(), buf);
#endif
}

//=========================================================

#if defined(WIN32) && defined(__GLIBCXX__)
// function to get the string representation of the bitwise combination of std::ios_base::openmode flags
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
  wifilestream(const std::wstring &wfilename, std::ios_base::openmode mode) : fp(NULL), buffer(NULL) {
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
  __gnu_cxx::stdio_filebuf<char>* buffer;
};

// class to open a file for writing whose path contains non ascii characters (MinGW only)
class wofilestream : public std::ostream {
public:
  wofilestream(const std::wstring &wfilename, std::ios_base::openmode open_mode) : fp(NULL), buffer(NULL) {
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
  __gnu_cxx::stdio_filebuf<char>* buffer;
};
#endif

//=========================================================

std::istream *tlp::getInputFileStream(const std::string &filename, std::ios_base::openmode mode) {
#ifndef WIN32
  // On Linux and Mac OS, UTF-8 encoded paths are supported by std::ifstream
  return new std::ifstream(filename.c_str(), mode);
#else
  // On Windows, the path name (possibly containing non ascii characters) has to be converted to UTF-16 in order to open a stream
  std::wstring utf16filename;
  utf8::utf8to16(filename.begin(), filename.end(), std::back_inserter(utf16filename));
#ifdef __GLIBCXX__
  // With MinGW, it's a little bit tricky to get an input stream
  return new wifilestream(utf16filename,mode);
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

std::ostream *tlp::getOutputFileStream(const std::string &filename, std::ios_base::openmode open_mode) {
#ifndef WIN32
  // On Linux and Mac OS, UTF-8 encoded paths are supported by std::ofstream
  return new std::ofstream(filename.c_str(), open_mode);
#else
  // On Windows, the path name (possibly containing non ascii characters) has to be converted to UTF-16 in order to open a stream
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
