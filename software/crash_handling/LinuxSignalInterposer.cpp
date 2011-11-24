/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

// This file has to be compiled on Linux hosts into a shared library.
// It allows to override the following system functions : sigaction, signal and sigset.
// The purpose is to prevent custom signal handlers being overwritten by libraries
// linked against the application (e.g. graphics driver).
// To make this feature work, the environment variable LD_PRELOAD has to be filled
// with the path to the resulting shared library before launching the application.
// For instance : $ LD_PRELOAD=<path to the compiled shared library> <path_to_application_executable> <args>

#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <iostream>
#include <algorithm>
#include <vector>

#include <unistd.h>
#include <dlfcn.h>

using namespace std;

// the custom signal handler defined in an extern source file
// (rename it according to yours or define another one here)
extern void dumpStack(int sig, siginfo_t * info, void * ucontext);

typedef int SigactionFunc(int, const struct sigaction *, struct sigaction *);
static SigactionFunc *real_sigaction = NULL;
static vector<int> handledSignals;

// this function will be called when the shared libary is loaded
static void installSignalHandlers(void) __attribute__((constructor));
static void installSignalHandlers(void) {

	// fill this vector according to the signals you want to handle
	handledSignals.push_back(SIGSEGV);
	handledSignals.push_back(SIGBUS);
	handledSignals.push_back(SIGABRT);
	handledSignals.push_back(SIGILL);
	handledSignals.push_back(SIGFPE);

	// the real sigaction function is retrieved
	if (real_sigaction == NULL) {
		real_sigaction = reinterpret_cast<SigactionFunc *>(dlsym(RTLD_NEXT, "sigaction"));
	}

	// custom signal handler is installed for the signals in the vector
	struct sigaction action;
	sigemptyset(&action.sa_mask);
	action.sa_flags = SA_RESTART | SA_SIGINFO;
	action.sa_sigaction = &dumpStack;

	for (size_t i = 0 ; i < handledSignals.size() ; ++i) {
		real_sigaction(handledSignals[i], &action, NULL);
	}
}

// some typedef on function pointers
typedef void SigHandlerFunc(int);
typedef SigHandlerFunc* SignalFunc(int, SigHandlerFunc*);

// redefinition of the signal function
// if the signal passed as first parameter is already treated by our custom handler,
// do nothing and return SIG_DFL
// if the signal is not treated by our custom handler, call the real signal function
SigHandlerFunc *signal (int sig, SigHandlerFunc *disp) {
	static SignalFunc *func = NULL;

	if (func == NULL) {
		func = reinterpret_cast<SignalFunc *>(dlsym(RTLD_NEXT, "signal"));
	}

	if (std::find(handledSignals.begin(), handledSignals.end(), sig) != handledSignals.end()) {
		return SIG_DFL;
	} else {
		return func(sig, disp);
	}
}
// redefinition of the sigset function
// if the signal passed as first parameter is already treated by our custom handler,
// do nothing and return SIG_DFL
// if the signal is not treated by our custom handler, call the real sigset function
SigHandlerFunc *sigset (int sig, SigHandlerFunc *disp) {
	static SignalFunc *func = NULL;

	if (func == NULL) {
		func = reinterpret_cast<SignalFunc *>(dlsym(RTLD_NEXT, "sigset"));
	}

	if(std::find(handledSignals.begin(), handledSignals.end(), sig) != handledSignals.end()) {
		return SIG_DFL;
	} else {
		return (func(sig, disp));
	}
}

// redefinition of the sigaction function
// if the signal passed as first parameter is already treated by our custom handler,
// do nothing and return 0
// if the signal is not treated by our custom handler, call the real sigaction function
int sigaction(int sig, const struct sigaction *act, struct sigaction *oact) {
	if (std::find(handledSignals.begin(), handledSignals.end(), sig) != handledSignals.end()) {
		return 0;
	} else {
		return real_sigaction(sig, act, oact);
	}
}
