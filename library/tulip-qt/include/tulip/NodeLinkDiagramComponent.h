#ifndef _Tulip_NODELINKDIAGRAMCOMPONENT_H
#define _Tulip_NODELINKDIAGRAMCOMPONENT_H

#include "GlMainView.h"


namespace tlp {

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

    QWidget *construct(QWidget *parent);

    void setData(Graph *graph,DataSet dataSet);
    void getData(Graph **graph,DataSet *dataSet);

    std::list<std::pair<QWidget *,std::string> > getConfigurationWidget();

    void specificEventFilter(QObject *object,QEvent *event);
    void buildContextMenu(QObject *object,QMouseEvent *event,QMenu *contextMenu);
    void computeContextMenuAction(QAction *action);

  protected :

    QMenu *viewMenu;
    QMenu *dialogMenu;
    QMenu *optionsMenu;
    QMenu *exportImageMenu;
    QAction *actionTooltips;
    QAction *actionsGridOptions;
    QAction *actionZOrderingOptions;
    QAction *actionAntialiasingOptions;
    QAction *augmentedDisplayDialogAction;
    QAction* addRemoveAction;
    QAction* selectAction;
    QAction* deleteAction;
    QAction* goAction;
    QAction* ungroupAction;
    QAction* propAction;
    bool isNode;
    int itemId;

    GridOptionsWidget *gridOptionsWidget;
    RenderingParametersDialog *renderingParametersDialog;
    LayerManagerWidget *layerManagerWidget;

    std::map<std::string,DataSet> algorithmInfoDataSet;

    void addLayer(tlp::GlScene*, const std::string&, tlp::GlLayer*);
    void modifyLayer(tlp::GlScene*, const std::string&, tlp::GlLayer*);
    void checkAlgorithmResult();

  protected slots:
    void showDialog(QAction*);
    void gridOptions();

  public slots:
    void centerView();
    void draw();
    void refresh();
    void init();
    void setGraph(Graph *graph);

    void elementSelectedSlot(unsigned int id,bool isNode) {
      emit elementSelected(id,isNode);
    }

  };

}

#endif






