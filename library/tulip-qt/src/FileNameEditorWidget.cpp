#include "tulip/FileNameEditorWidget.h"
#include "FileNameEditorWidgetData.h"
#include <QtGui/QFileDialog>

using namespace tlp;
FileNameEditorWidget::FileNameEditorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileNameEditorWidget),basePath("./")
{
    ui->setupUi(this);
    connect(ui->lineEdit, SIGNAL(textChanged(const QString &)), this,
          SIGNAL(fileNameChanged(const QString &)));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(buttonPressed()));
}

FileNameEditorWidget::~FileNameEditorWidget()
{
    delete ui;
}

QString FileNameEditorWidget::fileName() const {
  return ui->lineEdit->text();
}
void FileNameEditorWidget::setFileName(const QString &s) {
  ui->lineEdit->setText(s);
}
QString FileNameEditorWidget::filter() const {
  return fileFilter;
}
void FileNameEditorWidget::setFilter(const QString &f) {
  fileFilter = f;
}
QString FileNameEditorWidget::getBasePath() const {
  return basePath;
}
void FileNameEditorWidget::setBasePath(const QString &f) {
  basePath = f;
}
void FileNameEditorWidget::buttonPressed() {
  QFileDialog *dlg = new QFileDialog(this, "Choose a file", basePath, fileFilter);
  dlg->setModal(true);
  dlg->setFileMode(QFileDialog::ExistingFile);
  if (dlg->exec() == QDialog::Accepted) {
    QString file = dlg->selectedFiles().first();
    if (!file.isNull()) {
      QStringList currentDir = QDir::currentPath().split(QDir::separator());
      QStringList filePath = QFileInfo(file).dir().absolutePath().split(QDir::separator());
      QString relativePath = "";
      while ((!currentDir.empty() && !filePath.empty()) && (currentDir.front()
          == filePath.front())) {
        currentDir.pop_front();
        filePath.pop_front();
      }
      while (!currentDir.empty()) {
        relativePath += "..";
        relativePath += QDir::separator();
        currentDir.pop_front();
      }
      if (!filePath.empty())
        relativePath += filePath.join((const QString) (QChar) QDir::separator())
        + QDir::separator();
      setFileName(relativePath + QFileInfo(file).fileName());
    }
  }
  delete dlg;
}
