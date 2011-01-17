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
#ifndef Tulip_LAYERMANAGERWIDGET_H
#define Tulip_LAYERMANAGERWIDGET_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include "tulip/LayerManagerWidgetData.h"

#include <QtGui/QTreeWidgetItem>

namespace tlp {

  class GlScene;
  class GlLayer;
  class GlComposite;
  class GlGraphComposite;
  class GlMainView;
  class GlMainWidget;

  class TLP_QT_SCOPE LayerManagerWidget : public QWidget, public Ui::LayerManagerWidgetData {
    Q_OBJECT

    GlMainView *mainView;
    GlMainWidget* observedMainWidget;
    bool holdUpdateView;

  public:
    LayerManagerWidget(QWidget* parent=0);

    void attachMainWidget(GlMainWidget *graphWidget);

    void addLayer(tlp::GlScene*, const std::string&, tlp::GlLayer*);
    void addComposite(tlp::GlComposite *composite,QTreeWidgetItem *parent);
    void createGraphCompositeItem(tlp::GlGraphComposite *glGraphComposite,QTreeWidgetItem *item);
    void updateLayer(const std::string& name,tlp::GlLayer* layer);
    void delLayer(tlp::GlScene*, const std::string&, tlp::GlLayer*);

  public slots:

    void apply();

  protected slots:

    void itemClicked(QTreeWidgetItem *,int);
    void applyVisibility();
    void applyVisibility(QTreeWidgetItem *item,GlComposite *composite);
  };

}

#endif // Tulip_LAYERMANAGERWIDGET_H
