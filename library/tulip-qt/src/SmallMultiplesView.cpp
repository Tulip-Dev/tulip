#include "tulip/SmallMultiplesView.h"

#include "tulip/GlMainWidget.h"
#include "tulip/GlGraphInputData.h"
#include "tulip/ForEach.h"
#include "tulip/TlpQtTools.h"
#include "tulip/Interactor.h"
#include "tulip/InteractorChainOfResponsibility.h"
#include "tulip/MouseInteractors.h"
#include "tulip/SmallMultiplesNavigatorComponent.h"
#include "tulip/QtGlSceneZoomAndPanAnimator.h"
#include "tulip/GlNode.h"
#include "tulip/DrawingTools.h"
#include "tulip/GlMainWidgetGraphicsView.h"
#include "tulip/GlMainWidgetItem.h"

#include <QtGui/QLabel>
#include <QtGui/QImage>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsProxyWidget>

#include <iostream>

using namespace std;

void zoomOnScreenRegion(tlp::GlMainWidget *glWidget, const tlp::BoundingBox &boundingBox, const std::string &layerName="Main", const bool optimalPath = true, const double velocity = 1.1, const double p = sqrt(1.6)) {
  if(boundingBox.isValid()){
    tlp::QtGlSceneZoomAndPanAnimator animator(glWidget, boundingBox, layerName, optimalPath, velocity, p);
    animator.animateZoomAndPan();
  }
}

namespace tlp {

//===========================================
//            VIEW FUNCTIONS
//===========================================
SmallMultiplesView::SmallMultiplesView()
  :AbstractView(), _glMainWidget(new GlMainWidget(0)), _zoomAnimationActivated(true), _maxLabelSize(-1), _spacing(1.7) {
  Observable::holdObservers();
  _glMainWidget->setData(newGraph(), DataSet());
  GlScene *scene = _glMainWidget->getScene();
  GlGraphInputData *inputData = _glMainWidget->getScene()->getGlGraphComposite()->getInputData();
  inputData->getElementColor()->setAllNodeValue(scene->getBackgroundColor());
  inputData->getElementShape()->setAllNodeValue(4);
  inputData->getElementLabelPosition()->setAllNodeValue(2);
  inputData->getElementFontSize()->setAllNodeValue(2);
  scene->getGlGraphComposite()->getRenderingParametersPointer()->setFontsType(1);
  scene->getGlGraphComposite()->getRenderingParametersPointer()->setLabelScaled(true);
  Observable::unholdObservers();

  // Move the graph composite from main layer to overview layer
  scene->addLayer(new GlLayer("overview",true));
  GlGraphComposite *graphComposite = scene->getGlGraphComposite();
  scene->getLayer("Main")->clear();
  scene->getLayer("overview")->addGlEntity(graphComposite,"overviewGraph");
  scene->addGlGraphCompositeInfo(scene->getLayer("overview"), graphComposite);
  scene->centerScene();

  connect(this, SIGNAL(changeData(int,int,SmallMultiplesView::Roles)), this, SLOT(dataChanged(int,int,SmallMultiplesView::Roles)));
  connect(this,SIGNAL(reverseItems(int,int)), this, SLOT(itemsReversed(int,int)));
}

SmallMultiplesView::~SmallMultiplesView() {
}

QWidget *SmallMultiplesView::construct(QWidget *parent) {
  QWidget *centralWidget = AbstractView::construct(parent);
  setCentralWidget(_glMainWidget);
  toggleInteractors(false);
  return centralWidget;
}

void SmallMultiplesView::setActiveInteractor(Interactor *interactor) {
  if (activeInteractor)
    activeInteractor->remove();
  interactor->install(_glMainWidget);
  activeInteractor = interactor;
}

//===========================================
//            DATA UPDATING
//===========================================
void SmallMultiplesView::dataChanged(int from, int to, Roles dataRoles) {
  refreshItems();
  for (int i=from; i <= to; ++i) {
    if (i >= _items.size())
      return;
    dataChanged(i, dataRoles);
  }
}

template<typename T, typename Proptype>
void applyVariant(QVariant v, Proptype *prop, tlp::node n) {
  if (!v.isValid() || v.isNull())
    return;
  T val = v.value<T>();
  prop->setNodeValue(n, val);
}

template<>
void applyVariant<QString, StringProperty>(QVariant v, StringProperty *prop, tlp::node n) {
  if (!v.isValid() || v.isNull())
    return;
  prop->setNodeValue(n, v.toString().toStdString());
}

void SmallMultiplesView::dataChanged(int id, SmallMultiplesView::Roles dataRoles) {
  if (id >= _items.size())
    return;
  Observable::holdObservers();
  node n = _items[id];

  GlGraphInputData *inputData = _glMainWidget->getScene()->getGlGraphComposite()->getInputData();
  if (dataRoles.testFlag(SmallMultiplesView::Texture))
    applyVariant<QString, StringProperty>(data(id, SmallMultiplesView::Texture), inputData->getElementTexture(), n);

  if (dataRoles.testFlag(SmallMultiplesView::Label)) {
    QVariant v = data(id, SmallMultiplesView::Label);
    if (v.isValid() && !v.isNull()) {
      QString str = v.toString();
      if (_maxLabelSize != -1 && str.size() > _maxLabelSize) {
        str.resize(_maxLabelSize);
        str.append("...");
      }
      inputData->getElementLabel()->setNodeValue(n, str.toStdString());
    }
  }

  if (dataRoles.testFlag(SmallMultiplesView::Position))
    applyVariant<tlp::Coord, LayoutProperty>(data(id, SmallMultiplesView::Position), inputData->getElementLayout(), n);
  Observable::unholdObservers();
}

void SmallMultiplesView::refreshItems() {
  Observable::holdObservers();
  int itemsCount = countItems();
  int nodesCount = _glMainWidget->getGraph()->numberOfNodes();

  for (int i=itemsCount; i < nodesCount; ++i)
    delItem(_items.size()-1);
  for (int i=itemsCount; i > nodesCount; --i)
    addItem();

  Observable::unholdObservers();
}

void SmallMultiplesView::addItem() {
  Observable::holdObservers();
  _items.push_back(_glMainWidget->getGraph()->addNode());
  Observable::unholdObservers();
}

void SmallMultiplesView::delItem(int id) {
  Observable::holdObservers();
  if (id >= _items.size())
    return;
  node n = _items[id];
  _items.erase(_items.begin() + id);
  _glMainWidget->getGraph()->delNode(n);
  Observable::unholdObservers();
}

int SmallMultiplesView::nodeItemId(node n) {
  for (int i=0; i < _items.size(); ++i)
    if (_items[i] == n)
      return i;
  return -1;
}

void SmallMultiplesView::itemsReversed(int a, int b) {
  if (a >= _items.size() || b >= _items.size())
    return;
  node na = _items[a];
  _items[a] = _items[b];
  _items[b] = na;
  dataChanged(a, SmallMultiplesView::Position);
  dataChanged(b, SmallMultiplesView::Position);
}

//===========================================
//            OVERVIEW FUNCTIONS
//===========================================
void SmallMultiplesView::selectItem(int i) {
  if (i > _items.size())
    return;
  if (_zoomAnimationActivated) {
    GlNode glNode(_items[i]);
    zoomOnScreenRegion(_glMainWidget, glNode.getBoundingBox(_glMainWidget->getScene()->getGlGraphComposite()->getInputData()), "overview");
  }
  emit itemSelected(i);
}

GlMainWidget *SmallMultiplesView::overview() const {
  return _glMainWidget;
}

void SmallMultiplesView::centerOverview() {
  if (_zoomAnimationActivated) {
    GlGraphInputData *inputData = _glMainWidget->getScene()->getGlGraphComposite()->getInputData();
    zoomOnScreenRegion(_glMainWidget, computeBoundingBox(_glMainWidget->getGraph(),inputData->getElementLayout(),inputData->getElementSize(), inputData->getElementRotation()), "overview");
  }
  else
    _glMainWidget->getScene()->centerScene();
}

void SmallMultiplesView::setOverviewVisible(bool f) {
  _glMainWidget->getScene()->getLayer("overview")->setVisible(f);
}

GlLayer *SmallMultiplesView::overviewLayer() const {
  return _glMainWidget->getScene()->getLayer("overview");
}

bool SmallMultiplesView::isOverviewVisible() const {
  return _glMainWidget->getScene()->getLayer("overview")->isVisible();
}

//===========================================
//               MODEL
//===========================================
QVariant SmallMultiplesView::data(int id, SmallMultiplesDataRole role) {
  if (role == Position) {
    QVariant v;
    int w = round(sqrt(countItems()));
    int row = abs(id / w);
    int col = id % w;

    // Compute spacing
    Coord c(col * _spacing, -1. * _spacing * row, 0);
    v.setValue<Coord>(c);
    return v;
  }
  return QVariant();
}

//===========================================
//               TOGGLABLES
//===========================================

void SmallMultiplesView::toggleInteractors(bool f) {
  list<Interactor *> interactors = getInteractors();
  int i=0;
  for (list<Interactor *>::iterator it = interactors.begin(); it != interactors.end(); ++it) {
    if (i++ > 0) { // FIXME: Find another way to detect the SmallMultiplesView navigation interactor.
      (*it)->getAction()->setEnabled(f);
      if (!f)
        (*it)->getAction()->setChecked(f);
    }
    else if (!f)// When we disable interactors, check the SmallMultiplesView navigation interactor.
      (*it)->getAction()->setChecked(true);
  }
}

}

//===========================================
//               INTERACTORS
//===========================================
using namespace tlp;

class SmallMultiplesNavigationInteractor: public InteractorChainOfResponsibility {
public:
  SmallMultiplesNavigationInteractor(): InteractorChainOfResponsibility(":/i_select.png", "Navigator") {
    setPriority(1);
    setConfigurationWidgetText(trUtf8("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
    "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
    "p, li { white-space: pre-wrap; }\n"
    "</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
    "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">Small Multiples View navigation</span></p>\n"
    "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt; font-weight:600;\"></p>\n"
    "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">This tool provides a way to move around in small multiples views.</span></p>\n"
    "<p style="
                            "\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"></p>\n"
    "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">Overview mode: </span><span style=\" font-size:8pt;\">Use the mouse left click button or the arrow keys to translate the point of view. Double click on a specific item to display it.</span></p>\n"
    "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"></p>\n"
    "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">Specifc view: </span><span style=\" font-size:8pt;\">If the view provides a camera, you can move the camera the same way you did in overview mode. Double cl"
                            "ick to switch back to the overview mode.</span></p></body></html>"));
  }

  void construct(){
    pushInteractorComponent(new SmallMultiplesNavigatorComponent());
  }

  virtual bool isCompatible(const std::string &) {
    return false;
  }
};

INTERACTORPLUGIN(SmallMultiplesNavigationInteractor, "InteractorSmallMultiplesNavigation", "Tulip Team", "01/11/2011", "Move into small multiples views", "1.0");
