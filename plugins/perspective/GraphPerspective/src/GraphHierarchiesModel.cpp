#include "GraphHierarchiesModel.h"

GraphHierarchiesModel::GraphHierarchiesModel(QObject *parent): QAbstractItemModel(parent) {
}

QModelIndex GraphHierarchiesModel::index(int row, int column, const QModelIndex &parent) const {

}

QModelIndex GraphHierarchiesModel::parent(const QModelIndex &child) const {

}

int GraphHierarchiesModel::rowCount(const QModelIndex &parent) const {

}

int GraphHierarchiesModel::columnCount(const QModelIndex &parent) const {

}

QVariant GraphHierarchiesModel::data(const QModelIndex &index, int role) const {

}

void GraphHierarchiesModel::addGraph(tlp::Graph *) {

}

void GraphHierarchiesModel::removeGraph(tlp::Graph *) {

}
