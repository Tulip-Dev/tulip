/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#ifndef SIMPLEPLUGINPROGRESSWIDGET_H
#define SIMPLEPLUGINPROGRESSWIDGET_H

#include <QtGui/QDialog>
#include <QtGui/QWidget>
#include <QtCore/QTime>

#include <tulip/PluginProgress.h>

namespace Ui {
class SimplePluginProgressWidgetData;
}

namespace tlp {


class TLP_QT_SCOPE SimplePluginProgressWidget: public QWidget, public tlp::PluginProgress {
  Q_OBJECT
  Ui::SimplePluginProgressWidgetData *_ui;

  QTime _lastUpdate;
  void checkLastUpdate();

  std::string _error;
  tlp::ProgressState _state;

public:
  explicit SimplePluginProgressWidget(QWidget *parent=NULL,Qt::WindowFlags f=0);
  virtual ~SimplePluginProgressWidget();

  void setComment(const std::string&);
  void setComment(const QString &);
  void setComment(const char *);

  tlp::ProgressState progress(int step, int max_step);

  void cancel();
  void stop();

  bool isPreviewMode() const;
  void setPreviewMode(bool drawPreview);
  void showPreview(bool showPreview);

  tlp::ProgressState state() const;

  std::string getError();
  void setError(const std::string &error);

protected slots:
  void cancelClicked();
  void stopClicked();
  void setPreview(bool);
};

class TLP_QT_SCOPE SimplePluginProgressDialog: public QDialog, public tlp::PluginProgress {
public:
  explicit SimplePluginProgressDialog(QWidget *parent=NULL);
  virtual ~SimplePluginProgressDialog();

  void setComment(const std::string&);
  void setComment(const QString &);
  void setComment(const char *);

  tlp::ProgressState progress(int step, int max_step);

  void cancel();
  void stop();

  bool isPreviewMode() const;
  void setPreviewMode(bool drawPreview);
  void showPreview(bool showPreview);

  tlp::ProgressState state() const;

  std::string getError();
  void setError(const std::string &error);

private:
  tlp::SimplePluginProgressWidget *_progress;
};

}

#endif // SIMPLEPLUGINPROGRESSWIDGET_H
///@endcond
