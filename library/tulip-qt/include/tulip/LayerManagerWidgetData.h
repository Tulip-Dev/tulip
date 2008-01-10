#ifndef LAYERMANAGERWIDGETDATA_H
#define LAYERMANAGERWIDGETDATA_H

#include <qvariant.h>

#include <QtGui/QTreeWidget>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QStandardItemModel>
#include <QtGui/QHeaderView>
#include <QtGui/QVBoxLayout>

class Ui_LayerManagerWidgetData
{
public:
    QTreeWidget* treeWidget;

    void setupUi(QWidget *LayerManagerWidgetData)
    {
      QVBoxLayout *layout = new QVBoxLayout;
      LayerManagerWidgetData->setLayout(layout);
      if (LayerManagerWidgetData->objectName().isEmpty())
        LayerManagerWidgetData->setObjectName(QString::fromUtf8("LayerManagerWidgetData"));
      LayerManagerWidgetData->resize(300, 400);
      QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(5));
      sizePolicy.setHorizontalStretch(0);
      sizePolicy.setVerticalStretch(0);
      sizePolicy.setHeightForWidth(LayerManagerWidgetData->sizePolicy().hasHeightForWidth());
      LayerManagerWidgetData->setSizePolicy(sizePolicy);
      LayerManagerWidgetData->setMinimumSize(QSize(132, 132));
      treeWidget = new QTreeWidget(LayerManagerWidgetData);
      layout->addWidget(treeWidget);
      treeWidget->setObjectName(QString::fromUtf8("tableView"));
      treeWidget->setGeometry(QRect(0, 0, 450, 180));
      treeWidget->setColumnCount(3);
      treeWidget->header()->resizeSection(0,173);
      treeWidget->header()->resizeSection(1,50);
      treeWidget->header()->resizeSection(2,50);
      treeWidget->setHeaderLabels(QStringList() << "Name" << "Visible" << "stencil");
      
      retranslateUi(LayerManagerWidgetData);
      
      QMetaObject::connectSlotsByName(LayerManagerWidgetData);
    } // setupUi
    
    void retranslateUi(QWidget *LayerManagerWidgetData)
    {
    LayerManagerWidgetData->setWindowTitle(QApplication::translate("LayerManagerWidgetData", "Layer Manager", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(LayerManagerWidgetData);
    } // retranslateUi

};

namespace Ui {
    class LayerManagerWidgetData: public Ui_LayerManagerWidgetData {};
} // namespace Ui

class LayerManagerWidgetData : public QDialog, public Ui::LayerManagerWidgetData
{
    Q_OBJECT

public:
  LayerManagerWidgetData(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = 0);
  ~LayerManagerWidgetData();

public slots:
    virtual void updateView();
    virtual void backColor();

protected slots:
    virtual void languageChange();

};

#endif // LAYERMANAGERWIDGETDATA_H
