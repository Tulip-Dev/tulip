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

#ifndef APIDATABASE_H_
#define APIDATABASE_H_

#include <QtCore/QSet>
#include <QtCore/QVector>
#include <QtCore/QHash>
#include <QtCore/QString>

class APIDataBase {

public :

  APIDataBase();

  void loadApiFile(const QString &apiFilePath);

  void addApiEntry(const QString &apiEntry);

  QSet<QString> getDictContentForType(const QString &type, const QString &prefix = "") const;
  QString getReturnTypeForMethodOrFunction(const QString &funcName) const;
  QVector<QVector<QString> > getParamTypesForMethodOrFunction(const QString &funcName) const;
  QVector<QString> findTypesContainingDictEntry(const QString &dictEntry) const;
  QSet<QString> getAllDictEntriesStartingWithPrefix(const QString &prefix) const;
  bool typeExists(const QString &type) const;


private :

  QHash<QString, QSet<QString> > dictContent;
  QHash<QString, QString> returnType;
  QHash<QString, QVector<QVector<QString> > > paramTypes;


};

#endif /* APIDATABASE_H_ */
