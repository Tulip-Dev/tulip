/*
 * Thread.cpp
 *
 *  Created on: 15 nov. 2008
 *      Author: Antoine Lambert
 */

#include <iostream>

#include "Thread.h"

using namespace std;

static void *runThread(void *thread) {
  Thread *threadToRun = (Thread *) thread;
  threadToRun->run();
  return NULL;
}


Thread::~Thread() {
  pthread_cancel(_posixThread);
}


void Thread::start() {
  if (pthread_create(&_posixThread, NULL, runThread, this) < 0) {
    cerr << "The thread creation failed" << endl;
  }
}


void Thread::join() {
  (void) pthread_join(_posixThread, NULL);
}


