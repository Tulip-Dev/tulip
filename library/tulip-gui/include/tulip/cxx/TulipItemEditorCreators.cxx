#include <QtGui/QLineEdit>

namespace tlp {

template<typename T>
QWidget* LineEditEditorCreator<T>::createWidget(QWidget *parent) const {
  return new QLineEdit(parent);
}

template<typename T>
void LineEditEditorCreator<T>::setEditorData(QWidget* editor, const QVariant &data,tlp::Graph*) {
  typename T::RealType val = data.value<typename T::RealType>();
  static_cast<QLineEdit*>(editor)->setText(T::toString(val).c_str());
}

template<typename T>
QVariant LineEditEditorCreator<T>::editorData(QWidget* editor,tlp::Graph*) {
  std::string strVal = static_cast<QLineEdit*>(editor)->text().toStdString();
  QVariant result;
  typename T::RealType val;

  if (T::fromString(val,strVal))
    result.setValue<typename T::RealType>(val);

  return result;
}

}
