/**
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

#ifndef ALGORITHMRUNNERITEM_H
#define ALGORITHMRUNNERITEM_H

#include <QWidget>

#include <tulip/DataSet.h>

namespace Ui {
class AlgorithmRunnerItem;
}
namespace tlp {
class Graph;
class ParameterListModel;
}

class AlgorithmRunnerItem : public QWidget {
  Q_OBJECT
  Ui::AlgorithmRunnerItem *_ui;
  QString _pluginName;
  tlp::Graph *_graph;
  bool _storeResultAsLocal;
  QPointF _dragStartPosition;
  tlp::DataSet _initData;
  static tlp::ParameterListModel *colorMappingModel;

public:
  explicit AlgorithmRunnerItem(QString pluginName, QWidget *parent = nullptr);
  ~AlgorithmRunnerItem() override;

  QString name() const;
  tlp::Graph *graph() const;
  tlp::DataSet data() const;

  void setFavorite(bool);

protected:
  void mousePressEvent(QMouseEvent *ev) override;
  void mouseMoveEvent(QMouseEvent *ev) override;

public slots:
  void setGraph(tlp::Graph *);
  void setData(const tlp::DataSet &data);
  void setStoreResultAsLocal(bool);
  void favoriteChanged(int state);

  void run(tlp::Graph *g = nullptr);

signals:
  void favorized(bool);

protected slots:
  void afterRun(tlp::Graph *, const tlp::DataSet &);
  void initModel();
};
#endif // ALGORITHMRUNNERITEM_H
