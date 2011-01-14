#include "tulip/SmallMultiplesView.h"

#include "tulip/GlMainWidget.h"
#include "tulip/GlGraphInputData.h"
#include "tulip/ForEach.h"
#include "tulip/AbstractSmallMultiplesModel.h"
#include "tulip/TlpQtTools.h"
#include "tulip/Interactor.h"
#include "tulip/InteractorChainOfResponsibility.h"
#include "tulip/MouseInteractors.h"
#include "tulip/SmallMultiplesNavigatorComponent.h"
#include "tulip/QtGlSceneZoomAndPanAnimator.h"
#include "tulip/GlNode.h"
#include "tulip/DrawingTools.h"
#include <QtGui/QLabel>
#include <QtGui/QImage>

using namespace std;

void zoomOnScreenRegion(tlp::GlMainWidget *glWidget, const tlp::BoundingBox &boundingBox, const bool optimalPath = true, const double velocity = 1.1, const double p = sqrt(1.6)) {
    tlp::QtGlSceneZoomAndPanAnimator animator(glWidget, boundingBox, optimalPath, velocity, p);
    animator.animateZoomAndPan();
}

namespace tlp {

//===========================================
//            VIEW FUNCTIONS
//===========================================
SmallMultiplesView::SmallMultiplesView(AbstractSmallMultiplesModel *model)
  :AbstractView(), _model(model), _overview(new GlMainWidget(0)), _zoomAnimationActivated(true), _autoDisableConfigurationWidget(true), _autoDisableInteractors(true) {
  Observable::holdObservers();
  assert(model);
  connect(_model, SIGNAL(dataChanged(int,int,AbstractSmallMultiplesModel::Roles)), this, SLOT(dataChanged(int,int,AbstractSmallMultiplesModel::Roles)));
  connect(_model, SIGNAL(reverseItems(int,int)), this, SLOT(reverseItems(int,int)));
  _overview->setData(newGraph(), DataSet());
  GlScene *scene = _overview->getScene();
  GlGraphInputData *inputData = _overview->getScene()->getGlGraphComposite()->getInputData();
  inputData->getElementColor()->setAllNodeValue(scene->getBackgroundColor());
  inputData->getElementShape()->setAllNodeValue(4);
  inputData->getElementLabelPosition()->setAllNodeValue(2);
  inputData->getElementFontSize()->setAllNodeValue(5);
  _overview->getScene()->getGlGraphComposite()->getRenderingParametersPointer()->setFontsType(1);
  _overview->getScene()->getGlGraphComposite()->getRenderingParametersPointer()->setLabelScaled(true);
  Observable::unholdObservers();
}

SmallMultiplesView::~SmallMultiplesView() {
  delete _model;
  if (!isOverview()) // Otherwise it's deleted along with the view's widget.
    delete _overview;
}

QWidget *SmallMultiplesView::construct(QWidget *parent) {
  QWidget *centralWidget = AbstractView::construct(parent);
  switchToOverview();
  return centralWidget;
}

//===========================================
//            DATA UPDATING
//===========================================
void SmallMultiplesView::dataChanged(int from, int to, AbstractSmallMultiplesModel::Roles dataRoles) {
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

void SmallMultiplesView::dataChanged(int id, AbstractSmallMultiplesModel::Roles dataRoles) {
  if (id >= _items.size())
    return;
  Observable::holdObservers();
  node n = _items[id];

  GlGraphInputData *inputData = _overview->getScene()->getGlGraphComposite()->getInputData();
  if (dataRoles.testFlag(AbstractSmallMultiplesModel::Texture))
    applyVariant<QString, StringProperty>(_model->data(id, AbstractSmallMultiplesModel::Texture), inputData->getElementTexture(), n);

  if (dataRoles.testFlag(AbstractSmallMultiplesModel::Label))
    applyVariant<QString, StringProperty>(_model->data(id, AbstractSmallMultiplesModel::Label), inputData->getElementLabel(), n);

  if (dataRoles.testFlag(AbstractSmallMultiplesModel::Position))
    applyVariant<tlp::Coord, LayoutProperty>(_model->data(id, AbstractSmallMultiplesModel::Position), inputData->getElementLayout(), n);
  Observable::unholdObservers();
}

void SmallMultiplesView::refreshItems() {
  Observable::holdObservers();
  int itemsCount = _model->countItems();
  int nodesCount = _overview->getGraph()->numberOfNodes();

  for (int i=itemsCount; i < nodesCount; ++i)
    delItem(_items.size()-1);
  for (int i=itemsCount; i > nodesCount; --i)
    addItem();

  if (itemsCount != nodesCount && _items.size() == 1) // Item count changed and we only have one item left
    emit itemSelected(0);

  Observable::unholdObservers();
}

void SmallMultiplesView::addItem() {
  Observable::holdObservers();
  _items.push_back(_overview->getGraph()->addNode());
  Observable::unholdObservers();
}

void SmallMultiplesView::delItem(int id) {
  Observable::holdObservers();
  if (id >= _items.size())
    return;
  node n = _items[id];
  _items.erase(_items.begin() + id);
  _overview->getGraph()->delNode(n);
  Observable::unholdObservers();
}

int SmallMultiplesView::nodeItemId(node n) {
  for (int i=0; i < _items.size(); ++i)
    if (_items[i] == n)
      return i;
  return -1;
}

void SmallMultiplesView::reverseItems(int a, int b) {
  if (a >= _items.size() || b >= _items.size())
    return;
  node na = _items[a];
  _items[a] = _items[b];
  _items[b] = na;
  dataChanged(a, AbstractSmallMultiplesModel::Position);
  dataChanged(b, AbstractSmallMultiplesModel::Position);
}

//===========================================
//            OVERVIEW FUNCTIONS
//===========================================
void SmallMultiplesView::selectItem(int i) {
  if (i > _items.size())
    return;
  if (_zoomAnimationActivated) {
    GlNode glNode(_items[i]);
    zoomOnScreenRegion(_overview, glNode.getBoundingBox(_overview->getScene()->getGlGraphComposite()->getInputData()));
  }
  emit itemSelected(i);
}

void SmallMultiplesView::switchToOverview(bool destroyOldWidget) {
  QWidget *w = centralWidget;
  setCentralWidget(_overview);
  if (destroyOldWidget)
    delete w;

  if (_autoDisableConfigurationWidget)
    for (uint i=0; i<_togglableConfigWidgets.size(); ++i)
      _togglableConfigWidgets[i]->setEnabled(true);

  if (_autoDisableInteractors)
    toggleInteractors(false);

  GlGraphInputData *inputData = _overview->getScene()->getGlGraphComposite()->getInputData();
  zoomOnScreenRegion(_overview, computeBoundingBox(_overview->getGraph(), inputData->elementLayout, inputData->elementSize, inputData->elementRotation));
}

void SmallMultiplesView::switchToWidget(QWidget *w) {
  if (_autoDisableConfigurationWidget)
    for (uint i=0; i<_togglableConfigWidgets.size(); ++i)
      _togglableConfigWidgets[i]->setEnabled(false);

  if (_autoDisableInteractors)
    toggleInteractors(true);

  setCentralWidget(w);
}

GlMainWidget *SmallMultiplesView::overview() const {
  return _overview;
}

bool SmallMultiplesView::isOverview() const {
  return centralWidget == _overview;
}

void SmallMultiplesView::centerOverview() {
  _overview->getScene()->centerScene();
}

//===========================================
//               MODEL
//===========================================
AbstractSmallMultiplesModel *SmallMultiplesView::model() {
  return _model;
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

void SmallMultiplesView::setTogglableConfigurationWidget(const std::vector<QWidget *> &v) {
  _togglableConfigWidgets = v;
}

}

//===========================================
//               INTERACTORS
//===========================================
using namespace tlp;

class SmallMultiplesNavigationInteractor: public InteractorChainOfResponsibility {
public:
  SmallMultiplesNavigationInteractor(): InteractorChainOfResponsibility(":/i_select.png", "test") {
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
