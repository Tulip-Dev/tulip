/*
 * StackWalker.h
 *
 *  Created on: Aug 14, 2011
 *      Author: antoine
 */

#ifndef STACKWALKER_H_
#define STACKWALKER_H_

#include <iostream>
#include <iomanip>
#include <string>
#include <stdint.h>
#include <cstdlib>

#ifndef _MSC_VER
#ifdef HAVE_BFD
#include "BfdWrapper.h"
#endif
#endif

#ifdef WIN32
#include <windows.h>
#endif 
 
class StackWalker {

public:

	virtual ~StackWalker() {}

	virtual void printCallStack(std::ostream &os, unsigned int maxDepth = 50) = 0;

	void printCallStackToStdErr(unsigned int maxDepth = 50) {
		printCallStack(std::cerr, maxDepth);
	}

	virtual void printFrameInfos(std::ostream &os, unsigned int frameId, int64_t pcAddress, std::string moduleName, std::string funcName="", int64_t symbolOffset=0, std::string fileName="", unsigned int line=0) {
		os << std::dec << std::setfill('0') << "#" << std::setw(2) << frameId
		   << " 0x" << std::hex << std::setw(16) << pcAddress << " in ";
		if (funcName != "") {
			os << funcName;
		} else {
			os << "??";
		}
		if (symbolOffset != 0) {
			os << " (+0x" << std::hex << symbolOffset << ")";
		}		
		if (fileName != "") {
			os << " at " << fileName << ":" << std::dec << line;
		}

		if (moduleName != "") {
			os << " from " << moduleName << std::endl;
		} else {
			os << " from ??" << std::endl;
		}
	}

};


#if defined(__linux)

#include <map>

class StackWalkerGCC : public StackWalker {

public:

	StackWalkerGCC();

	~StackWalkerGCC();

	void printCallStack(std::ostream &os, unsigned int maxDepth = 50);

	void setCallerAddress(void *callerAddress) {this->callerAddress = callerAddress;}

private:

	void *callerAddress;
#ifdef HAVE_BFD
	std::map<std::string, BfdWrapper *> bfdMap;
#endif

};

#elif defined(__MINGW32__)

#include <map>

class StackWalkerMinGW : public StackWalker {
	
	public:
	
		StackWalkerMinGW();
		~StackWalkerMinGW();

		void printCallStack(std::ostream &os, unsigned int maxDepth = 50);
		
		void setContext(LPCONTEXT context) {this->context = context;}
		
private:

	LPCONTEXT context;
#ifdef HAVE_BFD
	std::map<std::string, BfdWrapper *> bfdMap;		
#endif
};

#elif defined(_MSC_VER)

class StackWalkerMSVC : public StackWalker {

	public:
	
		StackWalkerMSVC();
		~StackWalkerMSVC();
		
		void printCallStack(std::ostream &os, unsigned int maxDepth = 50);
		
		void setContext(CONTEXT *context) {this->context = context;}
		
	private :

		CONTEXT *context;
		
};

#endif

#endif /* STACKWALKER_H_ */
