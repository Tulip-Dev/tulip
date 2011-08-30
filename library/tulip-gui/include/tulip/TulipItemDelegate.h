#ifndef TULIPITEMDELEGATE_H
#define TULIPITEMDELEGATE_H

#include <QtGui/QStyledItemDelegate>
#include "tulip/TulipItemEditorCreators.h"

namespace tlp {

class TLP_QT_SCOPE TulipItemDelegate : public QStyledItemDelegate {
  QMap<int,TulipItemEditorCreator*> _creators;

public:
  explicit TulipItemDelegate(QObject* parent=0);

};

}

#endif // TULIPITEMDELEGATE_H
