#include "%ProjectName:l%.%CppHeaderSuffix%"

#include <QLabel>

PLUGIN(%ProjectName:c%Composite);

using namespace tlp;

/*
 * %ProjectName:c%Composite
 */

%ProjectName:c%Composite::%ProjectName:c%Composite(const tlp::PluginContext*):
  tlp::%BaseInteractor%Composite(QIcon(":/path/to/icon"),trUtf8("Interactor's tooltip")) {
}

%ProjectName:c%Composite::~%ProjectName:c%Composite() {
}

void %ProjectName:c%Composite::construct() {
  push_back(new %ProjectName:c%Component());
  // Push other components here
}

bool %ProjectName:c%Composite::isCompatible(const std::string& viewName) {
  if (viewName.compare("%ViewName%") == 0) {
    return true;
  }
  // Check the view name here
  return false;
}

QWidget* %ProjectName:c%Composite::configurationWidget() const {
  return new QLabel(trUtf8("My interactor help string"));
}

unsigned int %ProjectName:c%Composite::priority() const {
  return 0;
}

QCursor %ProjectName:c%Composite::cursor() const {
  return QCursor();
}

void %ProjectName:c%Composite::undoIsDone() {
}

/*
 * %ProjectName:c%Component
 */

void %ProjectName:c%Component::init() {
  // The interactor has been installed on a new target
}

void %ProjectName:c%Component::clear() {
  // The interactor has been uninstalled from its previous target
}

void %ProjectName:c%Component::viewChanged(tlp::View* newView) {
}

bool %ProjectName:c%Component::eventFilter(QObject* obj, QEvent* event) {
  // User input handling
  return false;
}

@if "%BaseInteractor%" == "GLInteractor"
bool %ProjectName:c%Component::draw(tlp::GlMainWidget* glMainWidget) {
  // Do some drawing here
  return true;
}

bool %ProjectName:c%Component::compute(tlp::GlMainWidget* glMainWidget) {
  // Initialize OpenGL rendering
  return true;
}
@endif
