#ifndef TULIPTABLEWIDGETCOLUMNSELECTIONWIDGET_H
#define TULIPTABLEWIDGETCOLUMNSELECTIONWIDGET_H

#include <QtGui/QWidget>
#include <QtCore/QRegExp>
namespace Ui {
class TableColumnSelectionWidget;
}

class TulipTableWidgetColumnSelectionModel;

/**
  * @brief Widget used to display and hide columns in a Tulip table widget.
  **/
class TulipTableWidgetColumnSelectionWidget : public QWidget {
  Q_OBJECT

public:
  explicit TulipTableWidgetColumnSelectionWidget(QWidget *parent = NULL);
  ~TulipTableWidgetColumnSelectionWidget();
  /**
  * @brief Set the column selection model. Do not delete models it's up to the creator of the model to destruct it.
    *
    **/
  void setColumnSelectionModel(TulipTableWidgetColumnSelectionModel* model);

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

  void showCustomPatternProperties(const QString& pattern);

  void showContextMenu(const QPoint& position);

  void showSelectedColumns();
  void showOnlySelectedColumns();
  void hideSelectedColumns();
  void createNewColumn();
  void copyColumn();
  void deleteSelectedColumns();

private:
  Ui::TableColumnSelectionWidget *ui;
  TulipTableWidgetColumnSelectionModel *_tableColumnModel;

  /**
    * @brief Display all the columns with a name that match (or not) the given regexp.
    * @param match Tell if the columns must match or not the regexp to be shown.
    **/
  void selectColumns(const QRegExp& regExp,bool match);
};

#endif // TULIPTABLEWIDGETCOLUMNSELECTIONWIDGET_H
