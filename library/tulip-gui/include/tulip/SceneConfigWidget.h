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

#ifndef SCENECONFIGWIDGET_H
#define SCENECONFIGWIDGET_H

#include <QWidget>

#include <tulip/tulipconf.h>

namespace Ui {
class SceneConfigWidget;
}

namespace tlp {
class GlMainWidget;

class TLP_QT_SCOPE SceneConfigWidget : public QWidget {
  Q_OBJECT

  Ui::SceneConfigWidget *_ui;
  tlp::GlMainWidget *_glMainWidget;

  bool _resetting;

public:
  explicit SceneConfigWidget(QWidget *parent = nullptr);
  ~SceneConfigWidget() override;

signals:
  void settingsApplied();

public slots:
  void setGlMainWidget(tlp::GlMainWidget *);
  void resetChanges();
  void applySettings();
  void dynamicFontRBToggled(bool);

protected:
  bool eventFilter(QObject *, QEvent *) override;
};
}

#endif // SCENECONFIGWIDGET_H
///@endcond
