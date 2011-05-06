namespace tlp {

class  Observable {
%TypeHeaderCode
#include <tulip/Observable.h>
%End
    public:
    
        static void holdObservers();

        static void unholdObservers();

};

};













