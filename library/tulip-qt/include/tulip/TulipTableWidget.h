#ifndef TABLEWIDGETS_H
#define TABLEWIDGETS_H

#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif
#include <string>

#include <Qt3Support/q3button.h>
#include <Qt/qpainter.h>
#include <Qt3Support/q3table.h>
#include "tulip/Qt3ForTulip.h"

#include <tulip/Graph.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>

class QLineEdit;
class QPaintEvent;
class QPushButton;

namespace tlp {

class PropertyInterface;

#ifndef DOXYGEN_NOTFOR_USER

class ColorButton : public QButton {

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

class FilenameEditor : public QWidget {
  Q_OBJECT;
  Q_PROPERTY(QString fileName READ fileName WRITE setFileName);
  Q_PROPERTY(QString filter READ filter WRITE setFilter);
  
public:
  FilenameEditor(QWidget *, const char *);
  FilenameEditor(const QString &filter, QWidget *, const char *);
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

class SizeEditor : public QWidget {
  Q_OBJECT;
private:
  Size size;
  QLineEdit *edit[3];

public:
  SizeEditor(const Size &, QWidget *, const char *);
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
  CoordEditor(const Coord &, QWidget *, const char *);
  ~CoordEditor();
  Coord getCoord() const;
  void setFocus();
  
private slots:
  void changeX(const QString &);
 void changeY(const QString &);
 void changeZ(const QString &);
};

/* table items */
class ColorTableItem : public QTableItem {
private:
  QRgb color;
public:
  ColorTableItem(QTable *table, const QRgb &color);
  ~ColorTableItem();
  QRgb getColor() const;
  // void setPixmap(const QPixmap &p);
  // void setText(const QString &str);
  // QString text() const;
  
  QWidget *createEditor() const;
  void setContentFromEditor(QWidget *w);
  void paint(QPainter *p, const QColorGroup &cg, const QRect &cr, bool selected);  
  //  QSize sizeHint() const;
  int rtti() const;
};

class FileTableItem : public QTableItem {
public:
  FileTableItem(QTable *table);
  ~FileTableItem();
  int rtti() const;
  QWidget *createEditor() const;
  void setContentFromEditor(QWidget *w);
};

class SizeTableItem : public QTableItem {
private:
  Size size;
public:
  SizeTableItem(QTable *table);
  ~SizeTableItem();
  int rtti() const;
  
  void setSize(const Size &s);
  QWidget *createEditor() const;
  void setContentFromEditor(QWidget *w);
};

class CoordTableItem : public QTableItem {
private:
  Coord coord;
public:
  CoordTableItem(QTable *table);
  ~CoordTableItem();
  int rtti() const;
  
  void setCoord(const Coord &c);
  QWidget *createEditor() const;
  void setContentFromEditor(QWidget *w);
};

class GlyphTableItem : public QComboTableItem {
public:
  GlyphTableItem(QTable *, bool editable = false);
  ~GlyphTableItem();
  int rtti() const;
  QString text() const;
};

class EdgeShapeTableItem : public QComboTableItem {
public:
  EdgeShapeTableItem(QTable *, bool editable = false);
  ~EdgeShapeTableItem();
  int rtti() const;
  QString text() const;
};

class LabelPositionTableItem : public QComboTableItem {
public:
  LabelPositionTableItem(QTable *, bool editable = false);
  ~LabelPositionTableItem();
  int rtti() const;
  QString text() const;
};

class SelectionTableItem : public QCheckTableItem {
public:
  SelectionTableItem(QTable *table, const QString &txt);
  ~SelectionTableItem();
  int rtti() const;

  QString text() const;
};
#endif //DOXYGEN_NOTFOR_USER

/** \addtogroup Tulip_Widgets */ 
/*@{*/
/* this table handle displaying and editing node or edge properties */
class TulipTableWidget : public QTable {
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
  void paintCell(QPainter *, int row, int col, const QRect &, bool selected, const QColorGroup &);

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
