#ifndef FONTICONMANAGER_H
#define FONTICONMANAGER_H

#include <tulip/tulipconf.h>
#include <tulip/TulipFontAwesome.h>
#include <tulip/TulipMaterialDesignIcons.h>

#include <QMap>
#include <QPair>
#include <QIcon>

class QtAwesome;

namespace tlp {

class TLP_QT_SCOPE FontIconManager {

public:
  static FontIconManager *instance();

  QIcon getFontAwesomeIcon(fa::iconCodePoint icon, const QColor &color = QColor(50, 50, 50), const double scaleFactor = 1.0,
                           const QPointF &translation = QPointF(0, 0));

  QIcon getFontAwesomeIcon(fa::iconCodePoint icon, const QColor &color, const QColor &colorDisabled, const QColor &colorActive,
                           const QColor &colorSelected, const double scaleFactor = 1.0, const QPointF &translation = QPointF(0, 0));

  QIcon getMaterialDesignIcon(md::iconCodePoint icon, const QColor &color = QColor(50, 50, 50), const double scaleFactor = 1.0,
                              const QPointF &translation = QPointF(0, 0));

  QIcon getMaterialDesignIcon(md::iconCodePoint icon, const QColor &color, const QColor &colorDisabled, const QColor &colorActive,
                              const QColor &colorSelected, const double scaleFactor = 1.0, const QPointF &translation = QPointF(0, 0));

  static QIcon stackIcons(const QIcon &backIcon, const QIcon &frontIcon);

private:
  FontIconManager();

  QtAwesome *_faIconCreator;
  QtAwesome *_mdiIconCreator;

  QMap<QPair<fa::iconCodePoint, QString>, QIcon> _faIcons;
  QMap<QPair<md::iconCodePoint, QString>, QIcon> _mdiIcons;

  static FontIconManager *_instance;
};
}

#endif // FONTICONMANAGER_H
