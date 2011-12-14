#ifndef SHADOWFILTER_H
#define SHADOWFILTER_H
#include <QtCore/QObject>
#include <QtCore/QVector>
class ShadowFilter:public QObject {
  QVector<int>_0x7ff;
  int _0x7fe;
public:
  ShadowFilter(QObject*);
protected:
  bool eventFilter(QObject*,QEvent*);
};
#endif
