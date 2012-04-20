#ifndef PLUGINMODEL_H
#define PLUGINMODEL_H

#include <tulip/TulipModel.h>
#include <tulip/PluginLister.h>
#include <QtCore/QMap>
#include <QtCore/QSet>
#include <QtCore/QString>

namespace tlp {

template<typename PLUGIN>
class TLP_QT_SCOPE PluginModel : public tlp::TulipModel {
  struct TreeItem {
    TreeItem(QString name, TreeItem* parent = NULL): name(name), parent(parent) {}
    virtual ~TreeItem() {
      foreach(TreeItem* c, children)
      delete c;
    }
    TreeItem* addChild(QString name) {
      TreeItem* result = new TreeItem(name,this);
      children.push_back(result);
      return result;
    }

    QString name;
    TreeItem* parent;
    QList<TreeItem*> children;
  };
  TreeItem* _root;

  // FIXME: Non-optimized piece of crap, should be fixed
  void buildTree() {
    delete _root;
    _root = new TreeItem("root");
    QMap<QString,QMap<QString,QSet<QString> > > pluginTree;
    std::list<std::string> plugins = PluginLister::instance()->availablePlugins<PLUGIN>();

    for(std::list<std::string>::iterator it = plugins.begin(); it != plugins.end(); ++it) {
      std::string name = *it;
      const Plugin* plugin = PluginLister::instance()->pluginInformations(name);
      pluginTree[plugin->category().c_str()][plugin->group().c_str()]+=name.c_str();
    }

    foreach(QString cat, pluginTree.keys()) {
      TreeItem* catItem = _root->addChild(cat);

      if (pluginTree[cat].keys().size() > 1) {
        foreach(QString group, pluginTree[cat].keys()) {
          TreeItem* groupItem = catItem;

          if (group != "")
            groupItem = catItem->addChild(group);

          foreach(QString alg, pluginTree[cat][group])
          groupItem->addChild(alg);
        }
      }
      else {
        foreach(QString alg, pluginTree[cat][pluginTree[cat].keys()[0]])
        catItem->addChild(alg);
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
  explicit PluginModel(QObject *parent = 0): TulipModel(parent), _root(NULL) {
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

  int columnCount(const QModelIndex &parent = QModelIndex()) const {
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

//  Qt::ItemFlags flags(const QModelIndex &index) const {
//  }

//  QVariant headerData(int section, Qt::Orientation orientation, int role) const {
//  }

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

    return QVariant();
  }
};
}

#endif // PLUGINMODEL_H
