#ifndef MODELTEST_EXPORT_H
#define MODELTEST_EXPORT_H

#ifdef _WIN32
#  ifdef MODEL_TESTS_DLL
#    define MODEL_TESTS_SCOPE       __declspec(dllexport)
#  else
#    define MODEL_TESTS_SCOPE       __declspec(dllimport)
#  endif
#endif
#ifndef MODEL_TESTS_SCOPE
#  define MODEL_TESTS_SCOPE
#endif

#endif // MODELTEST_EXPORT_H
