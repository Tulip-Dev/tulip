#include "tulip/InteractorComposite.h"

using namespace tlp;

void InteractorComponent::setView(tlp::View *view) {
  _view = view;
}
tlp::View* InteractorComponent::view() const {
  return _view;
}
void InteractorComponent::init() {
}
bool InteractorComponent::eventFilter(QObject *, QEvent *) {
  return false;
}
// *******************************

InteractorComposite::InteractorComposite(const QIcon &icon, const QString &text)
  : Interactor(), _priority(0), _action(new QAction(icon,text,this)), _view(NULL), _lastTarget(NULL) {
}

InteractorComposite::~InteractorComposite() {
  foreach(InteractorComponent* i, _components)
    delete i;
}

unsigned int InteractorComposite::priority() const {
  return _priority;
}
void InteractorComposite::setPriority(unsigned int p) {
  _priority = p;
}

QCursor InteractorComposite::cursor() const {
  return QCursor();
}

tlp::View* InteractorComposite::view() const {
  return _view;
}

void InteractorComposite::undoIsDone() {
}

void InteractorComposite::setLastTarget(QObject *target) {
  _lastTarget = target;
}

QObject* InteractorComposite::lastTarget() const {
  return _lastTarget;
}

void InteractorComposite::setView(tlp::View *view) {
  _view = view;
  construct();
  foreach(InteractorComponent* i, _components)
    i->setView(view);
}

InteractorComposite::iterator InteractorComposite::begin() {
  return _components.begin();
}
InteractorComposite::iterator InteractorComposite::end() {
  return _components.end();
}
InteractorComposite::const_iterator InteractorComposite::begin() const {
  return _components.begin();
}
InteractorComposite::const_iterator InteractorComposite::end() const {
  return _components.end();
}
void InteractorComposite::push_back(InteractorComponent *i) {
  _components.push_back(i);
}
void InteractorComposite::push_front(InteractorComponent *i) {
  _components.push_front(i);
}
void InteractorComposite::install(QObject* target) {
  setLastTarget(target);
  if (target != NULL)
    foreach(InteractorComponent* i, _components)
      target->installEventFilter(i);
}
void InteractorComposite::uninstall() {
    foreach(InteractorComponent* i, _components)
      lastTarget()->removeEventFilter(i);

  install(NULL);
}

QAction* InteractorComposite::action() const {
  return _action;
}
