#include "tulip3/GlMainView3.h"

#include <QtGui/QMessageBox>
#include <QtGui/QMenu>

#include <QtGui/QFrame>
#include <QtGui/QWidget>
#include "tulip/GlMainWidget.h"
#include "tulip/GWOverviewWidget.h"

using namespace std;
using namespace tlp;

namespace tlp3 {

//==================================================
GlMainView3::GlMainView3() :
  AbstractView() {
}
//==================================================
GlMainView3::~GlMainView3() {
}
//==================================================
QWidget *GlMainView3::construct(QWidget *parent) {
  QWidget *widget = AbstractView::construct(parent);

  mainWidget = new GlMainWidget(widget, this);

  setCentralWidget(mainWidget);

  // Create overview widget
  overviewFrame = new QFrame(mainWidget);
  overviewFrame->setGeometry(QRect(0, 0, 100, 100));
  overviewFrame->setFrameShape(QFrame::StyledPanel);
  overviewFrame->setFrameShadow(QFrame::Raised);
  overviewFrame->setAutoFillBackground(true);
  QGridLayout *gridLayout_2 = new QGridLayout(overviewFrame);
  gridLayout_2->setSpacing(6);
  gridLayout_2->setMargin(0);
  overviewWidget = new GWOverviewWidget(overviewFrame);
  gridLayout_2->addWidget(overviewWidget, 0, 0, 1, 1);
  connect(overviewWidget, SIGNAL(hideOverview(bool)), this, SLOT(hideOverview(bool)));

  dialogMenu=new QMenu("Dialog");
  connect(dialogMenu, SIGNAL(triggered(QAction*)), SLOT(showDialog(QAction*)));
  overviewAction=dialogMenu->addAction("3D &Overview");
  overviewAction->setCheckable(true);
  overviewAction->setChecked(true);

  return widget;
}
//==================================================
void GlMainView3::createPicture(const std::string &pictureName, int width, int height) {
  createPicture(pictureName,width,height,false);
}
//==================================================
bool GlMainView3::createPicture(const std::string &pictureName, int width, int height, bool center, int zoom, int xOffset, int yOffset) {

  string extension = pictureName.substr(pictureName.find_last_of('.') + 1);

  for (unsigned int i = 0; i < extension.size(); ++i)
    extension[i] = tolower(extension[i]);

  if (extension.compare("eps") == 0) {
    if (!mainWidget->outputEPS(64000000, true, pictureName.c_str())) {
      QMessageBox::critical(0, "Save Picture Failed", "The file has not been saved.");
      return false;
    }

    return true;
  }
  else if (extension.compare("svg") == 0) {
    if (!mainWidget->outputSVG(64000000, pictureName.c_str())) {
      QMessageBox::critical(0, "Save Picture Failed", "The file has not been saved.");
      return false;
    }

    return true;
  }

  if (width == 0 && height == 0)
    mainWidget->createPicture(pictureName, mainWidget->width(), mainWidget->height(),center,zoom,xOffset,yOffset);
  else
    mainWidget->createPicture(pictureName, width, height,center,zoom,xOffset,yOffset);

  return true;
}

//==================================================
QImage GlMainView3::createPicture( int width, int height, bool center, int zoom, int xOffset, int yOffset) {

  if (width == 0 && height == 0)
    return mainWidget->createPicture(mainWidget->width(), mainWidget->height(),center,zoom,xOffset,yOffset);
  else
    return mainWidget->createPicture(width, height,center,zoom,xOffset,yOffset);
}

//==================================================
// Accessor and set
//==================================================
Graph *GlMainView3::getGraph() {
  return mainWidget->getGraph();
}
//==================================================
GlMainWidget *GlMainView3::getGlMainWidget() {
  return mainWidget;
}
//==================================================
void GlMainView3::buildContextMenu(QObject* object, QContextMenuEvent* event, QMenu* contextMenu) {
  AbstractView::buildContextMenu(object, event, contextMenu);
  contextMenu->addMenu(dialogMenu);
}

//==================================================
// GUI functions
//==================================================
void GlMainView3::draw() {
  mainWidget->draw();
}
//==================================================
void GlMainView3::refresh() {
  draw();
}
//==================================================
void GlMainView3::hideOverview(bool hide) {
  if (hide) {
    overviewFrame->hide();
  }
  else {
    overviewFrame->show();
  }

  overviewAction->setChecked(!hide);
  refresh();
}
//==================================================
void GlMainView3::showDialog(QAction* action) {
  string name(action->text().toStdString());

  if (name=="3D &Overview") {
    if(overviewFrame->isVisible()) {
      overviewFrame->hide();
    }
    else {
      overviewFrame->show();
      overviewWidget->show();
    }
  }
}
}

