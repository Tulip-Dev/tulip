#include "tulip/FontIconManager.h"
#include "tulip/TlpQtTools.h"

#include <QtAwesome.h>

using namespace tlp;

FontIconManager *FontIconManager::_instance(nullptr);

FontIconManager::FontIconManager() {
  _faIconCreator = new QtAwesome;
  _mdiIconCreator = new QtAwesome;
  _faIconCreator->initIconicFont(tlpStringToQString(TulipFontAwesome::getFontAwesomeTrueTypeFileLocation()));
  _mdiIconCreator->initIconicFont(tlpStringToQString(TulipMaterialDesignIcons::getMaterialDesignIconsTrueTypeFileLocation()));
}

FontIconManager *FontIconManager::instance() {
  if (!_instance) {
    _instance = new FontIconManager();
  }
  return _instance;
}

static QVariantMap getOptionsMap(const QColor &color, const QColor &colorDisabled, const QColor &colorActive, const QColor &colorSelected,
                                 const double scaleFactor, const QPointF &translation) {
  QVariantMap ret;
  ret.insert("color", color);
  ret.insert("color-disabled", colorDisabled);
  ret.insert("color-active", colorActive);
  ret.insert("color-selected", colorSelected);
  ret.insert("scale-factor", scaleFactor);
  ret.insert("translation", translation);
  return ret;
}

static QString getOptionsString(const QColor &color, const QColor &colorDisabled, const QColor &colorActive, const QColor &colorSelected,
                                const double scaleFactor, const QPointF &translation) {
  return color.name() + QString::number(color.alpha()) + colorDisabled.name() + QString::number(colorDisabled.alpha()) + colorActive.name() +
         QString::number(colorActive.alpha()) + colorSelected.name() + QString::number(colorSelected.alpha()) + QString::number(scaleFactor) +
         QString::number(translation.x()) + QString::number(translation.y());
}

QIcon FontIconManager::getFontAwesomeIcon(fa::iconCodePoint icon, const QColor &color, const double scaleFactor, const QPointF &translation) {
  return getFontAwesomeIcon(icon, color, color, color, color, scaleFactor, translation);
}

QIcon FontIconManager::getFontAwesomeIcon(fa::iconCodePoint icon, const QColor &color, const QColor &colorDisabled, const QColor &colorActive,
                                          const QColor &colorSelected, const double scaleFactor, const QPointF &translation) {
  QVariantMap options = getOptionsMap(color, colorDisabled, colorActive, colorSelected, scaleFactor, translation);
  QString optionsString = getOptionsString(color, colorDisabled, colorActive, colorSelected, scaleFactor, translation);
  auto p = qMakePair(icon, optionsString);
  if (!_faIcons.contains(p)) {
    _faIcons[p] = _faIconCreator->icon(static_cast<int>(icon), options);
  }
  return _faIcons[p];
}

QIcon FontIconManager::getMaterialDesignIcon(md::iconCodePoint icon, const QColor &color, const double scaleFactor, const QPointF &translation) {
  return getMaterialDesignIcon(icon, color, color, color, color, scaleFactor, translation);
}

QIcon FontIconManager::getMaterialDesignIcon(md::iconCodePoint icon, const QColor &color, const QColor &colorDisabled, const QColor &colorActive,
                                             const QColor &colorSelected, const double scaleFactor, const QPointF &translation) {
  QVariantMap options = getOptionsMap(color, colorDisabled, colorActive, colorSelected, scaleFactor, translation);
  QString optionsString = getOptionsString(color, colorDisabled, colorActive, colorSelected, scaleFactor, translation);
  auto p = qMakePair(icon, optionsString);
  if (!_mdiIcons.contains(p)) {
    _mdiIcons[p] = _mdiIconCreator->icon(static_cast<int>(icon), options);
  }
  return _mdiIcons[p];
}

QIcon FontIconManager::stackIcons(const QIcon &backIcon, const QIcon &frontIcon) {
  QPixmap pixmap = backIcon.pixmap(QSize(128, 128));
  QPainter painter(&pixmap);
  painter.drawPixmap(0, 0, frontIcon.pixmap(QSize(128, 128)));
  return QIcon(pixmap);
}
