/*
 * StackWalker.cpp
 *
 *  Created on: Aug 14, 2011
 *      Author: antoine
 */

#include "StackWalker.h"

#include <sstream>

#if defined(__linux)

#include <execinfo.h>
#include <cxxabi.h>

StackWalkerGCC::StackWalkerGCC() {}

StackWalkerGCC::~StackWalkerGCC() {
#ifdef HAVE_BFD
	std::map<std::string, BfdWrapper *>::iterator it = bfdMap.begin();
	for ( ; it != bfdMap.end() ; ++it) {
		delete it->second;
	}
#endif	
}

void StackWalkerGCC::printCallStack(std::ostream &os, unsigned int maxDepth) {
	void * array[128];
	int size = backtrace(array, 128);
	char ** messages = backtrace_symbols(array, size);

	if (messages == NULL)
		return;

	std::ostringstream oss;
	oss << callerAddress;
	std::string callerAddressStr = oss.str();

	int i = 1;
	while (i < size) {
		std::string msg(messages[i]);
		if (msg.find(callerAddressStr) != std::string::npos) {
			break;
		}
		++i;
	}

	int offset = i;
	for (; i < size ; ++i) {
		char *mangled_name = 0, *runtime_offset = 0,
				*offset_end = 0, *runtime_addr = 0,
				*runtime_addr_end = 0;

		if (static_cast<unsigned int>(i) > maxDepth)
			return;

		for (char *p = messages[i]; *p; ++p) {
			if (*p == '(') {
				mangled_name = p;
			} else if (*p == '+') {
				runtime_offset = p;
			} else if (*p == ')') {
				offset_end = p;
			} else if (*p == '[') {
				runtime_addr = p;
			} else if (*p == ']') {
				runtime_addr_end = p;
			}
		}

		if (mangled_name && runtime_offset && offset_end &&
				mangled_name < runtime_offset) {
			*mangled_name++ = '\0';
			*runtime_offset++ = '\0';
			*offset_end++ = '\0';
			*runtime_addr++ = '\0';
			*runtime_addr_end = '\0';

			char *dsoName = messages[i];
			std::string dsoNameStr(dsoName);

			int status;
			char * real_name = abi::__cxa_demangle(mangled_name, 0, 0, &status);

			char *end;
			int64_t runtimeAddr = static_cast<int64_t>(strtoll(runtime_addr, &end, 16));
			int64_t runtimeOffset = static_cast<int64_t>(strtoll(runtime_offset, &end, 0));

			std::pair<const char *, unsigned int> infos = std::make_pair("", 0);

#ifdef HAVE_BFD
			if (bfdMap.find(dsoNameStr) == bfdMap.end()) {
				bfdMap[dsoNameStr] = new BfdWrapper(dsoName);
			}
			infos =bfdMap[dsoNameStr]->getFileAndLineForAddress(mangled_name, runtimeAddr, runtimeOffset);		
#endif			

			if (status == 0) {
				if (std::string(infos.first) == "") {
					printFrameInfos(os, i - offset, runtimeAddr, dsoName, real_name, runtimeOffset);
				} else {
					printFrameInfos(os, i - offset, runtimeAddr, dsoName, real_name, runtimeOffset, infos.first, infos.second);
				}
			} else {
				if (std::string(infos.first) == "") {
					printFrameInfos(os, i - offset, runtimeAddr, dsoName, mangled_name, runtimeOffset);
				} else {
					printFrameInfos(os, i - offset, runtimeAddr, dsoName, mangled_name, runtimeOffset, infos.first, infos.second);
				}
			}
			free(real_name);
		}
	}
	free(messages);
}

#elif defined(__MINGW32__)

#include <imagehlp.h>
#include <cxxabi.h>

StackWalkerMinGW::StackWalkerMinGW() {}

StackWalkerMinGW::~StackWalkerMinGW() {
#ifdef HAVE_BFD  
	std::map<std::string, BfdWrapper *>::iterator it = bfdMap.begin();
	for ( ; it != bfdMap.end() ; ++it) {
		delete it->second;
	}
#endif	
}

void StackWalkerMinGW::printCallStack(std::ostream &os, unsigned int maxDepth) {
	HANDLE process = GetCurrentProcess();
	HANDLE thread = GetCurrentThread();

	SymSetOptions(SymGetOptions() | SYMOPT_UNDNAME | SYMOPT_LOAD_LINES);
	if (!SymInitialize(process, 0, TRUE)) {
		std::cerr << "Failed to init symbol context" << std::endl;
		return;
	}

	char procname[MAX_PATH];
	GetModuleFileNameA(NULL, procname, sizeof procname);

	STACKFRAME frame;
	memset(&frame,0,sizeof(frame));

	frame.AddrPC.Offset = context->Eip;
	frame.AddrPC.Mode = AddrModeFlat;
	frame.AddrStack.Offset = context->Esp;
	frame.AddrStack.Mode = AddrModeFlat;
	frame.AddrFrame.Offset = context->Ebp;
	frame.AddrFrame.Mode = AddrModeFlat;

	char symbol_buffer[sizeof(IMAGEHLP_SYMBOL) + 255];
	char module_name_raw[MAX_PATH];

	int depth = maxDepth;

	while(StackWalk(IMAGE_FILE_MACHINE_I386, 
			process,
			thread,
			&frame,
			context,
			0,
			SymFunctionTableAccess,
			SymGetModuleBase, 0)) {

		--depth;
		if (depth < 0)
			break;

		IMAGEHLP_SYMBOL *symbol = (IMAGEHLP_SYMBOL *)symbol_buffer;
		symbol->SizeOfStruct = (sizeof *symbol) + 255;
		symbol->MaxNameLength = 254;

		DWORD module_base = SymGetModuleBase(process, frame.AddrPC.Offset);
		
		int64_t symbolOffset = frame.AddrPC.Offset - module_base  - 0x1000 - 1;
		
		const char * module_name = "[unknown module]";
		if (module_base && 
				GetModuleFileNameA((HINSTANCE)module_base, module_name_raw, MAX_PATH)) {
			module_name = module_name_raw;
		}

		std::string moduleNameStr(module_name);

#ifdef HAVE_BFD		
		if (bfdMap.find(moduleNameStr) == bfdMap.end()) {
			bfdMap[moduleNameStr] = new BfdWrapper(module_name);
		}
#endif

		const char * func = NULL;

		DWORD dummy = 0;
		if (SymGetSymFromAddr(process, frame.AddrPC.Offset, &dummy, symbol)) {
			func = symbol->Name;
		}

		if (func) {
			int status;
			const char *toDemangle = (std::string("_") + std::string(func)).c_str();
			char *realName = abi::__cxa_demangle(toDemangle, 0, 0, &status);

			if (status == 0)
				func = realName;

		} 

#ifdef HAVE_BFD
		else {
			func = bfdMap[moduleNameStr]->getFunctionForAddress(frame.AddrPC.Offset);
		}
#endif

		if (func == NULL) {
			func = "";
		}

		std::pair<const char *, unsigned int> infos = std::make_pair("", 0);

#ifdef HAVE_BFD
		infos = bfdMap[moduleNameStr]->getFileAndLineForAddress(frame.AddrPC.Offset);
#endif

		if (infos.first == NULL || infos.second == 0) {
			printFrameInfos(os, maxDepth - depth - 1, frame.AddrPC.Offset, module_name, func, symbolOffset);
		} else {
			printFrameInfos(os, maxDepth - depth - 1, frame.AddrPC.Offset, module_name, func, symbolOffset, infos.first, infos.second);
		}
	}

	SymCleanup(process);
}

#elif defined(_MSC_VER)

#include <dbghelp.h>

StackWalkerMSVC::StackWalkerMSVC() {}

StackWalkerMSVC::~StackWalkerMSVC() {}

void StackWalkerMSVC::printCallStack(std::ostream &os, unsigned int maxDepth) {

	BOOL result;
	HANDLE process = GetCurrentProcess();
	HANDLE thread = GetCurrentThread();

	SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
	SymInitialize(process, NULL, TRUE);


	STACKFRAME        stack;
	ULONG               frame;
	IMAGEHLP_SYMBOL   *symbol = reinterpret_cast<IMAGEHLP_SYMBOL*>(malloc(sizeof(IMAGEHLP_SYMBOL)+2048*sizeof(TCHAR)));
	DWORD             displacement = 0;
	TCHAR name[1024];

	memset( &stack, 0, sizeof( STACKFRAME ) );

	symbol->SizeOfStruct  = sizeof( IMAGEHLP_SYMBOL );
	symbol->MaxNameLength = 2048;

	stack.AddrPC.Offset    = context->Eip;
	stack.AddrPC.Mode      = AddrModeFlat;
	stack.AddrStack.Offset = context->Esp;
	stack.AddrStack.Mode   = AddrModeFlat;
	stack.AddrFrame.Offset = context->Ebp;
	stack.AddrFrame.Mode   = AddrModeFlat;

	for( frame = 0; ; frame++ ) {
		result = StackWalk(
				IMAGE_FILE_MACHINE_I386,
				process,
				thread,
				&stack,
				context,
				NULL,
				SymFunctionTableAccess,
				SymGetModuleBase,
				NULL
		);

		if(!result) {
			break;
		}

		IMAGEHLP_MODULE image_module;
		IMAGEHLP_LINE image_line;

		image_module.SizeOfStruct = sizeof(IMAGEHLP_MODULE);
		image_line.SizeOfStruct = sizeof(IMAGEHLP_LINE);	

		std::string module_name = "";
		std::string symbol_name = "";
		std::string file_name = "";
		int line = 0;
		
		DWORD module_base = SymGetModuleBase(process, stack.AddrPC.Offset);
		
		if (SymGetModuleInfo(process,stack.AddrPC.Offset, &image_module)) {
			module_name = image_module.ImageName;
		} 
			
		if (SymGetSymFromAddr( process, ( ULONG )stack.AddrPC.Offset, &displacement, symbol )) {
			symbol_name = symbol->Name;
		} 

		if (SymGetLineFromAddr( process, ( ULONG )stack.AddrPC.Offset - 1, &displacement, &image_line )) {
			file_name = image_line.FileName;
			line = image_line.LineNumber;
		}

		printFrameInfos(os, frame, stack.AddrPC.Offset, module_name, symbol_name, stack.AddrPC.Offset - module_base, file_name, line);

	}
}

#endif		
