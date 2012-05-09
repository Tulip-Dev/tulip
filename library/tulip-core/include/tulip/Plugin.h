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

#ifndef TULIP_PLUGIN_H
#define TULIP_PLUGIN_H
#include <string>
#include <tulip/tulipconf.h>
#include <tulip/WithParameter.h>
#include <tulip/WithDependency.h>
#include <tulip/TulipRelease.h>
#include <tulip/PluginLister.h>
#include <tulip/PluginContext.h>

/**
 * \addtogroup plugins
 */
namespace tlp {

/*@{*/

/**
 * @brief Splits the string and returns everything befor the first dot ('.').
 * This is used to return major version number, as version numbers are formatted as X.Y.Z,
 * X being the major, Y the minor, and Z the patch version.
 *
 * @return string The part of the string befor the first dot.
 */
TLP_SCOPE std::string getMajor(const std::string &release);

/**
 * @brief Splits the string and return the minor version.
 * If the string does not contain any dot, then 0 is returned.
 * If the string contains only one dot (X.Y), then everything after the first dot is returned (Y).
 * If the string is a full version with two dots (X.Y.Z), everything between the first and last dots is returned (Y).
 * If there are more than two dots, everything between the first and last dots is returned.
 */
TLP_SCOPE std::string getMinor(const std::string &release);

/**
 * @brief Base interface for plug-in description.
 *
 * This class is not intented to be subclassed by plug-ins themselves, but by the factories who create the plug-ins.
 *
 * This class holds user informations, such as name of the author, date of creation, and miscellaneous informations.
 *
 * It also holds informations for the Tulip plug-in system, such as the version (used to know when there is an update for a plug-in),
 * the name (used as unique identifier to register the plug-in),
 * the Tulip version the plug-in was built with (to know whether the plug-in is compatible with the currently running version of TUlip),
 * and the group this plug-in belongs to (e.g. trees).
 *
 * The function that actually creates the plugin is in the FactoryInterface class in order to keep Plugin clean of template parameters, making it easier to use.
 *
 * @see FactoryInterface for more advanced operation such as plugin creation and retrieving dependencies.
 */
class TLP_SCOPE Plugin : public tlp::WithParameter, public tlp::WithDependency {
public:
  virtual ~Plugin() {}

  /**
    @brief The icon (preferably a thumbnail) of the plugin
    @return std::string the icon path
    */
  virtual std::string icon() const;

  /**
    @brief A string identifier for a plugin used for categorization purposes.
    @returns std::string the category of the plugin.
    */
  virtual std::string category() const=0;

  /**
   * @brief Returns the name of the plug-in, as registered in the Tulip plug-in system.
   * This name must be unique, and if multiple plug-ins have the same name,
   * only the latest encountered will be considered.
   * @return string the name of the plug-in.
   */
  virtual std::string name() const=0;

  /**
   * @brief Returns the name of the group this plug-in belongs to.
   * Groups and sub-groups are separated by two colons.
   * e.g. trees::planar trees
   * @return the group name of this plug-in.
   */
  virtual std::string group() const=0;

  /**
   * @brief The name of the author of this plug-in.
   *
   * @return the name of the author.
   */
  virtual std::string author() const=0;

  /**
   * @brief The creation date of the plug-in.
   * This date is in a free format, but most Tulip plug-ins use a DD/MM/YYYY
   *
   * @return the creation date.
   */
  virtual std::string date() const=0;

  /**
   * @brief Informations about the plug-in, from the plug-in author.
   * These informations can contains anything, and the developer is completely free to put anything here.
   * Most plug-ins by the Tulip team use an html format to generate help from these informations.
   * @return string The informations associated with this plug-in.
   */
  virtual std::string info() const=0;

  /**
   * @brief The release version of the plug-in, including major and minor.
   * The version should be X.Y, X being the major, and Y the minor.
   * @return string The release version.
   */
  virtual std::string release() const=0;

  /**
   * @brief The version of Tulip this plug-in was built with.
   * Tulip versions are X.Y.Z, X eing the major, Y the minor, and Z the patch.
   *
   * @return The Tulip version the plug-in was built with.
   */
  virtual std::string tulipRelease() const=0;

  /**
   * @brief Only the major of the plug-in version.
   * A version should be X.Y, X being the major.
   *
   * @return The major part of the plug-in version.
   */
  virtual std::string major() const;

  /**
   * @brief Only the minor of the plug-in version.
   * A version should be X.Y, Y being the major.
   *
   * @return The minor part of the plug-in version.
   */
  virtual std::string minor() const;

  /**
   * @return The major Tulip version the plug-in was built with.
   */
  virtual std::string tulipMajor() const;

  /**
   * @return Return the minor Tulip version this plug-in was built with.
   */
  virtual std::string tulipMinor() const;

  /**
  * @brief Returns the ID of the glyph this factory builds.
  * @TODO this member should be removed once there is a system in Tulip to handle glyphs.
  *
  * @return int the id of the glyph.
  **/
  virtual int id() const;
};

#define PLUGININFORMATIONS(NAME, AUTHOR, DATE, INFO, RELEASE, GROUP)\
std::string name() const { return NAME; } \
std::string author() const { return AUTHOR; }\
std::string date() const { return DATE; }  \
std::string info() const { return INFO; }  \
std::string release() const { return RELEASE; }\
std::string tulipRelease() const { return TULIP_RELEASE; }\
std::string group() const { return GROUP; }

/*@}*/
}

#endif //TULIP_PLUGIN_H
