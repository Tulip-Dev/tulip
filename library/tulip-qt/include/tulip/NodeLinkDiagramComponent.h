#ifndef _Tulip_NODELINKDIAGRAMCOMPONENT_H
#define _Tulip_NODELINKDIAGRAMCOMPONENT_H

#include "GlMainView.h"


namespace tlp {

  class TLP_QT_SIMPLE_SCOPE NodeLinkDiagramComponentCreator : public ViewCreator {

  public :
  
    View* create(const std::string &name,QWidget *parent);
  };

  class TLP_QT_SIMPLE_SCOPE NodeLinkDiagramComponent : public GlMainView {
    
    Q_OBJECT;

  public:

    NodeLinkDiagramComponent(const std::string &pluginName,QWidget *parent=NULL, const char *name=NULL);
    ~NodeLinkDiagramComponent();

    void setData(Graph *graph,DataSet dataSet);
    void getData(Graph **graph,DataSet *dataSet);

    void getInteractorsActionList(std::list<QAction*> &);
    void installInteractor(QAction *);

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
    QAction *renderingParametersDialogAction;
    QAction* selectAction;
    QAction* deleteAction;
    QAction* goAction;	
    QAction* ungroupAction;
    QAction* propAction;
    bool isNode;
    int itemId;

    GridOptionsWidget *gridOptionsWidget;
    RenderingParametersDialog *renderingParametersDialog;
    LayerManagerWidget *layerWidget;

    void constructInteractorsMap();
    void addLayer(tlp::GlScene*, const std::string&, tlp::GlLayer*);
    void modifyLayer(tlp::GlScene*, const std::string&, tlp::GlLayer*);

  protected slots:
    
    void centerView();
    void showDialog(QAction*);
    void gridOptions();

    public slots:
    void draw();
    void refresh();
    void init();
  
  };

}

#endif






