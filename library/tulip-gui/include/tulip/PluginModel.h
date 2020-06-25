/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include <algorithm>

#include <QIcon>
#include <QFont>
#include <QWidget>

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
  SimplePluginListModel(const std::list<std::string> &plugins, QObject *parent = nullptr);
  ~SimplePluginListModel() override;
  int columnCount(const QModelIndex & = QModelIndex()) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &) const override;
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QList<std::string> plugins() const;
  std::string pluginName(const QModelIndex &index) const;
};

template <typename PLUGIN>
class PluginModel : public tlp::TulipModel {
  struct TreeItem {
    TreeItem(QString name, QString info = "", TreeItem *parent = nullptr)
        : name(name), info(info), parent(parent) {}
    virtual ~TreeItem() {
      qDeleteAll(children);
    }
    TreeItem *addChild(QString name, QString info = "") {
      TreeItem *result = new TreeItem(name, info, this);
      children.push_back(result);
      return result;
    }

    QString name;
    QString info;
    TreeItem *parent;
    QList<TreeItem *> children;
  };
  TreeItem *_root;

  // FIXME: Non-optimized piece of crap, should be fixed
  void buildTree() {
    delete _root;
    _root = new TreeItem("root");
    QMap<QString, QMap<QString, QStringList>> pluginTree;
    std::list<std::string> plugins = PluginLister::availablePlugins<PLUGIN>();

    for (std::list<std::string>::iterator it = plugins.begin(); it != plugins.end(); ++it) {
      std::string name = *it;
      const Plugin &plugin = PluginLister::pluginInformation(name);
      pluginTree[tlp::tlpStringToQString(plugin.category())]
                [tlp::tlpStringToQString(plugin.group())]
                    .append(tlp::tlpStringToQString(name));
    }

    for (const QString &cat : pluginTree.keys()) {
      TreeItem *catItem = _root->addChild(cat);

      for (const QString &group : pluginTree[cat].keys()) {
        TreeItem *groupItem = catItem;

        if ((!group.isEmpty()) && (pluginTree[cat].keys().size() > 1))
          groupItem = catItem->addChild(group);

        // sort in case insensitive alphabetic order
        std::sort(pluginTree[cat][group].begin(), pluginTree[cat][group].end(), QStringCaseCmp);

        for (const QString &alg : pluginTree[cat][group]) {
          const Plugin &plugin = PluginLister::pluginInformation(tlp::QStringToTlpString(alg));
          std::string info = plugin.info();

          // set info only if they contain more than one word
          if (info.find(' ') != std::string::npos)
            groupItem->addChild(alg, tlp::tlpStringToQString(info));
          else
            groupItem->addChild(alg);
        }
      }
    }
  }

  QList<int> indexHierarchy(TreeItem *item) const {
    QList<int> result;
    TreeItem *parent = item->parent;
    TreeItem *child = item;

    while (child != _root) {
      result.push_front(parent->children.indexOf(child));
      parent = parent->parent;
      child = child->parent;
    }

    return result;
  }

public:
  explicit PluginModel(QObject *parent = nullptr) : TulipModel(parent), _root(nullptr) {
    buildTree();
  }
  ~PluginModel() override {
    delete _root;
  }

  int rowCount(const QModelIndex &parent = QModelIndex()) const override {
    TreeItem *item = _root;

    if (parent.isValid())
      item = static_cast<TreeItem *>(parent.internalPointer());

    return item->children.size();
  }

  int columnCount(const QModelIndex & = QModelIndex()) const override {
    return 1;
  }

  QModelIndex parent(const QModelIndex &child) const override {
    if (!child.isValid())
      return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem *>(child.internalPointer());

    if (childItem->parent == _root)
      return QModelIndex();

    QList<int> indexes = indexHierarchy(childItem->parent);
    int row = indexes[indexes.size() - 1];
    return createIndex(row, child.column(), childItem->parent);
  }

  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override {
    TreeItem *parentItem = _root;

    if (parent.isValid()) {
      parentItem = static_cast<TreeItem *>(parent.internalPointer());
    }

    if (row >= parentItem->children.size())
      return QModelIndex();

    return createIndex(row, column, parentItem->children[row]);
  }

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
    TreeItem *item = static_cast<TreeItem *>(index.internalPointer());

    if (role == Qt::DisplayRole)
      return item->name;
    else if (role == Qt::ToolTipRole) {
      if (item->info.isEmpty())
        return item->name;
      else
        return QString("<table><tr><td>%1</td></tr><tr><td><i>%2</i></td></tr></table>")
            .arg(item->name + ":")
            .arg(item->info);
    } else if (role == Qt::FontRole && !index.parent().parent().isValid()) {
      QFont f;
      QWidget *p = dynamic_cast<QWidget *>(QAbstractItemModel::parent());
      if (p)
        f = p->font();

      f.setBold(true);
      return f;
    } else if (role == Qt::DecorationRole && item->children.isEmpty() &&
               tlp::PluginLister::pluginExists(tlp::QStringToTlpString(item->name))) {
      const tlp::Plugin &p =
          tlp::PluginLister::pluginInformation(tlp::QStringToTlpString(item->name));
      QIcon icon(tlp::tlpStringToQString(p.icon()));
      return icon;
    }

    return QVariant();
  }

  Qt::ItemFlags flags(const QModelIndex &index) const override {
    Qt::ItemFlags result(QAbstractItemModel::flags(index));

    if (index.isValid()) {
      TreeItem *item = static_cast<TreeItem *>(index.internalPointer());

      if (!PluginLister::pluginExists<PLUGIN>(tlp::QStringToTlpString(item->name)))
        result = Qt::ItemIsEnabled;
    }

    return result;
  }
};
} // namespace tlp

#endif // PLUGINMODEL_H
///@endcond
