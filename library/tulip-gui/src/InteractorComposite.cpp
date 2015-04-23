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

#include "tulip/InteractorComposite.h"

#include <QAction>

using namespace tlp;

void InteractorComponent::setView(tlp::View *view) {
  _view = view;
  viewChanged(view);
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
  : Interactor(), _action(new QAction(icon,text,this)), _view(NULL), _lastTarget(NULL) {
}

InteractorComposite::~InteractorComposite() {
  qDeleteAll(_components);
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

  if (_lastTarget)
    connect(_lastTarget,SIGNAL(destroyed()),this,SLOT(lastTargetDestroyed()));
}

void InteractorComposite::lastTargetDestroyed() {
  if (sender() == lastTarget())
    _lastTarget = NULL;
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
    foreach(InteractorComponent* i, _components) {
    target->installEventFilter(i);
    i->init();
  }
}
void InteractorComposite::uninstall() {
  if (lastTarget() != NULL) {
    foreach(InteractorComponent* i, _components) {
      lastTarget()->removeEventFilter(i);
      i->clear();
    }
  }

  install(NULL);
}

QAction* InteractorComposite::action() const {
  return _action;
}
