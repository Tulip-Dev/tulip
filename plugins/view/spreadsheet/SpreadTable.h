// +-------------------------------------------------------------------------+
// | Tulip Software                                                          |
// | Author:  Morgan Mathiaut                                                |
// +-------------------------------------------------------------------------+
// | License:                                                                |
// |                                                                         |
// | Tulip is free software; you can redistribute it and/or modify           |
// | it under the terms of the GNU General Public License as published by    |
// | the Free Software Foundation; either version 2 of the License, or       |
// | (at your option) any later version.                                     |
// |                                                                         |
// | Tulip is distributed in the hope that it will be useful,                |
// | but WITHOUT ANY WARRANTY; without even the implied warranty of          |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
// | GNU General Public License for more details.                            |
// |                                                                         |
// | You should have received a copy of the GNU General Public License       |
// | along with QSimpleSheet; if not, write to the Free Software             |
// | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA|
// |                                                                         |
// +-------------------------------------------------------------------------+

// Based on

// +-------------------------------------------------------------------------+
// | QSimpleSheet simple spreadsheet                                         |
// | Copyright (C) 2008 Shahab <shahab.sh.70@gmail.com>                      |
// +-------------------------------------------------------------------------+
// | License:                                                                |
// |                                                                         |
// | QSimpleSheet is free software; you can redistribute it and/or modify    |
// | it under the terms of the GNU General Public License as published by    |
// | the Free Software Foundation; either version 2 of the License, or       |
// | (at your option) any later version.                                     |
// |                                                                         |
// | QSimpleSheet is distributed in the hope that it will be useful,         |
// | but WITHOUT ANY WARRANTY; without even the implied warranty of          |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
// | GNU General Public License for more details.                            |
// |                                                                         |
// | You should have received a copy of the GNU General Public License       |
// | along with QSimpleSheet; if not, write to the Free Software             |
// | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA|
// |                                                                         |
// +-------------------------------------------------------------------------+
// | Author:  Shahab Shirazi <shahab.sh.70@gmail.com>                        |
// +-------------------------------------------------------------------------+


#ifndef Tulip_SPREADTABLE_H
#define Tulip_SPREADTABLE_H

#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetSelectionRange>

#include <tulipconf.h>

#include "SpreadInsertColumnDialogData.h"

class QUndoStack;
class QMimeData;

namespace tlp {

  class Graph;

  class SpreadTableSort;
  class SpreadTable;
  
  class Graphic: public QWidget
  {
    Q_OBJECT
      
  public:
    Graphic(const QPoint& position,SpreadTable* parent);

    bool isActiv();
      
  protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    SpreadTable *tableWidget;
    bool isClicked;

  };

  class InsertColumnDialog : public QDialog, public Ui::InsertColumnDialogData {
    Q_OBJECT
  public:
    InsertColumnDialog(QWidget *parent=0):QDialog(parent) {
      setupUi(this);
    }
    std::string getName() {
      return lineEdit->text().toStdString();
    }
    std::string getType() {
      return comboBox->itemText(comboBox->currentIndex()).toStdString();
    }
  };

  class SpreadTable : public QTableWidget
  {
    Q_OBJECT
    
    public:

    enum View{
      NodesView,
      EdgesView
    };

    SpreadTable(QWidget* = 0);
    void setView(View view);

    //*************************************************************************************************
    void setGraph(Graph *graph);
    void addRow(unsigned int id);
    void delRow(unsigned int id);

    //*************************************************************************************************   
    void newSheet(int = 10, int = 10);
    bool writeSheet(const QString&);
    bool readSheet(const QString&);
    
    //(row,column,role) returns the item's text in the given position (row, column),
    //for the given role (DisplayRole, EditRole,...)
    QString getItemText(int, int, int = Qt::DisplayRole) const;
    //returns the text of the current cell for the given role
    QString getCurrentItemText(int = Qt::DisplayRole) const;
    //returns the text of cells in the selection for the given role
    QString getItemsText(const QTableWidgetSelectionRange&, int = Qt::DisplayRole) const;
    //returns the location of current cell in "AA1" format
    bool setItemText(int, int, const QString &);
    QByteArray getItemInByteArray(int, int) const;
    QByteArray getItemsInByteArray(const QTableWidgetSelectionRange&) const;
    int getItemEditor(int, int) const;
    bool setItemEditor(int, int, int);
    QString getCurrentItemLocation() const;
    QFont getItemFont(int, int) const;
    void setItemFont(int, int, const QFont&);
    int getItemAlignment(int, int) const;
    void setItemAlignment(int, int, int);
    QBrush getItemBackground(int, int) const;
    void setItemBackground(int, int, const QBrush&);
    QBrush getItemForeground(int, int) const;
    void setItemForeground(int, int, const QBrush&);
    
    void findNext(QString&, Qt::CaseSensitivity, int);
    void findPrevious(QString&, Qt::CaseSensitivity, int);
    
    void sort(SpreadTableSort);
    
    void insertRows(int, int);
    void insertColumns(int, int);
    
    void deleteContents(const QTableWidgetSelectionRange&);
    void deleteEntireRows(int, int);
    void deleteEntireColumns(int, int);
    
    void cut(QMimeData&); //cut data to a QMimeData
    void copy(QMimeData&); //copy data to a QMimeData
    bool paste(const QMimeData&, int, int); //paste data from a QMimeData, at row, column
    
    // receives an integer (column) and encode it 
    // into a string in format "A","B",...,"AA","AB",...
    static void encodePosition(QString&, int);
    // receives two integer (row, column) and encodes 
    // them into a string in format "A1","B1",...,"AA2","AB2",...
    static void encodePosition(QString&, int, int);
    // receives a QTableWidgetSelectionRange and encodes it
    // into a string in format "A1:B1"
    static void encodeRange(QString&, QTableWidgetSelectionRange);
    // receives a string in format "AB1" and decodes it into 
    // two integer (row, column)
    static bool decodePosition(QString, int&, int&);
    // receives a string in format "A1:B1" and decodes it into
    // a QTableWidgetSelectionRange
    static bool decodeRange(QString, QTableWidgetSelectionRange&);
    
    QUndoStack* getUndoStack() const;
    QTableWidgetSelectionRange getSelectedRange();
    enum {InsertAfter, InsertBefore};
    enum {FindInFormula, FindInDisplayText};
    enum {EditorRole = 32};
    enum {TextLineEditor=0, DateEditor, TimeEditor, IntegerEditor, DecimalEditor};
    enum {ImageType};
    
  public slots:
    void selectAll();
    void selectCurrentRow();
    void selectCurrentColumn();
    void userChangedItemDataSlot(int, int, QString&, QString&);
    void verticalScroll(int);
    void redrawView();
    
  private slots:
    void someThingChanged();
      
  signals:
    void userChangedItemDataSignal(int, int, QString&, QString&);
    void modified();
    void mouseReleasedSignal(SpreadTable*,QTableWidgetItem *);
      
  public:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    
  private:
    enum {Selected, Changed};
    enum {MagicWord = 0x93FE584F};
    void adjustHorizontalHeader();
    QTableWidgetItem* createGetItem(int, int);
    void reloadView();
    void loadCell(int minRow,int maxRow, int minColumn, int maxColumn);


    View view;
    QUndoStack *undoStack;
    Graphic *selectZone;
    QTableWidgetItem *firstSelectedItem;
    unsigned int firstSelectedItemRow;
    unsigned int firstSelectedItemColumn;
    unsigned int verticalBufferBegin;
    unsigned int horizontalBufferBegin;
    QTableWidgetItem *lastCell;
    QString lastFormule;

    //*************************************************************************************************
    Graph *graph;
    //*************************************************************************************************

  };

  class SpreadTableSort {
  public:
    enum SortDirection {SortRows, SortColumns};
    SpreadTableSort(const QTableWidgetSelectionRange&, const QList<int>&,
		    const QList<bool>&, SortDirection, Qt::CaseSensitivity, const QString&);
    bool operator()(int, int) const;
    void setTable(SpreadTable *t)
    { table = t; }
    QTableWidgetSelectionRange getRange() const
    { return range; }
    SortDirection getSortDirection() const
    { return sortDirection; }
    
  private:
    SpreadTable *table;
    QTableWidgetSelectionRange range;
    QList<int> sortKeys;
    QList<bool> ascending;
    SortDirection sortDirection;
    Qt::CaseSensitivity caseSensitivity;
    QStringList customOrder;
      
    int compare(const QString&, const QString&) const;
  };

}

#endif
