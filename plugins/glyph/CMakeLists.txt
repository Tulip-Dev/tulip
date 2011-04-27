INCLUDE_DIRECTORIES(${TulipInclude} ${TulipBuildInclude} ${TulipOGLInclude} ${TulipQtInclude} ${GLEW_INCLUDE_DIR})


##----------------------------------------------------------------------------------------------------------------------------
SET(Window_SRCS window.cpp)
ADD_LIBRARY(Window-${TulipVersion} SHARED ${Window_SRCS})
TARGET_LINK_LIBRARIES(Window-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(Window-${TulipVersion} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Circle_SRCS circle.cpp)
ADD_LIBRARY(Circle-${TulipVersion} SHARED ${Circle_SRCS})
TARGET_LINK_LIBRARIES(Circle-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(Circle-${TulipVersion} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Hexagone_SRCS hexagone.cpp)
ADD_LIBRARY(Hexagone-${TulipVersion} SHARED ${Hexagone_SRCS})
TARGET_LINK_LIBRARIES(Hexagone-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(Hexagone-${TulipVersion} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Triangle_SRCS triangle.cpp)
ADD_LIBRARY(Triangle-${TulipVersion} SHARED ${Triangle_SRCS})
TARGET_LINK_LIBRARIES(Triangle-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(Triangle-${TulipVersion} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Pentagone_SRCS pentagone.cpp)
ADD_LIBRARY(Pentagone-${TulipVersion} SHARED ${Pentagone_SRCS})
TARGET_LINK_LIBRARIES(Pentagone-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(Pentagone-${TulipVersion} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Square_SRCS square.cpp)
ADD_LIBRARY(Square-${TulipVersion} SHARED ${Square_SRCS})
TARGET_LINK_LIBRARIES(Square-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(Square-${TulipVersion} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Ring_SRCS ring.cpp)
ADD_LIBRARY(Ring-${TulipVersion} SHARED ${Ring_SRCS})
TARGET_LINK_LIBRARIES(Ring-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(Ring-${TulipVersion} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Billboard_SRCS billboard.cpp)
ADD_LIBRARY(Billboard-${TulipVersion} SHARED ${Billboard_SRCS})
TARGET_LINK_LIBRARIES(Billboard-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(Billboard-${TulipVersion} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Cube_SRCS cube.cpp)
ADD_LIBRARY(Cube-${TulipVersion} SHARED ${Cube_SRCS})
TARGET_LINK_LIBRARIES(Cube-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(Cube-${TulipVersion} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(CubeOutlinedTransparent_SRCS cubeoutlinedtransparent.cpp)
ADD_LIBRARY(CubeOutlinedTransparent-${TulipVersion} SHARED ${CubeOutlinedTransparent_SRCS})
TARGET_LINK_LIBRARIES(CubeOutlinedTransparent-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(CubeOutlinedTransparent-${TulipVersion} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Sphere_SRCS sphere.cpp)
ADD_LIBRARY(Sphere-${TulipVersion} SHARED ${Sphere_SRCS})
TARGET_LINK_LIBRARIES(Sphere-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(Sphere-${TulipVersion} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Cone_SRCS cone.cpp)
ADD_LIBRARY(Cone-${TulipVersion} SHARED ${Cone_SRCS})
TARGET_LINK_LIBRARIES(Cone-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(Cone-${TulipVersion} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Cylinder_SRCS cylinder.cpp)
ADD_LIBRARY(Cylinder-${TulipVersion} SHARED ${Cylinder_SRCS})
TARGET_LINK_LIBRARIES(Cylinder-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(Cylinder-${TulipVersion} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(HalfCylinder_SRCS halfcylinder.cpp)
ADD_LIBRARY(HalfCylinder-${TulipVersion} SHARED ${HalfCylinder_SRCS})
TARGET_LINK_LIBRARIES(HalfCylinder-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(HalfCylinder-${TulipVersion} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(ChristmasTree_SRCS christmastree.cpp)
ADD_LIBRARY(ChristmasTree-${TulipVersion} SHARED ${ChristmasTree_SRCS})
TARGET_LINK_LIBRARIES(ChristmasTree-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(ChristmasTree-${TulipVersion} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(RoundedBox_SRCS roundedbox.cpp)
ADD_LIBRARY(RoundedBox-${TulipVersion} SHARED ${RoundedBox_SRCS})
TARGET_LINK_LIBRARIES(RoundedBox-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(RoundedBox-${TulipVersion} ${LibTulipOglName})
##----------------------------------------------------------------------------------------------------------------------------

INSTALL(TARGETS 
	Circle-${TulipVersion}
	Hexagone-${TulipVersion}
	Triangle-${TulipVersion}
	Pentagone-${TulipVersion}
	Square-${TulipVersion}
	Ring-${TulipVersion}
	Billboard-${TulipVersion}
	Cube-${TulipVersion}
	CubeOutlinedTransparent-${TulipVersion}
	Sphere-${TulipVersion}
	Cone-${TulipVersion}
	Cylinder-${TulipVersion}
	HalfCylinder-${TulipVersion}
	ChristmasTree-${TulipVersion}
	Window-${TulipVersion}
	RoundedBox-${TulipVersion}
  DESTINATION ${TulipPluginsInstallDir}/glyphs/
  COMPONENT libtulip_ogl_plugins)
