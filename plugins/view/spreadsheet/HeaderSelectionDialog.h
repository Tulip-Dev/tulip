#ifndef HEADERSELECTIONDIALOG_H
#define HEADERSELECTIONDIALOG_H

#include <QDialog>
#include <set>
namespace Ui {
    class HeaderSelectionDialog;
}

class QTableView;
class HeaderSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HeaderSelectionDialog(QWidget *parent = 0);
    ~HeaderSelectionDialog();

    void setTableView(QTableView* tableView);

    static void updateHeaders(QTableView* tableView,const QString& title,QWidget* parent);

private:
    Ui::HeaderSelectionDialog *ui;

};

#endif // HEADERSELECTIONDIALOG_H
