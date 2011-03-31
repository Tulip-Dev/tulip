#ifndef BENCHTIMER_H
#define BENCHTIMER_H

#include <cstdlib>
#include <sys/time.h>

class BenchTimer {
  timeval timer[2];
public:
  inline timeval start() {
    gettimeofday(&this->timer[0], NULL);
    return this->timer[0];
  }

  inline timeval stop() {
    gettimeofday(&this->timer[1], NULL);
    return this->timer[1];
  }

  inline int duration() const {
    int secs(this->timer[1].tv_sec - this->timer[0].tv_sec);
    int usecs(this->timer[1].tv_usec - this->timer[0].tv_usec);

    if(usecs < 0) {
        --secs;
        usecs += 1000000;
    }
    return static_cast<int>(secs * 1000 + usecs / 1000.0 + 0.5);
  }
};


#endif // BENCHTIMER_H
