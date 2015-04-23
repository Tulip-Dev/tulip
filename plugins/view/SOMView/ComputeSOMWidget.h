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

#ifndef COMPUTESOMWIDGET_H_
#define COMPUTESOMWIDGET_H_

#include <QWidget>

#include <vector>
#include <string>

namespace Ui {
class SOMComputeWidget;
}

namespace tlp {

class Graph;
/**
 * @brief Class configuring properties selected and number of iteration for computing SOM.
 */
class ComputeSOMWidget: public QWidget {
  Q_OBJECT
  Ui::SOMComputeWidget* _ui;

public:
  ComputeSOMWidget(QWidget* parent = NULL);
  ~ComputeSOMWidget();
  unsigned number() const;
  void setNumber(unsigned uintValue);
  void clearLists();
  std::vector<std::string> getSelectedProperties();
  void setWidgetParameters(tlp::Graph *g, std::vector<std::string> &propertyFilterType);
  void setOutputPropertiesList(std::vector<std::string> &properties);
};
}
#endif /* COMPUTESOMWIDGET_H_ */
