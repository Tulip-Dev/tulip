#ifndef TABLEWIDGETS_H
#define TABLEWIDGETS_H

#include <string>

#include <QtGui/qpushbutton.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qpainter.h>
#include <QtGui/qitemdelegate.h>
#include <QtGui/qtablewidget.h>

#include <tulip/Graph.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>

class QLineEdit;
class QPaintEvent;

namespace tlp {

class PropertyInterface;

#ifndef DOXYGEN_NOTFOR_USER

class TLP_QT_SCOPE ColorButton : public QPushButton {

  Q_OBJECT;
  Q_PROPERTY(QRgb color READ getColor);
  
private:
  QRgb color;
public:
  ColorButton(const QRgb &c, QWidget *parent=0, const char *name=0, Qt::WFlags f=0);
  ~ColorButton();

  QRgb getColor() const;
  void paintEvent (QPaintEvent *);
  
public slots:
  void colorDialog();
};

class TLP_QT_SCOPE FilenameEditor : public QWidget {
  Q_OBJECT;
  Q_PROPERTY(QString fileName READ fileName WRITE setFileName);
  Q_PROPERTY(QString filter READ filter WRITE setFilter);
  
public:
  FilenameEditor(QWidget *);
  ~FilenameEditor();
  
  QString fileName() const;
  QString filter() const;

public slots:
  void setFileName(const QString &);
  void setFilter(const QString &);

signals:
  void fileNameChanged(const QString &);

private slots:
  void buttonPressed();

private:
  QLineEdit *lineedit;
  QPushButton *button;
  QString fileFilter;
};

class TLP_QT_SCOPE SizeEditor : public QWidget {
  Q_OBJECT;
private:
  Size size;
  QLineEdit *edit[3];

public:
  SizeEditor(const Size &, QWidget *);
  ~SizeEditor();
  Size getSize() const;
  void setFocus();
  
private slots:
  void changeW(const QString &);
  void changeH(const QString &);
  void changeD(const QString &);
};

class CoordEditor : public QWidget {
  Q_OBJECT;
  
private:
  Coord coord;
  QLineEdit *edit[3];

 public:
  CoordEditor(const Coord &, QWidget *);
  ~CoordEditor();
  Coord getCoord() const;
  void setFocus();
  
private slots:
  void changeX(const QString &);
  void changeY(const QString &);
  void changeZ(const QString &);
};

/* table items */
class TLP_QT_SCOPE TulipTableWidgetItem : public QTableWidgetItem {
public:
  TulipTableWidgetItem(int type) : QTableWidgetItem(type) {}
    TulipTableWidgetItem(QString s, int type = QTableWidgetItem::Type) : QTableWidgetItem(s, type) {}
  virtual QString textForTulip() const {
    return text();
  }
};

#endif //DOXYGEN_NOTFOR_USER

/** \addtogroup Tulip_Widgets */ 
/*@{*/
/* this table handle displaying and editing node or edge properties */
 class TLP_QT_SCOPE TulipTableWidget : public QTableWidget {
  Q_OBJECT;
  Q_PROPERTY(QColor backColor1 READ getBackColor1 WRITE setBackColor1 RESET resetBackColor1);
  Q_PROPERTY(QColor backColor2 READ getBackColor2 WRITE setBackColor2 RESET resetBackColor2);
  Q_PROPERTY(bool updateColumnTitle READ getUpdateColumnTitle WRITE setUpdateColumnTitle);

protected:
  QColor backColor1;
  QColor backColor2;
  bool updateColumnTitle;

public:
  TulipTableWidget(QWidget *, const char *);
  ~TulipTableWidget();

  void setTulipNodeItem(const PropertyInterface *, const std::string propertyName, const node &, const int row, const int col);
  QString getTulipNodeValue(const int row, const int col) const;
  void setTulipEdgeItem(const PropertyInterface *, const std::string propertyName, const edge &, const int row, const int col);
  QString getTulipEdgeValue(const int row, const int col) const;
  QColor getBackColor1() const;
  QColor getBackColor2() const;
  bool getUpdateColumnTitle() const;
  QColor backgroundColor(const int row) const;
  //void paintCell(QPainter *, int row, int col, const QRect &, bool selected, const QColorGroup &);

public slots:
  void setBackColor1(const QColor &);
  void resetBackColor1();
  void setBackColor2(const QColor &);
  void resetBackColor2();
  void setUpdateColumnTitle(const bool);
};

}
/*@}*/
#endif //PROPERTYWIDGETS_H
