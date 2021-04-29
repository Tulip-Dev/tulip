/*
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

#ifndef _TLPTOOLS_H
#define _TLPTOOLS_H

#include <iostream>
#include <climits>
#include <sys/types.h>
#include <tulip/tulipconf.h>

// The hash_combine function from the boost library
// Call it repeatedly to incrementally create a hash value from several variables.

// Explanation of the formula from StackOverflow
// (http://stackoverflow.com/questions/4948780/magic-numbers-in-boosthash-combine) :
// The magic number 0x9e3779b9 = 2^32 / ((1 + sqrt(5)) / 2) is the reciprocal of the golden ratio.
// It is supposed to be 32 random bits, where each is equally likely to be 0 or 1, and with no
// simple correlation between the bits.
// So including this number "randomly" changes each bit of the seed; as you say, this means that
// consecutive values will be far apart.
// Including the shifted versions of the old seed makes sure that, even if hash_value() has a fairly
// small range of values,
// differences will soon be spread across all the bits.
namespace std {
template <class T>
inline void tlp_hash_combine(std::size_t &seed, const T &v) {
  hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
} // namespace std

namespace tlp {
extern TLP_SCOPE const char PATH_DELIMITER;
extern TLP_SCOPE std::string TulipLibDir;
extern TLP_SCOPE std::string TulipPluginsPath;
extern TLP_SCOPE std::string TulipBitmapDir;
extern TLP_SCOPE std::string TulipShareDir;
extern TLP_SCOPE bool TulipProgramExiting;

/**
 * @ingroup Plugins
 *
 * @brief Initializes the Tulip library.
 * Looks for the Tulip plug-ins directory.
 * The plug-ins directory can be defined in different ways, given by order of prevalence :
 * 1. the TLP_DIR environment variable, if it has a value
 * 2. the appDirPath parameter, if it is not nullptr
 * 3. at that point, the Tulip paths will be retrieved from the path of the loaded Tulip shared
 * library
 *  (you must dispose of a standard Tulip installation for that feature to work).
 * 4. a fallback value of 'C:/Tulip/lib/' on windows, or '/usr/local/lib/' on Unix.
 */
extern TLP_SCOPE void initTulipLib(const char *appDirPath = nullptr);

/**
 * @ingroup Plugins
 *
 * @brief Demangles the name of a C++ class
 * @param className The mangled name of a class
 * @param hideTlp a flag to indicate if the 'tlp::' prefix
 * @return string The demangled name of a Tulip C++ class.
 */
TLP_SCOPE std::string demangleClassName(const char *className, bool hideTlp = false);

/**
 * @ingroup Plugins
 *
 * @brief Demangles the name of a C++ class defined in the tlp namespace.
 * @param className The mangled name of a class
 * @return string The demangled name of a Tulip C++ class
 * without the tlp:: prefix
 */
inline std::string demangleTlpClassName(const char *className) {
  return demangleClassName(className, true);
}

/**
 * @brief Returns an istream to read from a gzipped file (uses gzstream lib).
 * The stream has to be deleted after use.
 * @param name The name of the file to read from.
 * @param open_mode The mode to open the file with. Defaults to std::ios::in.
 * @return istream gzipped input stream from a file.
 */
TLP_SCOPE std::istream *getIgzstream(const std::string &name, int open_mode = std::ios::in);

/**
 * @brief Returns an ostream to write to a gzipped file (uses gzstream lib).
 * The stream has to be deleted after use.
 * @warning Don't forget to check the stream with ios::bad()!
 * @param name The name of the file to write to.
 * @param open_mode The mode to open the file with. Defaults to std::ios::out.
 * @return ostream gzipped output stream to a file.
 */
TLP_SCOPE std::ostream *getOgzstream(const std::string &name, int open_mode = std::ios::out);

/**
 * @brief Gives the value of the seed used for further initialization
 * of a random sequence (with further calls to rand() or random()).
 * @param seed the value of the seed.
 * Set seed to UINT_MAX if you need a random choice of the seed.
 */
TLP_SCOPE void setSeedOfRandomSequence(unsigned int seed = UINT_MAX);

/**
 * @brief Returns the value of the seed used for further initialization
 * of a random sequence
 */
TLP_SCOPE unsigned int getSeedOfRandomSequence();

/**
 * @brief Initializes a random sequence with the seed previously set
 * Further calls to rand() or random() will return the elements of
 * that sequence
 */
TLP_SCOPE void initRandomSequence();

/**
 * @brief Returns a random integer in the range [0, max] if max is positive or in the range [max, 0]
 * if max is negative
 */
TLP_SCOPE int randomInteger(int max);

/**
 * @brief Returns a random unsigned integer in the range [0, max]
 */
TLP_SCOPE unsigned int randomUnsignedInteger(unsigned int max);

/**
 * @brief Returns a random double in the range [0, max]
 */
TLP_SCOPE double randomDouble(double max = 1.0);

/**
 * @brief returns if a path exists
 * @param pathname an utf-8 encoded string containing the path to check
 * @return true if a file or directory according pathname can be accessed, falseif not and errno is
 * set
 */
TLP_SCOPE bool pathExist(const std::string &pathname);

/**
 * @brief Cross-platform function to get an input file stream. Its purpose is to support
 * paths on windows platform containing non-ascii characters.
 * @param filename an utf-8 encoded string containing the path of the file to open for performing
 * input operations
 * @param open_mode the stream opening mode flags (bitwise combination of std::ios_base::openmode
 * constants).
 */
TLP_SCOPE std::istream *getInputFileStream(const std::string &filename,
                                           std::ios_base::openmode open_mode = std::ios::in);

/**
 * @brief Cross-platform function to get an output file stream. Its purpose is to support
 * paths on windows platform containing non-ascii characters.
 * @param filename an utf-8 encoded string containing the path of the file to open for performing
 * output operations
 * @param open_mode the stream opening mode flags (bitwise combination of std::ios_base::openmode
 * constants).
 */
TLP_SCOPE std::ostream *getOutputFileStream(const std::string &filename,
                                            std::ios_base::openmode open_mode = std::ios::out);

///@cond DOXYGEN_HIDDEN
// Gui test mode
TLP_SCOPE bool inGuiTestingMode();
TLP_SCOPE void setGuiTestingMode(bool);
// exit handler registration
TLP_SCOPE void registerTulipExitHandler();
///@endcond
} // namespace tlp
#endif
