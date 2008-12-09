#include "SpreadCell.h"

#include "SpreadCalculator.h"
#include "SpreadTable.h"

#include <iostream> //for debugging

using namespace std;

namespace tlp {

  QDataStream& operator<<(QDataStream &out, const SpreadCell &cell)
  {
    out << cell.data(Qt::TextAlignmentRole).toInt()
	<< cell.data(Qt::BackgroundRole).value<QBrush>()
	<< cell.data(Qt::ForegroundRole).value<QBrush>()
	<< cell.data(Qt::FontRole).value<QFont>()
	<< cell.data(SpreadTable::EditorRole).toInt()
	<< cell.data(Qt::EditRole).toString();
    return out;
  }

  QDataStream& operator>>(QDataStream &in, SpreadCell &cell)
  {
    int alignment;
    QBrush background;
    QBrush foreground;
    QFont font;
    int editor;
    QString string;
    in >> alignment >> background >> foreground >> font >> editor >> string;
    cell.setData(Qt::TextAlignmentRole, alignment);
    cell.setData(Qt::BackgroundRole, background);
    cell.setData(Qt::ForegroundRole, foreground);
    cell.setData(Qt::FontRole, font);
    cell.setData(SpreadTable::EditorRole, editor);
    cell.setData(Qt::EditRole, "toto");
    return in;
  }

  SpreadCell::SpreadCell()
    : QTableWidgetItem()
  {
    setFlags(Qt::ItemIsSelectable);
    setData(Qt::TextAlignmentRole, static_cast<int>(Qt::AlignLeft|Qt::AlignVCenter));
    setData(Qt::BackgroundRole, QBrush(QColor(Qt::white)));
    setData(Qt::ForegroundRole, QBrush(QColor(Qt::black)));
    editor = SpreadTable::TextLineEditor;
  }

  QTableWidgetItem* SpreadCell::clone() const
  {
    return new SpreadCell(*this);
  }

  QString SpreadCell::computeValue(QString &data) {
    if (data.startsWith("/=")){
	data.remove(0,1);
	return data;
    } else {
      return SpreadCalculator::calculator()->calculate(static_cast<SpreadTable*>(tableWidget()), data);
    }
  }

  QVariant SpreadCell::data(int role) const
  {
    if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
      {
	QString cellData = QTableWidgetItem::data(Qt::EditRole).toString();
	if (cellData.startsWith("/="))
	  {
	    cellData.remove(0,1);
	    return cellData;
	  }
	else
	  return SpreadCalculator::calculator() ->
	    calculate(static_cast<SpreadTable*>(tableWidget()), row(), column());
      }
    else if (role == SpreadTable::EditorRole)
      return editor;
    else
      return QTableWidgetItem::data(role);
  }

  void SpreadCell::setData(int role, const QVariant &newValue)
  {
    if (role == SpreadTable::EditorRole)
      editor = newValue.toInt();
    else
      QTableWidgetItem::setData(role, newValue);
  }

}
