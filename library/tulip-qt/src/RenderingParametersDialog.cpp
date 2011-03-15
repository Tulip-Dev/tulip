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

  RenderingParametersDialog::RenderingParametersDialog(QWidget* parent) : QWidget(parent), glWidget(NULL) {
    setupUi(this);

    holdUpdateView=false;
    updateDensityLabels();
  }

  void RenderingParametersDialog::setGlMainWidget(GlMainWidget *glWidget) {

	this->glWidget = glWidget;

    GlGraphRenderingParameters param = glWidget->getScene()->getGlGraphComposite()->getRenderingParameters();

    holdUpdateView=true;
    arrows->setChecked( param.isViewArrow());
    colorInterpolation->setChecked( param.isEdgeColorInterpolate());
    sizeInterpolation->setChecked( param.isEdgeSizeInterpolate());
    ordering->setChecked( param.isElementOrdered());
    orthogonal->setChecked(glWidget->getScene()->isViewOrtho());
    edge3D->setChecked( param.isEdge3D());
    Color backgroundC = glWidget->getScene()->getBackgroundColor();
    setButtonColor(QColor(backgroundC[0],backgroundC[1],backgroundC[2]),background);
    Color selectionC = param.getSelectionColor();
    setButtonColor(QColor(selectionC[0],selectionC[1],selectionC[2]),selection);
    scaled->setChecked(param.isLabelScaled());

    if(param.getLabelsDensity()>0)
      density->setValue(param.getLabelsDensity()+5);
    else if(param.getLabelsDensity()<0)
      density->setValue(param.getLabelsDensity()-5);
    else
      density->setValue(0);

    updateDensityLabels();
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

    GlGraphRenderingParameters param = glWidget->getScene()->getGlGraphComposite()->getRenderingParameters();

    param.setViewArrow(arrows->isChecked());
    param.setEdgeColorInterpolate(colorInterpolation->isChecked());
    param.setEdgeSizeInterpolate(sizeInterpolation->isChecked());
    param.setElementOrdered(ordering->isChecked());
    glWidget->getScene()->setViewOrtho(orthogonal->isChecked());
    param.setEdge3D(edge3D->isChecked());
    param.setLabelScaled(scaled->isChecked());
    QColor backgroundC = background->palette().color(QPalette::Button);
    glWidget->getScene()->setBackgroundColor(Color(backgroundC.red(),backgroundC.green(),backgroundC.blue()));
    QColor selectionC = selection->palette().color(QPalette::Button);
    param.setSelectionColor(Color(selectionC.red(),selectionC.green(),selectionC.blue()));

    if(density->value()>5)
      param.setLabelsDensity(density->value()-5);
    else if(density->value()<-5)
      param.setLabelsDensity(density->value()+5);
    else
      param.setLabelsDensity(0);

    updateDensityLabels();
    param.setEdgesMaxSizeToNodesSize(blockEdgeSizeCheckBox->isChecked());
    param.setMinSizeOfLabel(minSizeSpinBox->value());
    param.setMaxSizeOfLabel(maxSizeSpinBox->value());

    glWidget->getScene()->getGlGraphComposite()->setRenderingParameters(param);
    emit viewNeedDraw();
    //glWidget->draw(true);
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

  void RenderingParametersDialog::updateDensityLabels(){
    QFont newFont=allLabels->font();
    newFont.setBold(false);

    allLabels->setFont(newFont);
    noOverlap->setFont(newFont);
    noLabels->setFont(newFont);

    newFont.setBold(true);

    if(density->value()<5 && density->value()>-5)
      noOverlap->setFont(newFont);
    else if(density->value()==-105)
      noLabels->setFont(newFont);
    else if(density->value()==105)
      allLabels->setFont(newFont);
  }

  void RenderingParametersDialog::labelSizeChanged(int){
    minSizeSpinBox->setMaximum(maxSizeSpinBox->value());
    maxSizeSpinBox->setMinimum(minSizeSpinBox->value());

    updateView();
  }
}
