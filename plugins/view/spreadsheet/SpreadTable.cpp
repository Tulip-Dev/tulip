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
#include <iostream> //NOTE for debuging
#include <cmath>
#include <QtGui/QPainter>
#include <QtGui/QMessageBox>
#include <QtGui/QMouseEvent>

#include <tulip/Graph.h>
#include <tulip/ForEach.h>
#include <tulip/AbstractProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/ColorProperty.h>

#include "SpreadTable.h"
#include "SpreadCell.h"
#include "SpreadItemDelegate.h"
#include "SpreadCommands.h"
#include "SpreadCalculator.h"
#include "SpreadView.h"

using namespace std;

#define TABLEHEIGHTBUFSIZE 100

namespace tlp {

Graphic::Graphic(const QPoint &position, SpreadTable *parent) :
	QWidget(parent->viewport()), tableWidget(parent), isClicked(false) {
	move(position.x() - 9, position.y() - 9);
	resize(10, 10);
}

bool Graphic::isActiv() {
	return isClicked;
}

void Graphic::paintEvent(QPaintEvent*) {
	QPainter painter(this);
	painter.setPen(Qt::black);
	QBrush brush(Qt::black);
	painter.fillRect(0, 0, 10, 10, brush);
}

void Graphic::mousePressEvent(QMouseEvent *) {
	isClicked = true;
}

void Graphic::mouseReleaseEvent(QMouseEvent *event) {
	tableWidget->mouseReleaseEvent(event);
	isClicked = false;
}

SpreadTable::SpreadTable(QWidget *parent, SpreadView* view) :
	QTableWidget(parent), selectZone(NULL), verticalBufferBegin(0), linkedView(view) {
	undoStack = new QUndoStack(this);
	setSelectionMode(QAbstractItemView::ContiguousSelection);
	setItemPrototype(new SpreadCell);
	SpreadItemDelegate *itemDelegate = new SpreadItemDelegate(QTableWidget::itemDelegate());
	setItemDelegate(itemDelegate);
	connect(itemDelegate, SIGNAL(dataChangedSignal(int, int, QString&, QString&)), this,
			SIGNAL(userChangedItemDataSignal(int, int, QString&, QString&)));
	connect(this, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(someThingChanged()));
	connect(this, SIGNAL(userChangedItemDataSignal(int, int, QString&, QString&)), this,
			SLOT(userChangedItemDataSlot(int, int, QString&, QString&)));
	connect((QObject*) verticalScrollBar(), SIGNAL(valueChanged(int)), this,
			SLOT(verticalScroll(int)));
}

void SpreadTable::setView(View view) {
	this->view = view;
}

///===================================================================

void SpreadTable::setGraph(Graph *graph) {
	this->graph = graph;
	updateHeaders();
	verticalBufferBegin = 0;
	reloadView();
}

void SpreadTable::addRow(unsigned int) {
	unsigned int numberOfRow;
	if (view == NodesView)
		numberOfRow = graph->numberOfNodes();
	else
		numberOfRow = graph->numberOfEdges();
	setRowCount(numberOfRow);

	reloadView();
}

void SpreadTable::delRow(unsigned int) {
	unsigned int numberOfRow;
	if (view == NodesView)
		numberOfRow = graph->numberOfNodes();
	else
		numberOfRow = graph->numberOfEdges();
	setRowCount(numberOfRow - 1);

	reloadView();
}
void SpreadTable::updateHeaders() {
	QStringList horizontalHeaderLabels;

	vector<string> selectedProperties;
	if (linkedView == NULL) {
		if (linkedView == NULL) {
			Iterator<std::string> *it = graph->getLocalProperties();
			while (it->hasNext()) {
				selectedProperties.push_back(it->next());
			}
			delete it;
			it = graph->getInheritedProperties();
			while (it->hasNext()) {
				selectedProperties.push_back(it->next());
			}
			delete it;
		}
	} else
		selectedProperties = linkedView->getSelectedProperties();

	int numberOfProp = selectedProperties.size();

	setColumnCount(numberOfProp);

	unsigned int numberOfRow;
	if (view == NodesView)
		numberOfRow = graph->numberOfNodes();
	else
		numberOfRow = graph->numberOfEdges();
	setRowCount(numberOfRow);

	adjustHorizontalHeader();

	for (vector<string>::iterator it = selectedProperties.begin(); it != selectedProperties.end(); ++it) {
	  horizontalHeaderLabels.append(QString::fromUtf8((*it).c_str()));
	}

	setHorizontalHeaderLabels(horizontalHeaderLabels);

}
void SpreadTable::reloadView() {

	int currentPropId = 0;

	vector<string> selectedProperties;
	if (linkedView == NULL) {
		if (linkedView == NULL) {
			Iterator<std::string> *it = graph->getLocalProperties();
			while (it->hasNext()) {
				selectedProperties.push_back(it->next());
			}
			delete it;
			it = graph->getInheritedProperties();
			while (it->hasNext()) {
				selectedProperties.push_back(it->next());
			}
			delete it;
		}
	} else
		selectedProperties = linkedView->getSelectedProperties();

	unsigned int numberOfRow;
	if (view == NodesView)
		numberOfRow = graph->numberOfNodes();
	else
		numberOfRow = graph->numberOfEdges();

	setRowCount(numberOfRow);

	clearContents();

	ColorProperty *colorProperty;
	colorProperty = graph->getProperty<ColorProperty> ("viewColor");
	ColorProperty *labelColorProperty;
	labelColorProperty = graph->getProperty<ColorProperty> ("viewLabelColor");
	BooleanProperty *selectionProperty;
	selectionProperty = graph->getProperty<BooleanProperty> ("viewSelection");

	//Iterator<std::string> *it = graph->getLocalProperties();

	for (vector<string>::iterator it = selectedProperties.begin(); it != selectedProperties.end(); ++it) {
		//while (it->hasNext()) {
		//string propName = it->next();
		string propName = *it;

		PropertyInterface *currentProperty = graph->getProperty(propName);
    Iterator<node> *itN=graph->getNodes();
    Iterator<edge> *itE=graph->getEdges();
    for(unsigned int i=0;i<verticalBufferBegin;++i){
      if (view == NodesView)
        itN->next();
      else
        itE->next();
    }
		for (unsigned int i = verticalBufferBegin; i < verticalBufferBegin + TABLEHEIGHTBUFSIZE
				&& i < numberOfRow; ++i) {
			SpreadCell *curCell = new SpreadCell;
      node n;
      edge e;

      if(view == NodesView)
        n=itN->next();
      else
        e=itE->next();

      if (colorProperty) {
				Color color;
				Color labelColor;
				if (view == NodesView) {
          if (!selectionProperty->getNodeValue(n))
            color = colorProperty->getNodeValue(n);
					else
						color = Color(255, 102, 255, 255);

          labelColor = labelColorProperty->getNodeValue(n);
				} else {
          if (!selectionProperty->getEdgeValue(e))
            color = colorProperty->getEdgeValue(e);
					else
						color = Color(255, 102, 255, 255);

          labelColor = labelColorProperty->getEdgeValue(e);
				}
				curCell->setBackground(QBrush(QColor(color[0], color[1], color[2], color[3])));
				curCell->setForeground(QBrush(QColor(labelColor[0], labelColor[1], labelColor[2],
						labelColor[3])));
			}
			if (view == NodesView)
        curCell->setData(Qt::EditRole, QString::fromUtf8(currentProperty->getNodeStringValue(n).c_str()));
			else
        curCell->setData(Qt::EditRole, QString::fromUtf8(currentProperty->getEdgeStringValue(e).c_str()));
			setItem(i, currentPropId, curCell);
		}
		currentPropId++;
  }
}

void SpreadTable::loadCell(int minRow, int maxRow, int minColumn, int maxColumn) {
	int i;
	Iterator<std::string> *it = graph->getLocalProperties();
	for (i = 0; i <= minColumn; ++i) {
		it->next();
	}
	for (int i = minColumn; i <= maxColumn; ++i) {
		string propName = it->next();
		PropertyInterface *currentProperty = graph->getProperty(propName);
		for (unsigned int j = minRow; j <= maxRow; ++j) {
			SpreadCell *curCell = new SpreadCell;
			if (view == NodesView)
				curCell->setData(Qt::EditRole, currentProperty->getNodeStringValue(node(j)).c_str());
			else
				curCell->setData(Qt::EditRole, currentProperty->getEdgeStringValue(edge(j)).c_str());
			setItem(j, i, curCell);
		}
	}
	delete it;
}

void SpreadTable::verticalScroll(int id) {
	if (id < verticalBufferBegin && verticalBufferBegin != 0) {
		if (id - TABLEHEIGHTBUFSIZE / 4 < 0)
			verticalBufferBegin = 0;
		else
			verticalBufferBegin = id - TABLEHEIGHTBUFSIZE / 4;
		reloadView();
	}

	unsigned int numberOfRow;
	if (view == NodesView)
		numberOfRow = graph->numberOfNodes();
	else
		numberOfRow = graph->numberOfEdges();

	if (id > verticalBufferBegin + TABLEHEIGHTBUFSIZE / 2 && verticalBufferBegin != numberOfRow
			- TABLEHEIGHTBUFSIZE) {
		verticalBufferBegin = id - TABLEHEIGHTBUFSIZE / 4;
		if (verticalBufferBegin > numberOfRow - TABLEHEIGHTBUFSIZE)
			verticalBufferBegin = numberOfRow - TABLEHEIGHTBUFSIZE;
		reloadView();
	}
}

///===================================================================

void SpreadTable::userChangedItemDataSlot(int row, int column, QString& oldData, QString& newData) {
	getUndoStack() -> push(new DataChanged(this, row, column, oldData, newData));
}

void SpreadTable::newSheet(int rowCount, int columnCount) {
	setRowCount(0);
	setColumnCount(0);
	setRowCount(rowCount);
	setColumnCount(columnCount);
	adjustHorizontalHeader();
	undoStack -> clear();
}

bool SpreadTable::writeSheet(const QString &fileName) {
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly)) {
		QMessageBox::warning(this, tr("Saving Failed"), tr("Cannot write to file %1:\n%2") .arg(
				fileName).arg(file.errorString()));
		return false;
	}

	setCursor(Qt::WaitCursor);

	QDataStream outStream(&file);
	outStream.setVersion(QDataStream::Qt_4_2);

	int rows = rowCount();
	int columns = columnCount();

	outStream << quint32(MagicWord);
	outStream << qint16(rows) << qint16(columns);

	QTableWidgetItem *curItem;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			curItem = item(i, j);
			if (curItem) {
				SpreadCell *curCell = static_cast<SpreadCell*> (curItem);
				outStream << qint16(i) << qint16(j) << *curCell;
			}
		}
	}

	unsetCursor();
	return true;
}

bool SpreadTable::readSheet(const QString &fileName) {
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly)) {
		QMessageBox::warning(this, tr("Reading Failed"), tr("Cannot read the file %1 :\n%2") .arg(
				fileName).arg(file.errorString()));
		return false;
	}

	QDataStream inStream(&file);
	inStream.setVersion(QDataStream::Qt_4_2);

	quint32 magicWord;
	inStream >> magicWord;
	if (magicWord != MagicWord) {
		QMessageBox::warning(this, tr("Bad File"), tr("Unknown file format!"));
		return false;
	}

	setCursor(Qt::WaitCursor);

	qint16 row, column;
	QString text;
	inStream >> row >> column;
	int rowNumber = 1000;
	int columnNumber = 1000;
	newSheet(rowNumber, columnNumber);

	for (int i = 0; i < rowNumber; i++) {
		for (int j = 0; j < columnNumber; ++j) {
			SpreadCell *curCell = new SpreadCell;
			curCell->setData(Qt::EditRole, "1");
			setItem(i, j, curCell);
		}
	}
	/*while(!file.atEnd())
	 {
	 SpreadCell *curCell = new SpreadCell;
	 inStream >> row;
	 if (row == -1)
	 //fetching rows and cells are finished, now start fetching graphics.
	 break;
	 Stream >> column >> *curCell;
	 setItem(row, column, curCell);
	 }

	 while(!file.atEnd())
	 {
	 qint16 graphicType;
	 inStream >> graphicType;
	 Graphic *newGraphic = createGraphic(graphicType);
	 newGraphic -> in(inStream);
	 addGraphic(newGraphic);
	 }*/

	unsetCursor();
	return true;
}

///===================================================================

QString SpreadTable::getItemText(int row, int column, int role) const {
	QTableWidgetItem *curItem = item(row, column);
	if (curItem)
		return curItem -> data(role).toString();
	else
		return "";
}

QString SpreadTable::getCurrentItemText(int role) const {
	return getItemText(currentRow(), currentColumn(), role);
}

QString SpreadTable::getItemsText(const QTableWidgetSelectionRange &range, int role) const {
	QString str;

	for (int i = range.topRow(); i <= range.bottomRow(); i++) {
		if (i != range.topRow())
			str += '\n';
		for (int j = range.leftColumn(); j <= range.rightColumn(); j++) {
			if (j != range.leftColumn())
				str += '\t';
			str += getItemText(i, j, role);
		}
	}
	return str;
}

bool SpreadTable::setItemText(int row, int column, const QString &text) {
	QTableWidgetItem *curItem = createGetItem(row, column);
	lastCell = curItem;
	lastFormule = text;
	QString result = text;
	result = ((SpreadCell*) curItem)->computeValue(result);

	PropertyInterface *currentProperty = graph->getProperty(
								string(horizontalHeaderItem(column)->text().toUtf8().data()));

	bool changementAccepted;
	if (view == NodesView)
	  changementAccepted = currentProperty->setNodeStringValue(node(row), string(result.toUtf8().data()));
	else
	  changementAccepted = currentProperty->setEdgeStringValue(edge(row), string(result.toUtf8().data()));

	//cout << "Set text : " << horizontalHeaderItem(column)->text().toStdString() << endl;
	if (!changementAccepted)
		curItem->setText("ERROR");
	return true;
}

QByteArray SpreadTable::getItemInByteArray(int row, int column) const {
	QByteArray byteArray;
	QDataStream outStream(&byteArray, QIODevice::WriteOnly);
	SpreadCell *cell = static_cast<SpreadCell*> (item(row, column));
	if (cell)
		outStream << *cell;
	return byteArray;
}

QByteArray SpreadTable::getItemsInByteArray(const QTableWidgetSelectionRange &range) const {
	QByteArray byteArray;
	QDataStream outStream(&byteArray, QIODevice::WriteOnly);

	outStream << range.rowCount() << range.columnCount();
	for (int i = range.topRow(); i <= range.bottomRow(); i++)
		for (int j = range.leftColumn(); j <= range.rightColumn(); j++)
			outStream << getItemInByteArray(i, j);
	return byteArray;
}

int SpreadTable::getItemEditor(int row, int column) const {
	QTableWidgetItem *curItem = item(row, column);
	if (!curItem)
		return 0;
	else
		return curItem -> data(EditorRole).toInt();
}

bool SpreadTable::setItemEditor(int row, int column, int editor) {
	QTableWidgetItem *curItem = createGetItem(row, column);
	curItem -> setData(EditorRole, editor);
	return true;
}

QString SpreadTable::getCurrentItemLocation() const {
	QString location;
	encodePosition(location, currentRow(), currentColumn());
	return location;
}

QFont SpreadTable::getItemFont(int row, int column) const {
	QTableWidgetItem *curItem = item(row, column);
	if (!curItem)
		return QFont();
	else
		return curItem -> data(Qt::FontRole).value<QFont> ();
}

void SpreadTable::setItemFont(int row, int column, const QFont &font) {
	createGetItem(row, column) -> setData(Qt::FontRole, font);
}

int SpreadTable::getItemAlignment(int row, int column) const {
	QTableWidgetItem *curItem = item(row, column);
	if (!curItem)
		return (Qt::AlignLeft | Qt::AlignVCenter);
	else
		return curItem -> data(Qt::TextAlignmentRole).toInt();
}

void SpreadTable::setItemAlignment(int row, int column, int alignment) {
	createGetItem(row, column) -> setData(Qt::TextAlignmentRole, alignment);
}

QBrush SpreadTable::getItemBackground(int row, int column) const {
	QTableWidgetItem *curItem = item(row, column);
	if (!curItem)
		return QBrush(QColor(Qt::white));
	else {
		QBrush brush = curItem -> data(Qt::BackgroundRole).value<QBrush> ();
		return brush;
	}
}

void SpreadTable::setItemBackground(int row, int column, const QBrush &brush) {
	createGetItem(row, column) -> setData(Qt::BackgroundRole, brush);
}

QBrush SpreadTable::getItemForeground(int row, int column) const {
	QTableWidgetItem *curItem = item(row, column);
	if (!curItem)
		return QBrush(QColor(Qt::black));
	else
		return curItem -> data(Qt::ForegroundRole).value<QBrush> ();
}

void SpreadTable::setItemForeground(int row, int column, const QBrush &brush) {
	createGetItem(row, column) -> setData(Qt::ForegroundRole, brush);
}

///===================================================================

void SpreadTable::findNext(QString &text, Qt::CaseSensitivity cs, int findIn) {
	if (text.isEmpty())
		return;

	int initialRow = currentRow();
	int initialColumn = currentColumn();
	int i = initialRow;
	int j = initialColumn;
	bool found = false;

	do {
		j++;
		if (j >= columnCount()) {
			j = 0;
			i++;
		}
		if (i >= rowCount())
			i = 0;

		QString itemText = findIn == FindInFormula ? getItemText(i, j, Qt::EditRole) : getItemText(
				i, j);
		if (itemText.contains(text, cs)) {
			found = true;
			break;
		}
	} while (i != initialRow || j != initialColumn);

	if (found)
		setCurrentCell(i, j);
}

void SpreadTable::findPrevious(QString &text, Qt::CaseSensitivity cs, int findIn) {
	if (text.isEmpty())
		return;

	int initialRow = currentRow();
	int initialColumn = currentColumn();
	int i = initialRow;
	int j = initialColumn;
	bool found = false;

	do {
		j--;
		if (j < 0) {
			j = columnCount() - 1;
			i--;
		}
		if (i < 0)
			i = rowCount() - 1;

		QString itemText = findIn == FindInFormula ? getItemText(i, j, Qt::EditRole) : getItemText(
				i, j);
		if (itemText.contains(text, cs)) {
			found = true;
			break;
		}
	} while (i != initialRow || j != initialColumn);

	if (found)
		setCurrentCell(i, j);
}

///===================================================================

void SpreadTable::sort(SpreadTableSort tableSort) {
	QTableWidgetSelectionRange range = tableSort.getRange();
	QList<int> rowsOrColumns; //TODO: can we do it without QList to get higher speed?

	if (tableSort.getSortDirection() == SpreadTableSort::SortRows)
		for (int i = range.topRow(); i <= range.bottomRow(); i++)
			rowsOrColumns.append(i);
	else
		for (int j = range.leftColumn(); j <= range.rightColumn(); j++)
			rowsOrColumns.append(j);

	tableSort.setTable(this);
	qStableSort(rowsOrColumns.begin(), rowsOrColumns.end(), tableSort);

	QTableWidgetItem *cells[range.rowCount()][range.columnCount()];
	for (int i = 0; i < range.rowCount(); i++)
		for (int j = 0; j < range.columnCount(); j++)
			cells[i][j] = takeItem(i + range.topRow(), j + range.leftColumn());

	if (tableSort.getSortDirection() == SpreadTableSort::SortRows) {
		for (int i = 0; i < range.rowCount(); i++)
			for (int j = 0; j < range.columnCount(); j++)
				setItem(i + range.topRow(), j + range.leftColumn(), cells[rowsOrColumns[i]
						- range.topRow()][j]);
	} else {
		for (int i = 0; i < range.rowCount(); i++)
			for (int j = 0; j < range.columnCount(); j++)
				setItem(i + range.topRow(), j + range.leftColumn(), cells[i][rowsOrColumns[j]
						- range.leftColumn()]);
	}
}

///===================================================================

void SpreadTable::insertRows(int position, int amount) {
	for (int i = 0; i < amount; i++)
		insertRow(position);

	QTableWidgetSelectionRange
			selectionRange(position, 0, position + amount - 1, columnCount() - 1);
	clearSelection();
	setRangeSelected(selectionRange, true);

	someThingChanged();
}

void SpreadTable::insertColumns(int, int) {
	InsertColumnDialog *dialog = new InsertColumnDialog(this);
	dialog->exec();

	string propertyName = dialog->getName();

	if (dialog->getType() == "boolean")
		graph->getProperty<BooleanProperty> (propertyName);
	else if (dialog->getType() == "integer")
		graph->getProperty<IntegerProperty> (propertyName);
	else if (dialog->getType() == "double")
		graph->getProperty<DoubleProperty> (propertyName);
	else if (dialog->getType() == "string")
		graph->getProperty<StringProperty> (propertyName);
	else if (dialog->getType() == "layout")
		graph->getProperty<LayoutProperty> (propertyName);
	else if (dialog->getType() == "color")
		graph->getProperty<ColorProperty> (propertyName);
	else
		assert(false);

	setGraph(graph);
}

///===================================================================

void SpreadTable::selectAll() {
	clearSelection();
	QTableWidget::selectAll();
}

void SpreadTable::selectCurrentRow() {
	selectRow(currentRow());
}

void SpreadTable::selectCurrentColumn() {
	selectColumn(currentColumn());
}

///===================================================================

void SpreadTable::cut(QMimeData &mimeData) {
	copy(mimeData);
	deleteContents(getSelectedRange());
}

void SpreadTable::copy(QMimeData &mimeData) {
	QTableWidgetSelectionRange range = getSelectedRange();
	mimeData.setData("Cell", getItemsInByteArray(range));
}

bool SpreadTable::paste(const QMimeData &mimeData, int row, int column) {
	if (mimeData.hasFormat("Cell")) {
		QByteArray byteArray = mimeData.data("Cell");
		QDataStream inStream(&byteArray, QIODevice::ReadOnly);
		SpreadCell *currentCell;
		int pasteDataRowCount;
		int pasteDataColumnCount;

		if (!inStream.atEnd())
			inStream >> pasteDataRowCount >> pasteDataColumnCount;
		if (pasteDataRowCount + row > rowCount() || pasteDataColumnCount + column > columnCount())
			return false;

		for (int i = 0; i < pasteDataRowCount; i++) {
			for (int j = 0; j < pasteDataColumnCount && !inStream.atEnd(); j++) {
				currentCell = new SpreadCell;
				QByteArray cellByteArray;
				inStream >> cellByteArray;
				if (!cellByteArray.isEmpty()) {
					QDataStream cellInStream(&cellByteArray, QIODevice::ReadOnly);
					cellInStream >> *currentCell;
				}
				setItem(row + i, column + j, currentCell);
			}
		}
		QTableWidgetSelectionRange selectionRange(row, column, row + pasteDataRowCount - 1, column
				+ pasteDataColumnCount - 1);
		clearSelection();
		setRangeSelected(selectionRange, true);
	} else if (mimeData.hasText()) {
		QString str = mimeData.text();
		int pasteContentRows = str.count('\n') + 1;
		int pasteContentColumns = str.count('\t') / pasteContentRows + 1;

		if (row + pasteContentRows > rowCount() || column + pasteContentColumns > columnCount())
			return false;

		QStringList sentences = str.split('\n');
		for (int i = 0; i < pasteContentRows; i++) {
			QStringList words = sentences[i].split('\t');
			for (int j = 0; j < pasteContentColumns; j++) {
				if (words[j].isEmpty() && !item(row + i, column + j))
					continue;
				setItemText(row + i, column + j, words[j]);
			}
		}
		QTableWidgetSelectionRange selectionRange(row, column, row + pasteContentRows - 1, column
				+ pasteContentColumns - 1);
		clearSelection();
		setRangeSelected(selectionRange, true);

	} else
		return false;

	return true;
}

///===================================================================

void SpreadTable::deleteContents(const QTableWidgetSelectionRange &range) {
	for (int i = range.topRow(); i <= range.bottomRow(); i++) {
		for (int j = range.leftColumn(); j <= range.rightColumn(); j++) {
			QTableWidgetItem *curItem = item(i, j);
			if (curItem)
				delete curItem;
			//      curItem = 0;  //NOTE is it needed?
		}
	}
	someThingChanged();
}

void SpreadTable::deleteEntireRows(int position, int amount) {
	for (int i = 0; i < amount; i++)
		removeRow(position);
	clearSelection();
	someThingChanged();
}

void SpreadTable::deleteEntireColumns(int position, int amount) {
	for (int i = 0; i < amount; i++)
		removeColumn(position);
	clearSelection();
	adjustHorizontalHeader();
	someThingChanged();
}

///===================================================================

void SpreadTable::encodePosition(QString &text, int column) {
	text.clear();
	QChar letter;

	letter = 'A' + column % 26;
	text.insert(0, letter);

	if (column >= 26) {
		column = column / 26;
		do {
			column--;
			QChar letter('A' + column % 26);
			text.insert(0, letter);
			column = column / 26;
		} while (column > 0);
	}
}

void SpreadTable::encodePosition(QString &text, int row, int column) {
	encodePosition(text, column);
	text += QString::number(row + 1);
}

void SpreadTable::encodeRange(QString &text, QTableWidgetSelectionRange range) {
	QString position;
	encodePosition(position, range.topRow(), range.leftColumn());
	text = position;
	text += ':';
	encodePosition(position, range.bottomRow(), range.rightColumn());
	text += position;
}

bool SpreadTable::decodePosition(QString text, int &row, int &column) {
	text = text.toUpper();
	QRegExpValidator validator(QRegExp("[A-Z]+[1-9][0-9]*"), 0);
	int pos = 0;
	if (validator.validate(text, pos) != QValidator::Acceptable)
		return false;

	row = column = 0;
	int letterCount = text.indexOf(QRegExp("[1-9]")); //returns the position of first digit (is equal to number of letters)

	for (int i = 0; i < letterCount; i++)
		column += static_cast<int> (std::pow(26.0, letterCount - i - 1)) * (text[i].unicode() - 'A'
				+ 1);
	column--;

	for (int j = letterCount; j < text.length(); j++)
		row = row * 10 + text[j].unicode() - '0';
	row--;

	return true;
}

bool SpreadTable::decodeRange(QString text, QTableWidgetSelectionRange &range) {
	int topRow, bottomRow, leftColumn, rightColumn;
	text = text.toUpper();

	int colonPosition = text.indexOf(':');
	if (colonPosition == -1)
		return false;
	if (!decodePosition(text.mid(0, colonPosition), topRow, leftColumn))
		return false;
	if (!decodePosition(text.mid(colonPosition + 1), bottomRow, rightColumn))
		return false;

	if (topRow > bottomRow) {
		int temp = topRow;
		topRow = bottomRow;
		bottomRow = temp;
	}
	if (leftColumn > rightColumn) {
		int temp = leftColumn;
		leftColumn = rightColumn;
		rightColumn = temp;
	}

	QTableWidgetSelectionRange r(topRow, leftColumn, bottomRow, rightColumn);
	range = r;
	return true;
}

///===================================================================

QUndoStack* SpreadTable::getUndoStack() const {
	return undoStack;
}

void SpreadTable::someThingChanged() {
	viewport() -> update();
	emit modified();
}

void SpreadTable::redrawView() {
	updateHeaders();
	reloadView();
}

QTableWidgetSelectionRange SpreadTable::getSelectedRange() {
	QList<QTableWidgetSelectionRange> range = selectedRanges();
	if (range.isEmpty())
		return QTableWidgetSelectionRange();
	return range.first();
}

void SpreadTable::adjustHorizontalHeader() {
	int columns = columnCount();
	QTableWidgetItem *item;
	for (int i = 0; i < columns; i++) {
		QString text;
		encodePosition(text, i);
		item = new QTableWidgetItem(text);
		setHorizontalHeaderItem(i, item);
	}
}

QTableWidgetItem* SpreadTable::createGetItem(int row, int column) {
	if (row >= rowCount() || column >= columnCount())
		return 0;
	QTableWidgetItem *curItem = item(row, column);
	if (!curItem) {
		curItem = new SpreadCell;
		setItem(row, column, curItem);
	}
	return curItem;
}

void SpreadTable::paintEvent(QPaintEvent *event) {
	/*   for(int i=0; i<graphics.size(); i++) //NOTE: is it needed?
	 graphics[i] -> update();*/
	QTableWidget::paintEvent(event);
}

void SpreadTable::mousePressEvent(QMouseEvent *event) {
	if ((event->modifiers() & Qt::ControlModifier) != 0) {
		insertColumns(column(itemAt(event->pos())), 1);
	} else {
		QTableWidget::mousePressEvent(event);

		firstSelectedItem = itemAt(event->pos());
		firstSelectedItemRow = row(firstSelectedItem);
		firstSelectedItemColumn = column(firstSelectedItem);

		/*if(selectZone)
		 delete selectZone;
		 selectZone=new Graphic(visualItemRect(itemAt(event->pos())).bottomRight(),this);
		 selectZone->show();*/
	}
}

void SpreadTable::mouseReleaseEvent(QMouseEvent *event) {
	QTableWidget::mouseReleaseEvent(event);
	if (selectZone) {
		if (selectZone->isActiv()) {
			Observable::holdObservers();

			//Selection :
			QList<QTableWidgetSelectionRange> ranges = selectedRanges();
			assert(ranges.size()==1);
			if (firstSelectedItemRow < verticalBufferBegin) {
				loadCell(firstSelectedItemRow, verticalBufferBegin - 1, ranges[0].leftColumn(),
						ranges[0].rightColumn());
			}
			if (firstSelectedItemRow > verticalBufferBegin + TABLEHEIGHTBUFSIZE) {
				loadCell(verticalBufferBegin + TABLEHEIGHTBUFSIZE, firstSelectedItemRow,
						ranges[0].leftColumn(), ranges[0].rightColumn());
			}

			QTableWidgetItem *beginCell = lastCell;
			QString beginFormula = lastFormule;
			if (ranges[0].topRow() >= firstSelectedItemRow) {
				for (int i = firstSelectedItemRow; i <= ranges[0].bottomRow(); ++i) {
					for (int j = ranges[0].leftColumn(); j <= ranges[0].rightColumn(); ++j) {
						if (firstSelectedItem == beginCell)
							setItemText(i, j, SpreadCalculator::modifyPosition(beginFormula,
									firstSelectedItemRow, firstSelectedItemColumn, i, j));
						else
							setItemText(i, j,
									item(firstSelectedItemRow, firstSelectedItemColumn)->text());
					}
				}
			} else {
				for (int i = ranges[0].topRow(); i <= firstSelectedItemRow; ++i) {
					for (int j = ranges[0].leftColumn(); j <= ranges[0].rightColumn(); ++j) {
						if (firstSelectedItem == beginCell)
							setItemText(i, j, SpreadCalculator::modifyPosition(beginFormula,
									firstSelectedItemRow, firstSelectedItemColumn, i, j));
						else
							setItemText(i, j,
									item(firstSelectedItemRow, firstSelectedItemColumn)->text());
					}
				}
			}

			if (selectZone)
				delete selectZone;
			selectZone = NULL;
			Observable::unholdObservers();
		}
	}
	if (!selectedItems().empty())
		emit mouseReleasedSignal(this, selectedItems()[0]);
	else
		emit mouseReleasedSignal(this, NULL);
}

void SpreadTable::setLinkedSpreadView(SpreadView* linkedView) {
	this->linkedView = linkedView;
}

///===================================================================
///===================================================================

SpreadTableSort::SpreadTableSort(const QTableWidgetSelectionRange &r, const QList<int> &sk,
		const QList<bool> &a, SortDirection sd, Qt::CaseSensitivity cs, const QString &co) :
	range(r), sortKeys(sk), ascending(a), sortDirection(sd), caseSensitivity(cs) {
	table = 0;
	if (!co.isEmpty())
		customOrder = co.split(QRegExp("\\s*,\\s*"));
}

bool SpreadTableSort::operator()(int firstPosition, int secondPosition) const {
	if (!table)
		return false;

	int comparisonResult = 0;
	for (int i = 0; i < sortKeys.size() && comparisonResult == 0; i++) {
		QString firstString;
		QString secondString;
		if (sortDirection == SortRows) {
			firstString = table->getItemText(firstPosition, sortKeys[i]);
			secondString = table->getItemText(secondPosition, sortKeys[i]);
		} else {
			firstString = table->getItemText(sortKeys[i], firstPosition);
			secondString = table->getItemText(sortKeys[i], secondPosition);
		}
		comparisonResult = compare(firstString, secondString);
		if (!ascending[i])
			comparisonResult *= -1;
	}

	return comparisonResult < 0;
}

int SpreadTableSort::compare(const QString &firstString, const QString &secondString) const {
	int comparisonResult = 0;
	if (customOrder.isEmpty())
		comparisonResult = firstString.compare(secondString, caseSensitivity);
	else {
		int indexOfFirstString = customOrder.indexOf(QRegExp(firstString, caseSensitivity));
		int indexOfSecondString = customOrder.indexOf(QRegExp(secondString, caseSensitivity));
		if (indexOfFirstString == indexOfSecondString)
			;
		else if (indexOfFirstString == -1 && indexOfSecondString != -1)
			comparisonResult = 1;
		else if (indexOfFirstString != -1 && indexOfSecondString == -1)
			comparisonResult = -1;
		else
			comparisonResult = indexOfFirstString < indexOfSecondString ? -1 : 1;
	}

	return comparisonResult;
}

}

