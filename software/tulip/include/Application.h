//-*-c++-*-
#ifndef Tulip_APPLICATION_H
#define Tulip_APPLICATION_H

#include <string>
#include <qapplication.h>
#include <tulip/Color.h>

class ElementPropertiesWidget;

class Application:public QApplication {
public:
  std::string bitmapPath;
  Application(int& argc, char ** argv); 
  ~Application();
};
#endif
