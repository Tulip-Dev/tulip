/*
  This file is part of the KDE libraries
  Copyright (C) 2005-2007 Daniel Molkentin <molkentin@kde.org>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License version 2 as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.    See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.    If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/
#ifndef KEXPANDABLEGROUPBOX_H
#define KEXPANDABLEGROUPBOX_H

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QAbstractButton>

class QScrollArea;
class QMouseEvent;
/**
  * @short An expandable container with a QComboBox-style API.
  *
  * A KExpandableGroupBox provides a title label with space that you can store arbitrary widgets in,
  * very much like a group box. However, unlike a group box, the contents is hidden by default and
  * can be expanded by a click on the title or the folding indicator (+/-).
  *
  * This class should be used within a QScrollArea, because the widget takes up more screen
  * estate when unfolded and might otherwise enlarge your dialog beyond the physical screen size.
  *
  * Example code:
  *
  * \code
  * QCheckBox *cb1 = new QCheckBox(tr("Option 1"));
  * QCheckBox *cb2 = new QCheckBox(tr("Option 2"));
  * QCheckBox *cb3 = new QCheckBox(tr("Option 3"));
  *
  * QWidget *widget = new QWidget;
  * QVBoxLayout *vbox = new QVBoxLayout(widget);
  * vbox->addWidget(cb1);
  * vbox->addWidget(cb2);
  * vbox->addWidget(cb3);
  *
  * KExpandableGroupBox *expWidget = new KExpandableGroupBox();
  * expWidget->setTitle(tr("Less Important Options"));
  * expWidget->setWidget(widget);
  *
  * QScrollArea *area = new QScrollArea;
  * area->setWindowTitle("Collapsible Widget Demo");
  * area->setWidgetResizable(true);
  *
  * QWidget *containerWidget = new QWidget
  * QVBoxLayout *lay = new QVBoxLayout(containerWidget);
  * lay->addWidget(collapsibleWidget);
  * \endcode
  *
  * @author Daniel Molkentin <molkentin@kde.org>
 */
class KExpandableGroupBox: public QWidget {
  Q_OBJECT
  Q_PROPERTY(bool expanded READ isExpanded WRITE setExpanded)
  Q_PROPERTY(QString title READ title WRITE setTitle)
  Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment)
  Q_PROPERTY(bool animateExpansion READ animateExpansion WRITE setAnimateExpansion)

public:
  /**
  * Default constructor.
  * @param parent is the parent widget
  * Use setTitle() and setWidget() to set the respective properties.
  */
  explicit KExpandableGroupBox(QWidget *parent=0);

  /**
  * Convinience constructor.
  * @param title is the title of the collaspsible widget
  * @param parent is the widget parent widget
  * Use setWidget() to set the widgets contents.
  */
  explicit KExpandableGroupBox(const QString& title, QWidget *parent = 0);
  ~KExpandableGroupBox();

  /**
  * @return the widgets title
  */
  QString title() const;

  /**
  * @return widgets wether the widget is expanded or not
  * @see setExpanded()
  */
  bool isExpanded() const;

  /**
  * @return the alignment of the title label
  * @see setAlignment()
  */
  Qt::Alignment alignment() const;

  /**
  * @return the widget contained
  * @see setWidget()
  */
  QWidget* widget() const;

  /**
  * Sets the inner widgets.
  * @see widget()
  */
  void setWidget(QWidget *w);

  /**
  * Sets the alignment of the title label.
  * @see alignment()
  */
  void setAlignment(Qt::Alignment a);

  /**
  * Sets wether or not the expansion of widget() should be animated.
  * @see animateExpansion()
  */
  void setAnimateExpansion(bool animate);

  /**
  * @return wether or not the expansion of widget() should be animated.
  * The default is true except on embedded platforms.
  * @see setAnimateExpansion()
  */
  bool animateExpansion() const;

  /** @reimp */
  QSize minimumSizeHint () const;


public slots:
  /**
  * Sets wether the widgets contents is expanded or not
  * @see isExpanded()
  */
  void setExpanded(bool expanded);

  /**
  * Sets the collapsible widgets title
  * @see title()
  */
  void setTitle(const QString& title);

protected:
  void mouseReleaseEvent(QMouseEvent *ev);
  void paintEvent(QPaintEvent*);

private slots:
  void animateExpansion(qreal);

private:
  void init();
  Q_DISABLE_COPY(KExpandableGroupBox)
  class Private;
  Private *d;
};

class ClickableLabel: public QLabel {
  Q_OBJECT
public:
  ClickableLabel(QWidget* parent = 0);
  ~ClickableLabel();
  void mouseReleaseEvent(QMouseEvent *e);
signals:
  void clicked();
};

class ArrowButton: public QAbstractButton {
public:
  ArrowButton(QWidget *parent = 0);
  ~ArrowButton();
  QSize sizeHint() const {
    return QSize(16, 16);
  }

protected:
  void paintEvent(QPaintEvent*);
};

#endif // KEXPANDABLEGROUPBOX_H
