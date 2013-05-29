INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${GLEW_INCLUDE_DIR})

MACRO(GLYPH name source)
  ADD_LIBRARY(${name}-${TulipVersion} SHARED ${source})
  TARGET_LINK_LIBRARIES(${name}-${TulipVersion} ${LibTulipCoreName} ${LibTulipOglName})
  INSTALL(TARGETS ${name}-${TulipVersion} DESTINATION ${TulipGlyphPluginsInstallDir} COMPONENT ${name})
ENDMACRO(GLYPH)

GLYPH(Cone cone.cpp)

GLYPH(Circle circle.cpp)
GLYPH(Sphere sphere.cpp)

GLYPH(Square square.cpp)
GLYPH(RoundedBox roundedbox.cpp)
GLYPH(Cube cube.cpp)

GLYPH(Window window.cpp)
GLYPH(Hexagone hexagone.cpp)
GLYPH(Triangle triangle.cpp)
GLYPH(Pentagone pentagone.cpp)
GLYPH(CubeOutlinedTransparent cubeoutlinedtransparent.cpp)
GLYPH(Ring ring.cpp)
GLYPH(Billboard billboard.cpp)
GLYPH(Cylinder cylinder.cpp)
GLYPH(HalfCylinder halfcylinder.cpp)
GLYPH(ChristmasTree christmastree.cpp)
GLYPH(Diamond diamond.cpp)
GLYPH(Cross cross.cpp)
GLYPH(GlowSphere glowsphere.cpp)
GLYPH(Star star.cpp)

## radialGradientTexture.png is used in glowsphere.cpp
INSTALL(FILES
  radialGradientTexture.png
  DESTINATION ${TulipBitmapInstallDir}
  COMPONENT tulip_plugins)
