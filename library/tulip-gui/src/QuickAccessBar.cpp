/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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

#include <QFontDatabase>
#include <QComboBox>
#include <QDebug>
#include <QListView>
#include <QMainWindow>
#include <QMouseEvent>
#include <QApplication>
#include <QGraphicsView>

#include <tulip/GraphPropertiesModel.h>
#include <tulip/GraphModel.h>
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
#include <tulip/SnapshotDialog.h>
#include <tulip/TulipItemDelegate.h>
#include <tulip/CaptionGraphicsSubItems.h>

class TopPopupComboBox: public QComboBox {
  QListView* _view;

public:
  TopPopupComboBox(QWidget* parent = NULL): QComboBox(parent), _view(NULL) {
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
    if (_view != NULL)
      _view->close();
  }

};

#include "ui_QuickAccessBar.h"

using namespace tlp;

QuickAccessBar::QuickAccessBar(QGraphicsItem *quickAccessBarItem, QWidget *parent)
  : QWidget(parent), _ui(new Ui::QuickAccessBar), _quickAccessBarItem(quickAccessBarItem), _mainView(NULL), delegate(new TulipItemDelegate(this)), _oldFontScale(1), _oldNodeScale(1),_captionsInitialized(false) {
  _ui->setupUi(this);
  _ui->backgroundColorButton->setDialogTitle("Choose the background color");
  _ui->nodeColorButton->setDialogTitle("Choose the node's default color");
  _ui->edgeColorButton->setDialogTitle("Choose the edge's default color");
  _ui->nodeBorderColorButton->setDialogTitle("Choose the default color for the border of nodes");
  _ui->edgeBorderColorButton->setDialogTitle("Choose the default color for the border of edges");
  _ui->labelColorButton->setDialogTitle("Choose the default color for the label of nodes or edges");
}

QuickAccessBar::~QuickAccessBar() {
  if(_captionsInitialized) {
    delete _captions[0];
    delete _captions[1];
    delete _captions[2];
    delete _captions[3];
  }

  delete _ui;
}

void QuickAccessBar::setGlMainView(GlMainView* v) {
  _mainView = v;
  reset();
}

void QuickAccessBar::reset() {
  _resetting = true;

  _ui->backgroundColorButton->setDialogParent(tlp::Perspective::instance() ? tlp::Perspective::instance()->mainWindow() : NULL);
  _ui->nodeColorButton->setDialogParent(tlp::Perspective::instance() ? tlp::Perspective::instance()->mainWindow() : NULL);
  _ui->edgeColorButton->setDialogParent(tlp::Perspective::instance() ? tlp::Perspective::instance()->mainWindow() : NULL);
  _ui->nodeBorderColorButton->setDialogParent(tlp::Perspective::instance() ? tlp::Perspective::instance()->mainWindow() : NULL);
  _ui->edgeBorderColorButton->setDialogParent(tlp::Perspective::instance() ? tlp::Perspective::instance()->mainWindow() : NULL);
  _ui->labelColorButton->setDialogParent(tlp::Perspective::instance() ? tlp::Perspective::instance()->mainWindow() : NULL);

  _ui->backgroundColorButton->setTulipColor(scene()->getBackgroundColor());
  _ui->colorInterpolationToggle->setChecked(renderingParameters()->isEdgeColorInterpolate());
  _ui->colorInterpolationToggle->setIcon((renderingParameters()->isEdgeColorInterpolate() ? QIcon(":/tulip/gui/icons/20/color_interpolation_enabled.png") : QIcon(":/tulip/gui/icons/20/color_interpolation_disabled.png")));
  _ui->sizeInterpolationToggle->setChecked(renderingParameters()->isEdgeSizeInterpolate());
  _ui->sizeInterpolationToggle->setIcon((renderingParameters()->isEdgeSizeInterpolate() ? QIcon(":/tulip/gui/icons/20/size_interpolation_enabled.png") : QIcon(":/tulip/gui/icons/20/size_interpolation_disabled.png")));
  _ui->showEdgesToggle->setChecked(renderingParameters()->isDisplayEdges());
  _ui->showEdgesToggle->setIcon((renderingParameters()->isDisplayEdges() ? QIcon(":/tulip/gui/icons/20/edges_enabled.png") : QIcon(":/tulip/gui/icons/20/edges_disabled.png")));
  _ui->showLabelsToggle->setChecked(renderingParameters()->isViewNodeLabel());
  _ui->showLabelsToggle->setIcon((renderingParameters()->isViewNodeLabel() ? QIcon(":/tulip/gui/icons/20/labels_enabled.png") : QIcon(":/tulip/gui/icons/20/labels_disabled.png")));
  _ui->labelsScaledToggle->setChecked(renderingParameters()->isLabelScaled());
  _ui->labelsScaledToggle->setIcon((renderingParameters()->isLabelScaled() ? QIcon(":/tulip/gui/icons/20/labels_scaled_enabled.png") : QIcon(":/tulip/gui/icons/20/labels_scaled_disabled.png")));
  updateFontButtonStyle();
  _resetting = false;
}

void QuickAccessBar::showHideNodesColorCaption() {
  showHideCaption(CaptionItem::NodesColorCaption);
}

void QuickAccessBar::showHideNodesSizeCaption() {
  showHideCaption(CaptionItem::NodesSizeCaption);
}

void QuickAccessBar::showHideEdgesColorCaption() {
  showHideCaption(CaptionItem::EdgesColorCaption);
}

void QuickAccessBar::showHideEdgesSizeCaption() {
  showHideCaption(CaptionItem::EdgesSizeCaption);
}

void QuickAccessBar::showHideCaption(CaptionItem::CaptionType captionType) {
  if(!_captionsInitialized) {
    _captionsInitialized=true;

    _captions[0]=new CaptionItem(_mainView);
    _captions[0]->create(CaptionItem::NodesColorCaption);
    _captions[0]->captionGraphicsItem()->setParentItem(_quickAccessBarItem);
    _captions[0]->captionGraphicsItem()->setVisible(false);

    _captions[1]=new CaptionItem(_mainView);
    _captions[1]->create(CaptionItem::NodesSizeCaption);
    _captions[1]->captionGraphicsItem()->setParentItem(_quickAccessBarItem);
    _captions[1]->captionGraphicsItem()->setVisible(false);

    _captions[2]=new CaptionItem(_mainView);
    _captions[2]->create(CaptionItem::EdgesColorCaption);
    _captions[2]->captionGraphicsItem()->setParentItem(_quickAccessBarItem);
    _captions[2]->captionGraphicsItem()->setVisible(false);

    _captions[3]=new CaptionItem(_mainView);
    _captions[3]->create(CaptionItem::EdgesSizeCaption);
    _captions[3]->captionGraphicsItem()->setParentItem(_quickAccessBarItem);
    _captions[3]->captionGraphicsItem()->setVisible(false);

    for(size_t i=0; i<4; i++) {
      connect(_captions[i]->captionGraphicsItem(),SIGNAL(interactionsActivated()),_captions[(i+1)%4]->captionGraphicsItem(),SLOT(removeInteractions()));
      connect(_captions[i]->captionGraphicsItem(),SIGNAL(interactionsActivated()),_captions[(i+2)%4]->captionGraphicsItem(),SLOT(removeInteractions()));
      connect(_captions[i]->captionGraphicsItem(),SIGNAL(interactionsActivated()),_captions[(i+3)%4]->captionGraphicsItem(),SLOT(removeInteractions()));
      connect(_captions[i],SIGNAL(filtering(bool)),_captions[(i+1)%4],SLOT(removeObservation(bool)));
      connect(_captions[i],SIGNAL(filtering(bool)),_captions[(i+2)%4],SLOT(removeObservation(bool)));
      connect(_captions[i],SIGNAL(filtering(bool)),_captions[(i+3)%4],SLOT(removeObservation(bool)));
    }
  }

  size_t captionIndice=0;

  if(captionType==CaptionItem::NodesSizeCaption)
    captionIndice=1;
  else if(captionType==CaptionItem::EdgesColorCaption)
    captionIndice=2;
  else if(captionType==CaptionItem::EdgesSizeCaption)
    captionIndice=3;

  _captions[captionIndice]->captionGraphicsItem()->setVisible(!_captions[captionIndice]->captionGraphicsItem()->isVisible());

  unsigned int numberVisible=0;

  for(size_t i=0; i<4; i++) {
    if(_captions[i]->captionGraphicsItem()->isVisible()) {
      _captions[i]->captionGraphicsItem()->setPos(QPoint(numberVisible*130,-260));
      numberVisible++;
    }
  }
}

void QuickAccessBar::takeSnapshot() {
  _mainView->openSnapshotDialog();
}

void QuickAccessBar::setBackgroundColor(const QColor& c) {
  if(scene()->getBackgroundColor()!=QColorToColor(c)) {
    scene()->setBackgroundColor(QColorToColor(c));
    _mainView->emitDrawNeededSignal();
    emit settingsChanged();
  }
}

void QuickAccessBar::setColorInterpolation(bool f) {
  if(renderingParameters()->isEdgeColorInterpolate()!=f) {
    renderingParameters()->setEdgeColorInterpolate(f);
    _ui->colorInterpolationToggle->setIcon((f ? QIcon(":/tulip/gui/icons/20/color_interpolation_enabled.png") : QIcon(":/tulip/gui/icons/20/color_interpolation_disabled.png")));
    _mainView->emitDrawNeededSignal();
    emit settingsChanged();
  }
}

void QuickAccessBar::setSizeInterpolation(bool f) {
  if(renderingParameters()->isEdgeSizeInterpolate()!=f) {
    renderingParameters()->setEdgeSizeInterpolate(f);
    _ui->sizeInterpolationToggle->setIcon((f ? QIcon(":/tulip/gui/icons/20/size_interpolation_enabled.png") : QIcon(":/tulip/gui/icons/20/size_interpolation_disabled.png")));
    _mainView->emitDrawNeededSignal();
    emit settingsChanged();
  }
}

void QuickAccessBar::setLabelColor(const QColor& c) {
  _mainView->graph()->push();

  Observable::holdObservers();
  ColorProperty* tmp = new ColorProperty(_mainView->graph());
  ColorProperty* colors = inputData()->getElementLabelColor();

  *tmp=*colors;

  if(colors->getNodeDefaultValue()!=QColorToColor(c)) {
    colors->setAllNodeValue(QColorToColor(c));

    node n;
    forEach(n, tmp->getNonDefaultValuatedNodes()) {
      colors->setNodeValue(n,tmp->getNodeValue(n));
    }
  }

  if(colors->getEdgeDefaultValue()!=QColorToColor(c)) {
    colors->setAllEdgeValue(QColorToColor(c));

    edge e;
    forEach(e, tmp->getNonDefaultValuatedEdges()) {
      colors->setEdgeValue(e,tmp->getEdgeValue(e));
    }
  }

  Observable::unholdObservers();
  emit settingsChanged();
}

void QuickAccessBar::setAllColorValues(unsigned int eltType,
                                       ColorProperty* prop, const Color &color) {
  BooleanProperty* selected = inputData()->getElementSelected();
  bool hasSelected = false;

  _mainView->graph()->push();

  Observable::holdObservers();

  if (eltType == NODE) {
    node n;
    forEach(n, selected->getNonDefaultValuatedNodes(_mainView->graph())) {
      prop->setNodeValue(n, color);
      hasSelected = true;
    }

    if (hasSelected == false)
      prop->setAllNodeValue(color);
  }
  else {
    edge e;
    forEach(e, selected->getNonDefaultValuatedEdges(_mainView->graph())) {
      prop->setEdgeValue(e, color);
      hasSelected = true;
    }

    if (hasSelected == false)
      prop->setAllEdgeValue(color);
  }

  Observable::unholdObservers();
  emit settingsChanged();
}

void QuickAccessBar::setNodeColor(const QColor& c) {
  setAllColorValues(NODE, inputData()->getElementColor(), QColorToColor(c));
}

void QuickAccessBar::setEdgeColor(const QColor& c) {
  setAllColorValues(EDGE, inputData()->getElementColor(), QColorToColor(c));
}

void QuickAccessBar::setNodeBorderColor(const QColor& c) {
  setAllColorValues(NODE, inputData()->getElementBorderColor(), QColorToColor(c));
}

void QuickAccessBar::setEdgeBorderColor(const QColor& c) {
  setAllColorValues(EDGE, inputData()->getElementBorderColor(), QColorToColor(c));
}

void QuickAccessBar::setAllValues(unsigned int eltType,
                                  PropertyInterface* prop) {
  QVariant val =
    TulipItemDelegate::showEditorDialog((tlp::ElementType) eltType,
                                        prop, _mainView->graph(),
                                        delegate, _mainView->getGlMainWidget());

  // Check if edition has been cancelled
  if (!val.isValid())
    return;

  BooleanProperty* selected = inputData()->getElementSelected();
  bool hasSelected = false;

  _mainView->graph()->push();

  Observable::holdObservers();

  if (eltType == NODE) {
    node n;
    forEach(n, selected->getNonDefaultValuatedNodes(_mainView->graph())) {
      GraphModel::setNodeValue(n.id, prop, val);
      hasSelected = true;
    }

    if (hasSelected == false)
      GraphModel::setAllNodeValue(prop, val);
  }
  else {
    edge e;
    forEach(e, selected->getNonDefaultValuatedEdges(_mainView->graph())) {
      GraphModel::setEdgeValue(e.id, prop, val);
      hasSelected = true;
    }

    if (hasSelected == false)
      GraphModel::setAllEdgeValue(prop, val);
  }

  Observable::unholdObservers();
  emit settingsChanged();
}

void QuickAccessBar::setNodeShape() {
  setAllValues(NODE, inputData()->getElementShape());
}

void QuickAccessBar::setEdgeShape() {
  setAllValues(EDGE, inputData()->getElementShape());
}

void QuickAccessBar::setNodeSize() {
  setAllValues(NODE, inputData()->getElementSize());
}

void QuickAccessBar::setEdgeSize() {
  setAllValues(EDGE, inputData()->getElementSize());
}

void QuickAccessBar::setNodeLabelPosition() {
  setAllValues(NODE, inputData()->getElementLabelPosition());
}

void QuickAccessBar::setEdgesVisible(bool v) {
  if(renderingParameters()->isDisplayEdges() != v) {
    renderingParameters()->setDisplayEdges(v);
    _ui->showEdgesToggle->setIcon((v ? QIcon(":/tulip/gui/icons/20/edges_enabled.png") : QIcon(":/tulip/gui/icons/20/edges_disabled.png")));
    _mainView->emitDrawNeededSignal();
    emit settingsChanged();
  }
}

void QuickAccessBar::setLabelsVisible(bool v) {
  if(renderingParameters()->isViewNodeLabel()!=v) {
    renderingParameters()->setViewNodeLabel(v);
    _ui->showLabelsToggle->setIcon((v ? QIcon(":/tulip/gui/icons/20/labels_enabled.png") : QIcon(":/tulip/gui/icons/20/labels_disabled.png")));
    _mainView->emitDrawNeededSignal();
    emit settingsChanged();
  }
}

void QuickAccessBar::setLabelsScaled(bool v) {
  if(renderingParameters()->isLabelScaled()!=v) {
    renderingParameters()->setLabelScaled(v);
    _ui->labelsScaledToggle->setIcon((v ? QIcon(":/tulip/gui/icons/20/labels_scaled_enabled.png") : QIcon(":/tulip/gui/icons/20/labels_scaled_disabled.png")));
    _mainView->emitDrawNeededSignal();
    emit settingsChanged();
  }
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

  _mainView->graph()->push();

  Observable::holdObservers();

  if(inputData()->getElementFont()->getNodeDefaultValue()!=dlg.font().fontFile().toStdString())
    inputData()->getElementFont()->setAllNodeValue(dlg.font().fontFile().toStdString());

  if(inputData()->getElementFont()->getEdgeDefaultValue()!=dlg.font().fontFile().toStdString())
    inputData()->getElementFont()->setAllEdgeValue(dlg.font().fontFile().toStdString());

  Observable::unholdObservers();
  updateFontButtonStyle();
  emit settingsChanged();
}

void QuickAccessBar::updateFontButtonStyle() {
  QString fontFile = inputData()->getElementFont()->getNodeDefaultValue().c_str();
  TulipFont selectedFont = TulipFont::fromFile(fontFile);
  _ui->fontButton->setStyleSheet("font-family: " + selectedFont.fontFamily() + "; "
                                 + (selectedFont.isItalic() ? "font-style: italic; " : "")
                                 + (selectedFont.isBold() ? "font-weight: bold; " : ""));
  //_ui->fontButton->setText(selectedFont.fontName() + (selectedFont.isBold() ? " Bold" : "") + (selectedFont.isItalic() ? " Italic" : ""));
}
