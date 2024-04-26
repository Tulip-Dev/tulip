/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#include "tulip/QuickAccessBar.h"

#include <QComboBox>
#include <QFontDatabase>
#include <QGraphicsView>
#include <QListView>
#include <QMouseEvent>

#include <tulip/GraphModel.h>
#include <tulip/TulipFontDialog.h>
#include <tulip/ColorProperty.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/TlpQtTools.h>
#include <tulip/GlMainView.h>
#include <tulip/Perspective.h>
#include <tulip/SnapshotDialog.h>
#include <tulip/TulipItemDelegate.h>
#include <tulip/CaptionGraphicsSubItems.h>
#include <tulip/Perspective.h>

using namespace tlp;

class TopPopupComboBox : public QComboBox {
  QListView *_view;

public:
  TopPopupComboBox(QWidget *parent = nullptr) : QComboBox(parent), _view(nullptr) {}

  bool eventFilter(QObject *, QEvent *ev) override {
    if (ev->type() == QEvent::MouseButtonPress) {
      QMouseEvent *mouseEv = static_cast<QMouseEvent *>(ev);

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
      auto pos = mouseEv->globalPos();
#else
      auto pos = mouseEv->globalPosition().toPoint();
#endif
      if (!_view->geometry().contains(pos))
        _view->close();
      else {
        setCurrentIndex(_view->indexAt(mouseEv->pos()).row());
        _view->close();
      }
    }

    return false;
  }

  void showPopup() override {
    QPoint mainWindowPos = tlp::Perspective::instance()->mainWindow()->pos();

    if (_view == nullptr) {
      _view = findChild<QListView *>();
      _view->installEventFilter(this);
      _view->viewport()->installEventFilter(this);
      _view->setParent(nullptr);
      _view->setMouseTracking(true);
    }

    _view->setAutoScroll(false);
    _view->setWindowFlags(Qt::Popup);
    _view->resize(width(), 200);
    _view->move(mapToGlobal(pos()).x() + mainWindowPos.x(),
                QCursor::pos().y() - 200 - height() / 2);
    _view->show();
  }

  void hidePopup() override {
    if (_view != nullptr)
      _view->close();
  }
};

QuickAccessBar::QuickAccessBar(QWidget *parent) : QWidget(parent), _mainView(nullptr) {}

#include "ui_QuickAccessBar.h"

QPushButton *QuickAccessBarImpl::showNodesButton() {
  return _ui->showNodesToggle;
}

QPushButton *QuickAccessBarImpl::showEdgesButton() {
  return _ui->showEdgesToggle;
}

QPushButton *QuickAccessBarImpl::showLabelsButton() {
  return _ui->showLabelsToggle;
}

QPushButton *QuickAccessBarImpl::showLabelScaled() {
  return _ui->labelsScaledToggle;
}

ColorButton *QuickAccessBarImpl::backgroundColorButton() {
  return _ui->backgroundColorButton;
}

QPushButton *QuickAccessBarImpl::showColorInterpolation() {
  return _ui->colorInterpolationToggle;
}

QuickAccessBarImpl::QuickAccessBarImpl(QGraphicsItem *quickAccessBarItem,
                                       QuickAccessButtons buttons, QWidget *parent)
    : QuickAccessBar(parent), _ui(new Ui::QuickAccessBar), _quickAccessBarItem(quickAccessBarItem),
      delegate(new TulipItemDelegate(this)), _oldFontScale(1), _oldNodeScale(1),
      _captionsInitialized(false) {
  QString ss = Perspective::styleSheet();
  ss.append(
      "#QuickAccessBar { background-color: rgba(255,255,255,100); } QPushButton:hover { border: 1px solid #A0A0A0; border-radius: 4; } QPushButton:pressed { background-color: #A0A0A0;}");
  setStyleSheet(ss);
  _ui->setupUi(this);

  connect(_ui->backgroundColorButton, SIGNAL(colorChanged(QColor)), this,
          SLOT(setBackgroundColor(const QColor &)));
  connect(_ui->colorInterpolationToggle, SIGNAL(clicked(bool)), this,
          SLOT(setColorInterpolation(bool)));
  connect(_ui->sizeInterpolationToggle, SIGNAL(clicked(bool)), this,
          SLOT(setSizeInterpolation(bool)));
  connect(_ui->labelColorButton, SIGNAL(clicked(bool)), this, SLOT(setLabelColor()));
  connect(_ui->nodesColorCaptionButton, SIGNAL(clicked(bool)), this,
          SLOT(showHideNodesColorCaption()));
  connect(_ui->nodesSizeCaptionButton, SIGNAL(clicked(bool)), this,
          SLOT(showHideNodesSizeCaption()));
  connect(_ui->edgesColorCaptionButton, SIGNAL(clicked(bool)), this,
          SLOT(showHideEdgesColorCaption()));
  connect(_ui->edgesSizeCaptionButton, SIGNAL(clicked(bool)), this,
          SLOT(showHideEdgesSizeCaption()));
  connect(_ui->showEdgesToggle, SIGNAL(clicked(bool)), this, SLOT(setEdgesVisible(bool)));
  connect(_ui->showLabelsToggle, SIGNAL(clicked(bool)), this, SLOT(setLabelsVisible(bool)));
  connect(_ui->labelsScaledToggle, SIGNAL(clicked(bool)), this, SLOT(setLabelsScaled(bool)));
  connect(_ui->showNodesToggle, SIGNAL(clicked(bool)), this, SLOT(setNodesVisible(bool)));
  connect(_ui->fontButton, SIGNAL(clicked(bool)), this, SLOT(selectFont()));
  connect(_ui->screenshotButton, SIGNAL(clicked(bool)), this, SLOT(takeSnapshot()));
  connect(_ui->nodeColorButton, SIGNAL(clicked(bool)), this, SLOT(setNodeColor()));
  connect(_ui->edgeColorButton, SIGNAL(clicked(bool)), this, SLOT(setEdgeColor()));
  connect(_ui->nodeBorderColorButton, SIGNAL(clicked(bool)), this, SLOT(setNodeBorderColor()));
  connect(_ui->edgeBorderColorButton, SIGNAL(clicked(bool)), this, SLOT(setEdgeBorderColor()));
  connect(_ui->nodeIconButton, SIGNAL(clicked(bool)), this, SLOT(setNodeIcon()));
  connect(_ui->nodeShapeButton, SIGNAL(clicked(bool)), this, SLOT(setNodeShape()));
  connect(_ui->edgeShapeButton, SIGNAL(clicked(bool)), this, SLOT(setEdgeShape()));
  connect(_ui->nodeSizeButton, SIGNAL(clicked(bool)), this, SLOT(setNodeSize()));
  connect(_ui->edgeSizeButton, SIGNAL(clicked(bool)), this, SLOT(setEdgeSize()));
  connect(_ui->labelPositionButton, SIGNAL(clicked(bool)), this, SLOT(setNodeLabelPosition()));

  _ui->backgroundColorButton->setDialogTitle("Choose the background color");

  if (buttons.testFlag(ALLBUTTONS))
    return;

  if (!buttons.testFlag(NODESCOLORCAPTION))
    _ui->nodesColorCaptionButton->hide();

  if (!buttons.testFlag(NODESSIZECAPTION))
    _ui->nodesSizeCaptionButton->hide();

  if (!buttons.testFlag(EDGESCOLORCAPTION))
    _ui->edgesColorCaptionButton->hide();

  if (!buttons.testFlag(EDGESIZECAPTION))
    _ui->edgesSizeCaptionButton->hide();

  if (!buttons.testFlag(SCREENSHOT))
    _ui->screenshotButton->hide();

  if (!buttons.testFlag(BACKGROUNDCOLOR))
    _ui->backgroundColorButton->hide();

  if (!buttons.testFlag(NODECOLOR))
    _ui->nodeColorButton->hide();

  if (!buttons.testFlag(EDGECOLOR))
    _ui->edgeColorButton->hide();

  if (!buttons.testFlag(NODEBORDERCOLOR))
    _ui->nodeBorderColorButton->hide();

  if (!buttons.testFlag(EDGEBORDERCOLOR))
    _ui->edgeBorderColorButton->hide();

  if (!buttons.testFlag(LABELCOLOR))
    _ui->labelColorButton->hide();

  if (!buttons.testFlag(COLORINTERPOLATION))
    _ui->colorInterpolationToggle->hide();

  if (!buttons.testFlag(SIZEINTERPOLATION))
    _ui->sizeInterpolationToggle->hide();

  if (!buttons.testFlag(SHOWEDGES))
    _ui->showEdgesToggle->hide();

  if (!buttons.testFlag(SHOWLABELS))
    _ui->showLabelsToggle->hide();

  if (!buttons.testFlag(LABELSSCALED))
    _ui->labelsScaledToggle->hide();

  if (!buttons.testFlag(NODESHAPE))
    _ui->nodeShapeButton->hide();

  if (!buttons.testFlag(NODEICON))
    _ui->nodeIconButton->hide();

  if (!buttons.testFlag(EDGESHAPE))
    _ui->edgeShapeButton->hide();

  if (!buttons.testFlag(NODESIZE))
    _ui->nodeSizeButton->hide();

  if (!buttons.testFlag(EDGESIZE))
    _ui->edgeSizeButton->hide();

  if (!buttons.testFlag(NODELABELPOSITION))
    _ui->labelPositionButton->hide();

  if (!buttons.testFlag(SELECTFONT))
    _ui->fontButton->hide();

  if (!buttons.testFlag(SHOWNODES))
    _ui->showNodesToggle->hide();
}

void QuickAccessBarImpl::addButtonAtEnd(QAbstractButton *button) {
  QLayoutItem *spacer = _ui->horizontalLayout->itemAt(_ui->horizontalLayout->count() - 1);
  _ui->horizontalLayout->removeItem(spacer);
  _ui->horizontalLayout->addWidget(button);
  _ui->horizontalLayout->addItem(spacer);
}

void QuickAccessBarImpl::addButtonsAtEnd(const QVector<QAbstractButton *> &buttonvect) {
  QLayoutItem *spacer = _ui->horizontalLayout->itemAt(_ui->horizontalLayout->count() - 1);
  _ui->horizontalLayout->removeItem(spacer);

  for (auto b : buttonvect) {
    _ui->horizontalLayout->addWidget(b);
  }

  _ui->horizontalLayout->addItem(spacer);
}

void QuickAccessBarImpl::addSeparator() {
  QFrame *sep = new QFrame(this);
  sep->setFrameShape(QFrame::VLine);
  sep->setFrameShadow(QFrame::Sunken);
  QLayoutItem *spacer = _ui->horizontalLayout->itemAt(_ui->horizontalLayout->count() - 1);
  _ui->horizontalLayout->removeItem(spacer);
  _ui->horizontalLayout->addWidget(sep);
  _ui->horizontalLayout->addItem(spacer);
}

QuickAccessBarImpl::~QuickAccessBarImpl() {
  if (_captionsInitialized) {
    delete _captions[0];
    delete _captions[1];
    delete _captions[2];
    delete _captions[3];
  }

  delete _ui;
}

void QuickAccessBar::setGlMainView(GlMainView *v) {
  _mainView = v;
  reset();
}

void QuickAccessBarImpl::reset() {
  _resetting = true;

  _ui->backgroundColorButton->setTulipColor(scene()->getBackgroundColor());
  _ui->colorInterpolationToggle->setChecked(renderingParameters()->isEdgeColorInterpolate());
  _ui->sizeInterpolationToggle->setChecked(renderingParameters()->isEdgeSizeInterpolate());
  _ui->showEdgesToggle->setChecked(renderingParameters()->isDisplayEdges());
  _ui->showNodesToggle->setChecked(renderingParameters()->isDisplayNodes());
  _ui->showLabelsToggle->setChecked(renderingParameters()->isViewNodeLabel());
  _ui->labelsScaledToggle->setChecked(renderingParameters()->isLabelScaled());
  updateFontButtonStyle();
  _resetting = false;
}

void QuickAccessBarImpl::showHideNodesColorCaption() {
  showHideCaption(CaptionItem::NodesColorCaption);
}

void QuickAccessBarImpl::showHideNodesSizeCaption() {
  showHideCaption(CaptionItem::NodesSizeCaption);
}

void QuickAccessBarImpl::showHideEdgesColorCaption() {
  showHideCaption(CaptionItem::EdgesColorCaption);
}

void QuickAccessBarImpl::showHideEdgesSizeCaption() {
  showHideCaption(CaptionItem::EdgesSizeCaption);
}

void QuickAccessBarImpl::showHideCaption(CaptionItem::CaptionType captionType) {
  if (!_captionsInitialized) {
    _captionsInitialized = true;

    _captions[0] = new CaptionItem(_mainView);
    _captions[0]->create(CaptionItem::NodesColorCaption);
    _captions[0]->captionGraphicsItem()->setParentItem(_quickAccessBarItem);
    _captions[0]->captionGraphicsItem()->setVisible(false);

    _captions[1] = new CaptionItem(_mainView);
    _captions[1]->create(CaptionItem::NodesSizeCaption);
    _captions[1]->captionGraphicsItem()->setParentItem(_quickAccessBarItem);
    _captions[1]->captionGraphicsItem()->setVisible(false);

    _captions[2] = new CaptionItem(_mainView);
    _captions[2]->create(CaptionItem::EdgesColorCaption);
    _captions[2]->captionGraphicsItem()->setParentItem(_quickAccessBarItem);
    _captions[2]->captionGraphicsItem()->setVisible(false);

    _captions[3] = new CaptionItem(_mainView);
    _captions[3]->create(CaptionItem::EdgesSizeCaption);
    _captions[3]->captionGraphicsItem()->setParentItem(_quickAccessBarItem);
    _captions[3]->captionGraphicsItem()->setVisible(false);

    for (size_t i = 0; i < 4; i++) {
      connect(_captions[i]->captionGraphicsItem(), SIGNAL(interactionsActivated()),
              _captions[(i + 1) % 4]->captionGraphicsItem(), SLOT(removeInteractions()));
      connect(_captions[i]->captionGraphicsItem(), SIGNAL(interactionsActivated()),
              _captions[(i + 2) % 4]->captionGraphicsItem(), SLOT(removeInteractions()));
      connect(_captions[i]->captionGraphicsItem(), SIGNAL(interactionsActivated()),
              _captions[(i + 3) % 4]->captionGraphicsItem(), SLOT(removeInteractions()));
      connect(_captions[i], SIGNAL(filtering(bool)), _captions[(i + 1) % 4],
              SLOT(removeObservation(bool)));
      connect(_captions[i], SIGNAL(filtering(bool)), _captions[(i + 2) % 4],
              SLOT(removeObservation(bool)));
      connect(_captions[i], SIGNAL(filtering(bool)), _captions[(i + 3) % 4],
              SLOT(removeObservation(bool)));
    }
  }

  size_t captionIndice = 0;

  if (captionType == CaptionItem::NodesSizeCaption)
    captionIndice = 1;
  else if (captionType == CaptionItem::EdgesColorCaption)
    captionIndice = 2;
  else if (captionType == CaptionItem::EdgesSizeCaption)
    captionIndice = 3;

  _captions[captionIndice]->captionGraphicsItem()->setVisible(
      !_captions[captionIndice]->captionGraphicsItem()->isVisible());

  unsigned int numberVisible = 0;

  for (size_t i = 0; i < 4; i++) {
    if (_captions[i]->captionGraphicsItem()->isVisible()) {
      _captions[i]->captionGraphicsItem()->setPos(QPoint(numberVisible * 130, -260));
      numberVisible++;
    }
  }
}

void QuickAccessBarImpl::takeSnapshot() {
  SnapshotDialog dlg(_mainView, _mainView->graphicsView()->window());
  dlg.exec();
}

void QuickAccessBarImpl::setBackgroundColor(const QColor &c) {
  if (scene()->getBackgroundColor() != QColorToColor(c)) {
    scene()->setBackgroundColor(QColorToColor(c));
    _mainView->emitDrawNeededSignal();
    emit settingsChanged();
  }
}

void QuickAccessBarImpl::setColorInterpolation(bool f) {
  if (renderingParameters()->isEdgeColorInterpolate() != f) {
    renderingParameters()->setEdgeColorInterpolate(f);
    _mainView->emitDrawNeededSignal();
    emit settingsChanged();
  }
}

void QuickAccessBarImpl::setSizeInterpolation(bool f) {
  if (renderingParameters()->isEdgeSizeInterpolate() != f) {
    renderingParameters()->setEdgeSizeInterpolate(f);
    _mainView->emitDrawNeededSignal();
    emit settingsChanged();
  }
}

void QuickAccessBarImpl::setLabelColor() {
  ColorProperty *labelColors = inputData()->getElementLabelColor();
  ColorProperty *labelBorderColors = inputData()->getElementLabelBorderColor();
  QVariant val = TulipItemDelegate::showEditorDialog(NODE, labelColors, _mainView->graph(),
                                                     delegate, _mainView->graphicsView()->window(),
                                                     UINT_MAX, QString("Select the label color"));

  // Check if edition has been cancelled
  if (!val.isValid())
    return;

  BooleanProperty *selected = inputData()->getElementSelected();
  bool hasSelected = false;

  _mainView->graph()->push();

  Observable::holdObservers();

  Color color = val.value<tlp::Color>();

  for (auto n : selected->getNonDefaultValuatedNodes(_mainView->graph())) {
    labelColors->setNodeValue(n, color);
    labelBorderColors->setNodeValue(n, color);
    hasSelected = true;
  }

  if (hasSelected == false) {
    labelColors->setAllNodeValue(color);
    labelBorderColors->setAllNodeValue(color);
  }

  for (auto e : selected->getNonDefaultValuatedEdges(_mainView->graph())) {
    labelColors->setEdgeValue(e, color);
    labelBorderColors->setEdgeValue(e, color);
    hasSelected = true;
  }

  if (hasSelected == false) {
    labelColors->setAllEdgeValue(color);
    labelBorderColors->setAllEdgeValue(color);
  }

  Observable::unholdObservers();
  _mainView->graph()->popIfNoUpdates();
  emit settingsChanged();
}

void QuickAccessBarImpl::setNodeColor() {
  setAllValues(NODE, inputData()->getElementColor(), QString("Select the color of nodes"));
}

void QuickAccessBarImpl::setEdgeColor() {
  setAllValues(EDGE, inputData()->getElementColor(), QString("Select the color of edges"));
}

void QuickAccessBarImpl::setNodeBorderColor() {
  setAllValues(NODE, inputData()->getElementBorderColor(),
               QString("Select the border color of nodes"));
}

void QuickAccessBarImpl::setEdgeBorderColor() {
  setAllValues(EDGE, inputData()->getElementBorderColor(),
               QString("Select the border color of edges"));
}

void QuickAccessBarImpl::setAllValues(unsigned int eltType, PropertyInterface *prop,
                                      QString dialogTitle) {
  QVariant val = TulipItemDelegate::showEditorDialog(
      static_cast<tlp::ElementType>(eltType), prop, _mainView->graph(), delegate,
      _mainView->graphicsView()->window(), UINT_MAX, dialogTitle);

  // Check if edition has been cancelled
  if (!val.isValid())
    return;

  BooleanProperty *selected = inputData()->getElementSelected();
  bool hasSelected = false;

  _mainView->graph()->push();

  Observable::holdObservers();

  if (eltType == NODE) {
    for (auto n : selected->getNonDefaultValuatedNodes(_mainView->graph())) {
      GraphModel::setNodeValue(n.id, prop, val);
      hasSelected = true;
    }

    if (hasSelected == false)
      GraphModel::setAllNodeValue(prop, val);
  } else {
    for (auto e : selected->getNonDefaultValuatedEdges(_mainView->graph())) {
      GraphModel::setEdgeValue(e.id, prop, val);
      hasSelected = true;
    }

    if (hasSelected == false)
      GraphModel::setAllEdgeValue(prop, val);
  }

  Observable::unholdObservers();
  _mainView->graph()->popIfNoUpdates();
  emit settingsChanged();
}

void QuickAccessBarImpl::setNodeShape() {
  setAllValues(NODE, inputData()->getElementShape(), QString("Select the shape of nodes"));
}

void QuickAccessBarImpl::setNodeIcon() {
  setAllValues(NODE, inputData()->getElementIcon(), QString("Select the icon of nodes"));
}

void QuickAccessBarImpl::setEdgeShape() {
  setAllValues(EDGE, inputData()->getElementShape(), QString("Select the shape of edges"));
}

void QuickAccessBarImpl::setNodeSize() {
  setAllValues(NODE, inputData()->getElementSize(), QString("Select the size of nodes"));
}

void QuickAccessBarImpl::setEdgeSize() {
  setAllValues(EDGE, inputData()->getElementSize(), QString("Select the size of edges"));
}

void QuickAccessBarImpl::setNodeLabelPosition() {
  setAllValues(NODE, inputData()->getElementLabelPosition(), QString("node label position"));
}

void QuickAccessBarImpl::setEdgesVisible(bool v) {
  if (renderingParameters()->isDisplayEdges() != v) {
    renderingParameters()->setDisplayEdges(v);
    _mainView->emitDrawNeededSignal();
    emit settingsChanged();
  }
}

void QuickAccessBarImpl::setNodesVisible(bool v) {
  if (renderingParameters()->isDisplayNodes() != v) {
    renderingParameters()->setDisplayNodes(v);
    _mainView->emitDrawNeededSignal();
    emit settingsChanged();
  }
}

void QuickAccessBarImpl::setLabelsVisible(bool v) {
  if (renderingParameters()->isViewNodeLabel() != v) {
    renderingParameters()->setViewNodeLabel(v);
    _mainView->emitDrawNeededSignal();
    emit settingsChanged();
  }
}

void QuickAccessBarImpl::setLabelsScaled(bool v) {
  if (renderingParameters()->isLabelScaled() != v) {
    renderingParameters()->setLabelScaled(v);
    _mainView->emitDrawNeededSignal();
    emit settingsChanged();
  }
}

GlGraphRenderingParameters *QuickAccessBar::renderingParameters() const {
  return scene()->getGlGraphComposite()->getRenderingParametersPointer();
}

GlGraphInputData *QuickAccessBar::inputData() const {
  return scene()->getGlGraphComposite()->getInputData();
}

GlScene *QuickAccessBar::scene() const {
  return _mainView->getGlMainWidget()->getScene();
}

void QuickAccessBarImpl::selectFont() {
  TulipFontDialog dlg(_mainView->graphicsView()->window());
  dlg.selectFont(TulipFont::fromFile(inputData()->getElementFont()->getNodeDefaultValue().c_str()));

  if (dlg.exec() != QDialog::Accepted || !dlg.font().exists())
    return;

  _mainView->graph()->push();

  Observable::holdObservers();

  inputData()->getElementFont()->setAllNodeValue(QStringToTlpString(dlg.font().fontFile()));
  inputData()->getElementFont()->setAllEdgeValue(QStringToTlpString(dlg.font().fontFile()));
  inputData()->getElementFontSize()->setAllNodeValue(dlg.fontSize());
  inputData()->getElementFontSize()->setAllEdgeValue(dlg.fontSize());

  Observable::unholdObservers();
  _mainView->graph()->popIfNoUpdates();
  updateFontButtonStyle();
  emit settingsChanged();
}

void QuickAccessBarImpl::updateFontButtonStyle() {
  QString fontFile = inputData()->getElementFont()->getNodeDefaultValue().c_str();
  TulipFont selectedFont = TulipFont::fromFile(fontFile);
  _ui->fontButton->setStyleSheet("font-family: " + selectedFont.fontFamily() + "; " +
                                 (selectedFont.isItalic() ? "font-style: italic; " : "") +
                                 (selectedFont.isBold() ? "font-weight: bold; " : ""));
}
