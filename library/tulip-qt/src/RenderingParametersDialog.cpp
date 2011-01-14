/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include "tulip/RenderingParametersDialog.h"

#include <QtGui/QHeaderView>
#include <QtCore/QSettings>

#include <tulip/PreferenceManager.h>

#include "tulip/TlpQtTools.h"
#include "tulip/GWOverviewWidget.h"
#include "tulip/GlMainWidget.h"
#include "tulip/GlMainView.h"

using namespace std;

namespace tlp {

  RenderingParametersDialog::RenderingParametersDialog(QWidget* parent) : QWidget(parent) {
    setupUi(this);

    holdUpdateView=false;

  }

  void RenderingParametersDialog::setGlMainView(GlMainView *view){
    mainView=view;

    GlGraphRenderingParameters param = mainView->getGlMainWidget()->getScene()->getGlGraphComposite()->getRenderingParameters();

    holdUpdateView=true;
    arrows->setChecked( param.isViewArrow());
    colorInterpolation->setChecked( param.isEdgeColorInterpolate());
    sizeInterpolation->setChecked( param.isEdgeSizeInterpolate());
    ordering->setChecked( param.isElementOrdered());
    orthogonal->setChecked(mainView->getGlMainWidget()->getScene()->isViewOrtho());
    edge3D->setChecked( param.isEdge3D());
    Color backgroundC = mainView->getGlMainWidget()->getScene()->getBackgroundColor();
    setButtonColor(QColor(backgroundC[0],backgroundC[1],backgroundC[2]),background);
    Color selectionC = param.getSelectionColor();
    setButtonColor(QColor(selectionC[0],selectionC[1],selectionC[2]),selection);
    scaled->setChecked(param.isLabelScaled());
    overlap->setChecked(!param.isLabelOverlaped());
    frame->setEnabled(!param.isLabelOverlaped());
    density->setValue(param.getLabelsBorder());
    blockEdgeSizeCheckBox->setChecked(param.getEdgesMaxSizeToNodesSize());
    minSizeSpinBox->setValue(param.getMinSizeOfLabel());
    maxSizeSpinBox->setValue(param.getMaxSizeOfLabel());
    minSizeSpinBox->setMaximum(maxSizeSpinBox->value());
    maxSizeSpinBox->setMinimum(minSizeSpinBox->value());

    holdUpdateView=false;
  }

  void RenderingParametersDialog::updateView() {
    if(holdUpdateView)
      return;

    GlGraphRenderingParameters param = mainView->getGlMainWidget()->getScene()->getGlGraphComposite()->getRenderingParameters();

    param.setViewArrow(arrows->isChecked());
    param.setEdgeColorInterpolate(colorInterpolation->isChecked());
    param.setEdgeSizeInterpolate(sizeInterpolation->isChecked());
    param.setElementOrdered(ordering->isChecked());
    mainView->getGlMainWidget()->getScene()->setViewOrtho(orthogonal->isChecked());
    param.setEdge3D(edge3D->isChecked());
    param.setLabelScaled(scaled->isChecked());
    param.setLabelOverlaped(!overlap->isChecked());
    QColor backgroundC = background->palette().color(QPalette::Button);
    mainView->getGlMainWidget()->getScene()->setBackgroundColor(Color(backgroundC.red(),backgroundC.green(),backgroundC.blue()));
    QColor selectionC = selection->palette().color(QPalette::Button);
    param.setSelectionColor(Color(selectionC.red(),selectionC.green(),selectionC.blue()));
    param.setLabelsBorder(density->value());
    param.setEdgesMaxSizeToNodesSize(blockEdgeSizeCheckBox->isChecked());
    param.setMinSizeOfLabel(minSizeSpinBox->value());
    param.setMaxSizeOfLabel(maxSizeSpinBox->value());

    mainView->getGlMainWidget()->getScene()->getGlGraphComposite()->setRenderingParameters(param);
    emit viewNeedDraw();
    //mainView->getGlMainWidget()->draw(true);
  }

  void RenderingParametersDialog::backColor() {
    QColor col=background->palette().color(QPalette::Button);
    if(getColorDialog(col,0,"Color chooser",col))
      setButtonColor(col,background);
    updateView();
  }

  void RenderingParametersDialog::selectionColor() {
    QColor col=background->palette().color(QPalette::Button);
    if(getColorDialog(col,0,"Color chooser",col))
      setButtonColor(col,selection);
    updateView();
  }

  void RenderingParametersDialog::selectionSaveAtDefaultColor() {
    QColor selectionC = selection->palette().color(QPalette::Button);

    QSettings settings("TulipSoftware","Tulip");
    settings.beginGroup("Preference");
    settings.setValue("selectionColorR",selectionC.red());
    settings.setValue("selectionColorG",selectionC.green());
    settings.setValue("selectionColorB",selectionC.blue());
    settings.setValue("selectionColorA",selectionC.alpha());
    settings.endGroup();

    PreferenceManager::getInst().setSelectionColor(Color(selectionC.red(),selectionC.green(),selectionC.blue()));
  }

  void RenderingParametersDialog::setButtonColor(QColor tmp,QPushButton *button){
    if (tmp.isValid()) {
      QString colorStr;
      QString str;

      str.setNum(tmp.red(),16);
      if(str.size()!=2)
        str.insert(0,"0");
      colorStr.append(str);

      str.setNum(tmp.green(),16);
      if(str.size()!=2)
        str.insert(0,"0");
      colorStr.append(str);

      str.setNum(tmp.blue(),16);
      if(str.size()!=2)
        str.insert(0,"0");
      colorStr.append(str);

      QString textColor;
      int h,s,v;
      tmp.getHsv(&h, &s, &v);
      if (v < 128)
        textColor="ffffff";
      else
        textColor="000000";

      button->setStyleSheet("QPushButton { background-color: #"+colorStr+"; color: #"+textColor+" }");
    }
  }

  void RenderingParametersDialog::labelSizeChanged(int){
    minSizeSpinBox->setMaximum(maxSizeSpinBox->value());
    maxSizeSpinBox->setMinimum(minSizeSpinBox->value());

    updateView();
  }
}
