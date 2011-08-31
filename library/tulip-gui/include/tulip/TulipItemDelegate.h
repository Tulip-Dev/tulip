#ifndef TULIPITEMDELEGATE_H
#define TULIPITEMDELEGATE_H

#include <QtGui/QStyledItemDelegate>
#include "tulip/TulipItemEditorCreators.h"

namespace tlp {

class TLP_QT_SCOPE TulipItemDelegate : public QStyledItemDelegate {
  QMap<int,TulipItemEditorCreator*> _creators;

public:
  explicit TulipItemDelegate(QObject* parent=0);

  virtual ~TulipItemDelegate();

  template<typename T>
  void registerCreator(tlp::TulipItemEditorCreator*);

  void unregisterCreator(tlp::TulipItemEditorCreator*);
  template<typename T>
  void unregisterCreator();

  template<typename T>
  tlp::TulipItemEditorCreator* creator() const;

  tlp::TulipItemEditorCreator* creator(int) const;
};

}

#endif // TULIPITEMDELEGATE_H
