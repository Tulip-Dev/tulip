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

#ifndef PARALLEL_COORDINATES_CONFIG_DIALOG_H
#define PARALLEL_COORDINATES_CONFIG_DIALOG_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <QDialog>

#include <tulip/Size.h>
#include <tulip/Color.h>
#include <tulip/Graph.h>

namespace Ui {
class ConfigDialog;
}

class QWidget;
class QShowEvent;

namespace tlp {

class ParallelCoordinatesGraphProxy;

class ParallelCoordinatesConfigDialog : public QDialog {

  Q_OBJECT

public :

  ParallelCoordinatesConfigDialog(ParallelCoordinatesGraphProxy *data, QWidget *parent = 0);
  ~ParallelCoordinatesConfigDialog();

  std::vector<std::string> getSelectedProperties() const;

  ElementType getDataLocation() const;
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
  void setGraphProxy(ParallelCoordinatesGraphProxy *graphProxy);
  unsigned int getUnhighlightedEltsColorsAlphaValue() const;
  void setUnhighlightedEltsColorsAlphaValue(const unsigned int alphaValue);

  std::string getLinesTextureFilename() const;
  void setLinesTextureFilename(const std::string &linesTextureFileName);

  void updateSelectedProperties();

  void showEvent (QShowEvent * event);

public slots:

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


  std::vector<std::string> propertyTypesFilter;
  ElementType dataLocationBak;
  Color bgColorBak;
  unsigned int axisHeightBak;
  bool drawPointOnAxisBak;
  unsigned int minAxisPointSizeBak;
  unsigned int maxAxisPointSizeBak;
  unsigned int linesColorAlphaValueBak;
  unsigned int unhighlightedEltsColorsAlphaValueBak;
  bool linesTextureBak;
  bool userTextureBak;
  std::string userTextureFileBak;
  Ui::ConfigDialog* _ui;

};

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // PARALLEL_COORDINATES_CONFIG_DIALOG_H

