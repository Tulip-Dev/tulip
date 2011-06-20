#ifndef SIMPLEPLUGINPROGRESSWIDGET_H
#define SIMPLEPLUGINPROGRESSWIDGET_H

#include <QtGui/QDialog>
#include <QtGui/QWidget>

#include <tulip/SimplePluginProgress.h>

namespace Ui {
class SimplePluginProgressWidgetData;
}

namespace tlp {


class TLP_QT_SCOPE SimplePluginProgressWidget: public QWidget, public tlp::SimplePluginProgress {
  Q_OBJECT
  Ui::SimplePluginProgressWidgetData *_ui;

public:
  explicit SimplePluginProgressWidget(QWidget *parent=0,Qt::WindowFlags f=0);

  void setComment(std::string);
  void setComment(const QString &);
  void setComment(const char *);

protected:
  virtual void progress_handler(int step, int max_step);
  virtual void preview_handler(bool);
};

class TLP_QT_SCOPE SimplePluginProgressDialog: public QDialog {
public:
  explicit SimplePluginProgressDialog(QWidget *parent=0);
  virtual ~SimplePluginProgressDialog();
  tlp::PluginProgress *progress() const { return _progress; }

private:
  tlp::PluginProgress *_progress;
};

}

#endif // SIMPLEPLUGINPROGRESSWIDGET_H
