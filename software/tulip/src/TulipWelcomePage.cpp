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

#include "TulipWelcomePage.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QDesktopServices>
#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QStyle>

#include <tulip/TulipSettings.h>
#include <tulip/PluginManager.h>
#include <tulip/PluginLister.h>
#include <tulip/Perspective.h>

#include "PerspectiveItemWidget.h"
#include "ui_TulipWelcomePage.h"

using namespace tlp;

TulipWelcomePage::TulipWelcomePage(QWidget *parent)
    : QWidget(parent), _ui(new Ui::TulipWelcomePageData) {
  _ui->setupUi(this);

  // Finalize Ui
  _ui->openProjectButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DirHomeIcon));
  connect(_ui->openProjectButton, SIGNAL(clicked()), this, SIGNAL(openProject()));

  // Recent documents list
  TulipSettings::checkRecentDocuments();
  QStringList recentDocs = TulipSettings::recentDocuments();

  if (!recentDocs.empty()) {
    QString txt;

    for (const QString &txt2 : recentDocs)
      txt += "<p><span><img src=\":/tulip/gui/ui/list_bullet_arrow.png\"></img>   <a href=\"" +
             txt2 + "\">" + txt2 + "</a>" + "</span></p><p/>";

    _ui->recentDocumentsLabel->setText(txt);
  }

  std::list<std::string> perspectives = PluginLister::availablePlugins<tlp::Perspective>();

  for (std::list<std::string>::iterator it = perspectives.begin(); it != perspectives.end(); ++it) {
    _ui->perspectivesFrame->layout()->addWidget(new PerspectiveItemWidget(it->c_str()));
  }

  _ui->perspectivesFrame->layout()->addItem(
      new QSpacerItem(0, 0, QSizePolicy::Maximum, QSizePolicy::Expanding));
}

TulipWelcomePage::~TulipWelcomePage() {
  delete _ui;
}

void TulipWelcomePage::recentFileLinkActivated(const QString &link) {
  if (!QFileInfo(link).exists())
    QMessageBox::critical(this, "Error", "Selected recent project does not exist anymore");
  else
    emit openFile(link);
}
