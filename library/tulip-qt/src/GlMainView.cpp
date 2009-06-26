#include "tulip/GlMainView.h"

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QMenu>
#include <QtGui/QImageWriter>

#include "tulip/FindSelectionWidget.h"
#include "tulip/TabWidget.h"
#include "tulip/GWOverviewWidget.h"


using namespace std;

namespace tlp {

  //==================================================
  GlMainView::GlMainView():
    AbstractView() {
  }
  //==================================================
  GlMainView::~GlMainView() {
  }
  //==================================================
  QWidget *GlMainView::construct(QWidget *parent) {
  	QWidget *widget=AbstractView::construct(parent);

  	mainWidget=new GlMainWidget(widget,this);

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
  	connect(overviewWidget,SIGNAL(hideOverview(bool)),this,SLOT(hideOverview(bool)));


  	//Export Menu
  	exportImageMenu=new QMenu("&Save Picture as ");
  	// Tulip known formats (see GlGraph)
  	// formats are sorted, "~" is just an end marker
  	char *tlpFormats[] = {"EPS", "SVG", "~"};
  	unsigned int i = 0;
  	//Image PopuMenu
  	// int Qt 4, output formats are not yet sorted and uppercased
  	list<QString> formats;
  	// first add Tulip known formats
  	while (strcmp(tlpFormats[i], "~") != 0)
  	  formats.push_back(tlpFormats[i++]);
  	// uppercase and insert all Qt formats
  	foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
  	  char* tmp = format.data();
  	  for (int i = strlen(tmp) - 1; i >= 0; --i)
  	    tmp[i] = toupper(tmp[i]);
  	  formats.push_back(QString(tmp));
  	}
  	// sort before inserting in exportImageMenu
  	formats.sort();
  	foreach(QString str, formats)
  	exportImageMenu->addAction(str);

  	connect(exportImageMenu, SIGNAL(triggered(QAction*)), SLOT(exportImage(QAction*)));

  	return widget;
  }
  //==================================================
  void GlMainView::createPicture(const std::string &pictureName,int width, int height){
    mainWidget->createPicture(pictureName,width,height);
  }

  //==================================================
  // Accessor and set
  //==================================================
  Graph *GlMainView::getGraph() {
    return mainWidget->getGraph();
  }
  //==================================================
  GlMainWidget *GlMainView::getGlMainWidget() {
    return mainWidget;
  }
  //==================================================
  void GlMainView::buildContextMenu(QObject *object,QMouseEvent *event,QMenu *contextMenu) {
    contextMenu->addMenu(exportImageMenu);
  }

  //==================================================
  void GlMainView::exportImage(QAction* action) {
    string name = action->text().toStdString();
    QString s(QFileDialog::getSaveFileName());
    if (s.isNull()) return;
    if (name=="EPS") {
      if (!mainWidget->outputEPS(64000000,true,s.toAscii().data()))
	QMessageBox::critical( 0, "Save Picture Failed",
			       "The file has not been saved.");
      return;
    } else if (name=="SVG") {
      if (!mainWidget->outputSVG(64000000,s.toAscii().data()))
	QMessageBox::critical( 0, "Save Picture Failed",
			       "The file has not been saved.");
      return;
    }

    int width,height;
    width = mainWidget->width();
    height = mainWidget->height();
    /*unsigned char* image= mainWidget->getImage();
    QPixmap pm(width,height);
    QPainter painter;
    painter.begin(&pm);
    for (int y=0; y<height; y++)
      for (int x=0; x<width; x++) {
	painter.setPen(QColor(image[(height-y-1)*width*3+(x)*3],
			      image[(height-y-1)*width*3+(x)*3+1],
			      image[(height-y-1)*width*3+(x)*3+2]));
	painter.drawPoint(x,y);
      }
    painter.end();
    free(image);
    pm.save( s, name.c_str());*/
    mainWidget->createPicture(s.toAscii().data(),width,height);
  }
  //==================================================
  // GUI functions
  //==================================================
  void  GlMainView::draw() {
    mainWidget->draw();
  }
  //==================================================
  void GlMainView::refresh() {
    draw();
  }
  //==================================================
  void GlMainView::hideOverview(bool hide) {
    if(hide) {
      overviewFrame->hide();
    }else{
      overviewFrame->show();
    }
    overviewAction->setChecked(!hide);
  }
}

