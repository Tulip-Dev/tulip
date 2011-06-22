#ifndef SYSTEMDEFINITION_H
#define SYSTEMDEFINITION_H

#if defined(_WIN32)
#define OS_PLATFORM "win"
#elif defined(__APPLE__)
#define OS_PLATFORM "mac"
#elif defined(__linux)
#define OS_PLATFORM "linux"
#else
#define OS_PLATFORM "other"
#endif

#if defined(__x86_64__) || defined(_M_X64)
#define OS_ARCHITECTURE "x86_64"
#elif defined(__i386__) || defined(_M_IX86)
#define OS_ARCHITECTURE "x86"
#endif

#if defined(__clang__)
#define OS_COMPILER "Clang"
#elif defined(__GNUC__)
#define OS_COMPILER "gcc"
#elif defined(_MSC_VER)
#define OS_COMPILER "MSVC"
#endif

#endif // SYSTEMDEFINITION_H
