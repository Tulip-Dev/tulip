#include "tulip/SmallMultiplesView.h"

#include "tulip/AbstractSmallMultiplesModel.h"
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

SmallMultiplesView::SmallMultiplesView(AbstractSmallMultiplesModel *model)
  :AbstractView(), _model(model), _overview(new GlMainWidget(0)), _zoomAnimationActivated(true) {
  Observable::holdObservers();
  assert(model);
  connect(_model, SIGNAL(dataChanged(int)), this, SLOT(dataChanged(int)));
  connect(_model, SIGNAL(dataChanged()), this, SLOT(dataChanged()));
  _overview->setData(newGraph(), DataSet());
  GlScene *scene = _overview->getScene();
  GlGraphInputData *inputData = _overview->getScene()->getGlGraphComposite()->getInputData();
  inputData->elementColor->setAllNodeValue(scene->getBackgroundColor());
  inputData->elementShape->setAllNodeValue(4);
  inputData->elementLabelPosition->setAllNodeValue(2);
  _overview->getScene()->getGlGraphComposite()->getRenderingParametersPointer()->setFontsType(1);
  Observable::unholdObservers();
}

SmallMultiplesView::~SmallMultiplesView() {
  delete _model;
  if (!isOverview()) // Otherwise it's deleted along with the view's widget.
    delete _overview;
}

QWidget *SmallMultiplesView::construct(QWidget *parent) {
  QWidget *centralWidget = AbstractView::construct(parent);
  setCentralWidget(_overview);
  toggleInteractors(false);
  return centralWidget;
}

GlMainWidget *SmallMultiplesView::overview() const {
  return _overview;
}

void SmallMultiplesView::dataChanged() {
  Observable::holdObservers();
  refreshItemsCount();
  for (int i=0; i < _items.size(); ++i)
    dataChanged(i);
  Observable::unholdObservers();
}

void SmallMultiplesView::dataChanged(int id) {
  Observable::holdObservers();

  if (id >= _items.size())
    return;
  node n = _items[id];
  GlGraphInputData *inputData = _overview->getScene()->getGlGraphComposite()->getInputData();

  // Texture
  QVariant textureVariant = _model->data(id, AbstractSmallMultiplesModel::Texture);
  inputData->elementTexture->setNodeValue(n, (textureVariant.isValid() ? textureVariant.toString().toStdString() : ""));

  // Label
  QVariant labelVariant = _model->data(id, AbstractSmallMultiplesModel::Label);
  inputData->elementLabel->setNodeValue(n, (labelVariant.isValid() ? labelVariant.toString().toStdString() : ""));

  // Position
  QVariant positionVariant = _model->data(id, AbstractSmallMultiplesModel::Position);
  if (positionVariant.isValid())
    inputData->elementLayout->setNodeValue(n,positionVariant.value<Coord>());

  Observable::unholdObservers();
}

void SmallMultiplesView::itemAdded() {
  _items.push_back(_overview->getGraph()->addNode());
}

void SmallMultiplesView::itemDeleted(int id) {
  Observable::holdObservers();
  if (id >= _items.size())
    return;
  node n = _items[id];
  _items.erase(_items.begin() + id);
  _overview->getGraph()->delNode(n);
  Observable::unholdObservers();
}

void SmallMultiplesView::selectItem(int i) {
  if (i > _items.size())
    return;

  if (_zoomAnimationActivated) {
    GlNode glNode(_items[i]);
    zoomOnScreenRegion(_overview, glNode.getBoundingBox(_overview->getScene()->getGlGraphComposite()->getInputData()));
  }

  itemSelected(i);
}

void SmallMultiplesView::refreshItemsCount() {
  Observable::holdObservers();
  node n;
  stableForEach (n, _overview->getGraph()->getNodes())
      _overview->getGraph()->delNode(n);
  _items.clear();
  int itemCount = _model->countItems();
  for (int i=0; i < itemCount; ++i)
    itemAdded();
  Observable::unholdObservers();
}

AbstractSmallMultiplesModel *SmallMultiplesView::model() {
  return _model;
}

bool SmallMultiplesView::isOverview() const {
  return centralWidget == _overview;
}

void SmallMultiplesView::switchToOverview(bool destroyOldWidget) {
  QWidget *w = centralWidget;
  setCentralWidget(_overview);
  if (destroyOldWidget)
    delete w;

  GlGraphInputData *inputData = _overview->getScene()->getGlGraphComposite()->getInputData();
  zoomOnScreenRegion(_overview, computeBoundingBox(_overview->getGraph(), inputData->elementLayout, inputData->elementSize, inputData->elementRotation));
}

void SmallMultiplesView::switchToWidget(QWidget *w) {
  setCentralWidget(w);
}

void SmallMultiplesView::toggleInteractors(bool f) {
  list<Interactor *> interactors = getInteractors();
  int i=0;
  for (list<Interactor *>::iterator it = interactors.begin(); it != interactors.end(); ++it) {
    if (i++ > 0) { // FIXME: Find another way to detect the SmallMultiplesView navigation interactor.
      (*it)->getAction()->setEnabled(f);
      (*it)->getAction()->setChecked(f);
    }
    else if (!f)// When we disable interactors, check the SmallMultiplesView navigation interactor.
      (*it)->getAction()->setChecked(true);
  }
}

int SmallMultiplesView::nodeItemId(node n) {
  for (int i=0; i < _items.size(); ++i)
    if (_items[i] == n)
      return i;
  return -1;
}

}

/*
 * Default interactor for small multiples views
 */
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
