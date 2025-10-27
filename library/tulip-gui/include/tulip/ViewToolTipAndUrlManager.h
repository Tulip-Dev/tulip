/*
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
///@cond DOXYGEN_HIDDEN

#ifndef VIEW_TOOLTIP_AND_URL_MANAGER_H
#define VIEW_TOOLTIP_AND_URL_MANAGER_H

class QWidget;

#include <QMenu>

#include <tulip/View.h>
#include <tulip/DataSet.h>

namespace tlp {
class StringProperty;

class TLP_QT_SCOPE ViewToolTipAndUrlManager : public QObject {
  Q_OBJECT

  View *_view;
  QWidget *_widget;
  std::string _url, _contextMenuUrl;

public:
  ViewToolTipAndUrlManager(tlp::View *view, QWidget *widget);

  // the functions below must be called by the associated view
  // when overloading the View class corresponding methods
  void setState(const tlp::DataSet &);
  void state(tlp::DataSet &) const;
  void fillContextMenu(QMenu *menu);
  void fillContextMenu(QMenu *menu, node n);
  void fillContextMenu(QMenu *menu, edge e);

protected slots:
  void manageToolTips();
  void openUrl();
  void viewGraphSet(tlp::Graph *);

protected:
  bool eventFilter(QObject *, QEvent *e) override;
  StringProperty *getAttributeProperty(const std::string &attribute);
};
} // namespace tlp

#endif /* VIEW_TOOLTIP_AND_URL_MANAGER_H */

///@endcond
