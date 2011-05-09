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

#include "ui_RenderingParametersDialog.h"

using namespace std;

namespace tlp {

  RenderingParametersDialog::RenderingParametersDialog(QWidget* parent) : QWidget(parent), _ui(new Ui::RenderingParametersDialogData), glWidget(NULL) {
    _ui->setupUi(this);

    holdUpdateView=false;
    updateDensityLabels();
  }

  void RenderingParametersDialog::setGlMainWidget(GlMainWidget *glWidget) {

	this->glWidget = glWidget;

    GlGraphRenderingParameters param = glWidget->getScene()->getGlGraphComposite()->getRenderingParameters();

    holdUpdateView=true;
    _ui->arrows->setChecked( param.isViewArrow());
    _ui->colorInterpolation->setChecked( param.isEdgeColorInterpolate());
    _ui->sizeInterpolation->setChecked( param.isEdgeSizeInterpolate());
    _ui->ordering->setChecked( param.isElementOrdered());
    _ui->orthogonal->setChecked(glWidget->getScene()->isViewOrtho());
    _ui->edge3D->setChecked( param.isEdge3D());
    Color backgroundC = glWidget->getScene()->getBackgroundColor();
    setButtonColor(QColor(backgroundC[0],backgroundC[1],backgroundC[2]),_ui->background);
    Color selectionC = param.getSelectionColor();
    setButtonColor(QColor(selectionC[0],selectionC[1],selectionC[2]),_ui->selection);
    _ui->scaled->setChecked(param.isLabelScaled());

    if(param.getLabelsDensity()>0)
      _ui->density->setValue(param.getLabelsDensity()+5);
    else if(param.getLabelsDensity()<0)
      _ui->density->setValue(param.getLabelsDensity()-5);
    else
      _ui->density->setValue(0);

    updateDensityLabels();
    _ui->blockEdgeSizeCheckBox->setChecked(param.getEdgesMaxSizeToNodesSize());
    _ui->minSizeSpinBox->setValue(param.getMinSizeOfLabel());
    _ui->maxSizeSpinBox->setValue(param.getMaxSizeOfLabel());
    _ui->minSizeSpinBox->setMaximum(_ui->maxSizeSpinBox->value());
    _ui->maxSizeSpinBox->setMinimum(_ui->minSizeSpinBox->value());

    holdUpdateView=false;
  }

  void RenderingParametersDialog::updateView() {
    if(holdUpdateView)
      return;

    GlGraphRenderingParameters param = glWidget->getScene()->getGlGraphComposite()->getRenderingParameters();

    param.setViewArrow(_ui->arrows->isChecked());
    param.setEdgeColorInterpolate(_ui->colorInterpolation->isChecked());
    param.setEdgeSizeInterpolate(_ui->sizeInterpolation->isChecked());
    param.setElementOrdered(_ui->ordering->isChecked());
    glWidget->getScene()->setViewOrtho(_ui->orthogonal->isChecked());
    param.setEdge3D(_ui->edge3D->isChecked());
    param.setLabelScaled(_ui->scaled->isChecked());
    QColor backgroundC = _ui->background->palette().color(QPalette::Button);
    glWidget->getScene()->setBackgroundColor(Color(backgroundC.red(),backgroundC.green(),backgroundC.blue()));
    QColor selectionC = _ui->selection->palette().color(QPalette::Button);
    param.setSelectionColor(Color(selectionC.red(),selectionC.green(),selectionC.blue()));

    if(_ui->density->value()>5)
      param.setLabelsDensity(_ui->density->value()-5);
    else if(_ui->density->value()<-5)
      param.setLabelsDensity(_ui->density->value()+5);
    else
      param.setLabelsDensity(0);

    updateDensityLabels();
    param.setEdgesMaxSizeToNodesSize(_ui->blockEdgeSizeCheckBox->isChecked());
    param.setMinSizeOfLabel(_ui->minSizeSpinBox->value());
    param.setMaxSizeOfLabel(_ui->maxSizeSpinBox->value());

    glWidget->getScene()->getGlGraphComposite()->setRenderingParameters(param);
    emit viewNeedDraw();
    //glWidget->draw(true);
  }

  void RenderingParametersDialog::backColor() {
    QColor col=_ui->background->palette().color(QPalette::Button);
    if(getColorDialog(col,0,"Color chooser",col))
      setButtonColor(col,_ui->background);
    updateView();
  }

  void RenderingParametersDialog::selectionColor() {
    QColor col=_ui->background->palette().color(QPalette::Button);
    if(getColorDialog(col,0,"Color chooser",col))
      setButtonColor(col,_ui->selection);
    updateView();
  }

  void RenderingParametersDialog::selectionSaveAtDefaultColor() {
    QColor selectionC = _ui->selection->palette().color(QPalette::Button);

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
    QFont newFont=_ui->allLabels->font();
    newFont.setBold(false);

    _ui->allLabels->setFont(newFont);
    _ui->noOverlap->setFont(newFont);
    _ui->noLabels->setFont(newFont);

    newFont.setBold(true);

    if(_ui->density->value()<5 && _ui->density->value()>-5)
      _ui->noOverlap->setFont(newFont);
    else if(_ui->density->value()==-105)
      _ui->noLabels->setFont(newFont);
    else if(_ui->density->value()==105)
      _ui->allLabels->setFont(newFont);
  }

  void RenderingParametersDialog::labelSizeChanged(int){
    _ui->minSizeSpinBox->setMaximum(_ui->maxSizeSpinBox->value());
    _ui->maxSizeSpinBox->setMinimum(_ui->minSizeSpinBox->value());

    updateView();
  }

  void RenderingParametersDialog::toggleEdge3DMenu(bool f) {
    _ui->edge3D->setEnabled(f);
  }

  void RenderingParametersDialog::toggleScaledLabelsMenu(bool f) {
    _ui->scaled->setEnabled(f);
    if (!f)
      _ui->scaled->setChecked(true);
  }

  void RenderingParametersDialog::toggleLabelSizesLimit(bool f) {
    _ui->frame_2->setEnabled(f);
  }

  void RenderingParametersDialog::toggleOrthogonalMenu(bool f) {
    _ui->orthogonal->setEnabled(f);
  }

  void RenderingParametersDialog::toggleBackgroundMenu(bool f) {
    _ui->background->setEnabled(f);
  }
}
