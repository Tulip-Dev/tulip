#ifndef HEADERSELECTIONDIALOG_H
#define HEADERSELECTIONDIALOG_H

#include <QDialog>
#include <set>
namespace Ui {
    class HeaderSelectionDialog;
}

class QHeaderView;
class HeaderSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HeaderSelectionDialog(QWidget *parent = 0);
    ~HeaderSelectionDialog();

    void setHeaderView(QHeaderView* headerView);

    std::set<int> selectedIndexes()const;

    static void updateHeaders(QHeaderView* header,QWidget* parent);

private:
    Ui::HeaderSelectionDialog *ui;

private slots:
    void hideAll();
    void showAll();
};

#endif // HEADERSELECTIONDIALOG_H
