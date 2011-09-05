namespace tlp {

template<typename T>
void TulipItemDelegate::registerCreator(tlp::TulipItemEditorCreator *c) {
  int id = qMetaTypeId<T>();
  if (_creators.contains(id))
    return;
  _creators[id] = c;
}

template<typename T>
void TulipItemDelegate::unregisterCreator() {
  int id = qMetaTypeId<T>();
  if (_creators.contains(id))
    _creators.remove(id);
}

template<typename T>
tlp::TulipItemEditorCreator* TulipItemDelegate::creator() const {
  return creator(qMetaTypeId<T>());
}

}
