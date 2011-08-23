#include "HeaderFrame.h"

#include <QtGui/QSplitter>

#include "ui_HeaderFrame.h"

void switchToLabel(Ui::HeaderFrameData *ui, bool f=true) {
  ui->titleLabel->setVisible(f);
  ui->menusCombo->setVisible(!f);
}

HeaderFrame::HeaderFrame(QWidget *parent):
  QWidget(parent), _ui(new Ui::HeaderFrameData), _expanded(true) {
  _ui->setupUi(this);
  switchToLabel(_ui);
  connect(_ui->menusCombo,SIGNAL(currentIndexChanged(QString)),this,SIGNAL(menuChanged(QString)));
  connect(_ui->expandButton,SIGNAL(toggled(bool)),this,SLOT(setExpanded(bool)));
}

HeaderFrame::~HeaderFrame() {
  delete _ui;
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

bool HeaderFrame::isExpandable() const {
  return _ui->expandButton->isVisible();
}

void HeaderFrame::setExpandable(bool f) {
  _ui->expandButton->setVisible(f);
}

void HeaderFrame::expand(bool e) {
  _ui->expandButton->setChecked(e);
}

void HeaderFrame::setExpanded(bool e) {
  _expanded = e;

  QWidget *pw = parentWidget();
  if (!pw)
    return;
  foreach(QObject *obj, pw->children()) {
    QWidget *w = dynamic_cast<QWidget *>(obj);
    if (w && w != this)
      w->setVisible(e);
  }

  int maxH,minH;

  if (!e) {
    _oldHeightInfos = QPair<int,int>(pw->minimumHeight(),pw->maximumHeight());
    maxH = height();
    minH = height();
  }
  else {
    minH = _oldHeightInfos.first;
    maxH = _oldHeightInfos.second;
  }

  pw->setMinimumSize(pw->minimumWidth(),minH);
  pw->setMaximumSize(pw->maximumWidth(),maxH);

  emit expanded(e);
}

bool HeaderFrame::isExpanded() const {
  return _expanded;
}

QWidget *HeaderFrame::expandControl() const {
  return _ui->expandButton;
}
