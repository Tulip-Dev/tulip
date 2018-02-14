/*
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
///@cond DOXYGEN_HIDDEN

#ifndef VIEW_TOOLTIP_AND_URL_MANAGER_H
#define VIEW_TOOLTIP_AND_URL_MANAGER_H

#include <QMenu>

#include <tulip/View.h>
#include <tulip/DataSet.h>
#include <tulip/GlMainWidget.h>

namespace tlp {

class TLP_QT_SCOPE ViewToolTipAndUrlManager : public QObject {
  Q_OBJECT

  View *_view;
  GlMainWidget *_glMainWidget;
  bool _tooltips;
  std::string _urlPropName;
  std::string _url;
  
public:

  ViewToolTipAndUrlManager(tlp::View *view, tlp::GlMainWidget* widget);

  // the functions below must be called by the associated view
  // when overloading the View class corresponding methods
  void setState(const tlp::DataSet &);
  void state(tlp::DataSet &) const;
  void fillContextMenu(QMenu *menu);


protected slots:
  void displayToolTips(bool display);
  void setUrlProp(QAction *);

protected:
  bool eventFilter(QObject *, QEvent *e) override;
};

}

#endif /* VIEW_TOOLTIP_AND_URL_MANAGER_H */

///@endcond
