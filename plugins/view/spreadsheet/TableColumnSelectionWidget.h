#ifndef TABLECOLUMNSELECTIONWIDGET_H
#define TABLECOLUMNSELECTIONWIDGET_H

#include <QtGui/QWidget>
#include <QtCore/QRegExp>
class QTableView;
namespace Ui {
    class TableColumnSelectionWidget;
}

class TableViewColumnModel;

class TableColumnSelectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TableColumnSelectionWidget(QWidget *parent = 0);
    ~TableColumnSelectionWidget();
    void setTableView(QTableView* tableView);

private slots:

    /**
      * @brief Check or uncheck all properties.
      **/
    void checkUncheckAllStateChanged(int state);
    /**
      * @brief The selection is updated so update the check state of the button
      **/
    void updateCheckUncheckAllButtonState();

    void showVisualProperties();
    void showDataProperties();
    void showLocalProperties();
    void showInheritedProperties();
    void showPropertiesWithType();

    void showCustomPatternProperties(const QString& pattern);


private:
    Ui::TableColumnSelectionWidget *ui;
    TableViewColumnModel *_tableColumnModel;

    /**
      * @brief Display all the columns with a name that match (or not) the given regexp.
      * @param match Tell if the columns must match or not the regexp to be shown.
      **/
    void selectColumns(const QRegExp& regExp,bool match);
};

#endif // TABLECOLUMNSELECTIONWIDGET_H
