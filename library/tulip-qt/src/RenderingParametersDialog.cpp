#include "tulip/RenderingParametersDialog.h"

#include <QtGui/QHeaderView>
#include <QtGui/QColorDialog>

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
    Color inColor = mainView->getGlMainWidget()->getScene()->getBackgroundColor();
    setBackgroundColor(QColor(inColor[0],inColor[1],inColor[2]));
    fonts->setCurrentIndex(param.getFontsType());
    density->setValue(param.getLabelsBorder());

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
    QColor outColor = background->palette().color(QPalette::Button);
    mainView->getGlMainWidget()->getScene()->setBackgroundColor(Color(outColor.red(),outColor.green(),outColor.blue()));
    param.setLabelsBorder(density->value());

    mainView->getGlMainWidget()->getScene()->getGlGraphComposite()->setRenderingParameters(param);
    mainView->getGlMainWidget()->draw(true);
  }

  void RenderingParametersDialog::backColor() {
    setBackgroundColor(QColorDialog::getColor(background->palette().color(QPalette::Button), this));
    updateView();
  }

  void RenderingParametersDialog::setBackgroundColor(QColor tmp) {
    if (tmp.isValid()) {
      QPalette palette;
      palette.setColor(QPalette::Button, tmp);
      int h,s,v;
      tmp.getHsv(&h, &s, &v);
      if (v < 128)
	palette.setColor(QPalette::ButtonText, QColor(255, 255, 255));
      else
	palette.setColor(QPalette::ButtonText, QColor(0, 0, 0));
      background->setPalette(palette);
    }
  }
}
