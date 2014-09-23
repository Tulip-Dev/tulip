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

#ifndef DOCUMENTATIONNAVIGATOR_H
#define DOCUMENTATIONNAVIGATOR_H

#include <QDialog>
#include <QTabWidget>
#include <QToolButton>
#include <QWebView>

class DocumentationNavigator : public QDialog {
  Q_OBJECT

  QTabWidget* tabWidget;
  QWebView* currentDocView;
  QToolButton* backButton;
  QToolButton* forwardButton;

public:
  explicit DocumentationNavigator();
  static bool hasDocumentation();
  static void showDocumentation();

public slots:
  void goBack();
  void goForward();
  void setCurrentDocView(int);
  void updateButtons(bool ok = true);
};

#endif // DOCUMENTATIONNAVIGATOR_H
