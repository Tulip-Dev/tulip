/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
///@cond DOXYGEN_HIDDEN


#ifndef TULIPFONTDIALOG_H
#define TULIPFONTDIALOG_H

#include <QDialog>

#include <tulip/tulipconf.h>
#include <tulip/TulipFont.h>

namespace Ui {
class TulipFontDialog;
}

namespace tlp {

class TLP_QT_SCOPE TulipFontDialog : public QDialog {
  Q_OBJECT

  Ui::TulipFontDialog* _ui;
  TulipFont previousFont;
  int ok;

public:
  explicit TulipFontDialog(QWidget *parent = NULL);
  ~TulipFontDialog();
  TulipFont font() const;
  int fontSize() const;
  void done(int res) {
    ok = res;
    QDialog::done(res);
  }
  TulipFont getSelectedFont() const {
    return ok == QDialog::Accepted ? font() : previousFont;
  }


  static TulipFont getFont(QWidget* parent = NULL,const TulipFont& selectedFont=TulipFont());

public slots:
  void selectFont(const TulipFont&);

protected slots:
  void fontChanged();
};

}

#endif // TULIPFONTDIALOG_H
///@endcond
