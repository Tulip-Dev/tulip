#include "tulip/AbstractGraphicsView.h"

#include <tulip/Interactor.h>

using namespace tlp;
using namespace std;

namespace tlp_new {
// ===================================
AbstractGraphicsView::AbstractGraphicsView() {
}
// ===================================
AbstractGraphicsView::~AbstractGraphicsView() {
}
// ===================================
QWidget *AbstractGraphicsView::construct(QWidget *parent) {
}
// ===================================
QWidget *AbstractGraphicsView::getWidget() {
}
// ===================================
bool AbstractGraphicsView::createPicture(const string &pictureName, int width, int height, bool center, int zoom, int xOffset, int yOffset) {
}
// ===================================
void AbstractGraphicsView::setInteractors(const list<Interactor *> &interactors) {
}
// ===================================
list<Interactor *> AbstractGraphicsView::getInteractors() {
}
// ===================================
void AbstractGraphicsView::setActiveInteractor(Interactor *interactor) {
}
// ===================================
Interactor *AbstractGraphicsView::getActiveInteractor() {
}
// ===================================
QWidget *AbstractGraphicsView::getCentralWidget() {
}
// ===================================
void AbstractGraphicsView::buildInteractorsToolBar() {
}
// ===================================
}
