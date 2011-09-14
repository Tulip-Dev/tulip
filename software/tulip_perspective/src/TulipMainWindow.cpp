#include "TulipMainWindow.h"

#include <QtGui/QShortcut>
#include <QtGui/QAction>

TulipPerspectiveMainWindow::TulipPerspectiveMainWindow(QWidget *parent): QMainWindow(parent) {
#ifdef MEMORYCHECKER_ON
  QAction* a1 = new QAction(this);
  a1->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_C));
  a1->setShortcutContext(Qt::ApplicationShortcut);
  connect(a1,SIGNAL(triggered()),this,SLOT(clearMemoryChecker()));
  addAction(a1);

  QAction* a2 = new QAction(this);
  a2->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_P));
  a2->setShortcutContext(Qt::ApplicationShortcut);
  connect(a2,SIGNAL(triggered()),this,SLOT(printMemoryChecker()));
  addAction(a2);
#endif // MEMORYCHECKER_ON
}

void TulipPerspectiveMainWindow::printMemoryChecker() {
#ifdef MEMORYCHECKER_ON
  qWarning("prout");
  memory_checker_print_report();
#endif // MEMORYCHECKER_ON
}

void TulipPerspectiveMainWindow::clearMemoryChecker() {
#ifdef MEMORYCHECKER_ON
  qWarning("meuh");
  memory_checker_clear_report();
#endif // MEMORYCHECKER_ON
}
