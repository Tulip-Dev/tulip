#include "FiltersManagerItem.h"

#include "ui_FiltersManagerItem.h"
#include "ui_FiltersManagerAlgorithmItem.h"
#include "ui_FiltersManagerInvertItem.h"
#include "ui_FiltersManagerCompareItem.h"

#include <QtGui/QMenu>
#include <tulip/PropertyAlgorithm.h>

using namespace tlp;
using namespace std;

/*
  Helper classes
  */
FiltersManagerAlgorithmItem::FiltersManagerAlgorithmItem(QWidget* parent): AbstractFiltersManagerItem(parent), _ui(new Ui::FiltersManagerAlgorithmItem) {
  _ui->setupUi(this);
  string s;
  forEach(s,BooleanPluginLister::availablePlugins()) {
    _ui->algorithmCombo->addItem(s.c_str());
  }
}
void FiltersManagerAlgorithmItem::applyFilter() {
}
QString FiltersManagerAlgorithmItem::title() const {
}

FiltersManagerCompareItem::FiltersManagerCompareItem(QWidget* parent): AbstractFiltersManagerItem(parent), _ui(new Ui::FiltersManagerCompareItem) {
  _ui->setupUi(this);
}
void FiltersManagerCompareItem::applyFilter() {
}
QString FiltersManagerCompareItem::title() const {
}

FiltersManagerInvertItem::FiltersManagerInvertItem(QWidget* parent): AbstractFiltersManagerItem(parent), _ui(new Ui::FiltersManagerInvertItem) {
  _ui->setupUi(this);
}
void FiltersManagerInvertItem::applyFilter() {
}
QString FiltersManagerInvertItem::title() const {
}
// ===========

FiltersManagerItem::FiltersManagerItem(QWidget *parent): QFrame(parent), _ui(new Ui::FiltersManagerItem) {
  _ui->setupUi(this);
  setMode(Invalid);
}

FiltersManagerItem::~FiltersManagerItem() {
  delete _ui;
}

void FiltersManagerItem::setCompareMode() {
  setMode(Compare);
}

void FiltersManagerItem::setAlgorithmMode() {
  setMode(Algorithm);
}

void FiltersManagerItem::setInvertMode() {
  setMode(Invert);
}

void FiltersManagerItem::setMode(Mode m) {
  _ui->delButton->setVisible(m != Invalid);
  _ui->addButton->setVisible(m == Invalid);
  _ui->dataBox->setExpanded(m != Invalid);
  _ui->dataBox->setEnabled(m != Invalid);

  QWidget* w = NULL;
  if (m == Compare)
    w = new FiltersManagerCompareItem;
  else if (m == Invert)
    w = new FiltersManagerInvertItem;
  else if (m == Algorithm)
    w = new FiltersManagerAlgorithmItem;
  _ui->dataBox->setWidget(w);
}

FiltersManagerItem::Mode FiltersManagerItem::mode() const {
}

void FiltersManagerItem::addButtonClicked() {
  QMenu addMenu;
  addMenu.addAction(trUtf8("Invert selection"),this,SLOT(setInvertMode()));
  addMenu.addAction(trUtf8("Compare values"),this,SLOT(setCompareMode()));
  addMenu.addAction(trUtf8("Filtering algorithm"),this,SLOT(setAlgorithmMode()));
  addMenu.exec(QCursor().pos());
}
