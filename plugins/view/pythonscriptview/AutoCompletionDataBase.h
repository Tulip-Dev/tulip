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

#ifndef AUTOCOMPLETIONDATABASE_H_
#define AUTOCOMPLETIONDATABASE_H_

#include <QtCore/QString>
#include <QtCore/QSet>
#include <QtCore/QHash>

#include <tulip/Graph.h>

#include "APIDataBase.h"

class AutoCompletionDataBase {

public :

  AutoCompletionDataBase(APIDataBase *apiDb = NULL);

  void analyseCurrentScriptCode(const QString &code);

  QSet<QString> getAutoCompletionListForContext(const QString &context, const QString &editedFunction) const ;

  QString findTypeForExpr(const QString &expr, const QString &funcName) const ;

  void setGraph(tlp::Graph * graph) {
	  this->graph = graph;
  }

private :

  QSet<QString> getGraphPropertiesListIfContext(const QString &context, const QString &editedFunction) const ;

  QString getTypeNameForVar(const QString &varName);
  QString getTypeNameForExpr(const QString &varName);

  tlp::Graph *graph;
  APIDataBase *apiDb;
  QSet<QString> globalAutoCompletionList;
  QHash<QString, QSet<QString> > functionAutoCompletionList;
  QHash<QString, QHash<QString, QString> > varToType;
  QHash<QString, QString> iteratorType;

};

#endif /* AUTOCOMPLETIONDATABASE_H_ */
