#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QProcess>

int main(int argc,char**argv) {
    QCoreApplication app(argc,argv);
    QProcess process;
    process.start(QCoreApplication::applicationDirPath() + "/tulip/bin/tulip");
    process.waitForFinished(-1);
    return process.exitCode();
}
