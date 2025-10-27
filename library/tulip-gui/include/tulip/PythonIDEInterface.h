/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

///@cond DOXYGEN_HIDDEN
#ifndef PYTHONIDEINTERFACE_H
#define PYTHONIDEINTERFACE_H

#include <QString>
#include <QStringList>
#include <QFrame>

#include <tulip/tulipconf.h>

namespace tlp {
class GraphHierarchiesModel;
class TulipProject;

// this class defines the interface between the Graph Perspective
// and the Python IDE
class TLP_QT_SCOPE PythonIDEInterface : public QFrame {
public:
  static QString compiledVersion(bool full = false);
#ifdef WIN32
  static QString getPythonHome();
#endif

  virtual ~PythonIDEInterface() {}

  virtual bool projectNeedsPythonIDE(TulipProject *project) = 0;
  virtual bool hasUnsavedFiles() = 0;
  virtual void readProject(tlp::TulipProject *project) = 0;
  virtual void clearPythonCodeEditors() = 0;
  virtual bool isCurrentScriptExecuting() = 0;
  virtual void executeCurrentScript() = 0;
  virtual void stopCurrentScript() = 0;
  virtual void pauseCurrentScript() = 0;
  virtual void deleteStaticResources() = 0;

  // this class is used to really create a Python IDE
  // it must be derived in the tulip-python library
  // to ensure a usable PythonIDE exists
  class TLP_QT_SCOPE Builder {
    friend PythonIDEInterface;

  public:
    Builder();

  protected:
    virtual PythonIDEInterface *newIDE(GraphHierarchiesModel *model) = 0;
    virtual void loadPlugins() = 0;
  };

  // indicates if a Python IDE can be build
  static bool exists();
  // build a PythonIDE
  static PythonIDEInterface *newIDE(GraphHierarchiesModel *model);
  // ensure python plugins loading before Python IDE instantiation
  static void loadPlugins();
};

} // namespace tlp

#endif // PYTHONIDEINTERFACE_H
///@endcond
