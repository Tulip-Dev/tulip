//-*-c++-*-
#ifndef Tulip_APPLICATION_H
#define Tulip_APPLICATION_H

#include <string>
#include <qapplication.h>
#include <tulip/Color.h>

class ElementPropertiesWidget;

class Application:public QApplication {
public:
  ElementPropertiesWidget *nodeProperties;
  std::string bitmapPath;
  tlp::Color nodeColor;
  tlp::Color edgeColor;
  Application(int& argc, char ** argv); 
  ~Application();
};

#ifdef qApp
#undef qApp
extern  Application *qApp;
#endif
#endif
