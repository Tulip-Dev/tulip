#include "tulip/TulipProject.h"

namespace tlp {
TulipProject::TulipProject() {
}

TulipProject *TulipProject::open(const QString &file,int *errorCode, tlp::PluginProgress *progress) {
}

int TulipProject::save(const QString &file, tlp::PluginProgress *progress) {
}

// ==============================
//      FILES MANIPULATION
// ==============================
QStringList TulipProject::entryList(const QString &path) {
}

bool TulipProject::isDir(const QString &path) {
}

bool TulipProject::removeFile(const QString &path) {
}

bool TulipProject::removeDir(const QString &path) {
}

std::fstream *TulipProject::stdFileStream(const QString &path) {
}

QIODevice *TulipProject::fileStream(const QString &path) {
}

// ==============================
//      META-INFORMATIONS
// ==============================
QString TulipProject::name() const {
}

void TulipProject::setName(const QString &n) {

}

QString TulipProject::description() const {

}
void TulipProject::setDescription(const QString &d) {

}

QString TulipProject::author() const {

}
void TulipProject::setAuthor(const QString &a) {

}

QString TulipProject::perspective() const {

}
void TulipProject::setPerspective(const QString &p) {

}

QDate TulipProject::date() const {

}

QString TulipProject::version() const {

}


}
