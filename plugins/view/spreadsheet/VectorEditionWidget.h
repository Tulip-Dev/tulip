#ifndef VECTOREDITIONWIDGET_H
#define VECTOREDITIONWIDGET_H

#include <QWidget>
#include <tulip/tulipconf.h>
#include <tulip/PropertyTypes.h>
#include "TulipQVariantBuilder.h"
#include <QtCore/QVariant>
#include <QtCore/QAbstractListModel>


namespace Ui {
    class VectorEditionWidget;
}


    class ListPropertyWidgetTypeMangerInterface{
    public:
        virtual ~ListPropertyWidgetTypeMangerInterface(){}
        virtual std::string getDefaultStringValue() const=0;
        virtual QVariant getValue(unsigned int i) = 0;
        virtual QVariant getStringValue(unsigned int i) = 0;
        virtual int getElementNumber()const = 0;
        virtual bool setValue(unsigned int i,QVariant v) = 0;

        virtual void insertRow() = 0;
        virtual void deleteRow(unsigned int i) = 0;
    };

    template<typename TYPECLASS>
    class ListPropertyWidgetTypeManger : public ListPropertyWidgetTypeMangerInterface {
    public:
        ListPropertyWidgetTypeManger<TYPECLASS>(const std::vector<typename TYPECLASS::RealType>& elements) : elements(elements){
        }

        virtual ~ListPropertyWidgetTypeManger(){
        }

        std::string getDefaultStringValue() const{
            return TYPECLASS::toString(TYPECLASS::defaultValue());
        }

        virtual int getElementNumber() const{
            return elements.size();
        }

        virtual QVariant getStringValue(unsigned int i) {
            assert(i < elements.size());
            return QVariant(QString::fromUtf8(TYPECLASS::toString( elements[i]).c_str()));
        }

        virtual  QVariant getValue(unsigned int i){
            assert(i < elements.size());
            QVariant v;
            v.setValue<typename TYPECLASS::RealType>(elements[i]);
            return v;
        }

        virtual bool setValue(unsigned int i, QVariant v){
            assert(i < elements.size());
            if(v.isValid()){
                elements[i]=v.value<typename TYPECLASS::RealType>();
                return true;
            }else{
                return false;
            }
        }

        virtual void insertRow(){
            elements.push_back(TYPECLASS::defaultValue());
        }
        virtual void deleteRow(unsigned int row){
            assert(row < elements.size());
            elements.erase(elements.begin()+row);
        }

        const std::vector<typename TYPECLASS::RealType>&  getResultValue(){
            return elements;
        }

    private:
        std::vector<typename TYPECLASS::RealType> elements;
    };

    template<>
    class ListPropertyWidgetTypeManger<tlp::StringType> : public ListPropertyWidgetTypeMangerInterface {
    public:
        ListPropertyWidgetTypeManger<tlp::StringType>(const std::vector<std::string>& elements) : elements(elements){
        }
        std::string getDefaultStringValue() const{
            return std::string();
        }

        virtual int getElementNumber() const{
            return elements.size();
        }

        virtual QVariant getStringValue(unsigned int i) {
            return getValue(i);
        }

        virtual  QVariant getValue(unsigned int i){
            assert(i < elements.size());
            QVariant v(QString::fromUtf8(elements[i].c_str()));
            return v;
        }

        virtual bool setValue(unsigned int i, QVariant data){
            assert(i < elements.size());
            if(data.isValid()){
                elements[i]=data.toString().toStdString();
                return true;
            }else{
                return false;
            }
        }

        virtual void insertRow(){
            elements.push_back(std::string());
        }
        virtual void deleteRow(unsigned int row){
            assert(row < elements.size());
            elements.erase(elements.begin()+row);
        }

        const std::vector<std::string>&  getResultValue(){
            return elements;
        }

    private:
        std::vector<std::string> elements;
    };

    class ListPropertyWidgetModel : public QAbstractListModel{

    public :
    ListPropertyWidgetModel(TulipPropertyType elementsType,ListPropertyWidgetTypeMangerInterface *typeManager, QWidget* parent = 0);
    virtual ~ListPropertyWidgetModel();
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    virtual bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex());
    virtual bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());

    ListPropertyWidgetTypeMangerInterface *getInterface(){
        return elements;
    }
      private:
    ListPropertyWidgetTypeMangerInterface *elements;
    TulipPropertyType elementsType;
};


    class VectorEditionWidget : public QWidget
    {
        Q_OBJECT

    public:
        explicit VectorEditionWidget(QWidget *parent = 0);
        ~VectorEditionWidget();
        ListPropertyWidgetTypeMangerInterface *getInterface();
        void setInterface(TulipPropertyType elementsType,ListPropertyWidgetTypeMangerInterface *interf);

    private slots:
        void addRow();
        void removeRows();
        void setAll();

    private:
        Ui::VectorEditionWidget *ui;
    };

#endif // VECTOREDITIONWIDGET_H
