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
///@cond DOXYGEN_HIDDEN


#ifndef TULIPFONT_H
#define TULIPFONT_H

#include <QObject>
#include <QMap>

#include <tulip/tulipconf.h>

namespace tlp {

class TLP_QT_SCOPE TulipFont : public QObject {
  static QMap<QString,int> FONT_IDS;
  static int fontId(const QString&);

  Q_OBJECT
  Q_PROPERTY(bool bold READ isBold WRITE setBold)
  bool _bold;
  Q_PROPERTY(bool italic READ isItalic WRITE setItalic)
  bool _italic;
  Q_PROPERTY(QString fontName READ fontName WRITE setFontName)
  QString _fontName;
  Q_PROPERTY(QString fontFile READ fontFile)
  QString _fontFile;
  Q_PROPERTY(bool exists READ exists)
  Q_PROPERTY(int fontId READ fontId)
  Q_PROPERTY(QString fontFamily READ fontFamily)

  void refreshFontFile();
public:

  static QString tulipFontsDirectory();
  static QStringList installedFontNames();
  static TulipFont fromFile(const QString&);

  explicit TulipFont(QObject *parent = NULL);
  explicit TulipFont(const QString fontName, QObject *parent = NULL);
  TulipFont(const TulipFont&);

  TulipFont& operator=(const TulipFont&);
  bool isBold() const;
  bool isItalic() const;
  QString fontName() const;
  QString fontFile() const;
  bool exists() const;
  int fontId() const;
  QString fontFamily() const;

public slots:
  void setItalic(bool);
  void setBold(bool);
  void setFontName(const QString&);
};

}

#endif // TULIPFONT_H
///@endcond
