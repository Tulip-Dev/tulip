/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#ifndef TEXTUREFILEDIALOG_H
#define TEXTUREFILEDIALOG_H

#include <QDialog>

#include <tulip/tulipconf.h>
#include <tulip/TulipMetaTypes.h>
#include "ui_TextureFileDialog.h"

namespace Ui {
class TextureFileDialogData;
}

namespace tlp {

/**
  * @brief Provide a dialog that allow the user to choose
  * a file whose name may be empty
  *
  *
  **/
class TLP_QT_SCOPE TextureFileDialog : public QDialog {
  Q_OBJECT
public:
  Ui::TextureFileDialogData *ui;
  TextureFile _data;
  int ok;
  TextureFileDialog(QWidget *parent = nullptr);

  ~TextureFileDialog();

  void done(int res);

  void setData(const TextureFile &tf);

  const TextureFile &data() {
    return _data;
  }
public slots:
  void browse();
};
}
#endif // TEXTUREFILEDIALOG_H
///@endcond
