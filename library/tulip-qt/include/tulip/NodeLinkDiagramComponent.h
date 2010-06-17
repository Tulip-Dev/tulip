/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#ifndef _Tulip_NODELINKDIAGRAMCOMPONENT_H
#define _Tulip_NODELINKDIAGRAMCOMPONENT_H

#include "GlMainView.h"


namespace tlp {

  class GlMetaNodeRenderer;

  /** \brief Main view of old Tulip
   *
   * This class provide a graph view based to nodes and links
   * Rendering is do with OpenGl
   */
  class TLP_QT_SCOPE NodeLinkDiagramComponent : public GlMainView{

    Q_OBJECT;

  public:

    NodeLinkDiagramComponent();
    virtual ~NodeLinkDiagramComponent();

    virtual QWidget *construct(QWidget *parent);

    virtual void setData(Graph *graph,DataSet dataSet);
    virtual void getData(Graph **graph,DataSet *dataSet);

    virtual std::list<std::pair<QWidget *,std::string> > getConfigurationWidget();

    virtual void specificEventFilter(QObject *object,QEvent *event);
    virtual void buildContextMenu(QObject *object,QMouseEvent *event,QMenu *contextMenu);
    virtual void computeContextMenuAction(QAction *action);

    void emitRequestChangeGraph(Graph *graph){
      emit requestChangeGraph(this,graph);
    }

  protected :

    QMenu *viewMenu;
    QMenu *optionsMenu;
    QAction *actionTooltips;
    QAction *actionsGridOptions;
    QAction *actionZOrderingOptions;
    QAction *actionAntialiasingOptions;
    QAction *actionTrueMetaNodeOptions;
    QAction *augmentedDisplayDialogAction;
    QAction* addRemoveAction;
    QAction* selectAction;
    QAction* deleteAction;
    QAction* goAction;
    QAction* ungroupAction;
    QAction* propAction;
    bool isNode;
    bool qtMetaNode;
    int itemId;

    GridOptionsWidget *gridOptionsWidget;
    RenderingParametersDialog *renderingParametersDialog;
    LayerManagerWidget *layerManagerWidget;

    std::map<std::string,DataSet> algorithmInfoDataSet;

    GlMetaNodeRenderer *currentMetaNodeRenderer;

    void addLayer(tlp::GlScene*, const std::string&, tlp::GlLayer*);
    void modifyLayer(tlp::GlScene*, const std::string&, tlp::GlLayer*);
    void checkAlgorithmResult();

  protected slots:
    void showDialog(QAction*);
    void gridOptions();

  public slots:
    void centerView();
    void drawAfterRenderingParametersChange();
    virtual void draw();
    virtual void refresh();
    virtual void init();
    void setGraph(Graph *graph);
    void setGraph(Graph *graph,bool initView);

    void elementSelectedSlot(unsigned int id,bool isNode) {
      emit elementSelected(id,isNode);
    }

  };

}

#endif






