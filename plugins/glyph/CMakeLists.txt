INCLUDE_DIRECTORIES(${TulipInclude} ${TulipOGLInclude} ${TulipQtInclude})

##----------------------------------------------------------------------------------------------------------------------------
SET(Circle_SRCS circle.cpp)
ADD_LIBRARY(Circle-${TulipVersion} SHARED ${Circle_SRCS})
TARGET_LINK_LIBRARIES(Circle-${TulipVersion} tulip)
TARGET_LINK_LIBRARIES(Circle-${TulipVersion} tulip-ogl)
ADD_DEPENDENCIES(Circle-${TulipVersion} tulip tulip-ogl)

##----------------------------------------------------------------------------------------------------------------------------
SET(Hexagone_SRCS hexagone.cpp)
ADD_LIBRARY(Hexagone-${TulipVersion} SHARED ${Hexagone_SRCS})
TARGET_LINK_LIBRARIES(Hexagone-${TulipVersion} tulip)
TARGET_LINK_LIBRARIES(Hexagone-${TulipVersion} tulip-ogl)
ADD_DEPENDENCIES(Hexagone-${TulipVersion} tulip tulip-ogl)

##----------------------------------------------------------------------------------------------------------------------------
SET(Triangle_SRCS triangle.cpp)
ADD_LIBRARY(Triangle-${TulipVersion} SHARED ${Triangle_SRCS})
TARGET_LINK_LIBRARIES(Triangle-${TulipVersion} tulip)
TARGET_LINK_LIBRARIES(Triangle-${TulipVersion} tulip-ogl)
ADD_DEPENDENCIES(Triangle-${TulipVersion} tulip tulip-ogl)

##----------------------------------------------------------------------------------------------------------------------------
SET(Pentagone_SRCS pentagone.cpp)
ADD_LIBRARY(Pentagone-${TulipVersion} SHARED ${Pentagone_SRCS})
TARGET_LINK_LIBRARIES(Pentagone-${TulipVersion} tulip)
TARGET_LINK_LIBRARIES(Pentagone-${TulipVersion} tulip-ogl)
ADD_DEPENDENCIES(Pentagone-${TulipVersion} tulip tulip-ogl)

##----------------------------------------------------------------------------------------------------------------------------
SET(Square_SRCS square.cpp)
ADD_LIBRARY(Square-${TulipVersion} SHARED ${Square_SRCS})
TARGET_LINK_LIBRARIES(Square-${TulipVersion} tulip)
TARGET_LINK_LIBRARIES(Square-${TulipVersion} tulip-ogl)
ADD_DEPENDENCIES(Square-${TulipVersion} tulip tulip-ogl)

##----------------------------------------------------------------------------------------------------------------------------
SET(Ring_SRCS ring.cpp)
ADD_LIBRARY(Ring-${TulipVersion} SHARED ${Ring_SRCS})
TARGET_LINK_LIBRARIES(Ring-${TulipVersion} tulip)
TARGET_LINK_LIBRARIES(Ring-${TulipVersion} tulip-ogl)
ADD_DEPENDENCIES(Ring-${TulipVersion} tulip tulip-ogl)

##----------------------------------------------------------------------------------------------------------------------------
SET(Billboard_SRCS billboard.cpp)
ADD_LIBRARY(Billboard-${TulipVersion} SHARED ${Billboard_SRCS})
TARGET_LINK_LIBRARIES(Billboard-${TulipVersion} tulip)
TARGET_LINK_LIBRARIES(Billboard-${TulipVersion} tulip-ogl)
ADD_DEPENDENCIES(Billboard-${TulipVersion} tulip tulip-ogl)

##----------------------------------------------------------------------------------------------------------------------------
SET(Cube_SRCS cube.cpp)
ADD_LIBRARY(Cube-${TulipVersion} SHARED ${Cube_SRCS})
TARGET_LINK_LIBRARIES(Cube-${TulipVersion} tulip)
TARGET_LINK_LIBRARIES(Cube-${TulipVersion} tulip-ogl)
ADD_DEPENDENCIES(Cube-${TulipVersion} tulip tulip-ogl)

##----------------------------------------------------------------------------------------------------------------------------
SET(CubeOutlinedTransparent_SRCS cubeoutlinedtransparent.cpp)
ADD_LIBRARY(CubeOutlinedTransparent-${TulipVersion} SHARED ${CubeOutlinedTransparent_SRCS})
TARGET_LINK_LIBRARIES(CubeOutlinedTransparent-${TulipVersion} tulip)
TARGET_LINK_LIBRARIES(CubeOutlinedTransparent-${TulipVersion} tulip-ogl)
ADD_DEPENDENCIES(CubeOutlinedTransparent-${TulipVersion} tulip tulip-ogl)

##----------------------------------------------------------------------------------------------------------------------------
SET(Sphere_SRCS sphere.cpp)
ADD_LIBRARY(Sphere-${TulipVersion} SHARED ${Sphere_SRCS})
TARGET_LINK_LIBRARIES(Sphere-${TulipVersion} tulip)
TARGET_LINK_LIBRARIES(Sphere-${TulipVersion} tulip-ogl)
ADD_DEPENDENCIES(Sphere-${TulipVersion} tulip tulip-ogl)

##----------------------------------------------------------------------------------------------------------------------------
SET(Cone_SRCS cone.cpp)
ADD_LIBRARY(Cone-${TulipVersion} SHARED ${Cone_SRCS})
TARGET_LINK_LIBRARIES(Cone-${TulipVersion} tulip)
TARGET_LINK_LIBRARIES(Cone-${TulipVersion} tulip-ogl)
ADD_DEPENDENCIES(Cone-${TulipVersion} tulip tulip-ogl)

##----------------------------------------------------------------------------------------------------------------------------
SET(Cylinder_SRCS cylinder.cpp)
ADD_LIBRARY(Cylinder-${TulipVersion} SHARED ${Cylinder_SRCS})
TARGET_LINK_LIBRARIES(Cylinder-${TulipVersion} tulip)
TARGET_LINK_LIBRARIES(Cylinder-${TulipVersion} tulip-ogl)
ADD_DEPENDENCIES(Cylinder-${TulipVersion} tulip tulip-ogl)

##----------------------------------------------------------------------------------------------------------------------------
SET(HalfCylinder_SRCS halfcylinder.cpp)
ADD_LIBRARY(HalfCylinder-${TulipVersion} SHARED ${HalfCylinder_SRCS})
TARGET_LINK_LIBRARIES(HalfCylinder-${TulipVersion} tulip)
TARGET_LINK_LIBRARIES(HalfCylinder-${TulipVersion} tulip-ogl)
ADD_DEPENDENCIES(HalfCylinder-${TulipVersion} tulip tulip-ogl)

##----------------------------------------------------------------------------------------------------------------------------
SET(SquareBorderTextured_SRCS squarebordertextured.cpp)
ADD_LIBRARY(SquareBorderTextured-${TulipVersion} SHARED ${SquareBorderTextured_SRCS})
TARGET_LINK_LIBRARIES(SquareBorderTextured-${TulipVersion} tulip)
TARGET_LINK_LIBRARIES(SquareBorderTextured-${TulipVersion} tulip-ogl)
ADD_DEPENDENCIES(SquareBorderTextured-${TulipVersion} tulip tulip-ogl)

##----------------------------------------------------------------------------------------------------------------------------
SET(ChristmasTree_SRCS christmastree.cpp)
ADD_LIBRARY(ChristmasTree-${TulipVersion} SHARED ${ChristmasTree_SRCS})
TARGET_LINK_LIBRARIES(ChristmasTree-${TulipVersion} tulip)
TARGET_LINK_LIBRARIES(ChristmasTree-${TulipVersion} tulip-ogl)
ADD_DEPENDENCIES(ChristmasTree-${TulipVersion} tulip tulip-ogl)

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

