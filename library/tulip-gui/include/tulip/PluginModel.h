/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
///@cond DOXYGEN_HIDDEN


#ifndef PLUGINMODEL_H
#define PLUGINMODEL_H

#include <tulip/TulipModel.h>
#include <tulip/PluginLister.h>
#include <tulip/TlpQtTools.h>
#include <QtCore/QMap>
#include <QtCore/QSet>
#include <QtCore/QString>
#include <QtGui/QPixmap>
#include <QtGui/QFont>

namespace tlp {

template<typename PLUGIN>
class PluginListModel : public tlp::TulipModel {
private:
  QList<std::string> _list;
public:
  explicit PluginListModel(QObject *parent = NULL): TulipModel(parent), _list(QList<std::string>::fromStdList(PluginLister::instance()->availablePlugins<PLUGIN>())) {
  }
  virtual ~PluginListModel() {
  }

  virtual int columnCount ( const QModelIndex& = QModelIndex() ) const {
    return 1;
  }

  int rowCount(const QModelIndex &parent = QModelIndex()) const {
    if(parent.isValid())
      return 0;

    return PluginLister::instance()->availablePlugins<PLUGIN>().size();
  }

  QModelIndex parent(const QModelIndex &) const {
    return QModelIndex();
  }

  QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const {
    if(parent.isValid())
      return QModelIndex();

    return createIndex(row, column);
  }

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
    if(index.row() < _list.size()) {
      std::string name(_list[index.row()]);

      if(role == Qt::DisplayRole) {
        return name.c_str();
      }
      else if (role == Qt::DecorationRole) {
        const Plugin* p = PluginLister::pluginInformations(name);
        QPixmap pix(p->icon().c_str());
        delete p;
        return pix;
      }
    }

    return QVariant();
  }
};

template<typename PLUGIN>
class PluginModel : public tlp::TulipModel {
  struct TreeItem {
    TreeItem(QString name, QString infos = QString::null,
             TreeItem* parent = NULL): name(name), infos(infos), parent(parent) {}
    virtual ~TreeItem() {
      foreach(TreeItem* c, children)
      delete c;
    }
    TreeItem* addChild(QString name, QString infos = QString::null) {
      TreeItem* result = new TreeItem(name, infos, this);
      children.push_back(result);
      return result;
    }

    QString name;
    QString infos;
    TreeItem* parent;
    QList<TreeItem*> children;
  };
  TreeItem* _root;

  // FIXME: Non-optimized piece of crap, should be fixed
  void buildTree() {
    delete _root;
    _root = new TreeItem("root");
    QMap<QString,QMap<QString,QStringList > > pluginTree;
    std::list<std::string> plugins = PluginLister::instance()->availablePlugins<PLUGIN>();

    for(std::list<std::string>::iterator it = plugins.begin(); it != plugins.end(); ++it) {
      std::string name = *it;
      const Plugin* plugin = PluginLister::instance()->pluginInformations(name);
      pluginTree[plugin->category().c_str()][plugin->group().c_str()].append(name.c_str());
      delete plugin;
    }

    foreach(QString cat, pluginTree.keys()) {
      TreeItem* catItem = _root->addChild(cat);

      foreach(QString group, pluginTree[cat].keys()) {
        TreeItem* groupItem = catItem;

        if ((group != "") && (pluginTree[cat].keys().size() > 1))
          groupItem = catItem->addChild(group);

        // sort in case insensitive alphabetic order
        std::sort(pluginTree[cat][group].begin(),
                  pluginTree[cat][group].end(), QStringCaseCmp);

        foreach(QString alg, pluginTree[cat][group]) {
          const Plugin* plugin =
            PluginLister::instance()->pluginInformations(alg.toStdString());
          std::string infos = plugin->info();

          // set infos only if they contain more than one word
          if (infos.find(' ') != std::string::npos)
            groupItem->addChild(alg, infos.c_str());
          else
            groupItem->addChild(alg);

          delete plugin;
        }
      }
    }
  }

  QList<int> indexHierarchy(TreeItem* item) const {
    QList<int> result;
    TreeItem* parent = item->parent;
    TreeItem* child = item;

    while (child != _root) {
      result.push_front(parent->children.indexOf(child));
      parent = parent->parent;
      child = child->parent;
    }

    return result;
  }

public:
  explicit PluginModel(QObject *parent = NULL): TulipModel(parent), _root(NULL) {
    buildTree();
  }
  virtual ~PluginModel() {
    delete _root;
  }

  int rowCount(const QModelIndex &parent = QModelIndex()) const {
    TreeItem* item = _root;

    if (parent.isValid())
      item = (TreeItem*)parent.internalPointer();

    return item->children.size();
  }

  int columnCount(const QModelIndex & = QModelIndex()) const {
    return 1;
  }

  QModelIndex parent(const QModelIndex &child) const {
    if (!child.isValid())
      return QModelIndex();

    TreeItem* childItem = (TreeItem*)child.internalPointer();

    if (childItem->parent == _root)
      return QModelIndex();

    QList<int> indexes = indexHierarchy(childItem->parent);
    int row = indexes[indexes.size()-1];
    return createIndex(row,child.column(),childItem->parent);
  }

  QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const {
    TreeItem* parentItem = _root;

    if (parent.isValid()) {
      parentItem = (TreeItem*)parent.internalPointer();
    }

    if (row >= parentItem->children.size())
      return QModelIndex();

    return createIndex(row,column,parentItem->children[row]);
  }

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
    TreeItem* item = (TreeItem*)index.internalPointer();

    if (role == Qt::DisplayRole)
      return item->name;
    else if (role == Qt::ToolTipRole) {
      if (item->infos.isNull())
        return item->name;
      else
        return QString("<table><tr><td>%1</td></tr><tr><td><i>%2</i></td></tr></table>").arg(item->name + " :").arg(item->infos);
    }
    else if (role == Qt::FontRole && !index.parent().parent().isValid()) {
      QFont f;
      f.setBold(true);
      return f;
    }
    else if (role == Qt::DecorationRole && tlp::PluginLister::pluginExists(item->name.toStdString())) {
      const tlp::Plugin* p = tlp::PluginLister::pluginInformations(item->name.toStdString());
      QIcon icon(p->icon().c_str());
      delete p;
      return icon;
    }

    return QVariant();
  }

  virtual Qt::ItemFlags flags ( const QModelIndex& index ) const {
    Qt::ItemFlags result(QAbstractItemModel::flags(index));

    if(index.isValid()) {
      TreeItem* item = (TreeItem*)index.internalPointer();

      if (!PluginLister::instance()->pluginExists<PLUGIN>(item->name.toStdString()))
        result = Qt::ItemIsEnabled;
    }

    return result;
  }
};
}

#endif // PLUGINMODEL_H
///@endcond
