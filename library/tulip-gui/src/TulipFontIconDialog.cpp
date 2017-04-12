#include <tulip/TulipFontIconDialog.h>
#include <tulip/TulipFontAwesome.h>
#include <tulip/TlpQtTools.h>

#include <QRegExp>

#include <QtAwesome.h>

#include "ui_TulipFontIconDialog.h"

using namespace tlp;

static QtAwesome qtAwesome;

QIcon TulipFontIconDialog::getFontAwesomeIcon(const QString &iconName) {
  if (qtAwesome.fontName().isEmpty())
    qtAwesome.initFontAwesome(tlpStringToQString(TulipFontAwesome::getFontAwesomeTrueTypeFileLocation()));

  return qtAwesome.icon(static_cast<fa::iconCodePoint>(TulipFontAwesome::getFontAwesomeIconCodePoint(QStringToTlpString(iconName))));
}

TulipFontIconDialog::TulipFontIconDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::TulipFontIconDialog) {

  _ui->setupUi(this);

  connect(_ui->iconNameFilterLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(updateIconList()));

  updateIconList();

}

void TulipFontIconDialog::updateIconList() {
  _ui->iconListWidget->clear();

  QRegExp regexp(_ui->iconNameFilterLineEdit->text());

  std::vector<std::string> iconNames = TulipFontAwesome::getSupportedFontAwesomeIcons();

  for(std::vector<std::string>::const_iterator it = iconNames.begin(); it != iconNames.end(); ++it) {
    QString iconName = tlpStringToQString(*it);
    if (regexp.indexIn(iconName) != -1) {
      QIcon faIcon = getFontAwesomeIcon(iconName);
      _ui->iconListWidget->addItem(new QListWidgetItem(faIcon.pixmap(16, 16), iconName));
    }
  }

  if (_ui->iconListWidget->count() > 0) {
    _ui->iconListWidget->setCurrentRow(0);
  }

}

QString TulipFontIconDialog::getSelectedIconName() const {
  return _selectedIconName;
}

void TulipFontIconDialog::setSelectedIconName(const QString &iconName) {
  QList<QListWidgetItem *> items = _ui->iconListWidget->findItems(iconName, Qt::MatchExactly);
  if (!items.isEmpty()) {
    _ui->iconListWidget->setCurrentItem(items.at(0));
    _selectedIconName = iconName;
  }
}

void TulipFontIconDialog::accept() {
  if (_ui->iconListWidget->count() > 0) {
    _selectedIconName = _ui->iconListWidget->currentItem()->text();
  }
  QDialog::accept();
}

void TulipFontIconDialog::showEvent(QShowEvent *ev) {
  QDialog::showEvent(ev);

  _selectedIconName = _ui->iconListWidget->currentItem()->text();

  if (parentWidget())
    move(parentWidget()->window()->frameGeometry().topLeft() +
         parentWidget()->window()->rect().center() -
         rect().center());
}
