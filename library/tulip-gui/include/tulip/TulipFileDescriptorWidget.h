#ifndef TULIPFILEDESCRIPTORWIDGET_H
#define TULIPFILEDESCRIPTORWIDGET_H

#include <tulip/TulipMetaTypes.h>
#include <QtGui/QWidget>

class TLP_QT_SCOPE TulipFileDescriptorWidget: public QWidget {
  Q_OBJECT

  TulipFileDescriptor _data;
  QWidget* _display;
public:
  explicit TulipFileDescriptorWidget(QWidget* parent = 0);

  void setData(TulipFileDescriptor);
  TulipFileDescriptor data() const;

protected slots:
  void browse();
};

#endif // TULIPFILEDESCRIPTORWIDGET_H
