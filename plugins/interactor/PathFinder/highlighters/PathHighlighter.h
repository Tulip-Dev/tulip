/*
 * PathHighlighter.h
 *
 *  Created on: Apr 14, 2010
 *      Author: Ludwig Fiolka
 */

#ifndef PATHHIGHLIGHTER_H_
#define PATHHIGHLIGHTER_H_

#include <string>
#include <tulip/Node.h>
#include "PathFinding/Dikjstra/Dikjstra.h"

#include <QtCore/qobject.h>

namespace tlp {
class GlMainView;
class BooleanProperty;
class DoubleProperty;
class GlGraphInputData;
class GlMainWidget;
class GlLayer;
class GlScene;
class GlSimpleEntity;
}
class PathFinder;
class QWidget;


class PathHighlighter {
public:
  PathHighlighter(const std::string &name);
  virtual ~PathHighlighter();

  inline std::string getName() {
    return this->name;
  }
  virtual void highlight(const PathFinder *parent, tlp::GlMainWidget *glMainWidget, tlp::BooleanProperty *selection, tlp::node src, tlp::node tgt)=0;
  virtual void draw(tlp::GlMainWidget *glMainWidget) = 0;
  virtual PathHighlighter *clone() = 0;
  void clear();
  virtual bool isConfigurable()=0;
  virtual QWidget *getConfigurationWidget()=0;

protected:
  tlp::GlLayer *getWorkingLayer(tlp::GlScene *scene);
  tlp::GlGraphInputData *getInputData(tlp::GlMainWidget *glMainWidget);
  void addGlEntity(tlp::GlScene *scene, tlp::GlSimpleEntity *entity, bool deleteOnExit=true, const std::string &name="");

private:
  std::string name;
  tlp::GlScene *backupScene;
  std::map<std::string, bool> entities;
  int entityId;
};

#endif /* PATHHIGHLIGHTER_H_ */
