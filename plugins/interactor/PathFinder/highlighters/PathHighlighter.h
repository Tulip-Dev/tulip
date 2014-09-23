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

#ifndef PATHHIGHLIGHTER_H_
#define PATHHIGHLIGHTER_H_

#include <string>
#include <map>

#include <tulip/Node.h>
#include <tulip/Observable.h>

class QWidget;

namespace tlp {
class GlMainView;
class BooleanProperty;
class DoubleProperty;
class GlGraphInputData;
class GlMainWidget;
class GlLayer;
class GlScene;
class GlSimpleEntity;

class PathFinder;


class PathHighlighter : public Observable {
public:
  PathHighlighter(const std::string &name);
  virtual ~PathHighlighter();

  inline std::string getName() const {
    return this->name;
  }
  virtual void highlight(const PathFinder *parent, GlMainWidget *glMainWidget, BooleanProperty *selection, node src, node tgt)=0;
  virtual void draw(tlp::GlMainWidget *glMainWidget) = 0;
  void clear();
  virtual bool isConfigurable() const=0;
  virtual QWidget *getConfigurationWidget()=0;

protected:
  tlp::GlLayer *getWorkingLayer(tlp::GlScene *scene);
  tlp::GlGraphInputData *getInputData(tlp::GlMainWidget *glMainWidget);
  void addGlEntity(tlp::GlScene *scene, tlp::GlSimpleEntity *entity, bool deleteOnExit=true, const std::string &name="");
  void treatEvent(const Event &ev);

private:
  std::string name;
  tlp::GlScene *backupScene;
  std::map<std::string, bool> entities;
  int entityId;
};
}
#endif /* PATHHIGHLIGHTER_H_ */
