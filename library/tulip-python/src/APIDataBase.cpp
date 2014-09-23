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

#include "tulip/APIDataBase.h"

#include <QFile>
#include <QList>
#include <QStringList>
#include <QRegExp>
#include <QTextStream>

#include <tulip/PythonInterpreter.h>
#include <tulip/TlpTools.h>
#include <tulip/TlpQtTools.h>

using namespace tlp;

APIDataBase APIDataBase::_instance;

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

    if (line.startsWith("tulip.tlp.Vec3f.")) {
      addApiEntry(line.replace("Vec3f", "Coord"));
      addApiEntry(line.replace("Coord", "Size"));
    }
  }
}

void APIDataBase::addApiEntry(const QString &apiEnt) {
  QString apiEntry(apiEnt);
  int pos = apiEntry.indexOf('.');

  if (apiEntry.contains(QRegExp("^tulipogl.*\\..+"))) {
    apiEntry = apiEntry.mid(pos+1);
  }

  if (apiEntry.contains(QRegExp("^tulipgui.*\\..+"))) {
    apiEntry = apiEntry.mid(pos+1);
  }

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

    if (_dictContent.find(type) == _dictContent.end()) {
      _dictContent[type] = QSet<QString>();
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

        if (_paramTypes.find(wholeFuncName) == _paramTypes.end()) {
          _paramTypes[wholeFuncName] = QVector<QVector<QString> >();
        }

        _paramTypes[wholeFuncName].append(params);

        if (retType != "") {
          _returnType[wholeFuncName] = retType;
        }
      }
    }

    if (dictEntry != "" ) {
      _dictContent[type].insert(dictEntry);
    }

    pos = newPos;
  }
}

QSet<QString> APIDataBase::getTypesList() const {
  QSet<QString> ret;
  QList<QString> keys = _dictContent.keys();
  foreach(QString type, keys) {
    ret.insert(type);
  }
  return ret;
}

QSet<QString> APIDataBase::getDictContentForType(const QString &type, const QString &prefix) const {
  QSet<QString> ret;

  if (_dictContent.find(type) != _dictContent.end()) {
    foreach(QString s, _dictContent[type]) {
      if (s.toLower().startsWith(prefix.toLower())) {
        ret.insert(s);
      }
    }
  }

  return ret;
}

QString APIDataBase::getReturnTypeForMethodOrFunction(const QString &funcName) const {
  QString ret;

  if (_returnType.find(funcName) != _returnType.end()) {
    ret = _returnType[funcName];
  }

  return ret;
}

QVector<QVector<QString> > APIDataBase::getParamTypesForMethodOrFunction(const QString &funcName) const {
  QVector<QVector<QString> > ret;

  if (_paramTypes.find(funcName) != _paramTypes.end()) {
    ret = _paramTypes[funcName];
  }

  return ret;
}

bool APIDataBase::functionExists(const QString &funcName) const {
  return _paramTypes.find(funcName) != _paramTypes.end();
}

QVector<QString> APIDataBase::findTypesContainingDictEntry(const QString &dictEntry) const {
  QVector<QString> ret;
  QHashIterator<QString, QSet<QString> > i(_dictContent);

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
  QHashIterator<QString, QSet<QString> > i(_dictContent);

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
  return _dictContent.find(type) != _dictContent.end();
}

QString APIDataBase::getFullTypeName(const QString &t) const {
  QList<QString> keys = _dictContent.keys();
  foreach(QString type, keys) {
    int pos = type.lastIndexOf(t);

    if (pos != -1 && (pos + t.length()) == type.length() && (pos == 0 || type[pos-1] == '.')) {
      return type;
    }
  }
  return "";
}

bool APIDataBase::dictEntryExists(const QString &type, const QString &dictEntry) const {
  if (_dictContent.find(type) == _dictContent.end()) {
    return false;
  }

  return _dictContent[type].find(dictEntry) != _dictContent[type].end();
}
