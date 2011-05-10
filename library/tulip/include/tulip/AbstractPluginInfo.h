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

#ifndef TULIPPLUGIN_H
#define TULIPPLUGIN_H
#include <assert.h>
#include <string>
#include <tulip/tulipconf.h>
#include <tulip/TlpTools.h>
/**
 * \addtogroup plugins
 */ 
namespace tlp {
/*@{*/
/**
 * @brief Base interface for plug-in description.
 * This class is not intented to be subclassed by plug-ins themselves, but by the factories who create the plug-ins.
 * This class holds user informations, such as name of the author, date of creation, and miscellaneous informations.
 * It also holds informations for the Tulip plug-in system, such as the version (used to know when there is an update for a plug-in),
 * the name (used as unique identifier to register the plug-in),
 * the Tulip version the plug-in was built with (to know whether the plug-in is compatible with the currently running version of TUlip),
 * and the group this plug-in belongs to (e.g. trees).
 */

class AbstractPluginInfo {
public:
  virtual ~AbstractPluginInfo(){}

  /**
   * @brief Returns the name of the plug-in, as registered in the Tulip plug-in system.
   * This name must be unique, and if multiple plug-ins have the same name,
   * only the latest encountered will be considered.
   * @return string the name of the plug-in.
   */
  virtual  std::string getName() const=0;

  /**
   * @brief Returns the name of the group this plug-in belongs to.
   * Groups and sub-groups are separated by two colons.
   * e.g. trees::planar trees
   * @return the group name of this plug-in.
   */
  virtual std::string getGroup() const=0;

  /**
   * @brief The name of the author of this plug-in.
   *
   * @return the name of the author.
   */
  virtual  std::string getAuthor() const=0;

  /**
   * @brief The creation date of the plug-in.
   * This date is in a free format, but most Tulip plug-ins use a DD/MM/YYYY
   *
   * @return the creation date.
   */
  virtual  std::string getDate() const=0;

  /**
   * @brief Informations about the plug-in, from the plug-in author.
   * These informations can contains anything, and the developer is completely free to put anything here.
   * Most plug-ins by the Tulip team use an html format to generate help from these informations.
   * @return string The informations associated with this plug-in.
   */
  virtual  std::string getInfo() const=0;

  /**
   * @brief The release version of the plug-in, including major and minor.
   * The version should be X.Y, X being the major, and Y the minor.
   * @return string The release version.
   */
  virtual  std::string getRelease() const=0;
  
  /**
   * @brief The version of Tulip this plug-in was built with.
   * Tulip versions are X.Y.Z, X eing the major, Y the minor, and Z the patch.
   *
   * @return The Tulip version the plug-in was built with.
   */
  virtual  std::string getTulipRelease() const=0;
  
  /**
   * @brief Only the major of the plug-in version.
   * A version should be X.Y, X being the major.
   *
   * @return The major part of the plug-in version.
   */
  virtual  std::string getMajor() const {
    return tlp::getMajor(getRelease());
  }
  
  /**
   * @brief Only the minor of the plug-in version.
   * A version should be X.Y, Y being the major.
   *
   * @return The minor part of the plug-in version.
   */
  virtual  std::string getMinor() const {
    return tlp::getMinor(getRelease());
  }

  /**
   * @return The major Tulip version the plug-in was built with.
   */
  virtual  std::string getTulipMajor() const {
    return tlp::getMajor(getTulipRelease());
  }

  /**
   * @return Return the minor Tulip version this plug-in was built with.
   */
  virtual  std::string getTulipMinor() const  {
    return tlp::getMinor(getTulipRelease());
  }

  virtual int getId() const {
    return 0;
  }
};

template<class ObjectType, class Context> class PluginManager;

template <class PluginObject, class PluginContext>
class FactoryInterface : public AbstractPluginInfo {
  public:
    /**
     * @brief Creates a new Algorithm object.
     *
     * @param context The context for the new plug-in.
     * @return PluginObject* A newly created algorithm plug-in.
     **/
    virtual PluginObject* createPluginObject(PluginContext context) {
      (void) context;
      std::cerr << "error from FactoryInterface<" << typeid(PluginObject).name() << ", " << typeid(PluginContext).name() <<">" << std::endl;
      assert(false);
      return NULL;
    }
  };
 
typedef _DEPRECATED AbstractPluginInfo Plugin;

/*@}*/
}
#endif
