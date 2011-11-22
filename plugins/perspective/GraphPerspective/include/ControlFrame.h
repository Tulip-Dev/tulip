#ifndef CONTROLFRAME_H
#define CONTROLFRAME_H

#include <QtGui/QFrame>

namespace Ui {
class ControlFrame;
}

class ControlFrame: public QFrame {
  Ui::ControlFrame* _ui;
public:
  explicit ControlFrame(QWidget* parent = 0);
  virtual ~ControlFrame();
};

#endif // CONTROLFRAME_H
