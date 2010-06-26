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

#ifndef Tulip_SPREADCOMMANDS_H
#define Tulip_SPREADCOMMANDS_H

#include <QtGui/QUndoCommand>
#include <QtCore/QMimeData>
#include <QtCore/QPoint>

#include "SpreadTable.h"

namespace tlp {

  class DataChanged : public QUndoCommand
  {
  public:
    DataChanged(SpreadTable*, int, int, const QString&, const QString&);
    virtual void undo();
    virtual void redo();
      
  private:
    SpreadTable *table;
    int row;
    int column;
    QString oldData;
    QString newData;
  };

  class InsertRowsCommand : public QUndoCommand
  {
  public:
    InsertRowsCommand(SpreadTable*, int, int);
    virtual void undo();
    virtual void redo();
      
  private:
    SpreadTable *table;
    int amount;
    int position;
  };

  class InsertColumnsCommand : public QUndoCommand
  {
  public:
    InsertColumnsCommand(SpreadTable*, int, int);
    virtual void undo();
    virtual void redo();
      
  private:
    SpreadTable *table;
    int amount;
    int position;
  };

  class DeleteContents : public QUndoCommand
  {
  public:
    DeleteContents(SpreadTable*);
    virtual void undo();
    virtual void redo();
      
  private:
    SpreadTable *table;
    QTableWidgetSelectionRange range;
    QMimeData oldData;
  };

  class DeleteEntireRows : public QUndoCommand
  {
  public:
    DeleteEntireRows(SpreadTable*, int, int);
    virtual void undo();
    virtual void redo();
      
  private:
    SpreadTable *table;
    int position;
    int amount;
    QMimeData oldData;
  };

  class DeleteEntireColumns : public QUndoCommand
  {
  public:
    DeleteEntireColumns(SpreadTable*, int, int);
    virtual void undo();
    virtual void redo();
      
  private:
    SpreadTable *table;
    int position;
    int amount;
    QMimeData oldData;
  };

  class CutCommand : public QUndoCommand
  {
  public:
    CutCommand(SpreadTable*);
    virtual void undo();
    virtual void redo();
      
  private:
    SpreadTable *table;
    QMimeData oldData;
    QTableWidgetSelectionRange selectedRange;
  };

  class PasteCommand : public QUndoCommand
  {
  public:
    PasteCommand(SpreadTable*);
    virtual void undo();
    virtual void redo();
      
  private:
    SpreadTable *table;
    QMimeData oldData;
    QMimeData newData;
    int row;
    int column;
  };

  class ChangeCellsEditor : public QUndoCommand
  {
  public:
    ChangeCellsEditor(SpreadTable*, QTableWidgetSelectionRange, int);
    virtual void undo();
    virtual void redo();
      
  private:
    SpreadTable *table;
    QTableWidgetSelectionRange range;
    QList<int> oldEditors;
    int newEditor;
  };

  class ChangeCellsFont : public QUndoCommand
  {
  public:
    ChangeCellsFont(SpreadTable*, QTableWidgetSelectionRange, QFont);
    virtual void undo();
    virtual void redo();
      
  private:
    SpreadTable *table;
    QTableWidgetSelectionRange range;
    QList<QFont> oldFonts;
    QFont newFont;
  };

  class ChangeCellsAlignment : public QUndoCommand
  {
  public:
    ChangeCellsAlignment(SpreadTable*, QTableWidgetSelectionRange, int);
    virtual void undo();
    virtual void redo();
      
  private:
    SpreadTable *table;
    QTableWidgetSelectionRange range;
    QList<int> oldAlignments;
    int newAlignment;
  };

  class ChangeCellsBackground : public QUndoCommand
  {
  public:
    ChangeCellsBackground(SpreadTable*, QTableWidgetSelectionRange, QColor);
    virtual void undo();
    virtual void redo();
      
  private:
    SpreadTable *table;
    QTableWidgetSelectionRange range;
    QList<QColor> oldBackgrounds;
    QColor newBackground;
  };

  class ChangeCellsForeground : public QUndoCommand
  {
  public:
    ChangeCellsForeground(SpreadTable*, QTableWidgetSelectionRange, QColor);
    virtual void undo();
    virtual void redo();
      
  private:
    SpreadTable *table;
    QTableWidgetSelectionRange range;
    QList<QColor> oldForegrounds;
    QColor newForeground;
  };

  class SortCommand : public QUndoCommand
  {
  public:
    SortCommand(SpreadTable*, const SpreadTableSort&);
    virtual void undo();
    virtual void redo();
      
  private:
    SpreadTable *table;
    QMimeData oldData;
    QTableWidgetSelectionRange range;
    SpreadTableSort tableSort;
  };

}

#endif
