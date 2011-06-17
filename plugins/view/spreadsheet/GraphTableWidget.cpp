#include "GraphTableWidget.h"
#include <tulip/PropertyCreationDialog.h>
#include <tulip/CopyPropertyDialog.h>
#include <tulip/BooleanProperty.h>

#include <QtGui/QHeaderView>
#include <QtGui/QMenu>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QKeyEvent>

#include "GraphTableModel.h"
#include "TulipItemDelegate.h"

using namespace tlp;
using namespace std;
GraphTableWidget::GraphTableWidget(QWidget* parent):QTableView(parent),_graph(NULL),_type(NODE),_tulipTableModel(NULL)
{
    horizontalHeader()->setMovable(true);
}

void GraphTableWidget::setGraph(Graph* graph,ElementType element){
    _graph = graph;
    _type = element;
    _tulipTableModel = new GraphTableModel(graph,element,this);
    QTableView::setModel(_tulipTableModel);
    setItemDelegate(new TulipItemDelegate(this));    
}


bool GraphTableWidget::areAllElementsSelected(const QModelIndexList& elementsIndexes)const{
    BooleanProperty* selectionProperty = _graph->getProperty<BooleanProperty>("viewSelection");
    set<unsigned int> elements = indexListToIds(elementsIndexes);
    bool allSelected = true;
    for(set<unsigned int>::iterator it = elements.begin(); it != elements.end();++it){
        if(_type == NODE){
            if(!selectionProperty->getNodeValue(node(*it))){
                allSelected = false;
                break;
            }
        }else{
            if(!selectionProperty->getEdgeValue(edge(*it))){
                allSelected = false;
                break;
            }
        }
    }
    return allSelected;
}

set<unsigned int> GraphTableWidget::indexListToIds(const QModelIndexList& elementsIndexes)const{
    set<unsigned int> elements;
    for(QModelIndexList::const_iterator it = elementsIndexes.begin();it != elementsIndexes.end();++it){
        elements.insert(_tulipTableModel->idForIndex(*it));
    }
    return elements;
}


void GraphTableWidget::highlightElements(const std::set<unsigned int>& elementsToHighligh){
    QItemSelectionModel *itemSelectionModel = new QItemSelectionModel(_tulipTableModel);
    for(int i = 0 ; i < _tulipTableModel->rowCount() ; ++i){
        if(elementsToHighligh.find(_tulipTableModel->idForIndex(i))!=elementsToHighligh.end()){
            itemSelectionModel->select(_tulipTableModel->index(i,0),QItemSelectionModel::Select| QItemSelectionModel::Rows);
        }
    }
    QItemSelectionModel *oldSelectionModel = selectionModel();
    setSelectionModel(itemSelectionModel);
    oldSelectionModel->deleteLater();
}

void GraphTableWidget::scrollToFirstOf(const QModelIndexList& indexes){
    int xMin=INT_MAX;
    int yMin=INT_MAX;
    int rowMin = -1;
    int colMin = -1;
    for(QModelIndexList::const_iterator it = indexes.begin(); it != indexes.end();++it){
        QRect indexRect = visualRect(*it);
        if(indexRect.left() < xMin){
            xMin = indexRect.left();
            rowMin =( *it).row();
        }
        if(indexRect.top() < yMin){
            yMin = indexRect.top();
            colMin = (*it).column();
        }
    }
    if(rowMin != -1 && colMin != -1){
    QTableView::scrollTo(_tulipTableModel->index(rowMin,colMin),QAbstractItemView::PositionAtTop);
    }
}

void GraphTableWidget::highlightAndDisplayElements(const std::set<unsigned int>& elements){
    highlightElements(elements);
    scrollToFirstOf(selectionModel()->selectedIndexes());
}
