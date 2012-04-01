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

#include <QtCore/QFile>
#include <QtCore/QList>
#include <QtCore/QStringList>
#include <QtCore/QRegExp>
#include <QtCore/QTextStream>

#include <iostream>

using namespace std;

#include "APIDataBase.h"

APIDataBase APIDataBase::instance;

APIDataBase::APIDataBase() {
  addApiEntry("tlp.node.id");
  addApiEntry("tlp.edge.id");

  addApiEntry("list.append(x)");
  addApiEntry("list.extend(L)");
  addApiEntry("list.insert(i, x)");
  addApiEntry("list.remove(x)");
  addApiEntry("list.pop([i])");
  addApiEntry("list.index(x)");
  addApiEntry("list.count(x)");
  addApiEntry("list.sort()");
  addApiEntry("list.reverse()");

  addApiEntry("dict.clear()");
  addApiEntry("dict.copy()");
  addApiEntry("dict.fromkeys(seq[, value])");
  addApiEntry("dict.get(key[, default])");
  addApiEntry("dict.has_key(key)");
  addApiEntry("dict.items()");
  addApiEntry("dict.iteritems()");
  addApiEntry("dict.iterkeys()");
  addApiEntry("dict.keys()");
  addApiEntry("dict.pop(key[, default])");
  addApiEntry("dict.popitem()");
  addApiEntry("dict.setdefault(key[, default])");
  addApiEntry("dict.update([other])");
  addApiEntry("dict.values()");
  addApiEntry("dict.viewitems()");
  addApiEntry("dict.viewkeys()");
  addApiEntry("dict.viewvalues()");

}

void APIDataBase::loadApiFile(const QString &apiFilePath) {
  QFile apiFile(apiFilePath);

  if (!apiFile.exists())
    return;

  apiFile.open(QIODevice::ReadOnly | QIODevice::Text);
  QTextStream in(&apiFile);

  while (!in.atEnd()) {
    QString line = in.readLine();
    addApiEntry(line);
  }
}

void APIDataBase::addApiEntry(const QString &apiEnt) {
  QString apiEntry(apiEnt);
  int pos = apiEntry.indexOf('.');

  if (apiEntry.contains(QRegExp("^tulip.*\\..+"))) {

    apiEntry = apiEntry.mid(pos+1);
  }

  apiEntry.replace(QRegExp("\\?[0-9]+"), "");
  int parenPos = apiEntry.indexOf('(');
  bool func = parenPos != -1;
  QString withoutParams = apiEntry;
  QVector<QString> params;
  QString retType;

  if (func) {
    withoutParams = apiEntry.mid(0, parenPos);
    QString parameters = apiEntry.mid(parenPos+1, apiEntry.lastIndexOf(')') - parenPos - 1);

    if (parameters != "") {
      QStringList paramsList = parameters.split(',');
      foreach(QString param, paramsList) {
        params.append(param.trimmed());
      }
    }

    int retPos = apiEntry.indexOf("->");

    if (retPos != -1) {
      retType = apiEntry.mid(retPos+2).trimmed();
    }
  }

  pos = withoutParams.indexOf('.');

  while (pos != -1) {
    QString type = withoutParams.mid(0, pos);

    if (dictContent.find(type) == dictContent.end()) {
      dictContent[type] = QSet<QString>();
    }

    int newPos = withoutParams.indexOf('.', pos+1);

    QString dictEntry;

    if (newPos != -1) {
      dictEntry = withoutParams.mid(pos+1, newPos - pos - 1).trimmed();
    }
    else {
      dictEntry = withoutParams.mid(pos+1).trimmed();

      if (func) {

        QString wholeFuncName = type + "." + dictEntry;

        if (paramTypes.find(wholeFuncName) == paramTypes.end()) {
          paramTypes[wholeFuncName] = QVector<QVector<QString> >();
        }

        paramTypes[wholeFuncName].append(params);

        if (retType != "") {
          returnType[wholeFuncName] = retType;
        }
      }
    }

    if (dictEntry != "" ) {
      dictContent[type].insert(dictEntry);
    }

    pos = newPos;
  }
}

QSet<QString> APIDataBase::getTypesList() const {
  QSet<QString> ret;
  QList<QString> keys = dictContent.keys();
  foreach(QString type, keys) {
    ret.insert(type);
  }
  return ret;
}

QSet<QString> APIDataBase::getDictContentForType(const QString &type, const QString &prefix) const {
  QSet<QString> ret;

  if (dictContent.find(type) != dictContent.end()) {
    foreach(QString s, dictContent[type]) {
      if (s.toLower().startsWith(prefix.toLower())) {
        ret.insert(s);
      }
    }
  }

  return ret;
}

QString APIDataBase::getReturnTypeForMethodOrFunction(const QString &funcName) const {
  QString ret;

  if (returnType.find(funcName) != returnType.end()) {
    ret = returnType[funcName];
  }

  return ret;
}

QVector<QVector<QString> > APIDataBase::getParamTypesForMethodOrFunction(const QString &funcName) const {
  QVector<QVector<QString> > ret;

  if (paramTypes.find(funcName) != paramTypes.end()) {
    ret = paramTypes[funcName];
  }

  return ret;
}

bool APIDataBase::functionExists(const QString &funcName) const {
  return paramTypes.find(funcName) != paramTypes.end();
}

QVector<QString> APIDataBase::findTypesContainingDictEntry(const QString &dictEntry) const {
  QVector<QString> ret;
  QHashIterator<QString, QSet<QString> > i(dictContent);

  while (i.hasNext()) {
    i.next();
    foreach(QString s, i.value()) {
      if (s == dictEntry) {
        ret.append(i.key());
        break;
      }
    }
  }

  return ret;
}

QSet<QString> APIDataBase::getAllDictEntriesStartingWithPrefix(const QString &prefix) const {
  QSet<QString> ret;
  QHashIterator<QString, QSet<QString> > i(dictContent);

  while (i.hasNext()) {
    i.next();
    foreach(QString s, i.value()) {
      if (s.toLower().startsWith(prefix.toLower())) {
        ret.insert(s);
      }
    }
  }

  return ret;
}

bool APIDataBase::typeExists(const QString &type) const {
  return dictContent.find(type) != dictContent.end();
}

QString APIDataBase::getFullTypeName(const QString &t) const {
  QList<QString> keys = dictContent.keys();
  foreach(QString type, keys) {
    int pos = type.lastIndexOf(t);

    if (pos != -1 && (pos + t.length()) == type.length() && (pos == 0 || type[pos-1] == '.')) {
      return type;
    }
  }
  return "";
}

bool APIDataBase::dictEntryExists(const QString &type, const QString &dictEntry) const {
  if (dictContent.find(type) == dictContent.end()) {
    return false;
  }

  return dictContent[type].find(dictEntry) != dictContent[type].end();
}
