#ifndef FONTICONMANAGER_H
#define FONTICONMANAGER_H

#include <QtAwesome.h>
#include <tulip/tulipconf.h>

#include <QMap>
#include <QPair>

namespace tlp {

class TLP_QT_SCOPE FontIconManager {

public:

  static FontIconManager *instance();

  QIcon getFontAwesomeIcon(fa::iconCodePoint icon,
                           const QColor &color = QColor(50, 50, 50),
                           const double scaleFactor = 0.9,
                           const QPointF &translation = QPointF(0,0));

  QIcon getFontAwesomeIcon(fa::iconCodePoint icon,
                           const QColor &color,
                           const QColor &colorDisabled,
                           const QColor &colorActive,
                           const QColor &colorSelected,
                           const double scaleFactor = 0.9,
                           const QPointF &translation = QPointF(0,0));

  QIcon getMaterialDesignIcon(mdi::iconCodePoint icon,
                              const QColor &color = QColor(50, 50, 50),
                              const double scaleFactor = 0.9,
                              const QPointF &translation = QPointF(0,0));

  QIcon getMaterialDesignIcon(mdi::iconCodePoint icon,
                              const QColor &color,
                              const QColor &colorDisabled,
                              const QColor &colorActive,
                              const QColor &colorSelected,
                              const double scaleFactor = 0.9,
                              const QPointF &translation = QPointF(0,0));

  static QIcon stackIcons(const QIcon &backIcon, const QIcon &frontIcon);

private:

  FontIconManager();

  QtAwesome _faIconCreator;
  QtAwesome _mdiIconCreator;

  QMap<QPair<fa::iconCodePoint, QString>, QIcon> _faIcons;
  QMap<QPair<mdi::iconCodePoint, QString>, QIcon> _mdiIcons;

  static FontIconManager *_instance;
};

}

#endif // FONTICONMANAGER_H
