#include <QPropertyAnimation>
#include <QGraphicsView>
#include <QLabel>
#include <QGraphicsProxyWidget>

#include <tulip/MouseInteractors.h>
#include <tulip/Interactor.h>
#include <tulip/GlMainView.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlScene.h>


using namespace tlp;

/*
 In this class we create an GLInteractorComponent
 In first see InteractorPlugin
*/
class InteractorPluginComponent : public InteractorComponent {

  QGraphicsProxyWidget *_informationWidgetItem;
  QLabel *_informationLabel;

public :

  /*
  Basic constructor
  In this constructor we create a label to display information
  And a QGraphicsProxyWidget to show information in Tulip QGraphics system
  */
  InteractorPluginComponent() {
    _informationLabel=new QLabel();
    _informationWidgetItem=new QGraphicsProxyWidget();
    _informationWidgetItem->setWidget(_informationLabel);
    _informationWidgetItem->setVisible(false);
  }

  /*
  This function is call when the view associated to this interactor is changed
  So at this moment we add information QGraphicsItem to the QGraphicsScene
  */
  void viewChanged(View * view) {
    view->graphicsView()->scene()->addItem(_informationWidgetItem);
  }

  /*
  Most important function of an interactor component
  When an event arrive on your interactor : this function is call
  You have to process it and return true. If the event do nothing in your interactor : this function return false;
  */
  bool eventFilter(QObject *, QEvent* e) {

    /*
    If you have clicked on a node/edge, information widget is visible.
    And if you use the wheel of the mouse we hide the information widget
    */
    if(_informationWidgetItem->isVisible() && e->type()==QEvent::Wheel) {
      _informationWidgetItem->setVisible(false);
      return false;
    }

    /*
    Check if the event is a mouse event
    */
    QMouseEvent * qMouseEv = dynamic_cast<QMouseEvent *>(e);

    if(qMouseEv != NULL) {
      GlMainView *glMainView=dynamic_cast<GlMainView*>(view());


      /*
      Check if event is a left mouse button press
      */
      if (e->type() == QEvent::MouseButtonPress && qMouseEv->button() == Qt::LeftButton) {

        /*
        If you have clicked on a node/edge, information widget is visible.
        And if you reclick : hide it
        */
        if(_informationWidgetItem->isVisible()) {
          _informationWidgetItem->setVisible(false);
        }
        else {

          /*
          Select entities under the mouse cursor
          The result of selection is stored in SelectedEntity
          And pickNodesEdges return true if you click on node or edge
          */
          SelectedEntity selectedEntity;

          if (glMainView->getGlMainWidget()->pickNodesEdges(qMouseEv->x(),qMouseEv->y(),selectedEntity)) {

            /*
            Change text of the information label with
            - If you click on a node : "Click on node id : nodeId"
            - If you click on an edge : "Click on edge id : edgeId"
            */
            QString text("Click on ");

            if(selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED)
              text+="node ";
            else
              text+="edge ";

            text+=" id : "+QString::number(selectedEntity.getComplexEntityId());

            /*
            - Update QLabel with new text
            - Auto resize QLabel
            - Set position of the label at mouse position
            - Display it
            */
            _informationLabel->setText(text);
            _informationLabel->adjustSize();
            _informationWidgetItem->setPos(qMouseEv->pos());
            _informationWidgetItem->setVisible(true);

            /*
            Here we just add a small smooth animation on the label pop
            */
            QPropertyAnimation *animation = new QPropertyAnimation(_informationWidgetItem, "opacity");
            animation->setDuration(100);
            animation->setStartValue(0.);
            animation->setEndValue(0.99);
            animation->start();

            /*
            We have treated the event so we return true
            */
            return true;
          }
        }
      }
    }

    /*
    We don't have treated the event se we return false
    */
    return false;
  }
};

/*
 If you want to create an interactor plugin, the first think to do is to create a sub class of InteractorComposite
*/
class InteractorPlugin  : public InteractorComposite {

  QLabel *_configurationLabel;

public:
  /*
  This macro is here to register information about the plugin
  PLUGININFORMATION(Plugin name, Author, Date, Plugin long name, version, type of plugin (Navigation,Information...))
  */
  PLUGININFORMATION("InteractorPlugin", "Tulip Team", "05/10/2012", "Demo Interactor", "1.0", "Information")

  /*
  The constructor
  Here we specify the icon of interactor with Qt qrc system, and the ToolTip text of the plugin
  */
  InteractorPlugin(const tlp::PluginContext*):InteractorComposite(QIcon(":/i_interactor_plugin.png"),"Demo Interactor") {
    /*
    Here we create a label with a minimal configuration text
    This text will be displayed when you click on the name of the plugin
    */
    _configurationLabel=new QLabel(QString("This is a demo interactor.<br>")
                                   +"You can zoom and pan and display information if you click on a node/edge.");
  }

  ~InteractorPlugin() {
    delete _configurationLabel;
  }

  /*
  Here we construct the interactor
  An InteractorComposite is a list of InteractorComponent.
  It's like a chain of responsibility :
  - When an event arrive the first InteractorComponent catch it in eventFilter function.
  - If the InteractorComponent don't process this event (if eventFilter function return false), the event is passed to the second InteractorComponent

  In this example we have two InteractorComponent
  The first one : InteractorPluginComponent is the example InteractorComponent
  The second one : MouseNKeysNavigation is a standard Tulip InteractorComponent who activate Zoom and pan interaction
  */
  void construct() {
    push_back(new MouseNKeysNavigator);
    push_back(new InteractorPluginComponent);
  }

  /*
  In this function we say : this plugin is compatible with Node Link Diagram View
  If you don't implement this function your plugin won't be displayed in interactors tool bar
  */
  bool isCompatible(const std::string &viewName) const {
    return (viewName=="Node Link Diagram view");
  }

  /*
  In this function we say : _configurationLabel is the configuration widget
  */
  QWidget* configurationWidget() const {
    return _configurationLabel;
  }

  /*
  In Plugin system we have a priority system to display interactor icon
  If priority is low, you interactor will be at the left of the interactord tool bar
  */
  unsigned int priority() const {
    return 5;
  }

};

/*
With this macro you register your plugin in Tulip plugin engine
*/
PLUGIN(InteractorPlugin)
