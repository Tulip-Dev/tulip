/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#include <QIcon>
#include <QFont>

#include <tulip/TulipModel.h>
#include <tulip/TlpQtTools.h>
#include <tulip/PluginLister.h>

#include <string>

namespace tlp {
/*
 * @brief Build and manage a Qt Model of a list of plugins
 */
class TLP_QT_SCOPE SimplePluginListModel : public tlp::TulipModel {
private:
  QList<std::string> _list;

public:

  SimplePluginListModel(const QList<std::string>& plugins,QObject *parent = NULL);
  virtual ~SimplePluginListModel();
  int columnCount ( const QModelIndex& = QModelIndex() ) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &) const;
  QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QList<std::string> plugins()const;
  std::string pluginName(const QModelIndex& index) const;
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
      const Plugin& plugin = PluginLister::instance()->pluginInformation(name);
      pluginTree[tlp::tlpStringToQString(plugin.category())][tlp::tlpStringToQString(plugin.group())].append(tlp::tlpStringToQString(name));
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
          const Plugin& plugin =
            PluginLister::instance()->pluginInformation(tlp::QStringToTlpString(alg));
          std::string infos = plugin.info();

          // set infos only if they contain more than one word
          if (infos.find(' ') != std::string::npos)
            groupItem->addChild(alg, tlp::tlpStringToQString(infos));
          else
            groupItem->addChild(alg);
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
        return QString("<table><tr><td>%1</td></tr><tr><td><i>%2</i></td></tr></table>").arg(item->name + ":").arg(item->infos);
    }
    else if (role == Qt::FontRole && !index.parent().parent().isValid()) {
      QFont f;
      f.setBold(true);
      return f;
    }
    else if (role == Qt::DecorationRole && tlp::PluginLister::pluginExists(tlp::QStringToTlpString(item->name))) {
      const tlp::Plugin& p = tlp::PluginLister::pluginInformation(tlp::QStringToTlpString(item->name));
      QIcon icon(tlp::tlpStringToQString(p.icon()));
      return icon;
    }

    return QVariant();
  }

  virtual Qt::ItemFlags flags ( const QModelIndex& index ) const {
    Qt::ItemFlags result(QAbstractItemModel::flags(index));

    if(index.isValid()) {
      TreeItem* item = (TreeItem*)index.internalPointer();

      if (!PluginLister::instance()->pluginExists<PLUGIN>(tlp::QStringToTlpString(item->name)))
        result = Qt::ItemIsEnabled;
    }

    return result;
  }
};
}

#endif // PLUGINMODEL_H
///@endcond
