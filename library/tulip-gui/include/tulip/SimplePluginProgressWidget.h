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

#ifndef SIMPLEPLUGINPROGRESSWIDGET_H
#define SIMPLEPLUGINPROGRESSWIDGET_H

#include <tulip/PluginProgress.h>

#include <QTime>
#include <QWidget>
#include <QDialog>

namespace Ui {
class SimplePluginProgressWidgetData;
}

class QCloseEvent;

namespace tlp {

class TLP_QT_SCOPE SimplePluginProgressWidget : public QWidget, public tlp::PluginProgress {
  Q_OBJECT
  Ui::SimplePluginProgressWidgetData *_ui;

  QTime _lastUpdate;
  void checkLastUpdate();

  std::string _error;
  tlp::ProgressState _state;

public:
  explicit SimplePluginProgressWidget(QWidget *parent = nullptr, Qt::WindowFlags f = nullptr);
  ~SimplePluginProgressWidget() override;

  void setComment(const std::string &) override;
  void setComment(const QString &);
  void setComment(const char *);

  void setTitle(const std::string &) override {}

  tlp::ProgressState progress(int step, int max_step) override;

  void cancel() override;
  void stop() override;

  bool isPreviewMode() const override;
  void setPreviewMode(bool drawPreview) override;
  void showPreview(bool showPreview) override;

  tlp::ProgressState state() const override;

  std::string getError() override;
  void setError(const std::string &error) override;

protected:
  void closeEvent(QCloseEvent *) override;

public slots:
  void setCancelButtonVisible(bool);
  void setStopButtonVisible(bool);

protected slots:
  void cancelClicked();
  void stopClicked();
};

class TLP_QT_SCOPE SimplePluginProgressDialog : public QDialog, public tlp::PluginProgress {
  Q_OBJECT
public:
  explicit SimplePluginProgressDialog(QWidget *parent = nullptr);
  ~SimplePluginProgressDialog() override;

  void setComment(const std::string &) override;
  void setComment(const QString &);
  void setComment(const char *);

  void setTitle(const std::string &title) override;

  tlp::ProgressState progress(int step, int max_step) override;

  void cancel() override;
  void stop() override;

  bool isPreviewMode() const override;
  void setPreviewMode(bool drawPreview) override;
  void showPreview(bool showPreview) override;

  tlp::ProgressState state() const override;

  std::string getError() override;
  void setError(const std::string &error) override;

protected:
  void closeEvent(QCloseEvent *) override;

public slots:
  void setCancelButtonVisible(bool v);
  void setStopButtonVisible(bool v);

private:
  tlp::SimplePluginProgressWidget *_progress;
};
}

#endif // SIMPLEPLUGINPROGRESSWIDGET_H
///@endcond
