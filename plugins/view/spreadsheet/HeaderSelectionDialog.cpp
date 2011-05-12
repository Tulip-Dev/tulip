#include "headerselectiondialog.h"
#include "ui_headerselectiondialog.h"

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

void HeaderSelectionDialog::setHeaderView(QHeaderView* headerView){
    ui->listWidget->clear();
    for(int i=0;i<headerView->count();++i){
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        item->setText(headerView->model()->headerData(i,headerView->orientation()).toString());
        item->setData(Qt::UserRole,QVariant(i));
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(headerView->isSectionHidden(i)?Qt::Unchecked:Qt::Checked);
        ui->listWidget->addItem(item);
    }    
}

set<int> HeaderSelectionDialog::selectedIndexes()const{
    set<int> selected;
    for(int i = 0 ; i< ui->listWidget->count();++i){
        QListWidgetItem* item = ui->listWidget->item(i);
        if(item->checkState() == Qt::Checked){
            selected.insert(i);
        }
    }
    return selected;
}

void HeaderSelectionDialog::updateHeaders(QHeaderView* header,QWidget* parent){
    HeaderSelectionDialog *dialog = new HeaderSelectionDialog(parent);
    dialog->setHeaderView(header);
    if(dialog->exec() == QDialog::Accepted){
        set<int> selection = dialog->selectedIndexes();
        for(int i=0;i<header->count();++i){
            header->setSectionHidden(i,selection.find(i)==selection.end());
        }
    }
}
