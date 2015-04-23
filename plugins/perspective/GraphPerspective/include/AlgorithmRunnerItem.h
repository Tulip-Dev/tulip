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

#ifndef ALGORITHMRUNNERITEM_H
#define ALGORITHMRUNNERITEM_H

#include <QWidget>

#include <tulip/DataSet.h>

namespace Ui {
class AlgorithmRunnerItem;
}
namespace tlp {
class Graph;
}

class AlgorithmRunnerItem: public QWidget {
  Q_OBJECT
  Ui::AlgorithmRunnerItem* _ui;
  QString _pluginName;
  tlp::Graph* _graph;
  bool _storeResultAsLocal;
  QPointF _dragStartPosition;
  tlp::DataSet _initData;


public:
  explicit AlgorithmRunnerItem(QString pluginName, QWidget* parent = NULL);
  virtual ~AlgorithmRunnerItem();

  QString name() const;
  tlp::Graph *graph() const;
  tlp::DataSet data() const;

  void setFavorite(bool);

protected:
  virtual void mousePressEvent(QMouseEvent* ev);
  virtual void mouseMoveEvent(QMouseEvent* ev);

public slots:
  void setGraph(tlp::Graph*);
  void setData(const tlp::DataSet& data);
  void setStoreResultAsLocal(bool);

  void run(tlp::Graph* g = NULL);

signals:
  void favorized(bool);

protected slots:
  void afterRun(tlp::Graph*, const tlp::DataSet&);
  void initModel();
};
#endif // ALGORITHMRUNNERITEM_H
