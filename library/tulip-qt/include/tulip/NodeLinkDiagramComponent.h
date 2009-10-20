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

    void elementSelectedSlot(unsigned int id,bool isNode) {
      emit elementSelected(id,isNode);
    }

  };

}

#endif






