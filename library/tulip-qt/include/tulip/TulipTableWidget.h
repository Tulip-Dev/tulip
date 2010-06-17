/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#ifndef TABLEWIDGETS_H
#define TABLEWIDGETS_H

#include <string>

#include <QtGui/qpushbutton.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qpainter.h>
#include <QtGui/qitemdelegate.h>
#include <QtGui/qtablewidget.h>
#if QT_MINOR_REL < 5
#include <QtGui/qheaderview.h>
#endif

#include <tulip/Graph.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>

//#define STRINGLISTTABLEITEM_RTTI 1010

class QLineEdit;
class QPaintEvent;

namespace tlp {

class PropertyInterface;

#ifndef DOXYGEN_NOTFOR_USER

class DynamicTypeHandlerInterface {

public:
	virtual std::string getString() =0;

	virtual unsigned int size() = 0;

	virtual std::string get(unsigned int i) = 0;
	virtual void set(unsigned int i, const std::string& value) = 0;
	virtual void clear()=0;

};

class TLP_QT_SCOPE ColorButton: public QPushButton {

Q_OBJECT
	;Q_PROPERTY(QRgb color READ getColor)
	;

private:
	QRgb color;
public:
	ColorButton(const QRgb &c, QWidget *parent = 0, const char *name = 0, Qt::WFlags f = 0);
	~ColorButton();

	QRgb getColor() const;
	void paintEvent(QPaintEvent *);

public slots:
	void colorDialog();
};

class TLP_QT_SCOPE FilenameEditor: public QWidget {
Q_OBJECT
	;Q_PROPERTY(QString fileName READ fileName WRITE setFileName)
	;Q_PROPERTY(QString filter READ filter WRITE setFilter)
	;

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

class TLP_QT_SCOPE SizeEditor: public QWidget {
Q_OBJECT
	;
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

class CoordEditor: public QWidget {
Q_OBJECT
	;

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
class TLP_QT_SCOPE TulipTableWidgetItem: public QTableWidgetItem {
public:
	TulipTableWidgetItem(int type) :
		QTableWidgetItem(type) {
	}
	TulipTableWidgetItem(QString s, int type = QTableWidgetItem::Type) :
		QTableWidgetItem(s, type) {
	}
	virtual QTableWidgetItem * clone() const {
		return new TulipTableWidgetItem(text(), type());
	}

	virtual QString textForTulip() const {
		return text();
	}

	virtual void setTextFromTulip(const std::string& data) {
	  setText(QString::fromUtf8(data.c_str()));
	}
};

class ListItemPushButton: public QPushButton {
Q_OBJECT
	;
public:
	ListItemPushButton(DynamicTypeHandlerInterface* handler, TulipTableWidgetItem *orig,
			QWidget* parent, const QString& text);
	~ListItemPushButton();
	tlp::DynamicTypeHandlerInterface *getHandler() {
		return handler;
	}

private slots:
	void showListDialog();

private:
	tlp::TulipTableWidgetItem *orig;
	DynamicTypeHandlerInterface *handler;
};

#endif //DOXYGEN_NOTFOR_USER
/** \addtogroup Tulip_Widgets */
/*@{*/
/* this table handle displaying and editing node or edge properties */
class TLP_QT_SCOPE TulipTableWidget: public QTableWidget {
Q_OBJECT
	;Q_PROPERTY(QColor backColor1 READ getBackColor1 WRITE setBackColor1 RESET resetBackColor1)
	;Q_PROPERTY(QColor backColor2 READ getBackColor2 WRITE setBackColor2 RESET resetBackColor2)
	;Q_PROPERTY(bool updateColumnTitle READ getUpdateColumnTitle WRITE setUpdateColumnTitle)
	;

protected:

	QColor backColor1;
	QColor backColor2;
	bool updateColumnTitle;

public:
	TulipTableWidget(QWidget *, const char *);
	~TulipTableWidget();

	void setTulipNodeItem(const PropertyInterface *, const std::string propertyName, const node &,
			const int row, const int col);
	QString getTulipNodeValue(const int row, const int col) const;
	void setTulipEdgeItem(const PropertyInterface *, const std::string propertyName, const edge &,
			const int row, const int col);
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
