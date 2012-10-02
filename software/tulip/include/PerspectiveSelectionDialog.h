#ifndef PERSPECTIVESELECTIONDIALOG_H
#define PERSPECTIVESELECTIONDIALOG_H

#include <QtGui/QDialog>
#include <QtCore/QModelIndex>

namespace Ui {
class PerspectiveSelectionDialog;
}

class PerspectiveSelectionDialog : public QDialog {
  Q_OBJECT

  Ui::PerspectiveSelectionDialog* _ui;
  QString _perspective;
public:
  explicit PerspectiveSelectionDialog(QWidget *parent = 0);
  QString perspectiveName() const;

protected slots:
  void selectionChanged(const QModelIndex& a, const QModelIndex& b);
};

#endif // PERSPECTIVESELECTIONDIALOG_H
