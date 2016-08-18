/**
 * QtAwesome - use font-awesome (or other font icons) in your c++ / Qt Application
 *
 * MIT Licensed
 *
 * Copyright 2013-2015 - Reliable Bits Software by Blommers IT. All Rights Reserved.
 * Author Rick Blommers
 */

#include "QtAwesome.h"

#include <QDebug>
#include <QFile>
#include <QFontDatabase>

#include <iostream>

/// The font-awesome icon painter
class QtAwesomeCharIconPainter: public QtAwesomeIconPainter
{
public:
  virtual void paint( QtAwesome* awesome, QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state, const QVariantMap& options  )
  {
    Q_UNUSED(mode);
    Q_UNUSED(state);
    Q_UNUSED(options);

    painter->save();

    // set the correct color
    QColor color = options.value("color").value<QColor>();
    QString text = options.value("text").toString();

    if( mode == QIcon::Disabled ) {
      color = options.value("color-disabled").value<QColor>();
      QVariant alt = options.value("text-disabled");
      if( alt.isValid() ) {
        text = alt.toString();
      }
    } else if( mode == QIcon::Active ) {
      color = options.value("color-active").value<QColor>();
      QVariant alt = options.value("text-active");
      if( alt.isValid() ) {
        text = alt.toString();
      }
    } else if( mode == QIcon::Selected ) {
      color = options.value("color-selected").value<QColor>();
      QVariant alt = options.value("text-selected");
      if( alt.isValid() ) {
        text = alt.toString();
      }
    }
    painter->setPen(color);

    // add some 'padding' around the icon
    int drawSize = qRound(rect.height()*options.value("scale-factor").toFloat());
    QPointF translation = options.value("translation").toPointF();

    painter->translate(translation);
    painter->setFont( awesome->font(drawSize) );
    painter->drawText( rect, text, QTextOption( Qt::AlignCenter|Qt::AlignVCenter ) );
    painter->restore();
  }

};


//---------------------------------------------------------------------------------------


/// The painter icon engine.
class QtAwesomeIconPainterIconEngine : public QIconEngine
{

public:

  QtAwesomeIconPainterIconEngine( QtAwesome* awesome, QtAwesomeIconPainter* painter, const QVariantMap& options  )
    : awesomeRef_(awesome)
    , iconPainterRef_(painter)
    , options_(options)
  {
  }

  virtual ~QtAwesomeIconPainterIconEngine() {}

  QtAwesomeIconPainterIconEngine* clone() const
  {
    return new QtAwesomeIconPainterIconEngine( awesomeRef_, iconPainterRef_, options_ );
  }

  virtual void paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state)
  {
    Q_UNUSED( mode );
    Q_UNUSED( state );
    iconPainterRef_->paint( awesomeRef_, painter, rect, mode, state, options_ );
  }

  virtual QPixmap pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state)
  {
    QPixmap pm(size);
    pm.fill( Qt::transparent ); // we need transparency
    {
      QPainter p(&pm);
      paint(&p, QRect(QPoint(0,0),size), mode, state);
    }
    return pm;
  }

private:

  QtAwesome* awesomeRef_;                  ///< a reference to the QtAwesome instance
  QtAwesomeIconPainter* iconPainterRef_;   ///< a reference to the icon painter
  QVariantMap options_;                    ///< the options for this icon painter
};


//---------------------------------------------------------------------------------------

/// The default icon colors
QtAwesome::QtAwesome( QObject* parent )
  : QObject( parent )
{
  // initialize the default options
  setDefaultOption( "color", QColor(50,50,50) );
  setDefaultOption( "color-disabled", QColor(70,70,70,60));
  setDefaultOption( "color-active", QColor(10,10,10));
  setDefaultOption( "color-selected", QColor(10,10,10));
  setDefaultOption( "scale-factor", 0.9 );
  setDefaultOption( "scale-factor", 0.9 );
  setDefaultOption( "translation", QPointF(0,0) );

  setDefaultOption( "text", QVariant() );
  setDefaultOption( "text-disabled", QVariant() );
  setDefaultOption( "text-active", QVariant() );
  setDefaultOption( "text-selected", QVariant() );

  fontIconPainter_ = new QtAwesomeCharIconPainter();

}


QtAwesome::~QtAwesome()
{
  delete fontIconPainter_;
  qDeleteAll(painterMap_);
}

/// a specialized init function so font-awesome is loaded and initialized
/// this method return true on success, it will return false if the fnot cannot be initialized
/// To initialize QtAwesome with font-awesome you need to call this method
bool QtAwesome::initIconicFont(const QString &iconicFontFile)
{

  // The macro below internally calls "qInitResources_QtAwesome()". this initializes
  // the resource system. For a .pri project this isn't required, but when building and using a
  // static library the resource need to initialized first.
  ///
  // I've checked th qInitResource_* code and calling this method mutliple times shouldn't be any problem
  // (More info about this subject:  http://qt-project.org/wiki/QtResources)
  // Q_INIT_RESOURCE(QtAwesome);

  // load the font file
  QFile res(iconicFontFile);
  if(!res.open(QIODevice::ReadOnly)) {
    qDebug() << "Iconic font could not be loaded!";
    return false;
  }
  QByteArray fontData( res.readAll() );
  res.close();

  // fetch the given font
  int iconicFontId = QFontDatabase::addApplicationFontFromData(fontData);

  QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(iconicFontId);
  if( !loadedFontFamilies.empty() ) {
    fontName_= loadedFontFamilies.at(0);
  } else {
    return false;
  }

  return true;
}

/// Sets a default option. These options are passed on to the icon painters
void QtAwesome::setDefaultOption(const QString& name, const QVariant& value)
{
  defaultOptions_.insert( name, value );
}


/// Returns the default option for the given name
QVariant QtAwesome::defaultOption(const QString& name)
{
  return defaultOptions_.value( name );
}


// internal helper method to merge to option amps
static QVariantMap mergeOptions( const QVariantMap& defaults, const QVariantMap& override )
{
  QVariantMap result= defaults;
  if( !override.isEmpty() ) {
    QMapIterator<QString,QVariant> itr(override);
    while( itr.hasNext() ) {
      itr.next();
      result.insert( itr.key(), itr.value() );
    }
  }
  return result;
}


/// Creates an icon with the given code-point
QVariantMap ret;
QIcon QtAwesome::icon(int character, const QVariantMap &options)
{
  // create a merged QVariantMap to have default options and icon-specific options
  QVariantMap optionMap = mergeOptions( defaultOptions_, options );
  optionMap.insert("text", QString( QChar(character) ) );

  return icon( fontIconPainter_, optionMap );
}

/// Create a dynamic icon by simlpy supplying a painter object
/// The ownership of the painter is NOT transfered.
/// @param painter a dynamic painter that is going to paint the icon
/// @param optionmap the options to pass to the painter
QIcon QtAwesome::icon(QtAwesomeIconPainter* painter, const QVariantMap& optionMap )
{
  // Warning, when you use memoryleak detection. You should turn it of for the next call
  // QIcon's placed in gui items are often cached and not deleted when my memory-leak detection checks for leaks.
  // I'm not sure if it's a Qt bug or something I do wrong
  QtAwesomeIconPainterIconEngine* engine = new QtAwesomeIconPainterIconEngine( this, painter, optionMap  );
  return QIcon( engine );
}

/// Adds a named icon-painter to the QtAwesome icon map
/// As the name applies the ownership is passed over to QtAwesome
///
/// @param name the name of the icon
/// @param painter the icon painter to add for this name
void QtAwesome::give(const QString& name, QtAwesomeIconPainter* painter)
{
  delete painterMap_.value( name );   // delete the old one
  painterMap_.insert( name, painter );
}

/// Creates/Gets the icon font with a given size in pixels. This can be usefull to use a label for displaying icons
/// Example:
///
///    QLabel* label = new QLabel( QChar( icon_group ) );
///    label->setFont( awesome->font(16) )
QFont QtAwesome::font( int size )
{
  QFont font( fontName_);
  font.setPixelSize(size);
  return font;
}
