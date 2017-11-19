#ifndef NODESHAPEDIALOG_H
#define NODESHAPEDIALOG_H

#include <tulip/tulipconf.h>

#include <QDialog>
#include <QString>
#include <QPixmap>

namespace Ui {
class ShapeDialog;
}

namespace tlp {

class TLP_QT_SCOPE ShapeDialog : public QDialog {

  Q_OBJECT

  Ui::ShapeDialog *_ui;
  QString _selectedShapeName;
  std::list<std::pair<QString, QPixmap>> shapes;

public:
  ShapeDialog(std::list<std::pair<QString, QPixmap>> &nodeShapes, QWidget *parent = nullptr);
  ~ShapeDialog() override;

  QString getSelectedShapeName() const;

  void setSelectedShapeName(const QString &shapeName);

  void accept() override;

  void showEvent(QShowEvent *) override;

protected slots:
  void updateShapeList();
};
}

#endif
