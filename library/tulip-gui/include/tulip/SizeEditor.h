#ifndef SIZEEDITOR_H
#define SIZEEDITOR_H

#include <QtGui/QWidget>
#include <tulip/Size.h>
#include <tulip/tulipconf.h>

namespace Ui {
class SizeEditorData;
}

namespace tlp {

class TLP_QT_SCOPE SizeEditor: public QWidget {
  Q_OBJECT
  Q_PROPERTY(Size size READ size WRITE setSize)

  Ui::SizeEditorData* _ui;

public:
  explicit SizeEditor(QWidget *parent=0);
  virtual ~SizeEditor();
  tlp::Size size() const;

public slots:
  void setSize(const tlp::Size& s);
};

}

#endif // SIZEEDITOR_H
