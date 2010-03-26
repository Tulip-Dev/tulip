INCLUDE_DIRECTORIES(${TulipInclude} ${TulipBuildInclude} ${TulipOGLInclude} ${TulipQtInclude} ${GLEW_INCLUDE_DIR})

##----------------------------------------------------------------------------------------------------------------------------
SET(Circle_SRCS circle.cpp)
ADD_LIBRARY(Circle-${TulipVersion} SHARED ${Circle_SRCS})
TARGET_LINK_LIBRARIES(Circle-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(Circle-${TulipVersion} ${LibTulipOglName})
ADD_DEPENDENCIES(Circle-${TulipVersion} ${LibTulipName} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Hexagone_SRCS hexagone.cpp)
ADD_LIBRARY(Hexagone-${TulipVersion} SHARED ${Hexagone_SRCS})
TARGET_LINK_LIBRARIES(Hexagone-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(Hexagone-${TulipVersion} ${LibTulipOglName})
ADD_DEPENDENCIES(Hexagone-${TulipVersion} ${LibTulipName} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Triangle_SRCS triangle.cpp)
ADD_LIBRARY(Triangle-${TulipVersion} SHARED ${Triangle_SRCS})
TARGET_LINK_LIBRARIES(Triangle-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(Triangle-${TulipVersion} ${LibTulipOglName})
ADD_DEPENDENCIES(Triangle-${TulipVersion} ${LibTulipName} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Pentagone_SRCS pentagone.cpp)
ADD_LIBRARY(Pentagone-${TulipVersion} SHARED ${Pentagone_SRCS})
TARGET_LINK_LIBRARIES(Pentagone-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(Pentagone-${TulipVersion} ${LibTulipOglName})
ADD_DEPENDENCIES(Pentagone-${TulipVersion} ${LibTulipName} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Square_SRCS square.cpp)
ADD_LIBRARY(Square-${TulipVersion} SHARED ${Square_SRCS})
TARGET_LINK_LIBRARIES(Square-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(Square-${TulipVersion} ${LibTulipOglName})
ADD_DEPENDENCIES(Square-${TulipVersion} ${LibTulipName} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Ring_SRCS ring.cpp)
ADD_LIBRARY(Ring-${TulipVersion} SHARED ${Ring_SRCS})
TARGET_LINK_LIBRARIES(Ring-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(Ring-${TulipVersion} ${LibTulipOglName})
ADD_DEPENDENCIES(Ring-${TulipVersion} ${LibTulipName} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Billboard_SRCS billboard.cpp)
ADD_LIBRARY(Billboard-${TulipVersion} SHARED ${Billboard_SRCS})
TARGET_LINK_LIBRARIES(Billboard-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(Billboard-${TulipVersion} ${LibTulipOglName})
ADD_DEPENDENCIES(Billboard-${TulipVersion} ${LibTulipName} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Cube_SRCS cube.cpp)
ADD_LIBRARY(Cube-${TulipVersion} SHARED ${Cube_SRCS})
TARGET_LINK_LIBRARIES(Cube-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(Cube-${TulipVersion} ${LibTulipOglName})
ADD_DEPENDENCIES(Cube-${TulipVersion} ${LibTulipName} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(CubeOutlinedTransparent_SRCS cubeoutlinedtransparent.cpp)
ADD_LIBRARY(CubeOutlinedTransparent-${TulipVersion} SHARED ${CubeOutlinedTransparent_SRCS})
TARGET_LINK_LIBRARIES(CubeOutlinedTransparent-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(CubeOutlinedTransparent-${TulipVersion} ${LibTulipOglName})
ADD_DEPENDENCIES(CubeOutlinedTransparent-${TulipVersion} ${LibTulipName} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Sphere_SRCS sphere.cpp)
ADD_LIBRARY(Sphere-${TulipVersion} SHARED ${Sphere_SRCS})
TARGET_LINK_LIBRARIES(Sphere-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(Sphere-${TulipVersion} ${LibTulipOglName})
ADD_DEPENDENCIES(Sphere-${TulipVersion} ${LibTulipName} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Cone_SRCS cone.cpp)
ADD_LIBRARY(Cone-${TulipVersion} SHARED ${Cone_SRCS})
TARGET_LINK_LIBRARIES(Cone-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(Cone-${TulipVersion} ${LibTulipOglName})
ADD_DEPENDENCIES(Cone-${TulipVersion} ${LibTulipName} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(Cylinder_SRCS cylinder.cpp)
ADD_LIBRARY(Cylinder-${TulipVersion} SHARED ${Cylinder_SRCS})
TARGET_LINK_LIBRARIES(Cylinder-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(Cylinder-${TulipVersion} ${LibTulipOglName})
ADD_DEPENDENCIES(Cylinder-${TulipVersion} ${LibTulipName} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(HalfCylinder_SRCS halfcylinder.cpp)
ADD_LIBRARY(HalfCylinder-${TulipVersion} SHARED ${HalfCylinder_SRCS})
TARGET_LINK_LIBRARIES(HalfCylinder-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(HalfCylinder-${TulipVersion} ${LibTulipOglName})
ADD_DEPENDENCIES(HalfCylinder-${TulipVersion} ${LibTulipName} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(SquareBorderTextured_SRCS squarebordertextured.cpp)
ADD_LIBRARY(SquareBorderTextured-${TulipVersion} SHARED ${SquareBorderTextured_SRCS})
TARGET_LINK_LIBRARIES(SquareBorderTextured-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(SquareBorderTextured-${TulipVersion} ${LibTulipOglName})
ADD_DEPENDENCIES(SquareBorderTextured-${TulipVersion} ${LibTulipName} ${LibTulipOglName})

##----------------------------------------------------------------------------------------------------------------------------
SET(ChristmasTree_SRCS christmastree.cpp)
ADD_LIBRARY(ChristmasTree-${TulipVersion} SHARED ${ChristmasTree_SRCS})
TARGET_LINK_LIBRARIES(ChristmasTree-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(ChristmasTree-${TulipVersion} ${LibTulipOglName})
ADD_DEPENDENCIES(ChristmasTree-${TulipVersion} ${LibTulipName} ${LibTulipOglName})

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
	SquareBorderTextured-${TulipVersion}
	ChristmasTree-${TulipVersion}
	DESTINATION ${CMAKE_INSTALL_PREFIX}${TulipLibInstallDir}tlp/glyphs/)

