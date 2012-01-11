#include "tulip/TulipFontDialog.h"

#include <QtGui/QFontDatabase>
#include <QtGui/QPushButton>
#include <QtCore/QDebug>
#include "ui_TulipFontDialog.h"

using namespace tlp;

TulipFontDialog::TulipFontDialog(QWidget *parent): QDialog(parent), _ui(new Ui::TulipFontDialog) {
  _ui->setupUi(this);

  foreach(QString font, TulipFont::installedFontNames())
    _ui->nameList->addItem(font);

  bool hasFont = _ui->nameList->count()>0;
  _ui->settingsWidget->setEnabled(hasFont);
  _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(hasFont);
  if (hasFont) {
    _ui->nameList->setCurrentItem(_ui->nameList->item(0));
    fontChanged();
  }

}

TulipFont TulipFontDialog::font() const {
  TulipFont result;
  result.setFontName(_ui->nameList->currentItem()->text());
  result.setBold(_ui->styleList->currentIndex().row() == 1 || _ui->styleList->currentIndex().row() == 3);
  result.setItalic(_ui->styleList->currentIndex().row() == 2 || _ui->styleList->currentIndex().row() == 3);
  return result;
}

void TulipFontDialog::fontChanged() {
  if (_ui->nameList->currentItem()==NULL || _ui->styleList->currentItem()==NULL)
    return;

  if (sender() == _ui->sizeList) {
    _ui->sizeSpin->setValue(_ui->sizeList->currentItem()->text().toInt());
    return;
  }

  TulipFont selectedFont = font();
  QStringList families = QFontDatabase::applicationFontFamilies(selectedFont.fontId());
  QString fontFamily = "Arial";
  if (families.size()>0)
    fontFamily = families[0];
  _ui->preview->setStyleSheet("font-family: " + fontFamily + "; "
                              + (selectedFont.isItalic() ? "font-style: italic; " : "")
                              + (selectedFont.isBold() ? "font-weight: bold; " : "")
                              + "font-size: " + QString::number(_ui->sizeSpin->value()) + "px; "
                              );
}
int TulipFontDialog::fontSize() const {
  return _ui->sizeSpin->value();
}
