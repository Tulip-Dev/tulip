/**
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
#ifndef SIMPLEPLUGINPROGRESSWIDGET_H
#define SIMPLEPLUGINPROGRESSWIDGET_H

#include <QtGui/QDialog>
#include <QtGui/QWidget>
#include <QtCore/QTime>

#include <tulip/SimplePluginProgress.h>

namespace Ui {
class SimplePluginProgressWidgetData;
}

namespace tlp {


class TLP_QT_SCOPE SimplePluginProgressWidget: public QWidget, public tlp::SimplePluginProgress {
  Q_OBJECT
  Ui::SimplePluginProgressWidgetData *_ui;

  QTime _lastUpdate;
  void checkLastUpdate();

public:
  explicit SimplePluginProgressWidget(QWidget *parent=NULL,Qt::WindowFlags f=0);

  void setComment(const std::string&);
  void setComment(const QString &);
  void setComment(const char *);

protected:
  virtual void progress_handler(int step, int max_step);
  virtual void preview_handler(bool);
};

class TLP_QT_SCOPE SimplePluginProgressDialog: public QDialog, public tlp::SimplePluginProgress {
public:
  explicit SimplePluginProgressDialog(QWidget *parent=NULL);
  virtual ~SimplePluginProgressDialog();

  void setComment(const std::string&);
  void setComment(const QString &);
  void setComment(const char *);

protected:
  virtual void progress_handler(int step, int max_step);
  virtual void preview_handler(bool);

private:
  tlp::SimplePluginProgressWidget *_progress;
};

}

#endif // SIMPLEPLUGINPROGRESSWIDGET_H
