#include "tulip/QtOpenGlErrorViewer.h"

#include <QtGui/QMessageBox>
#include <QtGui/QDialog>
#include <QtCore/QSettings>

namespace tlp {

QtOpenGlErrorViewerWithAskAgainDialog::QtOpenGlErrorViewerWithAskAgainDialog(QWidget *parent):QDialog(parent){
  setupUi(this);
  connect(okButton,SIGNAL(clicked()),this,SLOT(accept()));
}

void QtOpenGlErrorViewer::displayError(const std::string &title,const std::string &errorMsg){
  QMessageBox::warning(parent, title.c_str(), errorMsg.c_str());
}

void QtOpenGlErrorViewer::displayErrorWithAskAgain(const std::string &title,const std::string &errorMsg){
  QSettings settings("TulipSoftware","Tulip");
  settings.beginGroup("QtOpenGlErrorViewer");
  bool askAgain=settings.value(title.c_str(),true).toBool();

  if(askAgain){
    QtOpenGlErrorViewerWithAskAgainDialog dialog;
    dialog.setWindowTitle(title.c_str());
    dialog.textLabel->setText(errorMsg.c_str());
    dialog.exec();
    settings.setValue(title.c_str(),dialog.askAgainCheckBox->isChecked());
  }

  settings.endGroup();

}

}
