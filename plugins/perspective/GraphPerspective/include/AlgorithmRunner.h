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
#include <tulip/DataSet.h>

class QToolButton;
namespace Ui {
class AlgorithmRunner;
class AlgorithmRunnerItem;
}
namespace tlp {
class Graph;
}
class AlgorithmRunnerItem;

class AlgorithmRunner: public QWidget {
  Q_OBJECT

  Ui::AlgorithmRunner* _ui;
  tlp::Graph* _graph;

  QList<AlgorithmRunnerItem*> _favorites;

public:
  explicit AlgorithmRunner(QWidget* parent = NULL);
  virtual ~AlgorithmRunner();

public slots:
  void setGraph(tlp::Graph*);
  void findPlugins();

protected slots:
  void setFilter(QString);
  void addFavorite(const QString& algName, const tlp::DataSet& data=tlp::DataSet());
  void removeFavorite(const QString& algName);
  void favorized(bool);

protected:
  bool eventFilter(QObject *, QEvent *);
};

#endif // ALGORITHMRUNNER_H
