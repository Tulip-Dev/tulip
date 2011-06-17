#ifndef GRAPHTABLEWIDGET_H
#define GRAPHTABLEWIDGET_H
#include <QtGui/QTableView>
#include <tulip/Graph.h>

class GraphTableModel;

/**
  * @brief Table widget object providing convinience functions for handling graph objects.
  **/
class GraphTableWidget : public QTableView
{
    Q_OBJECT
public:
    //Selected : all the elements are selected.
    //Unselected : all the elements are unselected.
    //PartiallySelected : only a part of the elements are selected.
    enum SelectionStatus{Selected,Unselected,PartiallySelected};


    GraphTableWidget(QWidget* parent=NULL);
    virtual ~GraphTableWidget(){}

    void setGraph(tlp::Graph* graph,tlp::ElementType element);

    tlp::ElementType elementType()const{
        return _type;
    }

    tlp::Graph* graph()const{
        return _graph;
    }

    GraphTableModel* graphModel()const{
        return _tulipTableModel;
    }

    /**
      * @brief Compute the selection status for the elements list.
      **/
    SelectionStatus selectionStatus(const QModelIndexList& elements)const;

    /**
      * @brief Convert the index list to ids.
      **/
    std::set<unsigned int> indexListToIds(const QModelIndexList& elementsIndexes)const;

    void highlightElements(const std::set<unsigned int>& elementsToHighligh);

    /**
      * @brief Scrolls the view if necessary to show a maximum of elements in the list. Compute the element at the top left corner and move the view port to it's position.
      **/
    void scrollToFirstOf(const QModelIndexList& indexes);

    /**
      * @brief Convinience function that highlight all the elements in the list and move the viewport of the table view on the element at the top left corner.
      **/
    void highlightAndDisplayElements(const std::set<unsigned int>& elements);

protected:
    void setModel(QAbstractItemModel *){}

    tlp::Graph* _graph;
    tlp::ElementType _type;
    GraphTableModel* _tulipTableModel;



    void updateHorizontalHeaderVisibility(bool checked);


};

#endif // GRAPHTABLEWIDGET_H
