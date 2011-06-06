#include "HeaderSelectionDialog.h"
#include "ui_HeaderSelectionDialog.h"
#include <QtCore/QRegExp>
#include <QtGui/QHeaderView>
#include <QtGui/QTableView>
#include <vector>
using namespace std;

HeaderSelectionDialog::HeaderSelectionDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::HeaderSelectionDialog)
{
    ui->setupUi(this);    
}

HeaderSelectionDialog::~HeaderSelectionDialog()
{
    delete ui;
}

void HeaderSelectionDialog::setTableView(QTableView* tableView){
    ui->widget->setTableView(tableView);
}


void HeaderSelectionDialog::updateHeaders(QTableView* tableView,const QString& title,QWidget* parent){
    HeaderSelectionDialog *dialog = new HeaderSelectionDialog(parent);
    dialog->setWindowTitle(title);
    dialog->setTableView(tableView);
    QHeaderView *header = tableView->horizontalHeader();
    vector<bool> originalSectionState(header->count());
    for(int i = 0 ; i < header->count();++i){
        originalSectionState[i]=header->isSectionHidden(i);
    }
    if(dialog->exec() == QDialog::Rejected){
        for(int i = 0 ; i < header->count();++i){
            header->setSectionHidden(i,originalSectionState[i]);
        }
    }
}
