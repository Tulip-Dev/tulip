/*
	  ColorScaleConfigDialog.h

   Created on: 13 fÃ©vr. 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

 */

#ifndef COLORSCALECONFIGDIALOG_H_
#define COLORSCALECONFIGDIALOG_H_

#include "ColorScaleConfigDialogData.h"
#include <tulip/ColorScale.h>

#include <QtGui/QDialog>
#include <QtCore/QString>

namespace tlp {

class TLP_QT_SCOPE ColorScaleConfigDialog : public QDialog, public Ui::ColorScaleDialog {

	Q_OBJECT

public :

	ColorScaleConfigDialog(ColorScale *colorScale, QWidget *parent = 0);

	void setColorScale(ColorScale *colorScale);

protected :

	void resizeEvent(QResizeEvent * event);
	void showEvent(QShowEvent * event);

private slots :

	void accept();
	void predefinedColorScaleRbToggled(bool toggled);
	void userColorScaleImageRbToggled(bool toggled);
	void pressButtonBrowse();
	void nbColorsValueChanged(int value);
	void colorTableItemDoubleClicked(QTableWidgetItem *item);
	void displaySavedGradientPreview();
	void displayUserGradientPreview();
	void saveCurrentColorScale();
	void deleteSavedColorScale();
	void reeditSaveColorScale(QListWidgetItem *savedColorScaleItem);

private :

	void setColorScaleFromImage(const QString &imageFilePath);
	void loadUserSavedColorScales();
	void displayGradientPreview(const QList<QColor> &colorsVector, bool gradient, QLabel *displayLabel);

	ColorScale *colorScale;
	std::string gradientsImageDirectory;


};

}

#endif /* COLORSCALECONFIGDIALOG_H_ */
