#ifndef COORDEDITOR_H
#define COORDEDITOR_H

#include <QtGui/QWidget>
#include <tulip/Coord.h>
#include <tulip/tulipconf.h>

namespace Ui {
class CoordEditor;
}

namespace tlp {
/**
* @brief Simple Coord edition widget.
**/
class TLP_QT_SCOPE CoordEditor : public QWidget {
  Q_OBJECT
  Q_PROPERTY(Coord coord READ coord WRITE setCoord NOTIFY coordChanged)

  Ui::CoordEditor *ui;
public:
  explicit CoordEditor(QWidget *parent = 0);
  ~CoordEditor();

  tlp::Coord coord() const;

public slots:
  void setCoord(const tlp::Coord& s);

signals:
  void coordChanged(tlp::Coord coord);

private slots:
  void coordUpdated();

};
}

#endif // COORDEDITOR_H
