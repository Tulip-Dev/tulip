#include "%ProjectName:l%.%CppHeaderSuffix%"

@if "%BaseView%" == "ViewWidget"
#include <QWidget>
@endif

using namespace tlp;

%ProjectName:c%::%ProjectName:c%(const tlp::PluginContext* context): %BaseView%() {
}

%ProjectName:c%::~%ProjectName:c%() {
}

QList<QWidget*> %ProjectName:c%::configurationWidgets() const {
  QList<QWidget*> result = %BaseView%::configurationWidgets();
  // Add configuration widgets
  return result;
}

void %ProjectName:c%::applySettings() {
  %BaseView%::applySettings();
  // Apply view settings
}

tlp::DataSet %ProjectName:c%::state() const {
  tlp::DataSet result;
  // Fill DataSet
  return result;
}

void %ProjectName:c%::setState(const tlp::DataSet& data) {
  // Restore view's state from DataSet
}

@if "%BaseView%" == "View"
void %ProjectName:c%::setupUi() {
  _graphicsView = new QGraphicsView();
  // Fill up the QGraphicsView instance returned by graphicsView()
}

QGraphicsView* %ProjectName:c%::graphicsView() const {
  return _graphicsView;
}

QGraphicsItem* %ProjectName:c%::centralItem() const {
  // Return the main background item in the view
  return NULL;
}
@endif

@if "%BaseView%" == "ViewWidget"
void %ProjectName:c%::setupWidget() {
  // setus up the central widget
  QWidget* widget = new QWidget();
  // fill up the central widget
  setCentralWidget(widget);
}

@endif

@if "%BaseView%" != "GlMainView"
void %ProjectName:c%::centerView() {
  // Reset the visualization's point of view
}

void %ProjectName:c%::draw(tlp::PluginProgress* pluginProgress) {
  // Redraw the visualization after the graph has changed
}

void %ProjectName:c%::refresh(tlp::PluginProgress* pluginProgress) {
  // Redraw the visualization without any modification made to the graph since the last draw
}

QPixmap %ProjectName:c%::snapshot(const QSize& outputSize) {
  QPixmap result;
  // Fill the pixmap with a snapshot of the view
  return result;
}
@endif

void %ProjectName:c%::interactorsInstalled(const QList<tlp::Interactor*>& interactors) {
}

void %ProjectName:c%::currentInteractorChanged(tlp::Interactor*) {
}

void %ProjectName:c%::graphChanged(tlp::Graph*) {
  // Point the visualization to the new graph
}

@if "%BaseView%" == "View"
void %ProjectName:c%::graphDeleted(tlp::Graph* parentGraph) {
  // Moves the visualization to the parent graph (or NULL if the previous graph was root)
  setGraph(parentGraph);
}

@endif

void %ProjectName:c%::fillContextMenu(QMenu* menu,const QPointF& point) {

  @if "%BaseView%" == "View"
  %BaseView%::fillContextMenu(menu, point);

  @endif
  // fill up context menu
}

PLUGIN(%ProjectName:c%)
