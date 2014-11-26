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

#include "DocumentationNavigator.h"

#include <QFile>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWebHistory>

#include <tulip/TulipRelease.h>
#include <tulip/TlpTools.h>

DocumentationNavigator::DocumentationNavigator(): QDialog(NULL, Qt::Window) {
  setWindowIcon(QIcon(":/logo32x32.ico"));
  setWindowTitle(QString("Tulip ") + TULIP_MM_VERSION + " documentation");

  QVBoxLayout* vLayout = new QVBoxLayout(this);
  vLayout->setContentsMargins(0,0,0,0);
  tabWidget = new QTabWidget(this);
  vLayout->addWidget(tabWidget);
  QToolBar* tb = new QToolBar();

  // create back button
  QIcon backIcon;
  backIcon.addFile(QString::fromUtf8(":/tulip/graphperspective/icons/16/back.png"), QSize(), QIcon::Normal, QIcon::Off);
  backButton = new QToolButton();
  backButton->setMinimumSize(QSize(16, 16));
  backButton->setMaximumSize(QSize(16, 16));
  backButton->setIcon(backIcon);
  backButton->setToolTip("Go Back a page");
  connect(backButton, SIGNAL(clicked()), this, SLOT(goBack()));
  tb->addWidget(backButton);

  // create forward button
  QIcon forwardIcon;
  forwardIcon.addFile(QString::fromUtf8(":/tulip/graphperspective/icons/16/forward.png"), QSize(), QIcon::Normal, QIcon::Off);
  forwardButton = new QToolButton();
  forwardButton->setMinimumSize(QSize(16, 16));
  forwardButton->setMaximumSize(QSize(16, 16));
  forwardButton->setIcon(forwardIcon);
  forwardButton->setToolTip("Move forward a page");
  connect(forwardButton, SIGNAL(clicked()), this, SLOT(goForward()));
  tb->addWidget(forwardButton);

  // put the toolbar with the back & forward buttons
  // in the top right corner of the tab widget
  tabWidget->setCornerWidget(tb);

  // create 3 doc views
  // for User handbook
  QWebView* docView = new QWebView();
  QString docRootPath = QString::fromUtf8(tlp::TulipShareDir.c_str()) + "doc/tulip-user/html/index.html";
#ifdef WIN32
  docView->load(QUrl("file:///"+docRootPath));
#else
  docView->load(QUrl("file://"+docRootPath));
#endif
  connect(docView, SIGNAL(loadFinished(bool)), this, SLOT(updateButtons()));
  // add user handbook doc view to tab widget
  tabWidget->addTab(docView, "User handbook");

  currentDocView = docView;

#ifdef BUILD_PYTHON_COMPONENTS
  // for Python doc
  docView = new QWebView();
  docRootPath = QString::fromUtf8(tlp::TulipShareDir.c_str()) + "doc/tulip-python/html/index.html";
#ifdef WIN32
  docView->load(QUrl("file:///"+docRootPath));
#else
  docView->load(QUrl("file://"+docRootPath));
#endif
  connect(docView, SIGNAL(loadFinished(bool)), this, SLOT(updateButtons()));
  // add python docview to tab widget
  tabWidget->addTab(docView, "Python documentation");
#endif

  // for Developer handbook
  docView = new QWebView();
  docRootPath = QString::fromUtf8(tlp::TulipShareDir.c_str()) + "doc/tulip-dev/html/index.html";
#ifdef WIN32
  docView->load(QUrl("file:///"+docRootPath));
#else
  docView->load(QUrl("file://"+docRootPath));
#endif
  connect(docView, SIGNAL(loadFinished(bool)), this, SLOT(updateButtons()));
  // add developer doc view to tab widget
  tabWidget->addTab(docView, "Developer handbook");

  connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(setCurrentDocView(int)));

  resize(1200, 700);
  updateButtons();
}

bool DocumentationNavigator::hasDocumentation() {
  return QFile(QString::fromUtf8(tlp::TulipShareDir.c_str()) +
               "doc/tulip-user/html/index.html").exists();
}

void DocumentationNavigator::showDocumentation() {
  // use a singleton
  static DocumentationNavigator* navigator = NULL;

  if (navigator == NULL)
    navigator = new DocumentationNavigator();

  navigator->show();
}

void DocumentationNavigator::goBack() {
  currentDocView->back();
}

void DocumentationNavigator::goForward() {
  currentDocView->forward();
}

void DocumentationNavigator::setCurrentDocView(int) {
  currentDocView = static_cast<QWebView*>(tabWidget->currentWidget());
  updateButtons();
}

void DocumentationNavigator::updateButtons(bool) {
  QWebHistory* history = currentDocView->history();
  backButton->setEnabled(history->canGoBack());
  forwardButton->setEnabled(history->canGoForward());
}

