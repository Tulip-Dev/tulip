#include "tulip/RenderingParametersDialog.h"

#include <QtGui/QHeaderView>
#include <QtGui/QColorDialog>
#include <QtCore/QSettings>

#include <tulip/PreferenceManager.h>

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
    fonts->setCurrentIndex(param.getFontsType());
    density->setValue(param.getLabelsBorder());
    blockEdgeSizeCheckBox->setChecked(param.getEdgesMaxSizeToNodesSize());

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
    param.setFontsType(fonts->currentIndex());
    QColor backgroundC = background->palette().color(QPalette::Button);
    mainView->getGlMainWidget()->getScene()->setBackgroundColor(Color(backgroundC.red(),backgroundC.green(),backgroundC.blue()));
    QColor selectionC = selection->palette().color(QPalette::Button);
    param.setSelectionColor(Color(selectionC.red(),selectionC.green(),selectionC.blue()));
    param.setLabelsBorder(density->value());
    param.setEdgesMaxSizeToNodesSize(blockEdgeSizeCheckBox->isChecked());

    mainView->getGlMainWidget()->getScene()->getGlGraphComposite()->setRenderingParameters(param);
    emit viewNeedDraw();
    //mainView->getGlMainWidget()->draw(true);
  }

  void RenderingParametersDialog::backColor() {
    setButtonColor(QColorDialog::getColor(background->palette().color(QPalette::Button), this),background);
    updateView();
  }

  void RenderingParametersDialog::selectionColor() {
    setButtonColor(QColorDialog::getColor(selection->palette().color(QPalette::Button), this),selection);
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
}
