#include "tulip/View.h"

#include <QtGui/QGridLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMenuBar>

using namespace std;

namespace tlp {

  TemplateFactory<ViewCreatorFactory,ViewCreator, ViewCreatorContext *> *ViewCreatorFactory::factory;  

  View::View(const std::string &pluginName,QWidget *parent) :pluginName(pluginName),QWidget(parent) {
    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(0);
    gridLayout->setMargin(0);

    mainLayout=new QVBoxLayout;

    menuBar=new QMenuBar;
    mainLayout->addWidget(menuBar);

    gridLayout->addLayout(mainLayout, 0, 0, 1, 1);
  }

  void View::setCentralWidget(QWidget *widget) {
    widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    mainLayout->addWidget(widget);
  }

}
