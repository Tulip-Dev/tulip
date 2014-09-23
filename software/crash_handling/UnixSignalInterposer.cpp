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

// This file has to be compiled on Unix hosts into a shared library
// or has to be linked statically against an executable/shared library.
// It allows to override the following system functions : sigaction, signal and sigset.
// The purpose is to prevent custom signal handlers being overwritten by libraries
// linked against the application (e.g. graphics driver).
// When compiled as a shared library the environment variable LD_PRELOAD (DYLD_INSERT_LIBRARIES on MacOS)
// has to be filled with the path to the resulting shared library before launching the application.
// For instance : $ LD_PRELOAD=<path to the compiled shared library> <path_to_application_executable> <args>

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <set>

#include <unistd.h>
#include <dlfcn.h>

#include "UnixSignalInterposer.h"

using namespace std;

// some typedef on function pointers
typedef SignalHandlerFunc* SignalFunc(int, SignalHandlerFunc*);
typedef int SigactionFunc(int, const struct sigaction *, struct sigaction *);

static SigactionFunc *real_sigaction = NULL;
static SignalFunc *real_signal = NULL;
static SignalFunc *real_sigset = NULL;

static set<int> handledSignals;

#ifndef __THROW
#define __THROW
#endif

template <typename TO, typename FROM> TO nasty_cast(FROM f) {
  union {
    FROM f;
    TO t;
  } u;
  u.f = f;
  return u.t;
}

// this function will be called when the library is loaded
static void initSignalInterposer(void) __attribute__((constructor));

static void initSignalInterposer(void) {
  real_sigaction = nasty_cast<SigactionFunc *>(dlsym(RTLD_NEXT, "sigaction"));
  real_signal = nasty_cast<SignalFunc *>(dlsym(RTLD_NEXT, "signal"));
  real_sigset = nasty_cast<SignalFunc *>(dlsym(RTLD_NEXT, "sigset"));
}

void installSignalHandler(int sig, SignalHandlerFunc *handler) {

  handledSignals.insert(sig);

  // custom signal handler is installed for the signals in the vector
  struct sigaction action;
  sigemptyset(&action.sa_mask);
  action.sa_handler = handler;

  real_sigaction(sig, &action, NULL);

}

void installSignalHandler(int sig, SigactionHandlerFunc *handler) {

  // fill this vector according to the signals you want to handle
  handledSignals.insert(sig);

  // custom signal handler is installed for the signals in the vector
  struct sigaction action;
  sigemptyset(&action.sa_mask);
  action.sa_flags = SA_RESTART | SA_SIGINFO;
  action.sa_sigaction = handler;

  real_sigaction(sig, &action, NULL);

}

// redefinition of the signal function
// if the signal passed as first parameter is already treated by our custom handler,
// do nothing and return SIG_DFL
// if the signal is not treated by our custom handler, call the real signal function
SignalHandlerFunc *signal (int sig, SignalHandlerFunc *handler) __THROW {

  if (handledSignals.find(sig) != handledSignals.end()) {
    return SIG_DFL;
  }
  else {
    //Init function if needed
    if(real_signal == NULL) {
      initSignalInterposer();
    }

    return real_signal(sig, handler);
  }

}

// redefinition of the sigset function
// if the signal passed as first parameter is already treated by our custom handler,
// do nothing and return SIG_DFL
// if the signal is not treated by our custom handler, call the real sigset function
SignalHandlerFunc *sigset(int sig, SignalHandlerFunc *handler) __THROW {

  if(handledSignals.find(sig) != handledSignals.end()) {
    return SIG_DFL;
  }
  else {
    //Init function if needed
    if(real_sigset == NULL) {
      initSignalInterposer();
    }

    return real_sigset(sig, handler);
  }

}

// redefinition of the sigaction function
// if the signal passed as first parameter is already treated by our custom handler,
// do nothing and return 0
// if the signal is not treated by our custom handler, call the real sigaction function
int sigaction(int sig, const struct sigaction *act, struct sigaction *oact) __THROW {

  if (handledSignals.find(sig) != handledSignals.end() ) {
    return 0;
  }
  else {
    //Init function if needed
    if(real_sigaction == NULL) {
      initSignalInterposer();
    }

    return real_sigaction(sig, act, oact);
  }

}
