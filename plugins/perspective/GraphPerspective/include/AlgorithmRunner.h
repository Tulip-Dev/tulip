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
#ifndef ALGORITHMRUNNER_H
#define ALGORITHMRUNNER_H

#include <QtGui/QWidget>

class QToolButton;
namespace Ui {
class AlgorithmRunner;
class AlgorithmRunnerItem;
}
namespace tlp {
class Graph;
}

class AlgorithmRunner: public QWidget {
  Q_OBJECT

  Ui::AlgorithmRunner* _ui;

public:
  explicit AlgorithmRunner(QWidget* parent = 0);
  virtual ~AlgorithmRunner();

public slots:
  void setGraph(tlp::Graph*);

protected slots:
  void setFilter(QString);
};

class AlgorithmRunnerItem: public QWidget {
  Q_OBJECT
  Ui::AlgorithmRunnerItem* _ui;
  QString _pluginName;
  tlp::Graph* _graph;
  bool _localMode;

public:
  explicit AlgorithmRunnerItem(QString pluginName, QWidget* parent = 0);
  virtual ~AlgorithmRunnerItem();

  QString name() const;

public slots:
  void setGraph(tlp::Graph*);
  void setLocalMode(bool);

  void run(tlp::Graph* g = NULL);
};

#endif // ALGORITHMRUNNER_H
