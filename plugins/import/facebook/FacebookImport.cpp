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

#include <tulip/PythonInterpreter.h>
#include <tulip/ForEach.h>
#include <QDialog>
#include <QApplication>
#include <QFileDialog>
#include <QWebView>
#include <QVBoxLayout>
#ifndef QT_NO_OPENSSL
#include <QSslSocket>
#endif
#include <QMessageBox>

#include "FacebookImport.h"
#include "ui_FacebookImport.h"

using namespace tlp;
using namespace std;

static const QUrl facebookAuthUrl(QUrl::fromEncoded("https://graph.facebook.com/oauth/authorize?scope=user_photos"
                                  "%2Cads_management%2Cuser_notes%2Cuser_relationships%2Cuser_religion_politics"
                                  "%2Cuser_education_history%2Cuser_activities%2Cuser_online_presence%2Cuser_status"
                                  "%2Cuser_photo_video_tags%2Cuser_location%2Cuser_checkins%2Cuser_likes%2Cread_mailbox"
                                  "%2Cxmpp_login%2Cread_friendlists%2Cread_requests%2Cuser_events%2Cuser_groups"
                                  "%2Cuser_website%2Cuser_birthday%2Cuser_relationship_details%2Cuser_videos%2Cemail"
                                  "%2Cread_stream%2Cread_insights%2Cuser_interests%2Cuser_about_me%2Cuser_hometown"
                                  "%2Cuser_work_history%2Cfriends_hometown%2Cfriends_online_presence%2Cfriends_location"
                                  "%2Cfriends_checkins%2Cfriends_groups%2Cfriends_website%2Cfriends_relationships"
                                  "%2Cfriends_events%2Cfriends_about_me%2Cfriends_likes%2Cfriends_interests"
                                  "%2Cfriends_religion_politics%2Cfriends_videos%2Cfriends_birthday%2Cfriends_photos"
                                  "%2Cfriends_work_history%2Cfriends_status%2Cfriends_activities%2Cfriends_photo_video_tags"
                                  "%2Cfriends_notes%2Cfriends_relationship_details%2Cfriends_education_history"
                                  "%2Cmanage_friendlists%2Crsvp_event%2Ccreate_event%2Cpublish_stream%2Coffline_access"
                                  "%2Cpublish_checkins%2Csms%2Cmanage_pages&redirect_uri=https%3A%2F%2Fwww.facebook.com"
                                  "%2Fconnect%2Flogin_success.html&type=user_agent&client_id=404726846276317"));

FacebookImport::FacebookImport(const PluginContext*  context) : ImportModule(context) {}

string FacebookImport::icon() const {
  return ":/tulip/facebook/facebook-logo.jpg";
}


FacebookConnectWidget::FacebookConnectWidget(QWidget *parent) : QWidget(parent), _ui(new Ui::FacebookConnectWidget) {
  _ui->setupUi(this);
  connect(_ui->avatarsCB, SIGNAL(toggled(bool)), this, SLOT(activateDownloadPath(bool)));
  connect(_ui->browseButton, SIGNAL(clicked()), this, SLOT(pickDirectory()));
}

void FacebookConnectWidget::pickDirectory() {
  QString dir = QFileDialog::getExistingDirectory(this, tr("Select a directory in which to download avatars"),".", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  _ui->downloadPath->setText(dir);
}

void FacebookConnectWidget::activateDownloadPath(bool activate) {
  _ui->browseButton->setEnabled(activate);
  _ui->downloadPath->setEnabled(activate);
}

std::string FacebookConnectWidget::getAvatarsDlPath() const {
  if (_ui->avatarsCB->isChecked()) {
    return _ui->downloadPath->text().toStdString();
  }
  else {
    return "";
  }
}

// import plugins must implement bool importGraph()
bool FacebookImport::importGraph() {
  if (QApplication::instance()) {

#ifdef QT_NO_OPENSSL
    QMessageBox::critical(NULL, "SSL Error", "Qt misses SSL support, required for Facebook authentification.");
    return false;
#else

    if (!QSslSocket::supportsSsl()) {
      QMessageBox::critical(NULL, "SSL Error", "Qt seems to miss SSL support, required for Facebook authentification."
                            "If you are on Windows platform, you can try to download the Win32 OpenSSL binaries "
                            "from http://slproweb.com/download/Win32OpenSSL_Light-1_0_1c.exe, install them, and it should solve the issue");
      return false;
    }

    QDialog *dialog = new QDialog();
    dialog->setWindowTitle("Connect to your Facebook account");
    QVBoxLayout *layout = new QVBoxLayout;
    FacebookConnectWidget *fbcw = new FacebookConnectWidget();
    QWebView *webView = fbcw->_ui->webView;
    std::string tmpDir;
    PythonInterpreter::getInstance()->importModule("tulip_facebook");
    PythonInterpreter::getInstance()->evalSingleStatementAndGetValue("tulip_facebook.getTempDir()", tmpDir);
    fbcw->_ui->downloadPath->setText(tmpDir.c_str());
    webView->setUrl(facebookAuthUrl);
    webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks );
    connect(webView, SIGNAL(urlChanged(const QUrl &)), this, SLOT(urlChanged(const QUrl &)));
    connect(this, SIGNAL(authentificationDone()), dialog, SLOT(accept()));
    layout->addWidget(fbcw);
    dialog->setLayout(layout);
    dialog->setModal(true);
    dialog->resize(600, 600);
    dialog->exec();
    bool ret = false;

    if (!accessToken.isEmpty()) {
      PythonInterpreter::getInstance()->setProcessQtEventsDuringScriptExecution(true);
      ret = PythonInterpreter::getInstance()->callFunctionFourParams("tulip_facebook", "importFacebookGraph", graph, accessToken.toStdString(), pluginProgress, fbcw->getAvatarsDlPath());
      PythonInterpreter::getInstance()->setProcessQtEventsDuringScriptExecution(false);
    }

    delete dialog;
    return ret;
#endif
  }
  else {
    return false;
  }
}

void FacebookImport::urlChanged(const QUrl & url) {
  const QString tokenStr = "access_token";
  QString urlStr = url.toString();

  if (urlStr.startsWith("https://www.facebook.com/connect/login_success.html")) {
    int pos = urlStr.indexOf(tokenStr);
    int pos2 = urlStr.indexOf("&expires_in");

    if (pos != -1) {
      pos = pos+tokenStr.size()+1;
      accessToken = urlStr.mid(pos, pos2-pos);
    }

    emit authentificationDone();
  }
}

PLUGIN(FacebookImport)
