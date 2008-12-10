//-*-c++-*-
/*
 Author: Antoine Lambert

 Email : antoine.lambert@labri.fr

 Last modification : 12/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef PARALLEL_COORDINATES_CONFIG_DIALOG_H
#define PARALLEL_COORDINATES_CONFIG_DIALOG_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <list>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QGroupBox>
#include <QtGui/QLineEdit>
#include <QtGui/qevent.h>

#include <tulip/Reflect.h>
#include <tulip/Size.h>


#include "ParallelCoordinatesGraphProxy.h"
#include "ParallelCoordinatesConfigDialogData.h"

namespace tlp {

class TLP_QT_SCOPE ParallelCoordinatesConfigDialog : public QDialog, public Ui::Dialog {

  Q_OBJECT

public :

  ParallelCoordinatesConfigDialog(ParallelCoordinatesGraphProxy *data, QWidget *parent = 0);

  std::vector<std::string> getSelectedProperties() const;

  const ElementType getDataLocation() const;
  void setDataLocation(const ElementType location);
  unsigned int getAxisHeight() const;
  unsigned int getSpaceBetweenAxis() const;
  void setSpaceBetweenAxis(const unsigned int spaceBetweenAxis);
  void setAxisHeight(const unsigned int axisHeight);
  bool drawPointOnAxis() const;
  void setDrawPointOnAxis(const bool drawPointOnAxis);
  Size getAxisPointMinSize() const;
  void setAxisPointMinSize(const unsigned int axisPointMinSize);
  Size getAxisPointMaxSize() const;
  void setAxisPointMaxSize(const unsigned int axisPointMaxSize);
  void setLinesColorAlphaValue(unsigned int value);
  unsigned int getLinesColorAlphaValue() const;
  Color getBackgroundColor() const;
  void setBackgroundColor(const Color &color);
  void setGraphProxy(ParallelCoordinatesGraphProxy *graphProxy) {this->graphProxy = graphProxy;}

  std::string getLinesTextureFilename() const;
  void setLinesTextureFilename(const std::string &linesTextureFileName);

  void updateSelectedProperties();

  void showEvent (QShowEvent * event);

public slots:

  void pressButtonAdd();
  void pressButtonRem();
  void pressButtonUp();
  void pressButtonDown();
  void pressButtonBrowse();
  void accept();
  void reject();
  void userTextureRbToggled(bool checked);
  void minAxisPointSizeValueChanged(int newValue);
  void maxAxisPointSizeValueChanged(int newValue);
  void pressColorButton();

private :

  std::vector<std::string> selectedProperties;
  ParallelCoordinatesGraphProxy *graphProxy;

  void backupConfiguration();
  void restoreBackupConfiguration();


  ElementType dataLocationBak;
  Color bgColorBak;
  unsigned int axisHeightBak;
  bool drawPointOnAxisBak;
  unsigned int minAxisPointSizeBak;
  unsigned int maxAxisPointSizeBak;
  unsigned int linesColorAlphaValueBak;
  bool linesTextureBak;
  bool userTextureBak;
  std::string userTextureFileBak;

};

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // PARALLEL_COORDINATES_CONFIG_DIALOG_H

