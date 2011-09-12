#ifndef PARAMETERLISTMODEL_H
#define PARAMETERLISTMODEL_H

#include "tulip/TulipModel.h"
#include <tulip/WithParameter.h>
#include <tulip/DataSet.h>
#include <tulip/TulipMetaTypes.h>

namespace tlp {

class TLP_QT_SCOPE ParameterListModel : public TulipModel {
  struct ParamInfos {
    inline ParamInfos(bool m,const QString &n,const QString &d, const std::string& t): mandatory(m), name(n),desc(d), type(t) {}
    bool mandatory;
    QString name;
    QString desc;
    std::string type;
  };

  struct ParamInfosSorter {
    bool operator()(ParamInfos a, ParamInfos b);
  };

  std::vector<ParamInfos> _params;
  tlp::DataSet _data;
  tlp::Graph* _graph;

public:
  explicit ParameterListModel(const tlp::ParameterList &params, tlp::Graph *graph=0, QObject *parent=0);

  QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &child) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role);
};

}

#endif // PARAMETERLISTMODEL_H
