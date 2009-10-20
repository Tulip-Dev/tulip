#include "tulip/BaseGraphicsViewComponent.h"

#include <QtGui/QMenuBar>
#include <QtGui/QImageWriter>
#include <QtGui/QStatusBar>
#include <QtGui/QInputDialog>
#include <QtGui/QMouseEvent>
#include <QtGui/QToolTip>
#include <QtGui/QFrame>

#include <tulip/ExtendedClusterOperation.h>
#include <tulip/ColorProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/StableIterator.h>
#include <tulip/StringProperty.h>

#include <tulip/GlMetaNodeTrueRenderer.h>
#include <tulip/GlRectTextured.h>

#include "tulip/GWOverviewWidget.h"
#include "tulip/RenderingParametersDialog.h"
#include "tulip/LayerManagerWidget.h"
#include "tulip/AugmentedDisplayDialog.h"
#include "tulip/GridOptionsWidget.h"
#include "tulip/InteractorManager.h"
#include "tulip/QtMetaNodeRenderer.h"
#include "tulip/TlpQtTools.h"
#include "tulip/GlMainWidgetGraphicsView.h"
#include "tulip/Interactor.h"
#include "tulip/NodeLinkDiagramComponent.h"
#include "tulip/ViewPluginsManager.h"
#include "tulip/TabWidgetHidableMenuGraphicsProxy.h"

using namespace std;

namespace tlp {

  BaseGraphicsViewComponent::BaseGraphicsViewComponent(const string &realViewName):realViewName(realViewName),graphicsView(NULL),baseView(ViewPluginsManager::getInst().createView(realViewName)){}

  BaseGraphicsViewComponent::~BaseGraphicsViewComponent() {}

  QWidget *BaseGraphicsViewComponent::construct(QWidget *parent) {
    QWidget *widget=AbstractView::construct(parent);
    baseView->construct(parent);
    connect(baseView,SIGNAL(elementSelected(unsigned int,bool)),this,SLOT(elementSelectedSlot(unsigned int,bool)));

    widget->resize(512, 512);

    graphicsView = new GlMainWidgetGraphicsView(this,widget,((GlMainView*)baseView)->getGlMainWidget(),((GlMainView *)baseView)->getOverviewWidget(),((GlMainView *)baseView)->getOverviewAction());

    setCentralWidget(graphicsView);

    list<pair<QWidget *,string> > configWidgets=baseView->getConfigurationWidget();

    for(list<pair<QWidget *,string> >::iterator it=configWidgets.begin();it!=configWidgets.end();++it){
      graphicsView->addToTabWidget((*it).second,(*it).first);
    }

    return widget;

  }
  void BaseGraphicsViewComponent::draw() {
    baseView->draw();
    graphicsView->draw();
  }

  void BaseGraphicsViewComponent::refresh() {
    baseView->refresh();
    graphicsView->redraw();
  }

  void BaseGraphicsViewComponent::init() {
    baseView->init();
    draw();
  }

  void BaseGraphicsViewComponent::elementSelectedSlot(unsigned int id,bool node){
    emit elementSelected(id,node);
  }

  string BaseGraphicsViewComponent::getRealViewName() {
    return realViewName;
  }


}

