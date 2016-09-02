#include "TimeMeasurer.h"

#ifndef __EMSCRIPTEN__
#include <chrono>
using namespace std::chrono;
#else
#include <emscripten.h>
#endif

TimeMeasurer::TimeMeasurer() : _startTime(0), _endTime(0) {
  reset();
}

void TimeMeasurer::reset() {
#ifndef __EMSCRIPTEN__
  _startTime = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
#else
  _startTime = emscripten_get_now();
#endif
}

unsigned int TimeMeasurer::getElapsedTime() {
#ifndef __EMSCRIPTEN__
  _endTime = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
#else
  _endTime = emscripten_get_now();
#endif
  return (_endTime - _startTime);
}

unsigned int TimeMeasurer::getCurrentTime() {
#ifndef __EMSCRIPTEN__
  return duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
#else
  return emscripten_get_now();
#endif
}
