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
#ifndef ALGORITHMRUNNER_H
#define ALGORITHMRUNNER_H

#include <QWidget>

#include <tulip/PluginModel.h>
#include <tulip/Algorithm.h>

#include "AlgorithmRunnerItem.h"

namespace Ui {
class AlgorithmRunner;
class AlgorithmRunnerItem;
}

class QToolButton;

namespace tlp {
class Graph;
}

class AlgorithmRunner: public QWidget {
  Q_OBJECT

  Ui::AlgorithmRunner* _ui;
  tlp::Graph* _graph;
  QToolButton* _storeResultAsLocalButton;
  QAction* _resultAsLocalPropAction;

  QList<AlgorithmRunnerItem*> _favorites;

  void refreshTreeUi(QWidget* w);
  void buildTreeUi(QWidget* w, tlp::PluginModel<tlp::Algorithm>* model, const QModelIndex& parent, bool root = false);
  void insertItem(QWidget* w, const QString& name);

public:
  explicit AlgorithmRunner(QWidget* parent = NULL);
  virtual ~AlgorithmRunner();

signals:
  void setStoreResultAsLocal(bool);

public slots:
  void setGraph(tlp::Graph*);
  void findPlugins();

  void refreshPluginsList();

protected slots:
  void setFilter(QString);
  void addFavorite(const QString& algName, const tlp::DataSet& data=tlp::DataSet());
  void removeFavorite(const QString& algName);
  void favorized(bool);
  void setStoreResultAsLocal(QAction*);
  void expanded(bool);

protected:
  bool eventFilter(QObject *, QEvent *);
};

#endif // ALGORITHMRUNNER_H
