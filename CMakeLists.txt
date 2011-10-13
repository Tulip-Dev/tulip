FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(TULIP 4.0.0 REQUIRED)

INCLUDE_DIRECTORIES(${TULIP_INCLUDE_DIR} ${QT_INCLUDE_DIR} library/tulip-core/include)

SET(EXE_SRCS
main.cpp
c1.cpp
c2.cpp
)

SET(LIB_SRCS
library/tulip-core/src/TulipPlugin.cpp
library/tulip-core/src/TulipPluginsManager.cpp
)

ADD_LIBRARY(tulip-core-additions ${LIB_SRCS})
ADD_EXECUTABLE(test-plugins ${EXE_SRCS})

TARGET_LINK_LIBRARIES(tulip-core-additions ${TULIP_LIBRARIES} ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(test-plugins ${TULIP_LIBRARIES} ${QT_LIBRARIES} tulip-core-additions)

INSTALL(TARGETS tulip-core-additions DESTINATION ${TULIP_DIR}/lib)
INSTALL(TARGETS test-plugins DESTINATION ${TULIP_DIR}/bin)

