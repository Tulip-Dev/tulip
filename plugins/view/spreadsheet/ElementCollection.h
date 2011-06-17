#ifndef ELEMENTCOLLECTION_H
#define ELEMENTCOLLECTION_H

#include <tulip/tulipconf.h>
#include <vector>
#include <QtCore/QAbstractListModel>
#include <QtCore/QMap>
#include <QtGui/QIcon>
#include <QtCore/QStringList>
#include <QtCore/QList>


    class ElementCollection : public QAbstractListModel
    {
        Q_OBJECT
        public:
        enum ElementCollectionType{SelectionType = 33};

            explicit ElementCollection(QObject *parent = 0);
            ElementCollection(const QStringList &strings, QObject *parent = 0);
            ElementCollection(const ElementCollection& other);

            int rowCount(const QModelIndex &parent = QModelIndex()) const;
            QVariant data(const QModelIndex &index, int role) const;
            bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

            Qt::ItemFlags flags(const QModelIndex &index) const;

            bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
            bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

            void addElement(const QString& label);

            void setAllElementSelection(bool selected);
            void setElementSelection(int element,bool selected);
            QList<int> selectedElement()const;

//            void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
        ElementCollection &operator=( const ElementCollection & );
        private:
            QList<QMap<int,QVariant> > _data;

    };

#endif // ELEMENTCOLLECTION_H
