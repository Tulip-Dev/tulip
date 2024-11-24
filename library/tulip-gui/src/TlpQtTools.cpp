/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>

#include <tulip/TlpQtTools.h>

#include <iostream>
#include <tulip/tuliphash.h>

#include <QColorDialog>
#include <QMetaEnum>
#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#if defined(__MINGW32__)
#include <QSslSocket>
#endif
#include <QTcpSocket>
#include <QTimer>
#include <QWidget>
#include <QCheckBox>
#include <QListWidget>
#include <QRadioButton>
#include <QTreeView>
#include <QWizard>

#include <tulip/TulipSettings.h>
#include <tulip/Interactor.h>
#include <tulip/SystemDefinition.h>
#include <tulip/PluginLibraryLoader.h>
#include <tulip/PluginLister.h>
#include <tulip/GlyphManager.h>
#include <tulip/EdgeExtremityGlyphManager.h>
#include <tulip/OpenGlConfigManager.h>
#include <tulip/GlTextureManager.h>
#include <tulip/PythonIDEInterface.h>
#include <tulip/TulipItemEditorCreators.h>
#include <tulip/GlOffscreenRenderer.h>
/**
 * For openDataSetDialog function : see OpenDataSet.cpp
 */
using namespace std;
using namespace tlp;

/**
 * Init property type to property label conversion map
 **/
static tlp_hash_map<string, QString> &buildPropertyTypeToPropertyTypeLabelMap() {
  static tlp_hash_map<string, QString> propertyTypeToPropertyTypeLabel;
  propertyTypeToPropertyTypeLabel[BooleanProperty::propertyTypename] = QString("Boolean");
  propertyTypeToPropertyTypeLabel[ColorProperty::propertyTypename] = QString("Color");
  propertyTypeToPropertyTypeLabel[DoubleProperty::propertyTypename] = QString("Double");
  propertyTypeToPropertyTypeLabel[GraphProperty::propertyTypename] = QString("Graph");
  propertyTypeToPropertyTypeLabel[IntegerProperty::propertyTypename] = QString("Integer");
  propertyTypeToPropertyTypeLabel[LayoutProperty::propertyTypename] = QString("Layout");
  propertyTypeToPropertyTypeLabel[SizeProperty::propertyTypename] = QString("Size");
  propertyTypeToPropertyTypeLabel[StringProperty::propertyTypename] = QString("String");
  propertyTypeToPropertyTypeLabel[BooleanVectorProperty::propertyTypename] =
      QString("BooleanVector");
  propertyTypeToPropertyTypeLabel[ColorVectorProperty::propertyTypename] = QString("ColorVector");
  propertyTypeToPropertyTypeLabel[CoordVectorProperty::propertyTypename] = QString("CoordVector");
  propertyTypeToPropertyTypeLabel[DoubleVectorProperty::propertyTypename] = QString("DoubleVector");
  propertyTypeToPropertyTypeLabel[IntegerVectorProperty::propertyTypename] =
      QString("IntegerVector");
  propertyTypeToPropertyTypeLabel[SizeVectorProperty::propertyTypename] = QString("SizeVector");
  propertyTypeToPropertyTypeLabel[StringVectorProperty::propertyTypename] = QString("StringVector");
  return propertyTypeToPropertyTypeLabel;
}

// Property type to property label conversion map
static const tlp_hash_map<string, QString> &propertyTypeToPropertyTypeLabelMap =
    buildPropertyTypeToPropertyTypeLabelMap();
/**
 * Init property type label to property type conversion map
 **/
static map<QString, string> buildPropertyTypeLabelToPropertyTypeMap() {
  static map<QString, string> propertyTypeLabelToPropertyType;
  propertyTypeLabelToPropertyType[QString("Boolean")] = BooleanProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("Color")] = ColorProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("Double")] = DoubleProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("Graph")] = GraphProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("Integer")] = IntegerProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("Layout")] = LayoutProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("Size")] = SizeProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("String")] = StringProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("BooleanVector")] =
      BooleanVectorProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("ColorVector")] = ColorVectorProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("CoordVector")] = CoordVectorProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("DoubleVector")] = DoubleVectorProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("IntegerVector")] =
      IntegerVectorProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("SizeVector")] = SizeVectorProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("StringVector")] = StringVectorProperty::propertyTypename;
  return propertyTypeLabelToPropertyType;
}
// Property type label to property type conversion map
static const map<QString, string> &propertyTypeLabelToPropertyTypeMap =
    buildPropertyTypeLabelToPropertyTypeMap();

namespace tlp {

bool getColorDialog(const QColor &color, QWidget *parent, const QString &title, QColor &result) {

  QColor newColor = QColorDialog::getColor(
      color, parent, title, QColorDialog::ShowAlphaChannel | QColorDialog::DontUseNativeDialog);

  if (newColor.isValid()) {
    result = newColor;
    return true;
  } else
    return false;
}

QString propertyTypeToPropertyTypeLabel(const string &typeName) {
  auto it = propertyTypeToPropertyTypeLabelMap.find(typeName);
  return it != propertyTypeToPropertyTypeLabelMap.end() ? it->second : QString();
}

string propertyTypeLabelToPropertyType(const QString &typeNameLabel) {
  auto it = propertyTypeLabelToPropertyTypeMap.find(typeNameLabel);
  return it != propertyTypeLabelToPropertyTypeMap.end() ? it->second : string();
}

QString getPluginPackageName(const QString &pluginName) {
  return pluginName.simplified().remove(' ').toLower() + "-" + TULIP_VERSION + "-" + OS_PLATFORM +
         OS_ARCHITECTURE + "-" + OS_COMPILER + ".zip";
}

QString getPluginLocalInstallationDir() {
  return QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).at(0) + "/plugins";
}

QString localPluginsPath() {
  return getPluginLocalInstallationDir() + '/';
}

static QMap<QString, QString> _texFiles;

void registerTextureFile(QString name, QString textureFile) {
  _texFiles[name] = textureFile;
}

QString getRegisteredTextureFile(QString name) {
  return _texFiles[name];
}

void clearRegisteredTextureFiles() {
  _texFiles.clear();
}

// we define a specific GlTextureLoader allowing to load a GlTexture
// from a QImage
class GlTextureFromQImageLoader : public GlTextureLoader {
public:
  // redefine the inherited method
  bool loadTexture(const std::string &filename, GlTexture &glTexture) override {
    QString qFilename = tlpStringToQString(filename);
    QImage image(qFilename);
    if (image.isNull()) {
      // use registered texture file if any
      qFilename = getRegisteredTextureFile(QString::fromUtf8(filename.c_str()));
      if (!qFilename.isEmpty())
        image.load(qFilename);
    }

    if (image.isNull()) {
      if (!QFile(QString::fromUtf8(filename.c_str())).exists())
        tlp::warning() << "Error when loading texture, the file named \"" << filename.c_str()
                       << "\" does not exist" << std::endl;
      else
        tlp::warning() << "Error when loading texture from " << filename.c_str() << std::endl;

      return false;
    }

    // store icon preview of the loaded texture in the icon pool used by the Tulip spreadsheet view
    addIconToPool(qFilename, QIcon(QPixmap::fromImage(image)));

    bool canUseMipmaps = OpenGlConfigManager::isExtensionSupported("GL_ARB_framebuffer_object") ||
                         OpenGlConfigManager::isExtensionSupported("GL_EXT_framebuffer_object");

    unsigned int width = image.width();
    unsigned int height = image.height();

    bool isSprite = false;

    if (width != height) {
      bool widthPowerOfTwo = false;
      bool heightPowerOfTwo = false;

      for (unsigned int i = 1; i <= width; i *= 2) {
        if (i == width)
          widthPowerOfTwo = true;
      }

      for (unsigned int i = 1; i <= height; i *= 2) {
        if (i == height)
          heightPowerOfTwo = true;
      }

      if (widthPowerOfTwo && heightPowerOfTwo) {
        isSprite = true;
      }
    }

    int spriteNumber = 1;

    if (isSprite) {
      if (width > height) {
        spriteNumber = width / height;
      } else {
        spriteNumber = height / width;
      }
    }

    GLuint *textureNum = new GLuint[spriteNumber];

    image = image.mirrored().convertToFormat(QImage::Format_RGBA8888);

    glTexture.width = width;
    glTexture.height = height;
    glTexture.spriteNumber = spriteNumber;
    glTexture.id = new GLuint[spriteNumber];

    glGenTextures(spriteNumber, textureNum);

    glEnable(GL_TEXTURE_2D);

    if (!isSprite) {
      glBindTexture(GL_TEXTURE_2D, textureNum[0]);

      glTexture.id[0] = textureNum[0];

      int glFmt = image.hasAlphaChannel() ? GL_RGBA : GL_RGB;

      glTexImage2D(GL_TEXTURE_2D, 0, glFmt, width, height, 0, glFmt, GL_UNSIGNED_BYTE,
                   image.constBits());

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      if (canUseMipmaps) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
      } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      }

    } else {
      QImage *images = new QImage[spriteNumber];

      if (width > height) {
        QRect rect(0, 0, height, height);

        for (int i = 0; i < spriteNumber; i++) {
          images[i] = image.copy(rect);
          rect.translate(height, 0);
        }
      } else {
        QRect rect(0, 0, width, width);

        for (int i = 0; i < spriteNumber; i++) {
          images[i] = image.copy(rect);
          rect.translate(0, width);
        }
      }

      width = images[0].width();
      height = images[0].height();

      for (int i = 0; i < spriteNumber; i++) {
        glBindTexture(GL_TEXTURE_2D, textureNum[i]);

        glTexture.id[i] = textureNum[i];

        int GLFmt = images[i].hasAlphaChannel() ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, GLFmt, width, height, 0, GLFmt, GL_UNSIGNED_BYTE,
                     images[i].bits());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (canUseMipmaps) {
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
          glGenerateMipmap(GL_TEXTURE_2D);
        } else {
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
      }

      delete[] images;
    }

    delete[] textureNum;

    glDisable(GL_TEXTURE_2D);

    return true;
  }
};

void initTulipSoftware(tlp::PluginLoader *loader) {

  QLocale::setDefault(QLocale(QLocale::English));
  TulipSettings::applyProxySettings();
  TulipSettings::initSeedOfRandomSequence();

  QDir::home().mkpath(tlp::localPluginsPath());
  QLocale::setDefault(QLocale(QLocale::English));

#if defined(__APPLE__)
  QApplication::addLibraryPath(QApplication::applicationDirPath() + "/../");
  QApplication::addLibraryPath(QApplication::applicationDirPath() + "/../lib/");
#elif defined(WIN32)
#if defined(__MINGW32__)
  // When using MSYS2 platform to compile Tulip, force the dynamic loading of
  // OpenSSL libraries Qt was compiled against before Python initialization to
  // avoid a DLL Hell on windows
  QSslSocket::supportsSsl();
#endif
  // Python on windows can be installed for current user only.
  // In that case, the Python dll is not located in system path but in the Python home directory.
  // So add the Python home directory in the Dll search paths in order to be able to load plugins
  // depending on Python.
  auto pythonHome = tlp::PythonIDEInterface::getPythonHome();
  if (!pythonHome.isEmpty()) {
#if (QT_VERSION > QT_VERSION_CHECK(6, 6, 0))
    SetDllDirectory(const_cast<wchar_t *>(pythonHome.toStdWString().c_str()));
#else
    SetDllDirectory(pythonHome.toUtf8().data());
#endif
  }
#endif

  tlp::initTulipLib();
  initQTypeSerializers();
  // initialize Texture loader
  GlTextureManager::setTextureLoader(new GlTextureFromQImageLoader());
  // Load plugins
  tlp::PluginLibraryLoader::loadPluginsFromDir(
      tlp::TulipPluginsPath, loader,
      QStringToTlpString(tlp::getPluginLocalInstallationDir()) + "/lib/tulip");
  tlp::PluginLibraryLoader::loadPluginsFromDir(
      QStringToTlpString(tlp::getPluginLocalInstallationDir()), loader);
  tlp::PluginLister::checkLoadedPluginsDependencies(loader);
  tlp::InteractorLister::initInteractorsDependencies();
  tlp::GlyphManager::loadGlyphPlugins();
  tlp::EdgeExtremityGlyphManager::loadGlyphPlugins();

  // Explicitly create a shared OpenGL context to
  // ensure it is initialized before using it
  if (GlOffscreenRenderer::getInstance()->isValid()) {
    GlOffscreenRenderer::getInstance()->makeOpenGLContextCurrent();
    auto vs = OpenGlConfigManager::getOpenGLVersionString();
    if (vs.find("Mesa Intel(R) HD Graphics") == 0)
      // workaround for GL_SELECT rendering crash (Tulip bugs #813)
      // when using the Mesa crocus driver for legacy Intel GPUs
      qputenv("DRAW_USE_LLVM", "0");
    GlOffscreenRenderer::getInstance()->doneOpenGLContextCurrent();
  }
}

// tlp::debug redirection
class QDebugOStream : public std::ostream {
  class QDebugStreamBuf : public std::streambuf {
  protected:
    string buf;
    int_type overflow(int c) override {
      if (c == '\n') {
        qDebug() << buf;
        buf.clear();
      } else
        buf += c;

      return c;
    }

    std::streamsize xsputn(const char *p, std::streamsize n) override {
      if (p[n - 1] == '\n') {
        buf += std::string(p, n - 1);
        qDebug() << buf.c_str();
        buf.clear();
      } else
        buf += std::string(p, n);

      return n;
    }
  };

  QDebugStreamBuf qDebugBuf;

public:
  QDebugOStream() : std::ostream(&qDebugBuf) {}
};

static QDebugOStream *qDebugStream = nullptr;

void redirectDebugOutputToQDebug() {
  if (qDebugStream == nullptr)
    qDebugStream = new QDebugOStream();

  tlp::setDebugOutput(*qDebugStream);
}

// tlp::warning redirection
class QWarningOStream : public std::ostream {
  class QWarningStreamBuf : public std::streambuf {
  protected:
    string buf;
    int_type overflow(int c) override {
      if (!tlp::isWarningOutputEnabled())
        return 0;

      if (c == '\n') {
        qWarning() << buf.c_str();
        buf.clear();
      } else
        buf += c;

      return c;
    }

    std::streamsize xsputn(const char *p, std::streamsize n) override {
      if (!tlp::isWarningOutputEnabled())
        return 0;

      if (p[n - 1] == '\n') {
        buf += std::string(p, n - 1);
        qWarning() << buf.c_str();
        buf.clear();
      } else
        buf += std::string(p, n);

      return n;
    }
  };

  QWarningStreamBuf qWarningBuf;

public:
  QWarningOStream() : std::ostream(&qWarningBuf) {}
};

static QWarningOStream *qWarningStream = nullptr;

void redirectWarningOutputToQWarning() {
  if (qWarningStream == nullptr)
    qWarningStream = new QWarningOStream();

  tlp::setWarningOutput(*qWarningStream);
}

// tlp::error redirection
class QErrorOStream : public std::ostream {
  class QErrorStreamBuf : public std::streambuf {
  protected:
    string buf;
    int_type overflow(int c) override {
      if (c == '\n') {
        qCritical() << buf.c_str();
        buf.clear();
      } else
        buf += c;

      return c;
    }

    std::streamsize xsputn(const char *p, std::streamsize n) override {
      if (p[n - 1] == '\n') {
        buf += std::string(p, n - 1);
        qCritical() << buf.c_str();
        buf.clear();
      } else
        buf += std::string(p, n);

      return n;
    }
  };

  QErrorStreamBuf qErrorBuf;

public:
  QErrorOStream() : std::ostream(&qErrorBuf) {}
};

static QErrorOStream *qErrorStream = nullptr;

void redirectErrorOutputToQCritical() {
  if (qErrorStream == nullptr)
    qErrorStream = new QErrorOStream();

  tlp::setErrorOutput(*qErrorStream);
}

class NoQtUserInputFilter : public QObject {
protected:
  bool eventFilter(QObject *obj, QEvent *event) override;
};

bool NoQtUserInputFilter::eventFilter(QObject *, QEvent *event) {
  switch (event->type()) {
  case QEvent::KeyPress:
  case QEvent::KeyRelease:
  case QEvent::MouseButtonPress:
  case QEvent::MouseButtonDblClick:
  case QEvent::MouseMove:
  case QEvent::HoverEnter:
  case QEvent::HoverLeave:
  case QEvent::HoverMove:
  case QEvent::DragEnter:
  case QEvent::DragLeave:
  case QEvent::DragMove:
  case QEvent::Drop:
    return true;

  default:
    return false;
  }
}

static NoQtUserInputFilter *disableQtUserInputFilter = nullptr;

void disableQtUserInput() {
  if (disableQtUserInputFilter)
    return;

  disableQtUserInputFilter = new NoQtUserInputFilter();
  QCoreApplication::instance()->installEventFilter(disableQtUserInputFilter);
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
}

void enableQtUserInput() {
  if (!disableQtUserInputFilter)
    return;

  QCoreApplication::instance()->removeEventFilter(disableQtUserInputFilter);
  delete disableQtUserInputFilter;
  disableQtUserInputFilter = nullptr;
  QApplication::restoreOverrideCursor();
}

void convertLikeFilter(QString &filter) {
  // filter is a sql like filter we must convert into a
  // a regexp filter
  // first escape all regexp meta chars in filter
  // except '[]^' which are also like meta chars
  QString metaChars = "$()*+.?\\{|}";
  int pos = 0;
  while (pos < filter.length()) {
    if (metaChars.indexOf(filter[pos]) != -1)
      // no escape for \ if it is followed by a like meta chars (% _)
      if ((filter[pos] != '\\') || (pos == filter.length()) ||
          (filter[pos + 1] != '%' && filter[pos + 1] != '_'))
        filter.insert(pos++, "\\");
    ++pos;
  }

  pos = 0;
  // replace all non escaped occurence of % by regexp .*
  while ((pos = filter.indexOf('%', pos)) >= 0) {
    if (pos == 0 || filter.at(pos - 1) != '\\')
      filter.replace(pos, 1, ".*");
    else
      filter.replace(pos - 1, 1, "");
  }
  pos = 0;
  // replace all non escaped occurence of _ by regexp .
  while ((pos = filter.indexOf('_', pos)) >= 0) {
    if (pos == 0 || filter.at(pos - 1) != '\\')
      filter.replace(pos, 1, ".");
    else
      filter.replace(pos - 1, 1, "");
  }
}

bool checkInternetAccess(unsigned int time) {
  QTcpSocket checkInternetSocket;
  // initiate a connection to google on https port (443)
  checkInternetSocket.connectToHost("google.com", 443);
  unsigned int steps = time / 100;
  if (time % 100)
    ++steps;
  bool error = false;
  // error detection
  QObject::connect(&checkInternetSocket, &QTcpSocket::errorOccurred, [&error]() { error = true; });
  while (steps) {
    --steps;
    time = 100;
    // wait for time elapsed
    QTimer::singleShot(100, [&time]() { time = 0; });
    while (time) {
      QApplication::processEvents();
      if (error) {
        switch (checkInternetSocket.error()) {
        case QAbstractSocket::HostNotFoundError:
        case QAbstractSocket::NetworkError:
        case QAbstractSocket::SocketAccessError:
          return false;
        default:
          break;
        }
      }
      if (checkInternetSocket.state() == QAbstractSocket::ConnectedState)
        return true;
    }
  }
  return checkInternetSocket.state() == QTcpSocket::ConnectedState;
}

#ifdef _LINUX
// define a specific class to fix the display of QCheckBox, QListWidget,
// QRadioButton and QTreeView when in dark mode
class CBRBsFixer : public QObject {
public:
  // QCheckBox, QListWidget, QRadioButton and QTreeView children must be fixed
  // at first show time
  bool eventFilter(QObject *obj, QEvent *ev) override {
    if (ev->type() == QEvent::Show) {
      QWidget *w = dynamic_cast<QWidget *>(obj);
      fixCBRBs(w);
      removeEventFilter(this);
    }
    return false;
  }

  void fixCBRBs(QWidget *parent) {
    if (dynamic_cast<QCheckBox *>(parent) || dynamic_cast<QRadioButton *>(parent) ||
        dynamic_cast<QListWidget *>(parent) || dynamic_cast<QTreeView *>(parent)) {
      // because their indicator border is displayed in black
      // we must use lightgray instead
      QPalette p = parent->palette();
      p.setColor(QPalette::Window, Qt::lightGray);
      parent->setPalette(p);
    } else {
      // a loop on children does not work when a QCheckBox, a QListWidget
      // a QRadioButton or a QTreeView is a child of a non current page
      // of a QTabWidget.
      // So we must do a loop on the different pages
      // if parent is a QTabWidget
      QTabWidget *tw = dynamic_cast<QTabWidget *>(parent);
      if (tw) {
        auto current = tw->currentIndex();
        auto cnt = tw->count();
        for (int i = 0; i < cnt; i++) {
          tw->setCurrentIndex(i);
          fixCBRBs(tw->widget(i));
        }
        tw->setCurrentIndex(current);
      } else {
        // with QWizard we must do a loop on its pages
        QWizard *wz = dynamic_cast<QWizard *>(parent);
        if (wz) {
          for (auto id : wz->pageIds())
            fixCBRBs(wz->page(id));
        } else {
          // loop on QWidget children
          for (auto widget : parent->findChildren<QWidget *>(QString(), Qt::FindDirectChildrenOnly))
            fixCBRBs(widget);
        }
      }
    }
  }
};

// fix display of QCheckBox, QListWidget, QRadioButton and QTreeView
// when in dark mode.
// Because their indicator border is displayed in black
// we must use lightgray instead
void tlpFixCBRBs(QWidget *parent) {
  static CBRBsFixer fixer;
  parent->installEventFilter(&fixer);
}
#endif

} // namespace tlp
