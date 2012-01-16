#include "tulip/QuickAccessBar.h"

#include <QtGui/QFontDatabase>

#include <tulip/TulipFontDialog.h>
#include <tulip/ForEach.h>
#include <tulip/ColorProperty.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/TlpQtTools.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlMainView.h>
#include <tulip/GlGraphInputData.h>
#include "ui_QuickAccessBar.h"

#include <QtCore/QDebug>

using namespace tlp;

QuickAccessBar::QuickAccessBar(QWidget *parent): QWidget(parent), _ui(new Ui::QuickAccessBar), _mainView(NULL), _oldFontScale(1), _oldNodeScale(1) {
  _ui->setupUi(this);
}

void QuickAccessBar::setGlMainView(GlMainView* v) {
  _mainView = v;
  reset();
}

void QuickAccessBar::reset() {
  _ui->backgroundColorButton->setTulipColor(scene()->getBackgroundColor());
  _ui->nodeColorButton->setTulipColor(inputData()->getElementColor()->getNodeDefaultValue());
  _ui->colorInterpolationToggle->setChecked(renderingParameters()->isEdgeColorInterpolate());
  _ui->showEdgesToggle->setChecked(renderingParameters()->isDisplayEdges());
  _ui->showEdgesToggle->setIcon((renderingParameters()->isDisplayEdges() ? QIcon(":/tulip/gui/icons/20/edges_enabled.png") : QIcon(":/tulip/gui/icons/20/edges_disabled.png")));
  _ui->labelColorButton->setTulipColor(inputData()->getElementLabelColor()->getNodeDefaultValue());
  _ui->showLabelsToggle->setChecked(renderingParameters()->isViewNodeLabel());
  _ui->labelPropertyCombo->clear();
  _ui->labelPropertyCombo->addItem(trUtf8("Values on labels"));
  std::string prop;
  forEach(prop,_mainView->graph()->getProperties())
  _ui->labelPropertyCombo->addItem(prop.c_str());
  updateFontButtonStyle();
}

void QuickAccessBar::takeSnapshot() {
}

void QuickAccessBar::setBackgroundColor(const QColor& c) {
  scene()->setBackgroundColor(QColorToColor(c));
  _mainView->emitDrawNeededSignal();
}

void QuickAccessBar::setColorInterpolation(bool f) {
  renderingParameters()->setEdgeColorInterpolate(f);
  _mainView->emitDrawNeededSignal();
}
void QuickAccessBar::scaleFont(int scale) {
  Observable::holdObservers();

  double realScale = scale/10.;
  IntegerProperty* in = inputData()->getElementFontSize();
  node n;
  forEach(n,_mainView->graph()->getNodes()) {
    int oldSize = in->getNodeValue(n);
    in->setNodeValue(n,(oldSize / _oldFontScale) * realScale);
  }
  edge e;
  forEach(e,_mainView->graph()->getEdges()) {
    int oldSize = in->getEdgeValue(e);
    in->setEdgeValue(e,ceil((oldSize * 1. / _oldFontScale) * realScale));
  }

  _oldFontScale = realScale;
  Observable::unholdObservers();
}

void QuickAccessBar::setLabelColor(const QColor& c) {
  Observable::holdObservers();
  ColorProperty* tmp = new ColorProperty(_mainView->graph());
  ColorProperty* colors = inputData()->getElementLabelColor();

  tmp->setAllNodeValue(QColorToColor(c));
  tmp->setAllEdgeValue(QColorToColor(c));

  node n;
  forEach(n, colors->getNonDefaultValuatedNodes()) {
    tmp->setNodeValue(n,colors->getNodeValue(n));
  }

  edge e;
  forEach(e, colors->getNonDefaultValuatedEdges()) {
    tmp->setEdgeValue(e,colors->getEdgeValue(e));
  }
  *colors = *tmp;
  Observable::unholdObservers();
}

void QuickAccessBar::setLabelProperty(const QString& prop) {
  Observable::holdObservers();

  StringProperty *out = inputData()->getElementLabel();

  if (_ui->labelPropertyCombo->currentIndex() == 0) {
    out->setAllNodeValue("");
    out->setAllEdgeValue("");
  }

  else if (_mainView->graph()->existProperty(prop.toStdString())) {
    PropertyInterface *in = _mainView->graph()->getProperty(prop.toStdString());

    node n;
    forEach(n, _mainView->graph()->getNodes())
    out->setNodeValue(n, in->getNodeStringValue(n));

    edge e;
    forEach(e, _mainView->graph()->getEdges())
    out->setEdgeValue(e, in->getEdgeStringValue(e));
  }

  Observable::unholdObservers();
}

void QuickAccessBar::setNodeColor(const QColor& c) {
  Observable::holdObservers();
  ColorProperty* tmp = new ColorProperty(_mainView->graph());
  ColorProperty* colors = inputData()->getElementColor();

  tmp->setAllNodeValue(QColorToColor(c));
  tmp->setAllEdgeValue(colors->getEdgeDefaultValue());

  node n;
  forEach(n, colors->getNonDefaultValuatedNodes()) {
    tmp->setNodeValue(n,colors->getNodeValue(n));
  }

  edge e;
  forEach(e, colors->getNonDefaultValuatedEdges()) {
    tmp->setEdgeValue(e,colors->getEdgeValue(e));
  }
  *colors = *tmp;
  Observable::unholdObservers();
}

void QuickAccessBar::scaleNodes(int scale) {
  Observable::holdObservers();

  double realScale = scale/10.;
  SizeProperty* in = inputData()->getElementSize();
  node n;
  forEach(n,_mainView->graph()->getNodes()) {
    Size oldSize = in->getNodeValue(n);
    in->setNodeValue(n,(oldSize / _oldFontScale) * realScale);
  }

  _oldFontScale = realScale;
  Observable::unholdObservers();
}

void QuickAccessBar::setEdgesVisible(bool v) {
  renderingParameters()->setDisplayEdges(v);
  _ui->showEdgesToggle->setIcon((v ? QIcon(":/tulip/gui/icons/20/edges_enabled.png") : QIcon(":/tulip/gui/icons/20/edges_disabled.png")));
  _mainView->emitDrawNeededSignal();
}

void QuickAccessBar::setLabelsVisible(bool v) {
  renderingParameters()->setViewNodeLabel(v);
  _mainView->emitDrawNeededSignal();
}

GlGraphRenderingParameters* QuickAccessBar::renderingParameters() const {
  return scene()->getGlGraphComposite()->getRenderingParametersPointer();
}

GlGraphInputData* QuickAccessBar::inputData() const {
  return scene()->getGlGraphComposite()->getInputData();
}

GlScene* QuickAccessBar::scene() const {
  return _mainView->getGlMainWidget()->getScene();
}

void QuickAccessBar::selectFont() {
  TulipFontDialog dlg;
  dlg.selectFont(TulipFont::fromFile(inputData()->getElementFont()->getNodeDefaultValue().c_str()));

  if (dlg.exec() != QDialog::Accepted || !dlg.font().exists())
    return;

  Observable::holdObservers();
  inputData()->getElementFont()->setAllNodeValue(dlg.font().fontFile().toStdString());
  inputData()->getElementFont()->setAllEdgeValue(dlg.font().fontFile().toStdString());
  Observable::unholdObservers();
  updateFontButtonStyle();
}

void QuickAccessBar::updateFontButtonStyle() {
  QString fontFile = inputData()->getElementFont()->getNodeDefaultValue().c_str();
  TulipFont selectedFont = TulipFont::fromFile(fontFile);
  _ui->fontButton->setStyleSheet("font-family: " + selectedFont.fontFamily() + "; "
                                 + (selectedFont.isItalic() ? "font-style: italic; " : "")
                                 + (selectedFont.isBold() ? "font-weight: bold; " : ""));
  _ui->fontButton->setText(selectedFont.fontName() + (selectedFont.isBold() ? " Bold" : "") + (selectedFont.isItalic() ? " Italic" : ""));
}
