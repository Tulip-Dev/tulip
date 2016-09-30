#ifndef TIMEMEASURER_H
#define TIMEMEASURER_H

class TimeMeasurer {

public:
  TimeMeasurer();

  void reset();

  unsigned int getElapsedTime();

  static unsigned int getCurrentTime();

private:
  double _startTime, _endTime;
};

#endif // TIMEMEASURER_H
