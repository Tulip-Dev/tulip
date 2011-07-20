#include "HeaderFrame.h"

#include "ui_HeaderFrame.h"

void switchToLabel(Ui::HeaderFrameData *ui, bool f=true) {
  ui->titleLabel->setVisible(f);
  ui->menusCombo->setVisible(!f);
}

HeaderFrame::HeaderFrame(QWidget *parent):
  QWidget(parent), _ui(new Ui::HeaderFrameData) {
  _ui->setupUi(this);
  switchToLabel(_ui);
  connect(_ui->menusCombo,SIGNAL(currentIndexChanged(QString)),this,SIGNAL(menuChanged(QString)));
}

QString HeaderFrame::title() const {
  return _ui->titleLabel->text();
}

void HeaderFrame::setTitle(const QString &title) {
  _ui->titleLabel->setText(title);

  if (_ui->menusCombo->count() == 0)
    switchToLabel(_ui);
}

QStringList HeaderFrame::menus() const {
  QStringList result;

  for(int i=0; i<_ui->menusCombo->count(); ++i)
    result << _ui->menusCombo->itemText(i);

  return result;
}

void HeaderFrame::setMenus(const QStringList &menus) {
  _ui->menusCombo->clear();
  switchToLabel(_ui,menus.size()==0);
  QString s;
  foreach(s,menus)
  _ui->menusCombo->addItem(s);
}
