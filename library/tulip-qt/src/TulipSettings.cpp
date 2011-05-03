#include "tulip/TulipSettings.h"

TulipSettings *TulipSettings::_instance = 0;

TulipSettings::TulipSettings(): QSettings("TulipSoftware","Tulip") {
}

QList<QString> TulipSettings::recentDocuments() const {
  QList<QVariant> recentDocumentsValue = value("app/recent_documents").toList();
  QList<QString> result;
  for (QList<QVariant>::iterator it = recentDocumentsValue.begin(); it != recentDocumentsValue.end(); ++it)
    result.append(it->toString());
  return result;
}

void TulipSettings::addToRecentDocuments(const QString &name) {
  QList<QVariant> recentDocumentsValue = value("app/recent_documents").toList();
  if (recentDocumentsValue.contains(name))
    recentDocumentsValue.removeAll(name);
  recentDocumentsValue.push_front(name);
  while (recentDocumentsValue.size() > 5)
    recentDocumentsValue.pop_back();
  setValue("app/recent_documents",recentDocumentsValue);
}
