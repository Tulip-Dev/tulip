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
#ifndef EXPORTWIZARD_H
#define EXPORTWIZARD_H

#include <QWizard>
#include <QModelIndex>
#include <tulip/WithParameter.h>

namespace Ui {
class ExportWizard;
}

namespace tlp {
class GraphHierarchiesModel;
}

class ExportWizard: public QWizard {
  Q_OBJECT

  Ui::ExportWizard* _ui;
  tlp::Graph* _graph;
public:
  explicit ExportWizard(tlp::Graph* g, const QString& exportFile,
                        QWidget *parent = NULL);
  virtual ~ExportWizard();

  QString algorithm() const;
  tlp::DataSet parameters() const;
  QString outputFile() const;

protected slots:
  void algorithmSelected (const QModelIndex& index);
  void updateFinishButton();

  void pathChanged(QString s);
  void browseButtonClicked();
};

#endif // EXPORTWIZARD_H
