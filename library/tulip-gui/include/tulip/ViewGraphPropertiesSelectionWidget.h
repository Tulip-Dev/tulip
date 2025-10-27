/**
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

#ifndef VIEWGRAPHPROERTIESSELECTIONWIDGET_H_
#define VIEWGRAPHPROERTIESSELECTIONWIDGET_H_

#include <vector>
#include <string>

#include <QWidget>

#include <tulip/Graph.h>

namespace Ui {
class ViewGraphPropertiesSelectionWidgetData;
}

namespace tlp {

class TLP_QT_SCOPE ViewGraphPropertiesSelectionWidget : public QWidget, public Observable {

  Q_OBJECT

public:
  ViewGraphPropertiesSelectionWidget(QWidget *parent = nullptr);
  ~ViewGraphPropertiesSelectionWidget() override;

  void setWidgetParameters(Graph *graph,
                           const std::vector<std::string> &graphPropertiesTypesFilter);
  std::vector<std::string> getSelectedGraphProperties();

  void setWidgetEnabled(const bool enabled);
  void setSelectedProperties(const std::vector<std::string> &selectedProperties);

  ElementType getDataLocation() const;
  void setDataLocation(const ElementType location);
  void enableEdgesButton(const bool enable);
  void treatEvent(const Event &) override;

  bool configurationChanged();

private:
  Ui::ViewGraphPropertiesSelectionWidgetData *_ui;
  Graph *graph;
  std::vector<std::string> graphPropertiesTypesFilter;
  std::vector<std::string> lastSelectedProperties;
  ElementType lastDataLocation;
};
} // namespace tlp

#endif /* VIEWGRAPHPROERTIESSELECTIONWIDGET_H_ */
