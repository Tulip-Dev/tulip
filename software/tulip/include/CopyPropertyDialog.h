#ifndef COPYPROPERTYDIALOG_H
#define COPYPROPERTYDIALOG_H
#include <string>
#include <vector>

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif

#include "CopyPropertyDialogData.h"
#include "tulip/Iterator.h"

class CopyPropertyDialog : public CopyPropertyDialogData
{ 
  Q_OBJECT;

public:
  CopyPropertyDialog(QWidget *parent = 0, const char *name = 0, bool modal = TRUE, Qt::WFlags fl = 0);
  void setProperties(std::string& srcProp,
		     std::vector<std::string>& localProps,
		     std::vector<std::string>& inheritedProps);
  enum destType {NEW, LOCAL, INHERITED};
  std::string getDestinationProperty(destType&);
  
public slots:  
  void setNew( bool );
  void setLocal( bool );
  void setInherited( bool );
};

#endif // COPYPROPERTYDIALOG_H
