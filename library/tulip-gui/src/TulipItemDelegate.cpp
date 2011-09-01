#include "tulip/TulipItemDelegate.h"

namespace tlp {

TulipItemDelegate::TulipItemDelegate(QObject* parent): QStyledItemDelegate(parent) {
  // Register base editors
}

TulipItemDelegate::~TulipItemDelegate() {
  foreach(tlp::TulipItemEditorCreator* v,_creators.values())
  delete v;
}

template<typename T>
void TulipItemDelegate::registerCreator(tlp::TulipItemEditorCreator* c) {
  _creators[qMetaTypeId<T>()] = c;
}

void TulipItemDelegate::unregisterCreator(tlp::TulipItemEditorCreator* c) {
  int k = _creators.key(c,INT_MIN);

  if (k != INT_MIN)
    _creators.remove(k);
}

template<typename T>
void TulipItemDelegate::unregisterCreator() {
  _creators.remove(qMetaTypeId<T>());
}

template<typename T>
tlp::TulipItemEditorCreator* TulipItemDelegate::creator() const {
  return creator(qMetaTypeId<T>());
}

tlp::TulipItemEditorCreator* TulipItemDelegate::creator(int typeId) const {
  tlp::TulipItemEditorCreator *result = NULL;

  if (_creators.contains(typeId))
    result = _creators[typeId];

  return result;
}

}
