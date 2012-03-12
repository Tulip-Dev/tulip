#include "tulip/QuickAccessBar.h"

#include <QtGui/QFontDatabase>
#include <QtGui/QComboBox>
#include <QtCore/QDebug>
#include <QtGui/QListView>
#include <QtGui/QMainWindow>
#include <QtGui/QMouseEvent>
#include <QtGui/QApplication>

#include <tulip/GraphPropertiesModel.h>
#include <tulip/TulipFontDialog.h>
#include <tulip/ForEach.h>
#include <tulip/ColorProperty.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/TlpQtTools.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlMainView.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/Perspective.h>

class TopPopupComboBox: public QComboBox {
  QListView* _view;

public:
  TopPopupComboBox(QWidget* parent = 0): QComboBox(parent), _view(NULL) {
  }

  bool eventFilter(QObject*, QEvent* ev) {
    if (ev->type() == QEvent::MouseButtonPress) {
      QMouseEvent* mouseEv = static_cast<QMouseEvent*>(ev);
      if (!_view->geometry().contains(mouseEv->globalPos()))
        _view->close();
      else {
        setCurrentIndex(_view->indexAt(mouseEv->pos()).row());
        _view->close();
      }
    }
    return false;
  }

  virtual void showPopup() {
    QPoint mainWindowPos = tlp::Perspective::instance()->mainWindow()->pos();
    if (_view == NULL) {
      _view = findChild<QListView*>();
      _view->installEventFilter(this);
      _view->viewport()->installEventFilter(this);
      _view->setParent(NULL);
      _view->setMouseTracking(true);
    }

    _view->setAutoScroll(false);
    _view->setWindowFlags(Qt::Popup);
    _view->resize(width(),200);
    _view->move(mapToGlobal(pos()).x()+mainWindowPos.x(),QCursor::pos().y()-200-height()/2);
    _view->show();
  }

  virtual void hidePopup() {
    _view->close();
  }

};

#include "ui_QuickAccessBar.h"

using namespace tlp;

QuickAccessBar::QuickAccessBar(QWidget *parent): QWidget(parent), _ui(new Ui::QuickAccessBar), _mainView(NULL), _oldFontScale(1), _oldNodeScale(1) {
  _ui->setupUi(this);
}

void QuickAccessBar::setGlMainView(GlMainView* v) {
  _mainView = v;
  reset();
}

void QuickAccessBar::reset() {
  _resetting = true;
  _ui->backgroundColorButton->setTulipColor(scene()->getBackgroundColor());
  _ui->nodeColorButton->setTulipColor(inputData()->getElementColor()->getNodeDefaultValue());
  _ui->colorInterpolationToggle->setChecked(renderingParameters()->isEdgeColorInterpolate());
  _ui->showEdgesToggle->setChecked(renderingParameters()->isDisplayEdges());
  _ui->showEdgesToggle->setIcon((renderingParameters()->isDisplayEdges() ? QIcon(":/tulip/gui/icons/20/edges_enabled.png") : QIcon(":/tulip/gui/icons/20/edges_disabled.png")));
  _ui->labelColorButton->setTulipColor(inputData()->getElementLabelColor()->getNodeDefaultValue());
  _ui->showLabelsToggle->setChecked(renderingParameters()->isViewNodeLabel());
  _ui->labelPropertyCombo->clear();
  _ui->labelPropertyCombo->addItem(trUtf8("Values on labels"));
  _ui->labelPropertyCombo->setModel(new GraphPropertiesModel<PropertyInterface>(trUtf8("Select property on labels"), _mainView->graph()));
  updateFontButtonStyle();
  _resetting = false;
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
    if (!_resetting) {
      qWarning("reset");
      out->setAllNodeValue("");
      out->setAllEdgeValue("");
    }
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
