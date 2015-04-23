#include <tulip/Observable.h>

namespace tlp {
class Graph;
}

class TLP_SCOPE ListenerClass : private tlp::Observable {

public:
  static void launchListenerClass(const tlp::Graph *graph);
private:
  ListenerClass();
  virtual void treatEvent(const tlp::Event&);
  static ListenerClass * instance;
};

