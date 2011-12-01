#ifndef MODALPOPUP_H
#define MODALPOPUP_H

#include <QtCore/QEvent>
#include <tulip/PlaceHolderWidget.h>
#include <tulip/tulipconf.h>

class TLP_QT_SCOPE ModalPopup : public PlaceHolderWidget {
  Q_OBJECT
  bool _deleteWidgetOnClose;

public:
  explicit ModalPopup(QWidget *parent = 0);
  virtual ~ModalPopup();

  bool deleteWidgetOnClose() const;

public slots:
  void setDeleteWidgetOnClose(bool);

protected:
  bool event(QEvent *);
};

#endif // MODALPOPUP_H
