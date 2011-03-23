#ifndef TULIPGRAPHICSLAYOUT_H
#define TULIPGRAPHICSLAYOUT_H

#include <QtGui/QGraphicsLayout>

namespace tlp {
class DataSet;

class TulipGraphicsLayout: public QGraphicsLayout {
public:
  TulipGraphicsLayout(const std::string &algName = "Random", DataSet *dataSet=0);
  virtual ~TulipGraphicsLayout();

  void setData(DataSet *dataSet=0);
  void setAlgorithm(const std::string &algName);

  void addItem(QGraphicsLayoutItem *);
  QGraphicsLayoutItem *itemAt(int index) const;
  void removeAt(int index);
  int count() const;

  void setGeometry(const QRectF &rect);

protected:
  QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;

private:
  std::string _algName;
  DataSet *_dataSet;

  QList<QGraphicsLayoutItem *> _items;
};
}

#endif // TULIPGRAPHICSLAYOUT_H
