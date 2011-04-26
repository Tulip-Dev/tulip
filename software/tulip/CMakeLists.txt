SET(SRCS
  src/main.cpp
)

ADD_EXECUTABLE(tulip_app ${SRCS})

INSTALL(TARGETS tulip_app
      DESTINATION bin)
