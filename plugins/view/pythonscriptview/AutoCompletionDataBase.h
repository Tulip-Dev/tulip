/**
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

#if defined(__GNUC__) && __GNUC__ >= 4 && ((__GNUC_MINOR__ == 2 && __GNUC_PATCHLEVEL__ >= 1) || (__GNUC_MINOR__ >= 3))
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

#ifndef AUTOCOMPLETIONDATABASE_H_
#define AUTOCOMPLETIONDATABASE_H_

#include <QtCore/QString>
#include <QtCore/QSet>
#include <QtCore/QHash>

#include <tulip/Graph.h>

#include "APIDataBase.h"

static char sepChar[] = {' ', '\t', '=', '(', '[', '{' , ',', '*', '+', '/', '^', '-', 0};

class AutoCompletionDataBase {

public :

  AutoCompletionDataBase(APIDataBase *apiDb = NULL);

  void setGraph(tlp::Graph * graph) {
    this->graph = graph;
  }

  void analyseCurrentScriptCode(const QString &code, const int currentLine, const bool interactiveSession=false, const QString &moduleName="");

  QSet<QString> getAutoCompletionListForContext(const QString &context, const QString &editedFunction, bool dotContext=false) ;

  QString getLastFoundType() const {
    return lastFoundType;
  }

  QString findTypeForExpr(const QString &expr, const QString &funcName) const ;

  QVector<QVector<QString> > getParamTypesForMethodOrFunction(const QString &type, const QString &funcName) const;

  QString getReturnTypeForMethodOrFunction(const QString &type, const QString &funcName) const;

  QSet<QString> getAllDictForType(const QString &type, const QString &prefix, const bool root=true) const;

  QString getTypeNameForVar(const QString &varName) const;

  QString getTypeNameForExpr(const QString &varName) const;

private :

  QSet<QString> getSubGraphsListIfContext(const QString &context, const QString &editedFunction) const ;
  QSet<QString> getGraphPropertiesListIfContext(const QString &context, const QString &editedFunction) const ;
  QSet<QString> getPluginParametersListIfContext(const QString &context, const QString &editedFunction) const;

  QString getClassAttributeType(const QString &className, const QString &classAttribute) const;

  tlp::Graph *graph;
  APIDataBase *apiDb;
  QSet<QString> globalAutoCompletionList;
  QHash<QString, QSet<QString> > functionAutoCompletionList;
  QHash<QString, QHash<QString, QString> > varToType;
  QHash<QString, QHash<QString, QString> > classAttributeToType;
  QHash<QString, QHash<QString, QString> > varToPluginName;
  QHash<QString, QHash<QString, QSet<QString> > > pluginParametersDataSet;
  QHash<QString, QString> iteratorType;
  //QHash<QString, QString> classType;
  QHash<QString, QSet<QString> > classContents;
  QHash<QString, QSet<QString> > classBases;
  QString lastFoundType;

};

#endif /* AUTOCOMPLETIONDATABASE_H_ */
