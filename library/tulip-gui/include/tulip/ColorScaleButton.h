#ifndef COLORSCALEBUTTON_H
#define COLORSCALEBUTTON_H

#include <QtGui/QPushButton>
#include <tulip/ColorScaleConfigDialog.h>

namespace tlp {

class TLP_QT_SCOPE ColorScaleButton: public QPushButton {
  Q_OBJECT
  ColorScale _colorScale;
  ColorScaleConfigDialog* _dlg;
public:
  static void paintScale(QPainter*,const QRect&, const ColorScale&);

  ColorScaleButton(ColorScale colorScale=ColorScale(), QWidget* parent=0);
  virtual ~ColorScaleButton();

  ColorScale colorScale() const;
  void setColorScale(const ColorScale& colorScale);
  void paintEvent(QPaintEvent *event);


protected slots:
  void showDialog();
};

}
#endif // COLORSCALEBUTTON_H
