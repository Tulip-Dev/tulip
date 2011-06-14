#include "tulip/SimplePluginProgressWidget.h"
#include <QtGui/QStylePainter>
#include <QtCore/QDebug>

#include <tulip/Observable.h>
#include "ui_SimplePluginProgressWidget.h"

namespace tlp {

SimplePluginProgressWidget::SimplePluginProgressWidget(QWidget *parent, Qt::WindowFlags f)
  :QWidget(parent,f), _ui(new Ui::SimplePluginProgressWidgetData) {
  _ui->setupUi(this);
  _ui->cancelButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton));
}

void SimplePluginProgressWidget::progress_handler(int step, int max_step) {
  _ui->progressBar->setValue(step);
  _ui->progressBar->setMaximum(max_step);
}

void SimplePluginProgressWidget::preview_handler(bool p) {
  if (p)
    Observable::unholdObservers();
  else
    Observable::holdObservers();
}

void SimplePluginProgressWidget::setComment(std::string s) {
  setComment(QString(s.c_str()));
}

void SimplePluginProgressWidget::setComment(const QString &s) {
  _ui->comment->setText("<b>" + s + "</b>");
}

void SimplePluginProgressWidget::setComment(const char *s) {
  setComment(std::string(s));
}

SimplePluginProgressDialog::SimplePluginProgressDialog(QWidget *parent): QDialog(parent) {
  setModal(true);
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->setContentsMargins(0,0,0,0);
  _progress = new SimplePluginProgressWidget(this);
  mainLayout->addWidget(static_cast<SimplePluginProgressWidget *>(_progress));
  setLayout(mainLayout);
}

}
