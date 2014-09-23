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

/**
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

#ifndef AUTOCOMPLETIONDATABASE_H_
#define AUTOCOMPLETIONDATABASE_H_

#include <QString>
#include <QSet>
#include <QHash>

#include <tulip/Graph.h>
#include <tulip/APIDataBase.h>

namespace tlp {

class TLP_PYTHON_SCOPE AutoCompletionDataBase {

  QSet<QString> getSubGraphsListIfContext(const QString &context, const QString &editedFunction) const ;
  QSet<QString> getGraphPropertiesListIfContext(const QString &context, const QString &editedFunction) const ;
  QSet<QString> getPluginParametersListIfContext(const QString &context, const QString &editedFunction) const;
  QString getClassAttributeType(const QString &className, const QString &classAttribute) const;
  QSet<QString> getGraphsAttributesListIfContext(const QString &context, const QString &editedFunction) const;

  tlp::Graph *_graph;
  APIDataBase *_apiDb;
  QSet<QString> _globalAutoCompletionList;
  QHash<QString, QSet<QString> > _functionAutoCompletionList;
  QHash<QString, QHash<QString, QString> > _varToType;
  QHash<QString, QHash<QString, QString> > _classAttributeToType;
  QHash<QString, QHash<QString, QString> > _varToPluginName;
  QHash<QString, QHash<QString, QSet<QString> > > _pluginParametersDataSet;
  QHash<QString, QString> _iteratorType;
  QHash<QString, QSet<QString> > _classContents;
  QHash<QString, QSet<QString> > _classBases;
  QString _lastFoundType;

public :

  AutoCompletionDataBase(APIDataBase *_apiDb = NULL);

  void setGraph(tlp::Graph * graph) {
    _graph = graph;
  }

  void analyseCurrentScriptCode(const QString &code, const int currentLine, const bool interactiveSession=false, const QString &moduleName="");

  QSet<QString> getAutoCompletionListForContext(const QString &context, const QString &editedFunction, bool dotContext=false);

  QString getLastFoundType() const {
    return _lastFoundType;
  }

  QString findTypeForExpr(const QString &expr, const QString &funcName) const ;

  QVector<QVector<QString> > getParamTypesForMethodOrFunction(const QString &type, const QString &funcName) const;

  QString getReturnTypeForMethodOrFunction(const QString &type, const QString &funcName) const;

  QSet<QString> getAllDictForType(const QString &type, const QString &prefix, const bool root=true) const;

  QString getTypeNameForVar(const QString &varName) const;

  QString getTypeNameForExpr(const QString &varName) const;

};

}

#endif /* AUTOCOMPLETIONDATABASE_H_ */
