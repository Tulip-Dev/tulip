/**
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

#ifndef ABOUTTULIPPAGE_H
#define ABOUTTULIPPAGE_H

#include <tulip/tulipconf.h>

#include <QWidget>

namespace Ui {
class AboutTulipPageData;
}

namespace tlp {

/**
  @brief The "About us/Getting help" screen displayed in the Tulip app startup window.
  This widget is mainly meant to provide links to the user and developer documentations, direct user
  to Tulip forums
  and provide basic offline support.
  */
class TLP_QT_SCOPE AboutTulipPage : public QWidget {
  Q_OBJECT
public:
  explicit AboutTulipPage(QWidget *parent = nullptr);
  ~AboutTulipPage() override;

private slots:

  void openUrlInBrowser(const QString &url);

private:
  Ui::AboutTulipPageData *_ui;
};
}

#endif // ABOUTTULIPPAGE_H
