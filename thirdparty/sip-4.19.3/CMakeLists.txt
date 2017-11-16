ADD_SUBDIRECTORY(siplib)
# avoid polluting source tree with .pyc python files
# generated during siplib configuration
FILE(GLOB_RECURSE PYCFILES *.pyc)
FILE(REMOVE ${PYCFILES})
