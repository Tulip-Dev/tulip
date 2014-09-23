/*
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
///@cond DOXYGEN_HIDDEN

#ifndef _Tulip_MOUSESHOWELEMENTINFOS_H
#define _Tulip_MOUSESHOWELEMENTINFOS_H

#include <tulip/InteractorComposite.h>
#include <tulip/Graph.h>

namespace Ui {
class ElementInformationsWidget;
}

class QWidget;
class QGraphicsProxyWidget;
class QTableView;
class QAbstractItemModel;
class QObject;

namespace tlp {

class View;
class ViewWidget;
struct SelectedEntity;

/**
 * We define a specific interactor to show element graph infos in eltProperties
 */
class TLP_QT_SCOPE MouseShowElementInfos : public InteractorComponent {

  Ui::ElementInformationsWidget* _ui;
public:
  MouseShowElementInfos();
  ~MouseShowElementInfos();
  virtual bool eventFilter(QObject* widget, QEvent* e);

  void viewChanged(View *);

  void clear();

  virtual bool pick(int x, int y, SelectedEntity &selectedEntity);

protected:

  ViewWidget *_view;
  QWidget *_informationsWidget;
  QGraphicsProxyWidget *_informationsWidgetItem;

  QTableView* tableView() const;

  /**
   * @brief buildModel create and returns the model to visualize edit elements parameters.
   * @param elementType the type of the element can be NODE or EDGE
   * @param elementId elementId the id of the element
   * @param parent the parent for the model creation.
   * @return
   */
  virtual QAbstractItemModel* buildModel(ElementType elementType, unsigned int elementId, QObject *parent)const;
  /**
   * @brief elementName returns the title of the element.
   * @param elementType the type of the element can be NODE or EDGE
   * @param elementId the id of the element
   * @return
   */
  virtual QString elementName(ElementType elementType, unsigned int elementId)const;
};

}



#endif
///@endcond
