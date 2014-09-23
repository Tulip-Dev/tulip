/**
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

#ifndef ABOUTTULIPPAGE_H
#define ABOUTTULIPPAGE_H

#include <QWidget>

namespace Ui {
class AboutTulipPageData;
}

/**
  @brief The "About us/Getting help" screen displayed in the Tulip app startup window.
  This widget is mainly meant to provide links to the user and developer documentations, direct user to Tulip forums
  and provide basic offline support.
  */
class AboutTulipPage : public QWidget {
public:
  explicit AboutTulipPage(QWidget *parent = 0);

  bool eventFilter(QObject *, QEvent *);

private:
  Ui::AboutTulipPageData *_ui;
};

#endif // ABOUTTULIPPAGE_H
