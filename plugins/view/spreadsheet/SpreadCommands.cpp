/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include "SpreadCommands.h"

#include <iostream> //NOTE: for debugging
#include <QtGui/QClipboard>

#include "SpreadCell.h"

namespace tlp {

  ///   Data Changed

  DataChanged::DataChanged(SpreadTable *t, int r, int c, const QString &od, const QString &nd)
    : table(t),
      row(r),
      column(c),
      oldData(od),
      newData(nd)
  {
    setText(QObject::tr("Modify Cell"));
  }

  void DataChanged::undo()
  {
    table -> setItemText(row, column, oldData);
    table -> setCurrentCell(row, column);
  }

  void DataChanged::redo()
  {
    table -> setItemText(row, column, newData);
    table -> setCurrentCell(row, column);
  }

  ///   Cut

  CutCommand::CutCommand(SpreadTable *t)
    : table(t)
  {
    setText(QObject::tr("Cut"));
    table -> copy(oldData);
    selectedRange = table -> getSelectedRange();
  }

  void CutCommand::undo()
  {
    table -> paste(oldData, selectedRange.topRow(), selectedRange.leftColumn());
  }

  void CutCommand::redo()
  {
    table -> deleteContents(selectedRange);
    QMimeData *newMimeData = new QMimeData;
    newMimeData -> setData("Cell", oldData.data("Cell"));
    QApplication::clipboard() -> setMimeData(newMimeData);
  }
  
  ///   Paste

  PasteCommand::PasteCommand(SpreadTable *t)
  {
    setText(QObject::tr("Paste"));
    table = t;
   
    if (QApplication::clipboard() -> mimeData() -> hasFormat("Cell"))
      {
	QByteArray byteArray = QApplication::clipboard() -> mimeData() -> data("Cell");
	QDataStream inStream(&byteArray, QIODevice::ReadOnly);
	newData.setData("Cell", byteArray);
	row = table -> currentRow();
	column = table -> currentColumn();
	int numOfRows;
	int numOfColumns;
	inStream >> numOfRows >> numOfColumns;
      
	QTableWidgetSelectionRange range(row, column, row+numOfRows-1, column+numOfColumns-1);
	oldData.setData("Cell", table -> getItemsInByteArray(range));
      }
    else if (QApplication::clipboard() -> mimeData() -> hasText())
      {
	QString string = QApplication::clipboard() -> mimeData() -> text();
	newData.setText(string);
	row = table -> currentRow();
	column = table -> currentColumn();
	int rowCount = string.count('\n')+1;
	int columnCount = string.count('\t')/rowCount + 1;
	QTableWidgetSelectionRange range(row, column, row+rowCount-1, column+columnCount-1);
	oldData.setData("Cell", table -> getItemsInByteArray(range));
      }
  }

  void PasteCommand::undo()
  {
    table -> paste(oldData, row, column);
  }

  void PasteCommand::redo()
  {
    table -> paste(newData, row, column);
  }

  ///   Insert Rows

  InsertRowsCommand::InsertRowsCommand(SpreadTable *t, int pos, int am)
  {
    setText(QObject::tr("Insert Row(s)"));
    table = t;
    position = pos == SpreadTable::InsertAfter ? 1 : 0;
    position += table -> currentRow();
    amount = am;
  }

  void InsertRowsCommand::undo()
  {
    table -> deleteEntireRows(position, amount);
  }

  void InsertRowsCommand::redo()
  {
    table -> insertRows(position, amount);
  }

  ///   Insert Columns

  InsertColumnsCommand::InsertColumnsCommand(SpreadTable *t, int pos, int am)
    : table(t)
  {
    setText(QObject::tr("Insert Column(s)"));
    position = pos == SpreadTable::InsertAfter ? 1 : 0;
    position += table -> currentColumn();
    amount = am;
  }

  void InsertColumnsCommand::undo()
  {
    table -> deleteEntireColumns(position, amount);
  }

  void InsertColumnsCommand::redo()
  {
    table -> insertColumns(position, amount);
  }

  ///   Delete Contents

  DeleteContents::DeleteContents(SpreadTable *t)
    : table (t)
  {
    setText(QObject::tr("Delete"));
    range = table -> getSelectedRange();
    table -> copy(oldData);
  }

  void DeleteContents::undo()
  {
    table -> paste(oldData, range.topRow(), range.leftColumn());
  }

  void DeleteContents::redo()
  {
    table -> deleteContents(range);
  }

  ///   Delete Entire Rows

  DeleteEntireRows::DeleteEntireRows(SpreadTable *t, int pos, int am)
  {
    setText(QObject::tr("Delete Row(s)"));
    table = t;
    position = pos;
    amount = am;
    QTableWidgetSelectionRange range(position, 0, position+amount-1, table->columnCount()-1);
    oldData.setData("Cell", table -> getItemsInByteArray(range));
  }

  void DeleteEntireRows::undo()
  {
    table -> insertRows(position, amount);
    table -> paste(oldData, position, 0);
  }

  void DeleteEntireRows::redo()
  {
    table -> deleteEntireRows(position, amount);
  }

  ///   Delete Entire Columns

  DeleteEntireColumns::DeleteEntireColumns(SpreadTable *t, int pos, int am)
  {
    setText(QObject::tr("Delete Column(s)"));
    table = t;
    position = pos;
    amount = am;
    QTableWidgetSelectionRange range(0, position, table->rowCount()-1, position+amount-1);
    oldData.setData("Cell", table -> getItemsInByteArray(range));
  }

  void DeleteEntireColumns::undo()
  {
    table -> insertColumns(position, amount);
    table -> paste(oldData, 0, position);
  }

  void DeleteEntireColumns::redo()
  {
    table -> deleteEntireColumns(position, amount);
  }

  ///   Change Cells Editor

  ChangeCellsEditor::ChangeCellsEditor(SpreadTable *t, QTableWidgetSelectionRange r, int e)
    : table(t),
      range(r),
      newEditor(e)
  {
    setText(QObject::tr("Change Data Format"));
   
    for (int i=0; i<range.rowCount(); i++)
      for (int j=0; j<range.columnCount(); j++)
	oldEditors.append(table -> getItemEditor(i+range.topRow(), j+range.leftColumn()));
  }

  void ChangeCellsEditor::undo()
  {
    for (int i=0; i<range.rowCount(); i++)
      for (int j=0; j<range.columnCount(); j++)
	table -> setItemEditor(i+range.topRow(), j+range.leftColumn(),
			       oldEditors[i*range.columnCount()+j]);
  }

  void ChangeCellsEditor::redo()
  {
    for (int i=0; i<range.rowCount(); i++)
      for (int j=0; j<range.columnCount(); j++)
	table -> setItemEditor(i+range.topRow(), j+range.leftColumn(), newEditor);
  }

  ///   Change Cells Font

  ChangeCellsFont::ChangeCellsFont(SpreadTable *t, QTableWidgetSelectionRange r, QFont f)
    : table(t),
      range(r),
      newFont(f)
  {
    setText(QObject::tr("Change Font"));
   
    for (int i=0; i<range.rowCount(); i++)
      for (int j=0; j<range.columnCount(); j++)
	oldFonts.append(table -> getItemFont(i+range.topRow(), j+range.leftColumn()));
  }

  void ChangeCellsFont::undo()
  {
    for (int i=0; i<range.rowCount(); i++)
      for (int j=0; j<range.columnCount(); j++)
	table -> setItemFont(i+range.topRow(), j+range.leftColumn(),
			     oldFonts[i*range.columnCount()+j]);
  }

  void ChangeCellsFont::redo()
  {
    for (int i=0; i<range.rowCount(); i++)
      for (int j=0; j<range.columnCount(); j++)
	table -> setItemFont(i+range.topRow(), j+range.leftColumn(), newFont);
  }

  ///   Change Cells Alignment

  ChangeCellsAlignment::ChangeCellsAlignment(SpreadTable *t, QTableWidgetSelectionRange r, int a)
    : table(t),
      range(r),
      newAlignment(a)
  {
    setText(QObject::tr("Change Alignment"));
   
    for (int i=0; i<range.rowCount(); i++)
      for (int j=0; j<range.columnCount(); j++)
	oldAlignments.append(table -> getItemAlignment(i+range.topRow(),
						       j+range.leftColumn()));
  }

  void ChangeCellsAlignment::undo()
  {
    for (int i=0; i<range.rowCount(); i++)
      for (int j=0; j<range.columnCount(); j++)
	table -> setItemAlignment(i+range.topRow(), j+range.leftColumn(),
				  oldAlignments[i*range.columnCount()+j]);
  }

  void ChangeCellsAlignment::redo()
  {
    for (int i=0; i<range.rowCount(); i++)
      for (int j=0; j<range.columnCount(); j++)
	table -> setItemAlignment(i+range.topRow(), j+range.leftColumn(), newAlignment);
  }

  ///   Change Cells Background

  ChangeCellsBackground::ChangeCellsBackground(SpreadTable *t, QTableWidgetSelectionRange r, QColor b)
    : table(t),
      range(r),
      newBackground(b)
  {
    setText(QObject::tr("Change Background"));
   
    for (int i=0; i<range.rowCount(); i++)
      for (int j=0; j<range.columnCount(); j++)
	oldBackgrounds.append(table -> getItemBackground(i+range.topRow(),
							 j+range.leftColumn()).color());
  }

  void ChangeCellsBackground::undo()
  {
    for (int i=0; i<range.rowCount(); i++)
      for (int j=0; j<range.columnCount(); j++)
	table -> setItemBackground(i+range.topRow(), j+range.leftColumn(),
				   QBrush(oldBackgrounds[i*range.columnCount()+j]));
  }

  void ChangeCellsBackground::redo()
  {
    for (int i=0; i<range.rowCount(); i++)
      for (int j=0; j<range.columnCount(); j++)
	table -> setItemBackground(i+range.topRow(), j+range.leftColumn(),
				   QBrush(newBackground));
  }


  ///   Change Cells Foreground

  ChangeCellsForeground::ChangeCellsForeground(SpreadTable *t, QTableWidgetSelectionRange r, QColor f)
    : table(t),
      range(r),
      newForeground(f)
  {
    setText(QObject::tr("Change Text Color"));
   
    for (int i=0; i<range.rowCount(); i++)
      for (int j=0; j<range.columnCount(); j++)
	oldForegrounds.append(table -> getItemForeground(i+range.topRow(),
							 j+range.leftColumn()).color());
  }

  void ChangeCellsForeground::undo()
  {
    for (int i=0; i<range.rowCount(); i++)
      for (int j=0; j<range.columnCount(); j++)
	table -> setItemForeground(i+range.topRow(), j+range.leftColumn(),
				   QBrush(oldForegrounds[i*range.columnCount()+j]));
  }

  void ChangeCellsForeground::redo()
  {
    for (int i=0; i<range.rowCount(); i++)
      for (int j=0; j<range.columnCount(); j++)
	table -> setItemForeground(i+range.topRow(), j+range.leftColumn(), QBrush(newForeground));
  }

  ///   Sort

  SortCommand::SortCommand(SpreadTable *t, const SpreadTableSort &ts)
    : table(t),
      tableSort(ts)
  {
    setText(QObject::tr("Sort"));
    range = table -> getSelectedRange();
    oldData.setData("Cell", table -> getItemsInByteArray(range));
  }

  void SortCommand::undo()
  {
    table -> paste(oldData, range.topRow(), range.leftColumn());
  }

  void SortCommand::redo()
  {
    table -> sort(tableSort);
  }

}
