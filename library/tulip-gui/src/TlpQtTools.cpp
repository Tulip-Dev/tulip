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

#include <GL/glew.h>

#include <tulip/TlpQtTools.h>

#include <ostream>
#include <ios>

#include <QDebug>
#include <QColorDialog>
#include <QAbstractButton>
#include <QMessageBox>
#include <QImage>
#include <QGLWidget>

#include <QDir>
#include <QApplication>
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif

#include <tulip/DataSet.h>
#include <tulip/TulipSettings.h>
#include <tulip/Interactor.h>
#include <tulip/View.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/GraphProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/SystemDefinition.h>
#include <tulip/TlpTools.h>
#include <tulip/PluginLibraryLoader.h>
#include <tulip/PluginManager.h>
#include <tulip/GlyphManager.h>
#include <tulip/EdgeExtremityGlyphManager.h>
#include <tulip/OpenGlConfigManager.h>
#include <tulip/GlTextureManager.h>
#include <tulip/TulipMetaTypes.h>
#include <tulip/PythonVersionChecker.h>

/**
 * For openDataSetDialog function : see OpenDataSet.cpp
 */
using namespace std;
using namespace tlp;

/**
  * Init property type to property label conversion map
  **/
static map<string,QString> buildPropertyTypeToPropertyTypeLabelMap() {
  map<string,QString> propertyTypeToPropertyTypeLabel;
  propertyTypeToPropertyTypeLabel[BooleanProperty::propertyTypename]=QString("Selection");
  propertyTypeToPropertyTypeLabel[ColorProperty::propertyTypename]=QString("Color");
  propertyTypeToPropertyTypeLabel[DoubleProperty::propertyTypename]=QString("Metric");
  propertyTypeToPropertyTypeLabel[GraphProperty::propertyTypename]=QString("Graph");
  propertyTypeToPropertyTypeLabel[IntegerProperty::propertyTypename]=QString("Integer");
  propertyTypeToPropertyTypeLabel[LayoutProperty::propertyTypename]=QString("Layout");
  propertyTypeToPropertyTypeLabel[SizeProperty::propertyTypename]=QString("Size");
  propertyTypeToPropertyTypeLabel[StringProperty::propertyTypename]=QString("String");
  propertyTypeToPropertyTypeLabel[BooleanVectorProperty::propertyTypename]=QString("BooleanVector");
  propertyTypeToPropertyTypeLabel[ColorVectorProperty::propertyTypename]=QString("ColorVector");
  propertyTypeToPropertyTypeLabel[CoordVectorProperty::propertyTypename]=QString("CoordVector");
  propertyTypeToPropertyTypeLabel[DoubleVectorProperty::propertyTypename]=QString("DoubleVector");
  propertyTypeToPropertyTypeLabel[IntegerVectorProperty::propertyTypename]=QString("IntegerVector");
  propertyTypeToPropertyTypeLabel[SizeVectorProperty::propertyTypename]=QString("SizeVector");
  propertyTypeToPropertyTypeLabel[StringVectorProperty::propertyTypename]=QString("StringVector");
  return propertyTypeToPropertyTypeLabel;
}
//Property type to property label conversion map
static const map<string,QString> propertyTypeToPropertyTypeLabelMap = buildPropertyTypeToPropertyTypeLabelMap();
/**
  * Init property type label to property type conversion map
  **/
static map<QString,string> buildPropertyTypeLabelToPropertyTypeMap() {
  map<QString,string> propertyTypeLabelToPropertyType;
  propertyTypeLabelToPropertyType[QString("Selection")]=BooleanProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("Color")]=ColorProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("Metric")]=DoubleProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("Graph")]=GraphProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("Integer")]=IntegerProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("Layout")]=LayoutProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("Size")]=SizeProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("String")]=StringProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("BooleanVector")]=BooleanVectorProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("ColorVector")]=ColorVectorProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("CoordVector")]=CoordVectorProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("DoubleVector")]=DoubleVectorProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("IntegerVector")]=IntegerVectorProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("SizeVector")]=SizeVectorProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("StringVector")]=StringVectorProperty::propertyTypename;
  return propertyTypeLabelToPropertyType;
}
//Property type label to property type conversion map
static const map<QString,string> propertyTypeLabelToPropertyTypeMap = buildPropertyTypeLabelToPropertyTypeMap();

namespace tlp {

bool getColorDialog(const QColor &color,QWidget *parent,const QString &title,QColor &result) {

  QColor newColor = QColorDialog::getColor(color, parent, title, QColorDialog::ShowAlphaChannel | QColorDialog::DontUseNativeDialog);

  if(newColor.isValid()) {
    result=newColor;
    return true;
  }
  else
    return false;

}

QString propertyTypeToPropertyTypeLabel(const string& typeName) {
  map<string,QString>::const_iterator  it = propertyTypeToPropertyTypeLabelMap.find(typeName);
  return it != propertyTypeToPropertyTypeLabelMap.end()?it->second:QString();
}

string propertyTypeLabelToPropertyType(const QString& typeNameLabel) {
  map<QString,string>::const_iterator  it = propertyTypeLabelToPropertyTypeMap.find(typeNameLabel);
  return it != propertyTypeLabelToPropertyTypeMap.end()?it->second:string();
}

QString getPluginPackageName(const QString& pluginName) {
  return pluginName.simplified().remove(' ').toLower() + /*"-" + info->getRelease().c_str() +*/ "-" + TULIP_VERSION + "-" + OS_PLATFORM + OS_ARCHITECTURE + "-" + OS_COMPILER + ".zip";
}

QString getPluginStagingDirectory() {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  return QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0) + "/staging/plugins";
#else
  return QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/staging/plugins";
#endif
}

QString getPluginLocalInstallationDir() {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  return QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0) + "/plugins";
#else
  return QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/plugins";
#endif
}

QString localPluginsPath() {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  return QString(QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0) + "/plugins/");
#else
  return QString(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/plugins/");
#endif
}

// we define a specific GlTextureLoader allowing to load a GlTexture
// from a QImage
class GlTextureFromQImageLoader :public GlTextureLoader {
public:
  // redefine the inherited method
  bool loadTexture(const std::string& filename, GlTexture& glTexture) {
    QImage image(QString::fromUtf8(filename.c_str()));

    if (image.isNull()) {
      if (!QFile(QString::fromUtf8(filename.c_str())).exists())
        tlp::error() << "Error when loading texture, the file named \"" << filename.c_str() << "\" does not exist" << std::endl;
      else
        tlp::error() << "Error when loading texture from " << filename.c_str() << std::endl;

      return false;
    }

    bool canUseMipmaps = OpenGlConfigManager::getInst().isExtensionSupported("GL_ARB_framebuffer_object") ||
                         OpenGlConfigManager::getInst().isExtensionSupported("GL_EXT_framebuffer_object");

    unsigned int width=image.width();
    unsigned int height=image.height();

    bool isSprite=false;

    if(width!=height) {
      bool widthPowerOfTwo=false;
      bool heightPowerOfTwo=false;

      for(unsigned int i=1; i<=width; i*=2) {
        if(i==width)
          widthPowerOfTwo=true;
      }

      for(unsigned int i=1; i<=height; i*=2) {
        if(i==height)
          heightPowerOfTwo=true;
      }

      if(widthPowerOfTwo && heightPowerOfTwo) {
        isSprite=true;
      }
    }

    int spriteNumber=1;

    if(isSprite) {
      if(width>height) {
        spriteNumber=width/height;
      }
      else {
        spriteNumber=height/width;
      }
    }

    GLuint* textureNum = new GLuint[spriteNumber];

    image = QGLWidget::convertToGLFormat(image);

    glTexture.width=width;
    glTexture.height=height;
    glTexture.spriteNumber=spriteNumber;
    glTexture.id=new GLuint[spriteNumber];

    glGenTextures(spriteNumber, textureNum);  //FIXME: handle case where no memory is available to load texture

    glEnable(GL_TEXTURE_2D);

    if(!isSprite) {
      glBindTexture(GL_TEXTURE_2D, textureNum[0]);

      glTexture.id[0]=textureNum[0];

      int GLFmt = image.hasAlphaChannel() ? GL_RGBA : GL_RGB;
      glTexImage2D(GL_TEXTURE_2D, 0, GLFmt, width, height, 0, GLFmt, GL_UNSIGNED_BYTE, image.bits());

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      if (canUseMipmaps) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
      }
      else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      }

    }
    else {
      QImage *images=new QImage[spriteNumber];

      if(width>height) {
        QRect rect(0,0,height,height);

        for(int i=0; i<spriteNumber; i++) {
          images[i]=image.copy(rect);
          rect.translate(height,0);
        }
      }
      else {
        QRect rect(0,0,width,width);

        for(int i=0; i<spriteNumber; i++) {
          images[i]=image.copy(rect);
          rect.translate(0,width);
        }
      }

      width=images[0].width();
      height=images[0].height();

      for(int i=0; i<spriteNumber; i++) {
        glBindTexture(GL_TEXTURE_2D, textureNum[i]);

        glTexture.id[i]=textureNum[i];

        int GLFmt = images[i].hasAlphaChannel() ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, GLFmt, width, height, 0, GLFmt, GL_UNSIGNED_BYTE, images[i].bits());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (canUseMipmaps) {
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
          glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }

      }

      delete[] images;
    }

    glDisable(GL_TEXTURE_2D);


    return true;
  }
};

void initTulipSoftware(tlp::PluginLoader* loader, bool removeDiscardedPlugins) {

  QLocale::setDefault(QLocale(QLocale::English));
  TulipSettings::instance().applyProxySettings();
  TulipSettings::instance().initSeedOfRandomSequence();

  if (TulipSettings::instance().isFirstRun()) {
    TulipSettings::instance().setFirstRun(false);
    TulipSettings::instance().addRemoteLocation(PluginManager::STABLE_LOCATION);
    TulipSettings::instance().addRemoteLocation(PluginManager::TESTING_LOCATION);
  }

  QDir::home().mkpath(tlp::localPluginsPath());
  QLocale::setDefault(QLocale(QLocale::English));

#if defined(__APPLE__)
  QApplication::addLibraryPath(QApplication::applicationDirPath() + "/../");
  QApplication::addLibraryPath(QApplication::applicationDirPath() + "/../lib/");
#endif

#ifdef BUILD_PYTHON_COMPONENTS
// MS stated that SetDllDirectory only exists since WinXP SP1
#if defined(WIN32) && (_WIN32_WINNT >= 0x0502)

  // Python on windows can be installed for current user only.
  // In that case, the Python dll is not located in system path but in the Python home directory.
  // So add the Python home directory in the Dll search paths in order to be able to load plugins depending on Python.
  if (tlp::PythonVersionChecker::isPythonVersionMatching()) {
    SetDllDirectory(tlp::PythonVersionChecker::getPythonHome().toStdString().c_str());
  }

#endif
#endif

  if (removeDiscardedPlugins) {
    foreach (const QString& plugin, tlp::PluginManager::markedForRemoval()) {
      QFile f(plugin);
      f.remove();
      tlp::PluginManager::unmarkForRemoval(plugin); // whether or not the removal succeeded, do not try again
    }
  }

  tlp::initTulipLib();
  initQTypeSerializers();
  // initialize Texture loader
  GlTextureManager::setTextureLoader(new GlTextureFromQImageLoader());

  tlp::TulipPluginsPath = std::string((tlp::localPluginsPath() + QDir::separator() + "lib" + QDir::separator() + "tulip").toUtf8().data()) +
                          tlp::PATH_DELIMITER +
                          tlp::TulipPluginsPath +
                          tlp::PATH_DELIMITER +
                          tlp::TulipPluginsPath + "/glyph" +
                          tlp::PATH_DELIMITER +
                          tlp::TulipPluginsPath + "/interactor" +
                          tlp::PATH_DELIMITER +
                          tlp::TulipPluginsPath + "/view" +
                          tlp::PATH_DELIMITER +
                          tlp::TulipPluginsPath + "/perspective" +
                          tlp::PATH_DELIMITER +
                          tlp::getPluginLocalInstallationDir().toUtf8().data();

  // Load plugins
  tlp::PluginLibraryLoader::loadPlugins(loader);
  tlp::PluginLister::checkLoadedPluginsDependencies(loader);
  tlp::InteractorLister::initInteractorsDependencies();
  tlp::GlyphManager::getInst().loadGlyphPlugins();
  tlp::EdgeExtremityGlyphManager::getInst().loadGlyphPlugins();
}

// tlp::debug redirection
class QDebugOStream :public std::ostream {
  class QDebugStreamBuf :public std::streambuf {
  protected:
    string buf;
    virtual int_type overflow(int c) {
      if (c == '\n') {
        qDebug() << buf;
        buf.clear();
      }
      else
        buf += c;

      return c;
    }

    virtual std::streamsize xsputn(const char *p, std::streamsize n) {
      if (p[n-1] == '\n') {
        buf += std::string(p, n - 1);
        qDebug() << buf.c_str();
        buf.clear();
      }
      else
        buf += std::string(p, n);

      return n;
    }
  };

  QDebugStreamBuf qDebugBuf;

public:
  QDebugOStream():std::ostream(&qDebugBuf) {}
};

static QDebugOStream* qDebugStream = NULL;

void redirectDebugOutputToQDebug() {
  if (qDebugStream == NULL)
    qDebugStream = new QDebugOStream();

  tlp::setDebugOutput(*qDebugStream);
}

// tlp::warning redirection
class QWarningOStream :public std::ostream {
  class QWarningStreamBuf :public std::streambuf {
  protected:
    string buf;
    virtual int_type overflow(int c) {
      if (c == '\n') {
        qWarning() << buf.c_str();
        buf.clear();
      }
      else
        buf += c;

      return c;
    }

    virtual std::streamsize xsputn(const char *p, std::streamsize n) {
      if (p[n-1] == '\n') {
        buf += std::string(p, n - 1);
        qWarning() << buf.c_str();
        buf.clear();
      }
      else
        buf += std::string(p, n);

      return n;
    }
  };

  QWarningStreamBuf qWarningBuf;

public:
  QWarningOStream() :std::ostream(&qWarningBuf) {}
};

static QWarningOStream* qWarningStream = NULL;

void redirectWarningOutputToQWarning() {
  if (qWarningStream == NULL)
    qWarningStream = new QWarningOStream();

  tlp::setWarningOutput(*qWarningStream);
}

// tlp::error redirection
class QErrorOStream :public std::ostream {
  class QErrorStreamBuf :public std::streambuf {
  protected:
    string buf;
    virtual int_type overflow(int c) {
      if (c == '\n') {
        qCritical() << buf.c_str();
        buf.clear();
      }
      else
        buf += c;

      return c;
    }

    virtual std::streamsize xsputn(const char *p, std::streamsize n) {
      if (p[n-1] == '\n') {
        buf += std::string(p, n - 1);
        qCritical() << buf.c_str();
        buf.clear();
      }
      else
        buf += std::string(p, n);

      return n;
    }
  };

  QErrorStreamBuf qErrorBuf;

public:
  QErrorOStream():std::ostream(&qErrorBuf) {}
};

static QErrorOStream* qErrorStream = NULL;

void redirectErrorOutputToQCritical() {
  if (qErrorStream == NULL)
    qErrorStream = new QErrorOStream();

  tlp::setErrorOutput(*qErrorStream);
}

// Gui Test mode
bool GuiTestingMode = false;

bool inGuiTestingMode() {
  return GuiTestingMode;
}

void setGuiTestingMode(bool enabled) {
  GuiTestingMode = enabled;
}

} // namespace tlp
