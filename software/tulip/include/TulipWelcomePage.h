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

#ifndef TULIPWELCOMEPAGE_H
#define TULIPWELCOMEPAGE_H

#include <QWidget>

class QVBoxLayout;

namespace Ui {
class TulipWelcomePageData;
}

class QNetworkReply;

/**
  @brief The widget containing the Welcome screen in tulip_app.
  This widget will allow a user to select an installed perspective, read latest news from the Tulip website, and display startup tips.
  */
class TulipWelcomePage: public QWidget {
  Q_OBJECT
public:
  explicit TulipWelcomePage(QWidget *parent=NULL);
  virtual ~TulipWelcomePage();

signals:
  void openPerspective(QString);
  void openProject();
  void openFile(QString);

protected slots:
  void rssReply(QNetworkReply *);
  void openLink(const QString &link);
  void recentFileLinkActivated(const QString&);

private:
  Ui::TulipWelcomePageData *_ui;
};

#endif // TULIPWELCOMEPAGE_H
